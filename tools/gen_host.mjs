import { readFile, writeFile } from 'fs/promises'

// these will not be generated
const skippedFunctions = [
  // these use vargs, which are complicated over wasm, and should be done cart-side
  'TraceLog',
  'TextFormat',

  // not sure why, but these are undefined, maybe raylib API change
  'GetClipboardImage',

  // fs callbacks are not supported (since we force use of physfs)
  'SetLoadFileDataCallback',
  'SetSaveFileDataCallback',
  'SetLoadFileTextCallback',
  'SetSaveFileTextCallback',

  // TODO: not sure how to handle, but should be doable
  `SetTraceLogCallback`
]

// for these, use FS variants (to use physfs)
const fsFunctions = [
  'FileExists',
  'DirectoryExists',
  'GetFileLength',
  'ChangeDirectory',
  'IsPathFile',
  'LoadDirectoryFiles',
  'LoadDirectoryFilesEx',
  'GetFileModTime'
]



// Helper to determine if a type needs pointer handling
function needsPointerHandling(type) {
  return type.includes('*') || type[0] === type[0].toUpperCase()
}

function isStringReturn(type) {
  return type === 'char *' || type === 'const char *'
}

function isSimpleType(type) {
  return ['bool', 'int', 'float', 'double', 'unsigned int', 'long'].includes(type)
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

function generateFunctionSignature(func, isWamr = false) {
  // Always return unsigned int for string returns or simple types
  let returnType = func.returnType === 'void' ||
    (needsPointerHandling(func.returnType) && !isStringReturn(func.returnType))
    ? 'void'
    : 'unsigned int'

  let params = []

  if (isWamr) {
    params.push('wasm_exec_env_t exec_env')
  }

  // Add output parameter for struct returns (but not for strings)
  if (needsPointerHandling(func.returnType) &&
      !isStringReturn(func.returnType) &&
      func.returnType !== 'void') {
    params.push(`unsigned int outPtr`)
  }

  // Add regular parameters
  params = params.concat((func.params || [])
    .map((param) => `unsigned int ${param.name}Ptr`))

  return `${returnType} host_${func.name}(${params.join(', ')})`
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

  const fname = fsFunctions.includes(func.name) ? `${func.name}FS` : func.name

  if (func.returnType === 'void') {
    body += `    ${func.name}(${paramList});\n`
  } else if (isSimpleType(func.returnType)) {
    // For simple types, return directly
    body += `    return ${fname}(${paramList});\n`
  } else if (isStringReturn(func.returnType)) {
    // For string returns
    body += `    char* result = ${fname}(${paramList});\n`
    body += `    return cart_set_string(result);\n`
  } else {
    // For struct returns, use output parameter
    body += `    ${func.returnType} out = ${fname}(${paramList});\n`
    body += `    cart_set_pointer(&out, sizeof(out), outPtr);\n`
  }

  // Cleanup
  func.params.forEach((param) => {
    const cleanup = generateParamCleanup(param)
    if (cleanup) {
      body += cleanup + '\n'
    }
  })

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
    if (!skippedFunctions.includes(func.name)) {
      output += `${generateFunctionSignature(func, true)} {\n`
      output += generateFunctionBody(func)
      output += `}\n\n`
    }
  })

  return output
}

function generateWamrSymbols(api) {
  const out = ['static NativeSymbol native_symbols[] = {']
  const functions = []
  for (const func of api) {
    if (!skippedFunctions.includes(func.name)) {
      functions.push(`  {"${func.name}", host_${func.name}, NULL}`)
    }
  }
  out.push(functions.join(',\n'))
  out.push('};')
  return out.join('\n')
}



const api = await fetch('https://raw.githubusercontent.com/RobLoach/raylib-api/refs/tags/v5.5.0/raylib.json').then(r => r.json())

// const { functions } = api

// do a small subset of functions, just to support demo
const demoFunctions = [
  'SetWindowSize',
  'LoadImage',
  'LoadTextureFromImage',
  'UnloadImage',
  'LoadFileText',
  'ClearBackground',
  'DrawTexture',
  'DrawText'
]
const functions = api.functions.filter(f => demoFunctions.includes(f.name))

// params is not set sometimes
for (const f of Object.values(functions)) {
  if (!f.params) {
    f.params = []
  }
}

await writeFile('host/src/null0_host_emscripten.h', [
  (await readFile('tools/gen_host_emscripten_header.h', 'utf8')),
  generateEmscriptenHeader(functions),
  (await readFile('tools/gen_host_emscripten_footer.h', 'utf8'))
].join('\n'))

await writeFile('host/src/null0_host_wamr.h', [
  (await readFile('tools/gen_host_wamr_header.h', 'utf8')),
  generateWamrHeader(functions),
  generateWamrSymbols(functions),
  (await readFile('tools/gen_host_wamr_footer.h', 'utf8'))
].join('\n'))
