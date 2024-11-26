import { readFile, writeFile } from 'fs/promises'

// these will not be generated
const skippedFunctions = [
  // these use vargs, which are complicated over wasm, and should be done cart-side
  'TraceLog',
  'TextFormat',

  // not sure why, but these are undefined
  'GetClipboardImage'
]

// Helper to determine if a type needs pointer handling
function needsPointerHandling(type) {
  return type.includes('*') || type[0] === type[0].toUpperCase()
}

function generateParamHandling(param) {
  if (needsPointerHandling(param.type)) {
    if (param.type === 'const char *') {
      return `    const char* ${param.name} = cart_get_string(${param.name}Ptr);`
    } else {
      return `    ${param.type}* ${param.name} = cart_get_pointer(${param.name}Ptr, sizeof(${param.type}));`
    }
  }
  return ''
}

function generateParamUsage(param) {
  if (param.type === 'const char *' || param.type.includes('*')) {
    return param.name // Don't dereference pointers
  } else if (needsPointerHandling(param.type)) {
    return `*${param.name}` // Dereference structs
  }
  return param.name + 'Ptr' // Use raw value for numbers and booleans
}

function generateParamCleanup(param) {
  if (needsPointerHandling(param.type)) {
    return `    free((void*)${param.name});`
  }
  return ''
}

function generateReturnHandling(func, returnVal) {
  if (func.returnType === 'void') {
    return ''
  }
  if (needsPointerHandling(func.returnType)) {
    return `    return cart_set_pointer(&${returnVal}, sizeof(${returnVal}));`
  }
  return `    return ${returnVal};`
}

function generateFunctionSignature(func, isWamr = false) {
  const returnType = func.returnType === 'void' ? 'void' : 'unsigned int'
  const wamrParam = isWamr ? 'wasm_exec_env_t exec_env, ' : ''

  const params = (func.params || [])
    .map((param) => {
      // For numbers and booleans, pass the value directly
      return `unsigned int ${param.name}Ptr`
    })
    .join(', ')

  return `${returnType} host_${func.name}(${wamrParam}${params})`
}

function isSimpleType(type) {
  return ['bool', 'int', 'float', 'double', 'unsigned int'].includes(type)
}

function generateFunctionBody(func) {
  let body = ''

  // Generate parameter handling
  func.params.forEach((param) => {
    const handling = generateParamHandling(param)
    if (handling) {
      body += handling + '\n'
    }
  })

  // Generate function call
  const paramList = func.params.map((p) => generateParamUsage(p)).join(', ')

  if (func.returnType === 'void') {
    body += `    ${func.name}(${paramList});\n`
  } else if (isSimpleType(func.returnType)) {
    // For simple types, return directly
    body += `    return ${func.name}(${paramList});\n`
  } else {
    // For complex types that need pointer handling
    body += `    ${func.returnType} out = ${func.name}(${paramList});\n`
  }

  // Cleanup
  func.params.forEach((param) => {
    const cleanup = generateParamCleanup(param)
    if (cleanup) {
      body += cleanup + '\n'
    }
  })

  // Return handling for complex types
  if (func.returnType !== 'void' && !isSimpleType(func.returnType)) {
    body += generateReturnHandling(func, 'out') + '\n'
  }

  return body
}

function generateEmscriptenHeader(api) {
  let output = ``

  api.forEach((func) => {
    if (!skippedFunctions.includes(func.name)) {
      output += `EMSCRIPTEN_KEEPALIVE ${generateFunctionSignature(func)} {\n`
      output += generateFunctionBody(func)
      output += `}\n\n`
    }
  })

  return output
}

function generateWamrHeader(api) {
  let output = ``

  api.forEach((func) => {
    output += `${generateFunctionSignature(func, true)} {\n`
    output += generateFunctionBody(func)
    output += `}\n\n`
  })

  return output
}

const api = JSON.parse(await readFile('build/_deps/raylib-src/parser/output/raylib_api.json', 'utf8'))

// params is not set sometimes
for (const f of Object.values(api.functions)) {
  if (!f.params) {
    f.params = []
  }
}

await writeFile('host/src/null0_host_emscripten.h', (await readFile('tools/gen_host_emscripten_header.h', 'utf8')) + generateEmscriptenHeader(api.functions) + (await readFile('tools/gen_host_emscripten_footer.h', 'utf8')))

await writeFile('host/src/null0_host_wamr.h', (await readFile('tools/gen_host_wamr_header.h', 'utf8')) + generateWamrHeader(api.functions) + (await readFile('tools/gen_host_wamr_footer.h', 'utf8')))
