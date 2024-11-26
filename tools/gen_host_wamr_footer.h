

// called on cart init
void null0_host_load(unsigned char* wasmBytes, int wasmBytesLen){
    uint32_t stack_size = 8092, heap_size = 8092;
    char error_buf[128];
    wasm_runtime_init();
    wasm_runtime_register_natives("env", native_symbols, sizeof(native_symbols) / sizeof(NativeSymbol));
    module = wasm_runtime_load(wasmBytes, wasmBytesLen, error_buf, sizeof(error_buf));
    module_inst = wasm_runtime_instantiate(module, stack_size, heap_size, error_buf, sizeof(error_buf));
    exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);

    wasm_function_inst_t cart_load = wasm_runtime_lookup_function(module_inst, "load");
    cart_update = wasm_runtime_lookup_function(module_inst, "update");
    cart_unload = wasm_runtime_lookup_function(module_inst, "unload");

    wasm_application_execute_main(module_inst, 0, NULL);
    if (cart_load != NULL) {
        if (!wasm_runtime_call_wasm(exec_env, cart_load, 0, NULL)) {
            // not fatal, but this will help with troubleshooting
            printf("load: %s\n", wasm_runtime_get_exception(module_inst));
        }
    }
}

// called on cart update
void null0_host_update(double timeMs){
    if (cart_update != NULL) {
        update_args[0] = timeMs;
        wasm_runtime_call_wasm(exec_env, cart_update, 2, update_args);
    }
}

// called on cart unload
void null0_host_unload(){
    if (cart_unload != NULL) {
        if (!wasm_runtime_call_wasm(exec_env, cart_unload, 0, NULL)) {
            // not fatal, but this will help with troubleshooting
            printf("unload: %s\n", wasm_runtime_get_exception(module_inst));
        }
    }
}
