#pragma once
#include <stdlib.h>
#include <string.h>

#include "wasm_c_api.h"
#include "wasm_export.h"

// Global reference to the WASM runtime instance
static wasm_module_t module;
static wasm_module_inst_t module_inst;
static wasm_function_inst_t func;
static wasm_exec_env_t exec_env;
static wasm_function_inst_t cart_update = NULL;
static wasm_function_inst_t cart_unload = NULL;
static uint64_t update_args[1];

// copy a pointer from cart to host
void* cart_get_pointer(unsigned int cartPtr, unsigned int len) {
    void* out = malloc(len);
    memcpy(out, wasm_runtime_addr_app_to_native(module_inst, (uint64_t)cartPtr), len);
    return out;
}

// copy a pointer from host to cart
unsigned int cart_set_pointer(void* hostPtr, unsigned int len, unsigned int cartPtr) {
  if (cartPtr ==0) {
    cartPtr = wasm_runtime_module_malloc(module_inst, len, NULL);
  }
    if (cartPtr == 0) {
        return 0;
    }
    void* nativePtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
    if (!nativePtr) {
        wasm_runtime_module_free(module_inst, cartPtr);
        return 0;
    }
    memcpy(nativePtr, hostPtr, len);
    return cartPtr;
}

// copy a string from cart to host
char* cart_get_string(unsigned int cartPtr) {
  void* nativePtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
  unsigned int len = strlen(nativePtr) + 1;
  return cart_get_pointer(cartPtr, len);
}

// copy a string from host to cart
unsigned int cart_set_string(char* hostPtr) {
  unsigned int len = strlen(hostPtr) + 1;
  unsigned int cartPtr = wasm_runtime_module_malloc(module_inst, len, NULL);
  void* nativePtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
  memcpy(nativePtr, hostPtr, len);
  return cartPtr;
}

// allocate cart-memory from host C
unsigned int cart_malloc(int size) {
  return wasm_runtime_module_malloc(module_inst, size, NULL);
}

// free cart-memory from host C
void cart_free (unsigned int ptr) {
  wasm_runtime_module_free(module_inst, ptr);
}
void host_InitWindow(wasm_exec_env_t exec_env, unsigned int widthPtr, unsigned int heightPtr, unsigned int titlePtr) {
    const char* title = cart_get_string(titlePtr);
    InitWindow(widthPtr, heightPtr, title);
    free((void*)title);
}

void host_CloseWindow(wasm_exec_env_t exec_env) {
    CloseWindow();
}

unsigned int host_WindowShouldClose(wasm_exec_env_t exec_env) {
    return WindowShouldClose();
}

unsigned int host_IsWindowReady(wasm_exec_env_t exec_env) {
    return IsWindowReady();
}

unsigned int host_IsWindowFullscreen(wasm_exec_env_t exec_env) {
    return IsWindowFullscreen();
}

unsigned int host_IsWindowHidden(wasm_exec_env_t exec_env) {
    return IsWindowHidden();
}

unsigned int host_IsWindowMinimized(wasm_exec_env_t exec_env) {
    return IsWindowMinimized();
}

unsigned int host_IsWindowMaximized(wasm_exec_env_t exec_env) {
    return IsWindowMaximized();
}

unsigned int host_IsWindowFocused(wasm_exec_env_t exec_env) {
    return IsWindowFocused();
}

unsigned int host_IsWindowResized(wasm_exec_env_t exec_env) {
    return IsWindowResized();
}

unsigned int host_IsWindowState(wasm_exec_env_t exec_env, unsigned int flagPtr) {
    return IsWindowState(flagPtr);
}

void host_SetWindowState(wasm_exec_env_t exec_env, unsigned int flagsPtr) {
    SetWindowState(flagsPtr);
}

void host_ClearWindowState(wasm_exec_env_t exec_env, unsigned int flagsPtr) {
    ClearWindowState(flagsPtr);
}

void host_ToggleFullscreen(wasm_exec_env_t exec_env) {
    ToggleFullscreen();
}

void host_ToggleBorderlessWindowed(wasm_exec_env_t exec_env) {
    ToggleBorderlessWindowed();
}

void host_MaximizeWindow(wasm_exec_env_t exec_env) {
    MaximizeWindow();
}

void host_MinimizeWindow(wasm_exec_env_t exec_env) {
    MinimizeWindow();
}

void host_RestoreWindow(wasm_exec_env_t exec_env) {
    RestoreWindow();
}

void host_SetWindowIcon(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    SetWindowIcon(*image);
    free((void*)image);
}

void host_SetWindowIcons(wasm_exec_env_t exec_env, unsigned int imagesPtr, unsigned int countPtr) {
    Image ** images = cart_get_pointer(imagesPtr, sizeof(Image *));
    SetWindowIcons(images, countPtr);
    free((void*)images);
}

void host_SetWindowTitle(wasm_exec_env_t exec_env, unsigned int titlePtr) {
    const char* title = cart_get_string(titlePtr);
    SetWindowTitle(title);
    free((void*)title);
}

void host_SetWindowPosition(wasm_exec_env_t exec_env, unsigned int xPtr, unsigned int yPtr) {
    SetWindowPosition(xPtr, yPtr);
}

void host_SetWindowMonitor(wasm_exec_env_t exec_env, unsigned int monitorPtr) {
    SetWindowMonitor(monitorPtr);
}

void host_SetWindowMinSize(wasm_exec_env_t exec_env, unsigned int widthPtr, unsigned int heightPtr) {
    SetWindowMinSize(widthPtr, heightPtr);
}

void host_SetWindowMaxSize(wasm_exec_env_t exec_env, unsigned int widthPtr, unsigned int heightPtr) {
    SetWindowMaxSize(widthPtr, heightPtr);
}

void host_SetWindowSize(wasm_exec_env_t exec_env, unsigned int widthPtr, unsigned int heightPtr) {
    SetWindowSize(widthPtr, heightPtr);
}

void host_SetWindowOpacity(wasm_exec_env_t exec_env, unsigned int opacityPtr) {
    SetWindowOpacity(opacityPtr);
}

void host_SetWindowFocused(wasm_exec_env_t exec_env) {
    SetWindowFocused();
}

void host_GetWindowHandle(wasm_exec_env_t exec_env, unsigned int outPtr) {
    void * out = GetWindowHandle();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_GetScreenWidth(wasm_exec_env_t exec_env) {
    return GetScreenWidth();
}

unsigned int host_GetScreenHeight(wasm_exec_env_t exec_env) {
    return GetScreenHeight();
}

unsigned int host_GetRenderWidth(wasm_exec_env_t exec_env) {
    return GetRenderWidth();
}

unsigned int host_GetRenderHeight(wasm_exec_env_t exec_env) {
    return GetRenderHeight();
}

unsigned int host_GetMonitorCount(wasm_exec_env_t exec_env) {
    return GetMonitorCount();
}

unsigned int host_GetCurrentMonitor(wasm_exec_env_t exec_env) {
    return GetCurrentMonitor();
}

void host_GetMonitorPosition(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int monitorPtr) {
    Vector2 out = GetMonitorPosition(monitorPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_GetMonitorWidth(wasm_exec_env_t exec_env, unsigned int monitorPtr) {
    return GetMonitorWidth(monitorPtr);
}

unsigned int host_GetMonitorHeight(wasm_exec_env_t exec_env, unsigned int monitorPtr) {
    return GetMonitorHeight(monitorPtr);
}

unsigned int host_GetMonitorPhysicalWidth(wasm_exec_env_t exec_env, unsigned int monitorPtr) {
    return GetMonitorPhysicalWidth(monitorPtr);
}

unsigned int host_GetMonitorPhysicalHeight(wasm_exec_env_t exec_env, unsigned int monitorPtr) {
    return GetMonitorPhysicalHeight(monitorPtr);
}

unsigned int host_GetMonitorRefreshRate(wasm_exec_env_t exec_env, unsigned int monitorPtr) {
    return GetMonitorRefreshRate(monitorPtr);
}

void host_GetWindowPosition(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Vector2 out = GetWindowPosition();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetWindowScaleDPI(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Vector2 out = GetWindowScaleDPI();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetMonitorName(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int monitorPtr) {
    const char * out = GetMonitorName(monitorPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_SetClipboardText(wasm_exec_env_t exec_env, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    SetClipboardText(text);
    free((void*)text);
}

void host_GetClipboardText(wasm_exec_env_t exec_env, unsigned int outPtr) {
    const char * out = GetClipboardText();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_EnableEventWaiting(wasm_exec_env_t exec_env) {
    EnableEventWaiting();
}

void host_DisableEventWaiting(wasm_exec_env_t exec_env) {
    DisableEventWaiting();
}

void host_ShowCursor(wasm_exec_env_t exec_env) {
    ShowCursor();
}

void host_HideCursor(wasm_exec_env_t exec_env) {
    HideCursor();
}

unsigned int host_IsCursorHidden(wasm_exec_env_t exec_env) {
    return IsCursorHidden();
}

void host_EnableCursor(wasm_exec_env_t exec_env) {
    EnableCursor();
}

void host_DisableCursor(wasm_exec_env_t exec_env) {
    DisableCursor();
}

unsigned int host_IsCursorOnScreen(wasm_exec_env_t exec_env) {
    return IsCursorOnScreen();
}

void host_ClearBackground(wasm_exec_env_t exec_env, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ClearBackground(*color);
    free((void*)color);
}

void host_BeginDrawing(wasm_exec_env_t exec_env) {
    BeginDrawing();
}

void host_EndDrawing(wasm_exec_env_t exec_env) {
    EndDrawing();
}

void host_BeginMode2D(wasm_exec_env_t exec_env, unsigned int cameraPtr) {
    Camera2D* camera = cart_get_pointer(cameraPtr, sizeof(Camera2D));
    BeginMode2D(*camera);
    free((void*)camera);
}

void host_EndMode2D(wasm_exec_env_t exec_env) {
    EndMode2D();
}

void host_BeginMode3D(wasm_exec_env_t exec_env, unsigned int cameraPtr) {
    Camera3D* camera = cart_get_pointer(cameraPtr, sizeof(Camera3D));
    BeginMode3D(*camera);
    free((void*)camera);
}

void host_EndMode3D(wasm_exec_env_t exec_env) {
    EndMode3D();
}

void host_BeginTextureMode(wasm_exec_env_t exec_env, unsigned int targetPtr) {
    RenderTexture2D* target = cart_get_pointer(targetPtr, sizeof(RenderTexture2D));
    BeginTextureMode(*target);
    free((void*)target);
}

void host_EndTextureMode(wasm_exec_env_t exec_env) {
    EndTextureMode();
}

void host_BeginShaderMode(wasm_exec_env_t exec_env, unsigned int shaderPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    BeginShaderMode(*shader);
    free((void*)shader);
}

void host_EndShaderMode(wasm_exec_env_t exec_env) {
    EndShaderMode();
}

void host_BeginBlendMode(wasm_exec_env_t exec_env, unsigned int modePtr) {
    BeginBlendMode(modePtr);
}

void host_EndBlendMode(wasm_exec_env_t exec_env) {
    EndBlendMode();
}

void host_BeginScissorMode(wasm_exec_env_t exec_env, unsigned int xPtr, unsigned int yPtr, unsigned int widthPtr, unsigned int heightPtr) {
    BeginScissorMode(xPtr, yPtr, widthPtr, heightPtr);
}

void host_EndScissorMode(wasm_exec_env_t exec_env) {
    EndScissorMode();
}

void host_BeginVrStereoMode(wasm_exec_env_t exec_env, unsigned int configPtr) {
    VrStereoConfig* config = cart_get_pointer(configPtr, sizeof(VrStereoConfig));
    BeginVrStereoMode(*config);
    free((void*)config);
}

void host_EndVrStereoMode(wasm_exec_env_t exec_env) {
    EndVrStereoMode();
}

void host_LoadVrStereoConfig(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int devicePtr) {
    VrDeviceInfo* device = cart_get_pointer(devicePtr, sizeof(VrDeviceInfo));
    VrStereoConfig out = LoadVrStereoConfig(*device);
    free((void*)device);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadVrStereoConfig(wasm_exec_env_t exec_env, unsigned int configPtr) {
    VrStereoConfig* config = cart_get_pointer(configPtr, sizeof(VrStereoConfig));
    UnloadVrStereoConfig(*config);
    free((void*)config);
}

void host_LoadShader(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int vsFileNamePtr, unsigned int fsFileNamePtr) {
    const char* vsFileName = cart_get_string(vsFileNamePtr);
    const char* fsFileName = cart_get_string(fsFileNamePtr);
    Shader out = LoadShader(vsFileName, fsFileName);
    free((void*)vsFileName);
    free((void*)fsFileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadShaderFromMemory(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int vsCodePtr, unsigned int fsCodePtr) {
    const char* vsCode = cart_get_string(vsCodePtr);
    const char* fsCode = cart_get_string(fsCodePtr);
    Shader out = LoadShaderFromMemory(vsCode, fsCode);
    free((void*)vsCode);
    free((void*)fsCode);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsShaderValid(wasm_exec_env_t exec_env, unsigned int shaderPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    return IsShaderValid(*shader);
    free((void*)shader);
}

unsigned int host_GetShaderLocation(wasm_exec_env_t exec_env, unsigned int shaderPtr, unsigned int uniformNamePtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    const char* uniformName = cart_get_string(uniformNamePtr);
    return GetShaderLocation(*shader, uniformName);
    free((void*)shader);
    free((void*)uniformName);
}

unsigned int host_GetShaderLocationAttrib(wasm_exec_env_t exec_env, unsigned int shaderPtr, unsigned int attribNamePtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    const char* attribName = cart_get_string(attribNamePtr);
    return GetShaderLocationAttrib(*shader, attribName);
    free((void*)shader);
    free((void*)attribName);
}

void host_SetShaderValue(wasm_exec_env_t exec_env, unsigned int shaderPtr, unsigned int locIndexPtr, unsigned int valuePtr, unsigned int uniformTypePtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    const void ** value = cart_get_pointer(valuePtr, sizeof(const void *));
    SetShaderValue(*shader, locIndexPtr, value, uniformTypePtr);
    free((void*)shader);
    free((void*)value);
}

void host_SetShaderValueV(wasm_exec_env_t exec_env, unsigned int shaderPtr, unsigned int locIndexPtr, unsigned int valuePtr, unsigned int uniformTypePtr, unsigned int countPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    const void ** value = cart_get_pointer(valuePtr, sizeof(const void *));
    SetShaderValueV(*shader, locIndexPtr, value, uniformTypePtr, countPtr);
    free((void*)shader);
    free((void*)value);
}

void host_SetShaderValueMatrix(wasm_exec_env_t exec_env, unsigned int shaderPtr, unsigned int locIndexPtr, unsigned int matPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    Matrix* mat = cart_get_pointer(matPtr, sizeof(Matrix));
    SetShaderValueMatrix(*shader, locIndexPtr, *mat);
    free((void*)shader);
    free((void*)mat);
}

void host_SetShaderValueTexture(wasm_exec_env_t exec_env, unsigned int shaderPtr, unsigned int locIndexPtr, unsigned int texturePtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    SetShaderValueTexture(*shader, locIndexPtr, *texture);
    free((void*)shader);
    free((void*)texture);
}

void host_UnloadShader(wasm_exec_env_t exec_env, unsigned int shaderPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    UnloadShader(*shader);
    free((void*)shader);
}

void host_GetScreenToWorldRay(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Ray out = GetScreenToWorldRay(*position, *camera);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetScreenToWorldRayEx(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr, unsigned int widthPtr, unsigned int heightPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Ray out = GetScreenToWorldRayEx(*position, *camera, widthPtr, heightPtr);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetWorldToScreen(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Vector2 out = GetWorldToScreen(*position, *camera);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetWorldToScreenEx(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr, unsigned int widthPtr, unsigned int heightPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Vector2 out = GetWorldToScreenEx(*position, *camera, widthPtr, heightPtr);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetWorldToScreen2D(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Camera2D* camera = cart_get_pointer(cameraPtr, sizeof(Camera2D));
    Vector2 out = GetWorldToScreen2D(*position, *camera);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetScreenToWorld2D(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Camera2D* camera = cart_get_pointer(cameraPtr, sizeof(Camera2D));
    Vector2 out = GetScreenToWorld2D(*position, *camera);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetCameraMatrix(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int cameraPtr) {
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Matrix out = GetCameraMatrix(*camera);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetCameraMatrix2D(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int cameraPtr) {
    Camera2D* camera = cart_get_pointer(cameraPtr, sizeof(Camera2D));
    Matrix out = GetCameraMatrix2D(*camera);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_SetTargetFPS(wasm_exec_env_t exec_env, unsigned int fpsPtr) {
    SetTargetFPS(fpsPtr);
}

unsigned int host_GetFrameTime(wasm_exec_env_t exec_env) {
    return GetFrameTime();
}

unsigned int host_GetTime(wasm_exec_env_t exec_env) {
    return GetTime();
}

unsigned int host_GetFPS(wasm_exec_env_t exec_env) {
    return GetFPS();
}

void host_SwapScreenBuffer(wasm_exec_env_t exec_env) {
    SwapScreenBuffer();
}

void host_PollInputEvents(wasm_exec_env_t exec_env) {
    PollInputEvents();
}

void host_WaitTime(wasm_exec_env_t exec_env, unsigned int secondsPtr) {
    WaitTime(secondsPtr);
}

void host_SetRandomSeed(wasm_exec_env_t exec_env, unsigned int seedPtr) {
    SetRandomSeed(seedPtr);
}

unsigned int host_GetRandomValue(wasm_exec_env_t exec_env, unsigned int minPtr, unsigned int maxPtr) {
    return GetRandomValue(minPtr, maxPtr);
}

void host_LoadRandomSequence(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int countPtr, unsigned int minPtr, unsigned int maxPtr) {
    int * out = LoadRandomSequence(countPtr, minPtr, maxPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadRandomSequence(wasm_exec_env_t exec_env, unsigned int sequencePtr) {
    int ** sequence = cart_get_pointer(sequencePtr, sizeof(int *));
    UnloadRandomSequence(sequence);
    free((void*)sequence);
}

void host_TakeScreenshot(wasm_exec_env_t exec_env, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    TakeScreenshot(fileName);
    free((void*)fileName);
}

void host_SetConfigFlags(wasm_exec_env_t exec_env, unsigned int flagsPtr) {
    SetConfigFlags(flagsPtr);
}

void host_OpenURL(wasm_exec_env_t exec_env, unsigned int urlPtr) {
    const char* url = cart_get_string(urlPtr);
    OpenURL(url);
    free((void*)url);
}

void host_SetTraceLogLevel(wasm_exec_env_t exec_env, unsigned int logLevelPtr) {
    SetTraceLogLevel(logLevelPtr);
}

void host_MemAlloc(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int sizePtr) {
    void * out = MemAlloc(sizePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_MemRealloc(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int ptrPtr, unsigned int sizePtr) {
    void ** ptr = cart_get_pointer(ptrPtr, sizeof(void *));
    void * out = MemRealloc(ptr, sizePtr);
    free((void*)ptr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_MemFree(wasm_exec_env_t exec_env, unsigned int ptrPtr) {
    void ** ptr = cart_get_pointer(ptrPtr, sizeof(void *));
    MemFree(ptr);
    free((void*)ptr);
}

void host_SetTraceLogCallback(wasm_exec_env_t exec_env, unsigned int callbackPtr) {
    TraceLogCallback* callback = cart_get_pointer(callbackPtr, sizeof(TraceLogCallback));
    SetTraceLogCallback(*callback);
    free((void*)callback);
}

void host_SetLoadFileDataCallback(wasm_exec_env_t exec_env, unsigned int callbackPtr) {
    LoadFileDataCallback* callback = cart_get_pointer(callbackPtr, sizeof(LoadFileDataCallback));
    SetLoadFileDataCallback(*callback);
    free((void*)callback);
}

void host_SetSaveFileDataCallback(wasm_exec_env_t exec_env, unsigned int callbackPtr) {
    SaveFileDataCallback* callback = cart_get_pointer(callbackPtr, sizeof(SaveFileDataCallback));
    SetSaveFileDataCallback(*callback);
    free((void*)callback);
}

void host_SetLoadFileTextCallback(wasm_exec_env_t exec_env, unsigned int callbackPtr) {
    LoadFileTextCallback* callback = cart_get_pointer(callbackPtr, sizeof(LoadFileTextCallback));
    SetLoadFileTextCallback(*callback);
    free((void*)callback);
}

void host_SetSaveFileTextCallback(wasm_exec_env_t exec_env, unsigned int callbackPtr) {
    SaveFileTextCallback* callback = cart_get_pointer(callbackPtr, sizeof(SaveFileTextCallback));
    SetSaveFileTextCallback(*callback);
    free((void*)callback);
}

void host_LoadFileData(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr, unsigned int dataSizePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** dataSize = cart_get_pointer(dataSizePtr, sizeof(int *));
    unsigned char * out = LoadFileData(fileName, dataSize);
    free((void*)fileName);
    free((void*)dataSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadFileData(wasm_exec_env_t exec_env, unsigned int dataPtr) {
    unsigned char ** data = cart_get_pointer(dataPtr, sizeof(unsigned char *));
    UnloadFileData(data);
    free((void*)data);
}

unsigned int host_SaveFileData(wasm_exec_env_t exec_env, unsigned int fileNamePtr, unsigned int dataPtr, unsigned int dataSizePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    void ** data = cart_get_pointer(dataPtr, sizeof(void *));
    return SaveFileData(fileName, data, dataSizePtr);
    free((void*)fileName);
    free((void*)data);
}

unsigned int host_ExportDataAsCode(wasm_exec_env_t exec_env, unsigned int dataPtr, unsigned int dataSizePtr, unsigned int fileNamePtr) {
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportDataAsCode(data, dataSizePtr, fileName);
    free((void*)data);
    free((void*)fileName);
}

void host_LoadFileText(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    char * out = LoadFileText(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadFileText(wasm_exec_env_t exec_env, unsigned int textPtr) {
    char ** text = cart_get_pointer(textPtr, sizeof(char *));
    UnloadFileText(text);
    free((void*)text);
}

unsigned int host_SaveFileText(wasm_exec_env_t exec_env, unsigned int fileNamePtr, unsigned int textPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    char ** text = cart_get_pointer(textPtr, sizeof(char *));
    return SaveFileText(fileName, text);
    free((void*)fileName);
    free((void*)text);
}

unsigned int host_FileExists(wasm_exec_env_t exec_env, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    return FileExistsFS(fileName);
    free((void*)fileName);
}

unsigned int host_DirectoryExists(wasm_exec_env_t exec_env, unsigned int dirPathPtr) {
    const char* dirPath = cart_get_string(dirPathPtr);
    return DirectoryExistsFS(dirPath);
    free((void*)dirPath);
}

unsigned int host_IsFileExtension(wasm_exec_env_t exec_env, unsigned int fileNamePtr, unsigned int extPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    const char* ext = cart_get_string(extPtr);
    return IsFileExtension(fileName, ext);
    free((void*)fileName);
    free((void*)ext);
}

unsigned int host_GetFileLength(wasm_exec_env_t exec_env, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    return GetFileLengthFS(fileName);
    free((void*)fileName);
}

void host_GetFileExtension(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    const char * out = GetFileExtension(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetFileName(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int filePathPtr) {
    const char* filePath = cart_get_string(filePathPtr);
    const char * out = GetFileName(filePath);
    free((void*)filePath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetFileNameWithoutExt(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int filePathPtr) {
    const char* filePath = cart_get_string(filePathPtr);
    const char * out = GetFileNameWithoutExt(filePath);
    free((void*)filePath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetDirectoryPath(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int filePathPtr) {
    const char* filePath = cart_get_string(filePathPtr);
    const char * out = GetDirectoryPath(filePath);
    free((void*)filePath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetPrevDirectoryPath(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int dirPathPtr) {
    const char* dirPath = cart_get_string(dirPathPtr);
    const char * out = GetPrevDirectoryPath(dirPath);
    free((void*)dirPath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetWorkingDirectory(wasm_exec_env_t exec_env, unsigned int outPtr) {
    const char * out = GetWorkingDirectory();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetApplicationDirectory(wasm_exec_env_t exec_env, unsigned int outPtr) {
    const char * out = GetApplicationDirectory();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_MakeDirectory(wasm_exec_env_t exec_env, unsigned int dirPathPtr) {
    const char* dirPath = cart_get_string(dirPathPtr);
    return MakeDirectory(dirPath);
    free((void*)dirPath);
}

unsigned int host_ChangeDirectory(wasm_exec_env_t exec_env, unsigned int dirPtr) {
    const char* dir = cart_get_string(dirPtr);
    return ChangeDirectoryFS(dir);
    free((void*)dir);
}

unsigned int host_IsPathFile(wasm_exec_env_t exec_env, unsigned int pathPtr) {
    const char* path = cart_get_string(pathPtr);
    return IsPathFileFS(path);
    free((void*)path);
}

unsigned int host_IsFileNameValid(wasm_exec_env_t exec_env, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    return IsFileNameValid(fileName);
    free((void*)fileName);
}

void host_LoadDirectoryFiles(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int dirPathPtr) {
    const char* dirPath = cart_get_string(dirPathPtr);
    FilePathList out = LoadDirectoryFilesFS(dirPath);
    free((void*)dirPath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadDirectoryFilesEx(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int basePathPtr, unsigned int filterPtr, unsigned int scanSubdirsPtr) {
    const char* basePath = cart_get_string(basePathPtr);
    const char* filter = cart_get_string(filterPtr);
    FilePathList out = LoadDirectoryFilesExFS(basePath, filter, scanSubdirsPtr);
    free((void*)basePath);
    free((void*)filter);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadDirectoryFiles(wasm_exec_env_t exec_env, unsigned int filesPtr) {
    FilePathList* files = cart_get_pointer(filesPtr, sizeof(FilePathList));
    UnloadDirectoryFiles(*files);
    free((void*)files);
}

unsigned int host_IsFileDropped(wasm_exec_env_t exec_env) {
    return IsFileDropped();
}

void host_LoadDroppedFiles(wasm_exec_env_t exec_env, unsigned int outPtr) {
    FilePathList out = LoadDroppedFiles();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadDroppedFiles(wasm_exec_env_t exec_env, unsigned int filesPtr) {
    FilePathList* files = cart_get_pointer(filesPtr, sizeof(FilePathList));
    UnloadDroppedFiles(*files);
    free((void*)files);
}

unsigned int host_GetFileModTime(wasm_exec_env_t exec_env, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    return GetFileModTimeFS(fileName);
    free((void*)fileName);
}

void host_CompressData(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int dataPtr, unsigned int dataSizePtr, unsigned int compDataSizePtr) {
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    int ** compDataSize = cart_get_pointer(compDataSizePtr, sizeof(int *));
    unsigned char * out = CompressData(data, dataSizePtr, compDataSize);
    free((void*)data);
    free((void*)compDataSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_DecompressData(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int compDataPtr, unsigned int compDataSizePtr, unsigned int dataSizePtr) {
    const unsigned char ** compData = cart_get_pointer(compDataPtr, sizeof(const unsigned char *));
    int ** dataSize = cart_get_pointer(dataSizePtr, sizeof(int *));
    unsigned char * out = DecompressData(compData, compDataSizePtr, dataSize);
    free((void*)compData);
    free((void*)dataSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_EncodeDataBase64(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int dataPtr, unsigned int dataSizePtr, unsigned int outputSizePtr) {
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    int ** outputSize = cart_get_pointer(outputSizePtr, sizeof(int *));
    char * out = EncodeDataBase64(data, dataSizePtr, outputSize);
    free((void*)data);
    free((void*)outputSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_DecodeDataBase64(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int dataPtr, unsigned int outputSizePtr) {
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    int ** outputSize = cart_get_pointer(outputSizePtr, sizeof(int *));
    unsigned char * out = DecodeDataBase64(data, outputSize);
    free((void*)data);
    free((void*)outputSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_ComputeCRC32(wasm_exec_env_t exec_env, unsigned int dataPtr, unsigned int dataSizePtr) {
    unsigned char ** data = cart_get_pointer(dataPtr, sizeof(unsigned char *));
    return ComputeCRC32(data, dataSizePtr);
    free((void*)data);
}

void host_ComputeMD5(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int dataPtr, unsigned int dataSizePtr) {
    unsigned char ** data = cart_get_pointer(dataPtr, sizeof(unsigned char *));
    unsigned int * out = ComputeMD5(data, dataSizePtr);
    free((void*)data);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ComputeSHA1(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int dataPtr, unsigned int dataSizePtr) {
    unsigned char ** data = cart_get_pointer(dataPtr, sizeof(unsigned char *));
    unsigned int * out = ComputeSHA1(data, dataSizePtr);
    free((void*)data);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadAutomationEventList(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    AutomationEventList out = LoadAutomationEventList(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadAutomationEventList(wasm_exec_env_t exec_env, unsigned int listPtr) {
    AutomationEventList* list = cart_get_pointer(listPtr, sizeof(AutomationEventList));
    UnloadAutomationEventList(*list);
    free((void*)list);
}

unsigned int host_ExportAutomationEventList(wasm_exec_env_t exec_env, unsigned int listPtr, unsigned int fileNamePtr) {
    AutomationEventList* list = cart_get_pointer(listPtr, sizeof(AutomationEventList));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportAutomationEventList(*list, fileName);
    free((void*)list);
    free((void*)fileName);
}

void host_SetAutomationEventList(wasm_exec_env_t exec_env, unsigned int listPtr) {
    AutomationEventList ** list = cart_get_pointer(listPtr, sizeof(AutomationEventList *));
    SetAutomationEventList(list);
    free((void*)list);
}

void host_SetAutomationEventBaseFrame(wasm_exec_env_t exec_env, unsigned int framePtr) {
    SetAutomationEventBaseFrame(framePtr);
}

void host_StartAutomationEventRecording(wasm_exec_env_t exec_env) {
    StartAutomationEventRecording();
}

void host_StopAutomationEventRecording(wasm_exec_env_t exec_env) {
    StopAutomationEventRecording();
}

void host_PlayAutomationEvent(wasm_exec_env_t exec_env, unsigned int eventPtr) {
    AutomationEvent* event = cart_get_pointer(eventPtr, sizeof(AutomationEvent));
    PlayAutomationEvent(*event);
    free((void*)event);
}

unsigned int host_IsKeyPressed(wasm_exec_env_t exec_env, unsigned int keyPtr) {
    return IsKeyPressed(keyPtr);
}

unsigned int host_IsKeyPressedRepeat(wasm_exec_env_t exec_env, unsigned int keyPtr) {
    return IsKeyPressedRepeat(keyPtr);
}

unsigned int host_IsKeyDown(wasm_exec_env_t exec_env, unsigned int keyPtr) {
    return IsKeyDown(keyPtr);
}

unsigned int host_IsKeyReleased(wasm_exec_env_t exec_env, unsigned int keyPtr) {
    return IsKeyReleased(keyPtr);
}

unsigned int host_IsKeyUp(wasm_exec_env_t exec_env, unsigned int keyPtr) {
    return IsKeyUp(keyPtr);
}

unsigned int host_GetKeyPressed(wasm_exec_env_t exec_env) {
    return GetKeyPressed();
}

unsigned int host_GetCharPressed(wasm_exec_env_t exec_env) {
    return GetCharPressed();
}

void host_SetExitKey(wasm_exec_env_t exec_env, unsigned int keyPtr) {
    SetExitKey(keyPtr);
}

unsigned int host_IsGamepadAvailable(wasm_exec_env_t exec_env, unsigned int gamepadPtr) {
    return IsGamepadAvailable(gamepadPtr);
}

void host_GetGamepadName(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int gamepadPtr) {
    const char * out = GetGamepadName(gamepadPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsGamepadButtonPressed(wasm_exec_env_t exec_env, unsigned int gamepadPtr, unsigned int buttonPtr) {
    return IsGamepadButtonPressed(gamepadPtr, buttonPtr);
}

unsigned int host_IsGamepadButtonDown(wasm_exec_env_t exec_env, unsigned int gamepadPtr, unsigned int buttonPtr) {
    return IsGamepadButtonDown(gamepadPtr, buttonPtr);
}

unsigned int host_IsGamepadButtonReleased(wasm_exec_env_t exec_env, unsigned int gamepadPtr, unsigned int buttonPtr) {
    return IsGamepadButtonReleased(gamepadPtr, buttonPtr);
}

unsigned int host_IsGamepadButtonUp(wasm_exec_env_t exec_env, unsigned int gamepadPtr, unsigned int buttonPtr) {
    return IsGamepadButtonUp(gamepadPtr, buttonPtr);
}

unsigned int host_GetGamepadButtonPressed(wasm_exec_env_t exec_env) {
    return GetGamepadButtonPressed();
}

unsigned int host_GetGamepadAxisCount(wasm_exec_env_t exec_env, unsigned int gamepadPtr) {
    return GetGamepadAxisCount(gamepadPtr);
}

unsigned int host_GetGamepadAxisMovement(wasm_exec_env_t exec_env, unsigned int gamepadPtr, unsigned int axisPtr) {
    return GetGamepadAxisMovement(gamepadPtr, axisPtr);
}

unsigned int host_SetGamepadMappings(wasm_exec_env_t exec_env, unsigned int mappingsPtr) {
    const char* mappings = cart_get_string(mappingsPtr);
    return SetGamepadMappings(mappings);
    free((void*)mappings);
}

void host_SetGamepadVibration(wasm_exec_env_t exec_env, unsigned int gamepadPtr, unsigned int leftMotorPtr, unsigned int rightMotorPtr, unsigned int durationPtr) {
    SetGamepadVibration(gamepadPtr, leftMotorPtr, rightMotorPtr, durationPtr);
}

unsigned int host_IsMouseButtonPressed(wasm_exec_env_t exec_env, unsigned int buttonPtr) {
    return IsMouseButtonPressed(buttonPtr);
}

unsigned int host_IsMouseButtonDown(wasm_exec_env_t exec_env, unsigned int buttonPtr) {
    return IsMouseButtonDown(buttonPtr);
}

unsigned int host_IsMouseButtonReleased(wasm_exec_env_t exec_env, unsigned int buttonPtr) {
    return IsMouseButtonReleased(buttonPtr);
}

unsigned int host_IsMouseButtonUp(wasm_exec_env_t exec_env, unsigned int buttonPtr) {
    return IsMouseButtonUp(buttonPtr);
}

unsigned int host_GetMouseX(wasm_exec_env_t exec_env) {
    return GetMouseX();
}

unsigned int host_GetMouseY(wasm_exec_env_t exec_env) {
    return GetMouseY();
}

void host_GetMousePosition(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Vector2 out = GetMousePosition();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetMouseDelta(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Vector2 out = GetMouseDelta();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_SetMousePosition(wasm_exec_env_t exec_env, unsigned int xPtr, unsigned int yPtr) {
    SetMousePosition(xPtr, yPtr);
}

void host_SetMouseOffset(wasm_exec_env_t exec_env, unsigned int offsetXPtr, unsigned int offsetYPtr) {
    SetMouseOffset(offsetXPtr, offsetYPtr);
}

void host_SetMouseScale(wasm_exec_env_t exec_env, unsigned int scaleXPtr, unsigned int scaleYPtr) {
    SetMouseScale(scaleXPtr, scaleYPtr);
}

unsigned int host_GetMouseWheelMove(wasm_exec_env_t exec_env) {
    return GetMouseWheelMove();
}

void host_GetMouseWheelMoveV(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Vector2 out = GetMouseWheelMoveV();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_SetMouseCursor(wasm_exec_env_t exec_env, unsigned int cursorPtr) {
    SetMouseCursor(cursorPtr);
}

unsigned int host_GetTouchX(wasm_exec_env_t exec_env) {
    return GetTouchX();
}

unsigned int host_GetTouchY(wasm_exec_env_t exec_env) {
    return GetTouchY();
}

void host_GetTouchPosition(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int indexPtr) {
    Vector2 out = GetTouchPosition(indexPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_GetTouchPointId(wasm_exec_env_t exec_env, unsigned int indexPtr) {
    return GetTouchPointId(indexPtr);
}

unsigned int host_GetTouchPointCount(wasm_exec_env_t exec_env) {
    return GetTouchPointCount();
}

void host_SetGesturesEnabled(wasm_exec_env_t exec_env, unsigned int flagsPtr) {
    SetGesturesEnabled(flagsPtr);
}

unsigned int host_IsGestureDetected(wasm_exec_env_t exec_env, unsigned int gesturePtr) {
    return IsGestureDetected(gesturePtr);
}

unsigned int host_GetGestureDetected(wasm_exec_env_t exec_env) {
    return GetGestureDetected();
}

unsigned int host_GetGestureHoldDuration(wasm_exec_env_t exec_env) {
    return GetGestureHoldDuration();
}

void host_GetGestureDragVector(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Vector2 out = GetGestureDragVector();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_GetGestureDragAngle(wasm_exec_env_t exec_env) {
    return GetGestureDragAngle();
}

void host_GetGesturePinchVector(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Vector2 out = GetGesturePinchVector();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_GetGesturePinchAngle(wasm_exec_env_t exec_env) {
    return GetGesturePinchAngle();
}

void host_UpdateCamera(wasm_exec_env_t exec_env, unsigned int cameraPtr, unsigned int modePtr) {
    Camera ** camera = cart_get_pointer(cameraPtr, sizeof(Camera *));
    UpdateCamera(camera, modePtr);
    free((void*)camera);
}

void host_UpdateCameraPro(wasm_exec_env_t exec_env, unsigned int cameraPtr, unsigned int movementPtr, unsigned int rotationPtr, unsigned int zoomPtr) {
    Camera ** camera = cart_get_pointer(cameraPtr, sizeof(Camera *));
    Vector3* movement = cart_get_pointer(movementPtr, sizeof(Vector3));
    Vector3* rotation = cart_get_pointer(rotationPtr, sizeof(Vector3));
    UpdateCameraPro(camera, *movement, *rotation, zoomPtr);
    free((void*)camera);
    free((void*)movement);
    free((void*)rotation);
}

void host_SetShapesTexture(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int sourcePtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Rectangle* source = cart_get_pointer(sourcePtr, sizeof(Rectangle));
    SetShapesTexture(*texture, *source);
    free((void*)texture);
    free((void*)source);
}

void host_GetShapesTexture(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Texture2D out = GetShapesTexture();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetShapesTextureRectangle(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Rectangle out = GetShapesTextureRectangle();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_DrawPixel(wasm_exec_env_t exec_env, unsigned int posXPtr, unsigned int posYPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPixel(posXPtr, posYPtr, *color);
    free((void*)color);
}

void host_DrawPixelV(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int colorPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPixelV(*position, *color);
    free((void*)position);
    free((void*)color);
}

void host_DrawLine(wasm_exec_env_t exec_env, unsigned int startPosXPtr, unsigned int startPosYPtr, unsigned int endPosXPtr, unsigned int endPosYPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLine(startPosXPtr, startPosYPtr, endPosXPtr, endPosYPtr, *color);
    free((void*)color);
}

void host_DrawLineV(wasm_exec_env_t exec_env, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int colorPtr) {
    Vector2* startPos = cart_get_pointer(startPosPtr, sizeof(Vector2));
    Vector2* endPos = cart_get_pointer(endPosPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLineV(*startPos, *endPos, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

void host_DrawLineEx(wasm_exec_env_t exec_env, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* startPos = cart_get_pointer(startPosPtr, sizeof(Vector2));
    Vector2* endPos = cart_get_pointer(endPosPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLineEx(*startPos, *endPos, thickPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

void host_DrawLineStrip(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLineStrip(points, pointCountPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawLineBezier(wasm_exec_env_t exec_env, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* startPos = cart_get_pointer(startPosPtr, sizeof(Vector2));
    Vector2* endPos = cart_get_pointer(endPosPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLineBezier(*startPos, *endPos, thickPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

void host_DrawCircle(wasm_exec_env_t exec_env, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircle(centerXPtr, centerYPtr, radiusPtr, *color);
    free((void*)color);
}

void host_DrawCircleSector(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int radiusPtr, unsigned int startAnglePtr, unsigned int endAnglePtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleSector(*center, radiusPtr, startAnglePtr, endAnglePtr, segmentsPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawCircleSectorLines(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int radiusPtr, unsigned int startAnglePtr, unsigned int endAnglePtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleSectorLines(*center, radiusPtr, startAnglePtr, endAnglePtr, segmentsPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawCircleGradient(wasm_exec_env_t exec_env, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int innerPtr, unsigned int outerPtr) {
    Color* inner = cart_get_pointer(innerPtr, sizeof(Color));
    Color* outer = cart_get_pointer(outerPtr, sizeof(Color));
    DrawCircleGradient(centerXPtr, centerYPtr, radiusPtr, *inner, *outer);
    free((void*)inner);
    free((void*)outer);
}

void host_DrawCircleV(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleV(*center, radiusPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawCircleLines(wasm_exec_env_t exec_env, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleLines(centerXPtr, centerYPtr, radiusPtr, *color);
    free((void*)color);
}

void host_DrawCircleLinesV(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleLinesV(*center, radiusPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawEllipse(wasm_exec_env_t exec_env, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusHPtr, unsigned int radiusVPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawEllipse(centerXPtr, centerYPtr, radiusHPtr, radiusVPtr, *color);
    free((void*)color);
}

void host_DrawEllipseLines(wasm_exec_env_t exec_env, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusHPtr, unsigned int radiusVPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawEllipseLines(centerXPtr, centerYPtr, radiusHPtr, radiusVPtr, *color);
    free((void*)color);
}

void host_DrawRing(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int innerRadiusPtr, unsigned int outerRadiusPtr, unsigned int startAnglePtr, unsigned int endAnglePtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRing(*center, innerRadiusPtr, outerRadiusPtr, startAnglePtr, endAnglePtr, segmentsPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawRingLines(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int innerRadiusPtr, unsigned int outerRadiusPtr, unsigned int startAnglePtr, unsigned int endAnglePtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRingLines(*center, innerRadiusPtr, outerRadiusPtr, startAnglePtr, endAnglePtr, segmentsPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawRectangle(wasm_exec_env_t exec_env, unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangle(posXPtr, posYPtr, widthPtr, heightPtr, *color);
    free((void*)color);
}

void host_DrawRectangleV(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Vector2* size = cart_get_pointer(sizePtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleV(*position, *size, *color);
    free((void*)position);
    free((void*)size);
    free((void*)color);
}

void host_DrawRectangleRec(wasm_exec_env_t exec_env, unsigned int recPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleRec(*rec, *color);
    free((void*)rec);
    free((void*)color);
}

void host_DrawRectanglePro(wasm_exec_env_t exec_env, unsigned int recPtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Vector2* origin = cart_get_pointer(originPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectanglePro(*rec, *origin, rotationPtr, *color);
    free((void*)rec);
    free((void*)origin);
    free((void*)color);
}

void host_DrawRectangleGradientV(wasm_exec_env_t exec_env, unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int topPtr, unsigned int bottomPtr) {
    Color* top = cart_get_pointer(topPtr, sizeof(Color));
    Color* bottom = cart_get_pointer(bottomPtr, sizeof(Color));
    DrawRectangleGradientV(posXPtr, posYPtr, widthPtr, heightPtr, *top, *bottom);
    free((void*)top);
    free((void*)bottom);
}

void host_DrawRectangleGradientH(wasm_exec_env_t exec_env, unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int leftPtr, unsigned int rightPtr) {
    Color* left = cart_get_pointer(leftPtr, sizeof(Color));
    Color* right = cart_get_pointer(rightPtr, sizeof(Color));
    DrawRectangleGradientH(posXPtr, posYPtr, widthPtr, heightPtr, *left, *right);
    free((void*)left);
    free((void*)right);
}

void host_DrawRectangleGradientEx(wasm_exec_env_t exec_env, unsigned int recPtr, unsigned int topLeftPtr, unsigned int bottomLeftPtr, unsigned int topRightPtr, unsigned int bottomRightPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* topLeft = cart_get_pointer(topLeftPtr, sizeof(Color));
    Color* bottomLeft = cart_get_pointer(bottomLeftPtr, sizeof(Color));
    Color* topRight = cart_get_pointer(topRightPtr, sizeof(Color));
    Color* bottomRight = cart_get_pointer(bottomRightPtr, sizeof(Color));
    DrawRectangleGradientEx(*rec, *topLeft, *bottomLeft, *topRight, *bottomRight);
    free((void*)rec);
    free((void*)topLeft);
    free((void*)bottomLeft);
    free((void*)topRight);
    free((void*)bottomRight);
}

void host_DrawRectangleLines(wasm_exec_env_t exec_env, unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleLines(posXPtr, posYPtr, widthPtr, heightPtr, *color);
    free((void*)color);
}

void host_DrawRectangleLinesEx(wasm_exec_env_t exec_env, unsigned int recPtr, unsigned int lineThickPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleLinesEx(*rec, lineThickPtr, *color);
    free((void*)rec);
    free((void*)color);
}

void host_DrawRectangleRounded(wasm_exec_env_t exec_env, unsigned int recPtr, unsigned int roundnessPtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleRounded(*rec, roundnessPtr, segmentsPtr, *color);
    free((void*)rec);
    free((void*)color);
}

void host_DrawRectangleRoundedLines(wasm_exec_env_t exec_env, unsigned int recPtr, unsigned int roundnessPtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleRoundedLines(*rec, roundnessPtr, segmentsPtr, *color);
    free((void*)rec);
    free((void*)color);
}

void host_DrawRectangleRoundedLinesEx(wasm_exec_env_t exec_env, unsigned int recPtr, unsigned int roundnessPtr, unsigned int segmentsPtr, unsigned int lineThickPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleRoundedLinesEx(*rec, roundnessPtr, segmentsPtr, lineThickPtr, *color);
    free((void*)rec);
    free((void*)color);
}

void host_DrawTriangle(wasm_exec_env_t exec_env, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
    Vector2* v1 = cart_get_pointer(v1Ptr, sizeof(Vector2));
    Vector2* v2 = cart_get_pointer(v2Ptr, sizeof(Vector2));
    Vector2* v3 = cart_get_pointer(v3Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangle(*v1, *v2, *v3, *color);
    free((void*)v1);
    free((void*)v2);
    free((void*)v3);
    free((void*)color);
}

void host_DrawTriangleLines(wasm_exec_env_t exec_env, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
    Vector2* v1 = cart_get_pointer(v1Ptr, sizeof(Vector2));
    Vector2* v2 = cart_get_pointer(v2Ptr, sizeof(Vector2));
    Vector2* v3 = cart_get_pointer(v3Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangleLines(*v1, *v2, *v3, *color);
    free((void*)v1);
    free((void*)v2);
    free((void*)v3);
    free((void*)color);
}

void host_DrawTriangleFan(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangleFan(points, pointCountPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawTriangleStrip(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangleStrip(points, pointCountPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawPoly(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int sidesPtr, unsigned int radiusPtr, unsigned int rotationPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPoly(*center, sidesPtr, radiusPtr, rotationPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawPolyLines(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int sidesPtr, unsigned int radiusPtr, unsigned int rotationPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPolyLines(*center, sidesPtr, radiusPtr, rotationPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawPolyLinesEx(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int sidesPtr, unsigned int radiusPtr, unsigned int rotationPtr, unsigned int lineThickPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPolyLinesEx(*center, sidesPtr, radiusPtr, rotationPtr, lineThickPtr, *color);
    free((void*)center);
    free((void*)color);
}

void host_DrawSplineLinear(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineLinear(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawSplineBasis(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineBasis(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawSplineCatmullRom(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineCatmullRom(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawSplineBezierQuadratic(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineBezierQuadratic(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawSplineBezierCubic(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineBezierCubic(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawSplineSegmentLinear(wasm_exec_env_t exec_env, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineSegmentLinear(*p1, *p2, thickPtr, *color);
    free((void*)p1);
    free((void*)p2);
    free((void*)color);
}

void host_DrawSplineSegmentBasis(wasm_exec_env_t exec_env, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    Vector2* p3 = cart_get_pointer(p3Ptr, sizeof(Vector2));
    Vector2* p4 = cart_get_pointer(p4Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineSegmentBasis(*p1, *p2, *p3, *p4, thickPtr, *color);
    free((void*)p1);
    free((void*)p2);
    free((void*)p3);
    free((void*)p4);
    free((void*)color);
}

void host_DrawSplineSegmentCatmullRom(wasm_exec_env_t exec_env, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    Vector2* p3 = cart_get_pointer(p3Ptr, sizeof(Vector2));
    Vector2* p4 = cart_get_pointer(p4Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineSegmentCatmullRom(*p1, *p2, *p3, *p4, thickPtr, *color);
    free((void*)p1);
    free((void*)p2);
    free((void*)p3);
    free((void*)p4);
    free((void*)color);
}

void host_DrawSplineSegmentBezierQuadratic(wasm_exec_env_t exec_env, unsigned int p1Ptr, unsigned int c2Ptr, unsigned int p3Ptr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* c2 = cart_get_pointer(c2Ptr, sizeof(Vector2));
    Vector2* p3 = cart_get_pointer(p3Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineSegmentBezierQuadratic(*p1, *c2, *p3, thickPtr, *color);
    free((void*)p1);
    free((void*)c2);
    free((void*)p3);
    free((void*)color);
}

void host_DrawSplineSegmentBezierCubic(wasm_exec_env_t exec_env, unsigned int p1Ptr, unsigned int c2Ptr, unsigned int c3Ptr, unsigned int p4Ptr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* c2 = cart_get_pointer(c2Ptr, sizeof(Vector2));
    Vector2* c3 = cart_get_pointer(c3Ptr, sizeof(Vector2));
    Vector2* p4 = cart_get_pointer(p4Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineSegmentBezierCubic(*p1, *c2, *c3, *p4, thickPtr, *color);
    free((void*)p1);
    free((void*)c2);
    free((void*)c3);
    free((void*)p4);
    free((void*)color);
}

void host_GetSplinePointLinear(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int tPtr) {
    Vector2* startPos = cart_get_pointer(startPosPtr, sizeof(Vector2));
    Vector2* endPos = cart_get_pointer(endPosPtr, sizeof(Vector2));
    Vector2 out = GetSplinePointLinear(*startPos, *endPos, tPtr);
    free((void*)startPos);
    free((void*)endPos);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetSplinePointBasis(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr, unsigned int tPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    Vector2* p3 = cart_get_pointer(p3Ptr, sizeof(Vector2));
    Vector2* p4 = cart_get_pointer(p4Ptr, sizeof(Vector2));
    Vector2 out = GetSplinePointBasis(*p1, *p2, *p3, *p4, tPtr);
    free((void*)p1);
    free((void*)p2);
    free((void*)p3);
    free((void*)p4);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetSplinePointCatmullRom(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr, unsigned int tPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    Vector2* p3 = cart_get_pointer(p3Ptr, sizeof(Vector2));
    Vector2* p4 = cart_get_pointer(p4Ptr, sizeof(Vector2));
    Vector2 out = GetSplinePointCatmullRom(*p1, *p2, *p3, *p4, tPtr);
    free((void*)p1);
    free((void*)p2);
    free((void*)p3);
    free((void*)p4);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetSplinePointBezierQuad(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int p1Ptr, unsigned int c2Ptr, unsigned int p3Ptr, unsigned int tPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* c2 = cart_get_pointer(c2Ptr, sizeof(Vector2));
    Vector2* p3 = cart_get_pointer(p3Ptr, sizeof(Vector2));
    Vector2 out = GetSplinePointBezierQuad(*p1, *c2, *p3, tPtr);
    free((void*)p1);
    free((void*)c2);
    free((void*)p3);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetSplinePointBezierCubic(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int p1Ptr, unsigned int c2Ptr, unsigned int c3Ptr, unsigned int p4Ptr, unsigned int tPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* c2 = cart_get_pointer(c2Ptr, sizeof(Vector2));
    Vector2* c3 = cart_get_pointer(c3Ptr, sizeof(Vector2));
    Vector2* p4 = cart_get_pointer(p4Ptr, sizeof(Vector2));
    Vector2 out = GetSplinePointBezierCubic(*p1, *c2, *c3, *p4, tPtr);
    free((void*)p1);
    free((void*)c2);
    free((void*)c3);
    free((void*)p4);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_CheckCollisionRecs(wasm_exec_env_t exec_env, unsigned int rec1Ptr, unsigned int rec2Ptr) {
    Rectangle* rec1 = cart_get_pointer(rec1Ptr, sizeof(Rectangle));
    Rectangle* rec2 = cart_get_pointer(rec2Ptr, sizeof(Rectangle));
    return CheckCollisionRecs(*rec1, *rec2);
    free((void*)rec1);
    free((void*)rec2);
}

unsigned int host_CheckCollisionCircles(wasm_exec_env_t exec_env, unsigned int center1Ptr, unsigned int radius1Ptr, unsigned int center2Ptr, unsigned int radius2Ptr) {
    Vector2* center1 = cart_get_pointer(center1Ptr, sizeof(Vector2));
    Vector2* center2 = cart_get_pointer(center2Ptr, sizeof(Vector2));
    return CheckCollisionCircles(*center1, radius1Ptr, *center2, radius2Ptr);
    free((void*)center1);
    free((void*)center2);
}

unsigned int host_CheckCollisionCircleRec(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int radiusPtr, unsigned int recPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    return CheckCollisionCircleRec(*center, radiusPtr, *rec);
    free((void*)center);
    free((void*)rec);
}

unsigned int host_CheckCollisionCircleLine(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int radiusPtr, unsigned int p1Ptr, unsigned int p2Ptr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    return CheckCollisionCircleLine(*center, radiusPtr, *p1, *p2);
    free((void*)center);
    free((void*)p1);
    free((void*)p2);
}

unsigned int host_CheckCollisionPointRec(wasm_exec_env_t exec_env, unsigned int pointPtr, unsigned int recPtr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    return CheckCollisionPointRec(*point, *rec);
    free((void*)point);
    free((void*)rec);
}

unsigned int host_CheckCollisionPointCircle(wasm_exec_env_t exec_env, unsigned int pointPtr, unsigned int centerPtr, unsigned int radiusPtr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    return CheckCollisionPointCircle(*point, *center, radiusPtr);
    free((void*)point);
    free((void*)center);
}

unsigned int host_CheckCollisionPointTriangle(wasm_exec_env_t exec_env, unsigned int pointPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    Vector2* p3 = cart_get_pointer(p3Ptr, sizeof(Vector2));
    return CheckCollisionPointTriangle(*point, *p1, *p2, *p3);
    free((void*)point);
    free((void*)p1);
    free((void*)p2);
    free((void*)p3);
}

unsigned int host_CheckCollisionPointLine(wasm_exec_env_t exec_env, unsigned int pointPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int thresholdPtr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    return CheckCollisionPointLine(*point, *p1, *p2, thresholdPtr);
    free((void*)point);
    free((void*)p1);
    free((void*)p2);
}

unsigned int host_CheckCollisionPointPoly(wasm_exec_env_t exec_env, unsigned int pointPtr, unsigned int pointsPtr, unsigned int pointCountPtr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    return CheckCollisionPointPoly(*point, points, pointCountPtr);
    free((void*)point);
    free((void*)points);
}

unsigned int host_CheckCollisionLines(wasm_exec_env_t exec_env, unsigned int startPos1Ptr, unsigned int endPos1Ptr, unsigned int startPos2Ptr, unsigned int endPos2Ptr, unsigned int collisionPointPtr) {
    Vector2* startPos1 = cart_get_pointer(startPos1Ptr, sizeof(Vector2));
    Vector2* endPos1 = cart_get_pointer(endPos1Ptr, sizeof(Vector2));
    Vector2* startPos2 = cart_get_pointer(startPos2Ptr, sizeof(Vector2));
    Vector2* endPos2 = cart_get_pointer(endPos2Ptr, sizeof(Vector2));
    Vector2 ** collisionPoint = cart_get_pointer(collisionPointPtr, sizeof(Vector2 *));
    return CheckCollisionLines(*startPos1, *endPos1, *startPos2, *endPos2, collisionPoint);
    free((void*)startPos1);
    free((void*)endPos1);
    free((void*)startPos2);
    free((void*)endPos2);
    free((void*)collisionPoint);
}

void host_GetCollisionRec(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int rec1Ptr, unsigned int rec2Ptr) {
    Rectangle* rec1 = cart_get_pointer(rec1Ptr, sizeof(Rectangle));
    Rectangle* rec2 = cart_get_pointer(rec2Ptr, sizeof(Rectangle));
    Rectangle out = GetCollisionRec(*rec1, *rec2);
    free((void*)rec1);
    free((void*)rec2);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadImage(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Image out = LoadImage(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadImageRaw(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int formatPtr, unsigned int headerSizePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Image out = LoadImageRaw(fileName, widthPtr, heightPtr, formatPtr, headerSizePtr);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadImageAnim(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr, unsigned int framesPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** frames = cart_get_pointer(framesPtr, sizeof(int *));
    Image out = LoadImageAnim(fileName, frames);
    free((void*)fileName);
    free((void*)frames);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadImageAnimFromMemory(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileTypePtr, unsigned int fileDataPtr, unsigned int dataSizePtr, unsigned int framesPtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    int ** frames = cart_get_pointer(framesPtr, sizeof(int *));
    Image out = LoadImageAnimFromMemory(fileType, fileData, dataSizePtr, frames);
    free((void*)fileType);
    free((void*)fileData);
    free((void*)frames);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadImageFromMemory(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileTypePtr, unsigned int fileDataPtr, unsigned int dataSizePtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    Image out = LoadImageFromMemory(fileType, fileData, dataSizePtr);
    free((void*)fileType);
    free((void*)fileData);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadImageFromTexture(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int texturePtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Image out = LoadImageFromTexture(*texture);
    free((void*)texture);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadImageFromScreen(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Image out = LoadImageFromScreen();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsImageValid(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    return IsImageValid(*image);
    free((void*)image);
}

void host_UnloadImage(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    UnloadImage(*image);
    free((void*)image);
}

unsigned int host_ExportImage(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int fileNamePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportImage(*image, fileName);
    free((void*)image);
    free((void*)fileName);
}

void host_ExportImageToMemory(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr, unsigned int fileTypePtr, unsigned int fileSizePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    const char* fileType = cart_get_string(fileTypePtr);
    int ** fileSize = cart_get_pointer(fileSizePtr, sizeof(int *));
    unsigned char * out = ExportImageToMemory(*image, fileType, fileSize);
    free((void*)image);
    free((void*)fileType);
    free((void*)fileSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_ExportImageAsCode(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int fileNamePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportImageAsCode(*image, fileName);
    free((void*)image);
    free((void*)fileName);
}

void host_GenImageColor(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Image out = GenImageColor(widthPtr, heightPtr, *color);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImageGradientLinear(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int directionPtr, unsigned int startPtr, unsigned int endPtr) {
    Color* start = cart_get_pointer(startPtr, sizeof(Color));
    Color* end = cart_get_pointer(endPtr, sizeof(Color));
    Image out = GenImageGradientLinear(widthPtr, heightPtr, directionPtr, *start, *end);
    free((void*)start);
    free((void*)end);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImageGradientRadial(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int densityPtr, unsigned int innerPtr, unsigned int outerPtr) {
    Color* inner = cart_get_pointer(innerPtr, sizeof(Color));
    Color* outer = cart_get_pointer(outerPtr, sizeof(Color));
    Image out = GenImageGradientRadial(widthPtr, heightPtr, densityPtr, *inner, *outer);
    free((void*)inner);
    free((void*)outer);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImageGradientSquare(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int densityPtr, unsigned int innerPtr, unsigned int outerPtr) {
    Color* inner = cart_get_pointer(innerPtr, sizeof(Color));
    Color* outer = cart_get_pointer(outerPtr, sizeof(Color));
    Image out = GenImageGradientSquare(widthPtr, heightPtr, densityPtr, *inner, *outer);
    free((void*)inner);
    free((void*)outer);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImageChecked(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int checksXPtr, unsigned int checksYPtr, unsigned int col1Ptr, unsigned int col2Ptr) {
    Color* col1 = cart_get_pointer(col1Ptr, sizeof(Color));
    Color* col2 = cart_get_pointer(col2Ptr, sizeof(Color));
    Image out = GenImageChecked(widthPtr, heightPtr, checksXPtr, checksYPtr, *col1, *col2);
    free((void*)col1);
    free((void*)col2);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImageWhiteNoise(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int factorPtr) {
    Image out = GenImageWhiteNoise(widthPtr, heightPtr, factorPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImagePerlinNoise(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int offsetXPtr, unsigned int offsetYPtr, unsigned int scalePtr) {
    Image out = GenImagePerlinNoise(widthPtr, heightPtr, offsetXPtr, offsetYPtr, scalePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImageCellular(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int tileSizePtr) {
    Image out = GenImageCellular(widthPtr, heightPtr, tileSizePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImageText(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    Image out = GenImageText(widthPtr, heightPtr, text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ImageCopy(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Image out = ImageCopy(*image);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ImageFromImage(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr, unsigned int recPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Image out = ImageFromImage(*image, *rec);
    free((void*)image);
    free((void*)rec);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ImageFromChannel(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr, unsigned int selectedChannelPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Image out = ImageFromChannel(*image, selectedChannelPtr);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ImageText(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr, unsigned int fontSizePtr, unsigned int colorPtr) {
    const char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Image out = ImageText(text, fontSizePtr, *color);
    free((void*)text);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ImageTextEx(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fontPtr, unsigned int textPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* text = cart_get_string(textPtr);
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    Image out = ImageTextEx(*font, text, fontSizePtr, spacingPtr, *tint);
    free((void*)font);
    free((void*)text);
    free((void*)tint);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ImageFormat(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int newFormatPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageFormat(image, newFormatPtr);
    free((void*)image);
}

void host_ImageToPOT(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int fillPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* fill = cart_get_pointer(fillPtr, sizeof(Color));
    ImageToPOT(image, *fill);
    free((void*)image);
    free((void*)fill);
}

void host_ImageCrop(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int cropPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Rectangle* crop = cart_get_pointer(cropPtr, sizeof(Rectangle));
    ImageCrop(image, *crop);
    free((void*)image);
    free((void*)crop);
}

void host_ImageAlphaCrop(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int thresholdPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageAlphaCrop(image, thresholdPtr);
    free((void*)image);
}

void host_ImageAlphaClear(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int colorPtr, unsigned int thresholdPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageAlphaClear(image, *color, thresholdPtr);
    free((void*)image);
    free((void*)color);
}

void host_ImageAlphaMask(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int alphaMaskPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Image* alphaMask = cart_get_pointer(alphaMaskPtr, sizeof(Image));
    ImageAlphaMask(image, *alphaMask);
    free((void*)image);
    free((void*)alphaMask);
}

void host_ImageAlphaPremultiply(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageAlphaPremultiply(image);
    free((void*)image);
}

void host_ImageBlurGaussian(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int blurSizePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageBlurGaussian(image, blurSizePtr);
    free((void*)image);
}

void host_ImageKernelConvolution(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int kernelPtr, unsigned int kernelSizePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    const float ** kernel = cart_get_pointer(kernelPtr, sizeof(const float *));
    ImageKernelConvolution(image, kernel, kernelSizePtr);
    free((void*)image);
    free((void*)kernel);
}

void host_ImageResize(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int newWidthPtr, unsigned int newHeightPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageResize(image, newWidthPtr, newHeightPtr);
    free((void*)image);
}

void host_ImageResizeNN(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int newWidthPtr, unsigned int newHeightPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageResizeNN(image, newWidthPtr, newHeightPtr);
    free((void*)image);
}

void host_ImageResizeCanvas(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int newWidthPtr, unsigned int newHeightPtr, unsigned int offsetXPtr, unsigned int offsetYPtr, unsigned int fillPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* fill = cart_get_pointer(fillPtr, sizeof(Color));
    ImageResizeCanvas(image, newWidthPtr, newHeightPtr, offsetXPtr, offsetYPtr, *fill);
    free((void*)image);
    free((void*)fill);
}

void host_ImageMipmaps(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageMipmaps(image);
    free((void*)image);
}

void host_ImageDither(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int rBppPtr, unsigned int gBppPtr, unsigned int bBppPtr, unsigned int aBppPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageDither(image, rBppPtr, gBppPtr, bBppPtr, aBppPtr);
    free((void*)image);
}

void host_ImageFlipVertical(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageFlipVertical(image);
    free((void*)image);
}

void host_ImageFlipHorizontal(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageFlipHorizontal(image);
    free((void*)image);
}

void host_ImageRotate(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int degreesPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageRotate(image, degreesPtr);
    free((void*)image);
}

void host_ImageRotateCW(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageRotateCW(image);
    free((void*)image);
}

void host_ImageRotateCCW(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageRotateCCW(image);
    free((void*)image);
}

void host_ImageColorTint(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int colorPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageColorTint(image, *color);
    free((void*)image);
    free((void*)color);
}

void host_ImageColorInvert(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageColorInvert(image);
    free((void*)image);
}

void host_ImageColorGrayscale(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageColorGrayscale(image);
    free((void*)image);
}

void host_ImageColorContrast(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int contrastPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageColorContrast(image, contrastPtr);
    free((void*)image);
}

void host_ImageColorBrightness(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int brightnessPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageColorBrightness(image, brightnessPtr);
    free((void*)image);
}

void host_ImageColorReplace(wasm_exec_env_t exec_env, unsigned int imagePtr, unsigned int colorPtr, unsigned int replacePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color* replace = cart_get_pointer(replacePtr, sizeof(Color));
    ImageColorReplace(image, *color, *replace);
    free((void*)image);
    free((void*)color);
    free((void*)replace);
}

void host_LoadImageColors(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Color * out = LoadImageColors(*image);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadImagePalette(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr, unsigned int maxPaletteSizePtr, unsigned int colorCountPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    int ** colorCount = cart_get_pointer(colorCountPtr, sizeof(int *));
    Color * out = LoadImagePalette(*image, maxPaletteSizePtr, colorCount);
    free((void*)image);
    free((void*)colorCount);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadImageColors(wasm_exec_env_t exec_env, unsigned int colorsPtr) {
    Color ** colors = cart_get_pointer(colorsPtr, sizeof(Color *));
    UnloadImageColors(colors);
    free((void*)colors);
}

void host_UnloadImagePalette(wasm_exec_env_t exec_env, unsigned int colorsPtr) {
    Color ** colors = cart_get_pointer(colorsPtr, sizeof(Color *));
    UnloadImagePalette(colors);
    free((void*)colors);
}

void host_GetImageAlphaBorder(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr, unsigned int thresholdPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Rectangle out = GetImageAlphaBorder(*image, thresholdPtr);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetImageColor(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr, unsigned int xPtr, unsigned int yPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Color out = GetImageColor(*image, xPtr, yPtr);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ImageClearBackground(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageClearBackground(dst, *color);
    free((void*)dst);
    free((void*)color);
}

void host_ImageDrawPixel(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawPixel(dst, posXPtr, posYPtr, *color);
    free((void*)dst);
    free((void*)color);
}

void host_ImageDrawPixelV(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int positionPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawPixelV(dst, *position, *color);
    free((void*)dst);
    free((void*)position);
    free((void*)color);
}

void host_ImageDrawLine(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int startPosXPtr, unsigned int startPosYPtr, unsigned int endPosXPtr, unsigned int endPosYPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawLine(dst, startPosXPtr, startPosYPtr, endPosXPtr, endPosYPtr, *color);
    free((void*)dst);
    free((void*)color);
}

void host_ImageDrawLineV(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int startPtr, unsigned int endPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* start = cart_get_pointer(startPtr, sizeof(Vector2));
    Vector2* end = cart_get_pointer(endPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawLineV(dst, *start, *end, *color);
    free((void*)dst);
    free((void*)start);
    free((void*)end);
    free((void*)color);
}

void host_ImageDrawLineEx(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int startPtr, unsigned int endPtr, unsigned int thickPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* start = cart_get_pointer(startPtr, sizeof(Vector2));
    Vector2* end = cart_get_pointer(endPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawLineEx(dst, *start, *end, thickPtr, *color);
    free((void*)dst);
    free((void*)start);
    free((void*)end);
    free((void*)color);
}

void host_ImageDrawCircle(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawCircle(dst, centerXPtr, centerYPtr, radiusPtr, *color);
    free((void*)dst);
    free((void*)color);
}

void host_ImageDrawCircleV(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int centerPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawCircleV(dst, *center, radiusPtr, *color);
    free((void*)dst);
    free((void*)center);
    free((void*)color);
}

void host_ImageDrawCircleLines(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawCircleLines(dst, centerXPtr, centerYPtr, radiusPtr, *color);
    free((void*)dst);
    free((void*)color);
}

void host_ImageDrawCircleLinesV(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int centerPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawCircleLinesV(dst, *center, radiusPtr, *color);
    free((void*)dst);
    free((void*)center);
    free((void*)color);
}

void host_ImageDrawRectangle(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawRectangle(dst, posXPtr, posYPtr, widthPtr, heightPtr, *color);
    free((void*)dst);
    free((void*)color);
}

void host_ImageDrawRectangleV(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int positionPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Vector2* size = cart_get_pointer(sizePtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawRectangleV(dst, *position, *size, *color);
    free((void*)dst);
    free((void*)position);
    free((void*)size);
    free((void*)color);
}

void host_ImageDrawRectangleRec(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int recPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawRectangleRec(dst, *rec, *color);
    free((void*)dst);
    free((void*)rec);
    free((void*)color);
}

void host_ImageDrawRectangleLines(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int recPtr, unsigned int thickPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawRectangleLines(dst, *rec, thickPtr, *color);
    free((void*)dst);
    free((void*)rec);
    free((void*)color);
}

void host_ImageDrawTriangle(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* v1 = cart_get_pointer(v1Ptr, sizeof(Vector2));
    Vector2* v2 = cart_get_pointer(v2Ptr, sizeof(Vector2));
    Vector2* v3 = cart_get_pointer(v3Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawTriangle(dst, *v1, *v2, *v3, *color);
    free((void*)dst);
    free((void*)v1);
    free((void*)v2);
    free((void*)v3);
    free((void*)color);
}

void host_ImageDrawTriangleEx(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int c1Ptr, unsigned int c2Ptr, unsigned int c3Ptr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* v1 = cart_get_pointer(v1Ptr, sizeof(Vector2));
    Vector2* v2 = cart_get_pointer(v2Ptr, sizeof(Vector2));
    Vector2* v3 = cart_get_pointer(v3Ptr, sizeof(Vector2));
    Color* c1 = cart_get_pointer(c1Ptr, sizeof(Color));
    Color* c2 = cart_get_pointer(c2Ptr, sizeof(Color));
    Color* c3 = cart_get_pointer(c3Ptr, sizeof(Color));
    ImageDrawTriangleEx(dst, *v1, *v2, *v3, *c1, *c2, *c3);
    free((void*)dst);
    free((void*)v1);
    free((void*)v2);
    free((void*)v3);
    free((void*)c1);
    free((void*)c2);
    free((void*)c3);
}

void host_ImageDrawTriangleLines(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* v1 = cart_get_pointer(v1Ptr, sizeof(Vector2));
    Vector2* v2 = cart_get_pointer(v2Ptr, sizeof(Vector2));
    Vector2* v3 = cart_get_pointer(v3Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawTriangleLines(dst, *v1, *v2, *v3, *color);
    free((void*)dst);
    free((void*)v1);
    free((void*)v2);
    free((void*)v3);
    free((void*)color);
}

void host_ImageDrawTriangleFan(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawTriangleFan(dst, points, pointCountPtr, *color);
    free((void*)dst);
    free((void*)points);
    free((void*)color);
}

void host_ImageDrawTriangleStrip(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawTriangleStrip(dst, points, pointCountPtr, *color);
    free((void*)dst);
    free((void*)points);
    free((void*)color);
}

void host_ImageDraw(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int srcPtr, unsigned int srcRecPtr, unsigned int dstRecPtr, unsigned int tintPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Image* src = cart_get_pointer(srcPtr, sizeof(Image));
    Rectangle* srcRec = cart_get_pointer(srcRecPtr, sizeof(Rectangle));
    Rectangle* dstRec = cart_get_pointer(dstRecPtr, sizeof(Rectangle));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    ImageDraw(dst, *src, *srcRec, *dstRec, *tint);
    free((void*)dst);
    free((void*)src);
    free((void*)srcRec);
    free((void*)dstRec);
    free((void*)tint);
}

void host_ImageDrawText(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int textPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int fontSizePtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    const char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawText(dst, text, posXPtr, posYPtr, fontSizePtr, *color);
    free((void*)dst);
    free((void*)text);
    free((void*)color);
}

void host_ImageDrawTextEx(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int fontPtr, unsigned int textPtr, unsigned int positionPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* text = cart_get_string(textPtr);
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    ImageDrawTextEx(dst, *font, text, *position, fontSizePtr, spacingPtr, *tint);
    free((void*)dst);
    free((void*)font);
    free((void*)text);
    free((void*)position);
    free((void*)tint);
}

void host_LoadTexture(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Texture2D out = LoadTexture(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadTextureFromImage(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Texture2D out = LoadTextureFromImage(*image);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadTextureCubemap(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr, unsigned int layoutPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    TextureCubemap out = LoadTextureCubemap(*image, layoutPtr);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadRenderTexture(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr) {
    RenderTexture2D out = LoadRenderTexture(widthPtr, heightPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsTextureValid(wasm_exec_env_t exec_env, unsigned int texturePtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    return IsTextureValid(*texture);
    free((void*)texture);
}

void host_UnloadTexture(wasm_exec_env_t exec_env, unsigned int texturePtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    UnloadTexture(*texture);
    free((void*)texture);
}

unsigned int host_IsRenderTextureValid(wasm_exec_env_t exec_env, unsigned int targetPtr) {
    RenderTexture2D* target = cart_get_pointer(targetPtr, sizeof(RenderTexture2D));
    return IsRenderTextureValid(*target);
    free((void*)target);
}

void host_UnloadRenderTexture(wasm_exec_env_t exec_env, unsigned int targetPtr) {
    RenderTexture2D* target = cart_get_pointer(targetPtr, sizeof(RenderTexture2D));
    UnloadRenderTexture(*target);
    free((void*)target);
}

void host_UpdateTexture(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int pixelsPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    const void ** pixels = cart_get_pointer(pixelsPtr, sizeof(const void *));
    UpdateTexture(*texture, pixels);
    free((void*)texture);
    free((void*)pixels);
}

void host_UpdateTextureRec(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int recPtr, unsigned int pixelsPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    const void ** pixels = cart_get_pointer(pixelsPtr, sizeof(const void *));
    UpdateTextureRec(*texture, *rec, pixels);
    free((void*)texture);
    free((void*)rec);
    free((void*)pixels);
}

void host_GenTextureMipmaps(wasm_exec_env_t exec_env, unsigned int texturePtr) {
    Texture2D ** texture = cart_get_pointer(texturePtr, sizeof(Texture2D *));
    GenTextureMipmaps(texture);
    free((void*)texture);
}

void host_SetTextureFilter(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int filterPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    SetTextureFilter(*texture, filterPtr);
    free((void*)texture);
}

void host_SetTextureWrap(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int wrapPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    SetTextureWrap(*texture, wrapPtr);
    free((void*)texture);
}

void host_DrawTexture(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTexture(*texture, posXPtr, posYPtr, *tint);
    free((void*)texture);
    free((void*)tint);
}

void host_DrawTextureV(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int positionPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextureV(*texture, *position, *tint);
    free((void*)texture);
    free((void*)position);
    free((void*)tint);
}

void host_DrawTextureEx(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int positionPtr, unsigned int rotationPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextureEx(*texture, *position, rotationPtr, scalePtr, *tint);
    free((void*)texture);
    free((void*)position);
    free((void*)tint);
}

void host_DrawTextureRec(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int sourcePtr, unsigned int positionPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Rectangle* source = cart_get_pointer(sourcePtr, sizeof(Rectangle));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextureRec(*texture, *source, *position, *tint);
    free((void*)texture);
    free((void*)source);
    free((void*)position);
    free((void*)tint);
}

void host_DrawTexturePro(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int sourcePtr, unsigned int destPtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Rectangle* source = cart_get_pointer(sourcePtr, sizeof(Rectangle));
    Rectangle* dest = cart_get_pointer(destPtr, sizeof(Rectangle));
    Vector2* origin = cart_get_pointer(originPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTexturePro(*texture, *source, *dest, *origin, rotationPtr, *tint);
    free((void*)texture);
    free((void*)source);
    free((void*)dest);
    free((void*)origin);
    free((void*)tint);
}

void host_DrawTextureNPatch(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int nPatchInfoPtr, unsigned int destPtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    NPatchInfo* nPatchInfo = cart_get_pointer(nPatchInfoPtr, sizeof(NPatchInfo));
    Rectangle* dest = cart_get_pointer(destPtr, sizeof(Rectangle));
    Vector2* origin = cart_get_pointer(originPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextureNPatch(*texture, *nPatchInfo, *dest, *origin, rotationPtr, *tint);
    free((void*)texture);
    free((void*)nPatchInfo);
    free((void*)dest);
    free((void*)origin);
    free((void*)tint);
}

unsigned int host_ColorIsEqual(wasm_exec_env_t exec_env, unsigned int col1Ptr, unsigned int col2Ptr) {
    Color* col1 = cart_get_pointer(col1Ptr, sizeof(Color));
    Color* col2 = cart_get_pointer(col2Ptr, sizeof(Color));
    return ColorIsEqual(*col1, *col2);
    free((void*)col1);
    free((void*)col2);
}

void host_Fade(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int colorPtr, unsigned int alphaPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color out = Fade(*color, alphaPtr);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_ColorToInt(wasm_exec_env_t exec_env, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    return ColorToInt(*color);
    free((void*)color);
}

void host_ColorNormalize(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Vector4 out = ColorNormalize(*color);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorFromNormalized(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int normalizedPtr) {
    Vector4* normalized = cart_get_pointer(normalizedPtr, sizeof(Vector4));
    Color out = ColorFromNormalized(*normalized);
    free((void*)normalized);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorToHSV(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Vector3 out = ColorToHSV(*color);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorFromHSV(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int huePtr, unsigned int saturationPtr, unsigned int valuePtr) {
    Color out = ColorFromHSV(huePtr, saturationPtr, valuePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorTint(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int colorPtr, unsigned int tintPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    Color out = ColorTint(*color, *tint);
    free((void*)color);
    free((void*)tint);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorBrightness(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int colorPtr, unsigned int factorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color out = ColorBrightness(*color, factorPtr);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorContrast(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int colorPtr, unsigned int contrastPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color out = ColorContrast(*color, contrastPtr);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorAlpha(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int colorPtr, unsigned int alphaPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color out = ColorAlpha(*color, alphaPtr);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorAlphaBlend(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int dstPtr, unsigned int srcPtr, unsigned int tintPtr) {
    Color* dst = cart_get_pointer(dstPtr, sizeof(Color));
    Color* src = cart_get_pointer(srcPtr, sizeof(Color));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    Color out = ColorAlphaBlend(*dst, *src, *tint);
    free((void*)dst);
    free((void*)src);
    free((void*)tint);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_ColorLerp(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int color1Ptr, unsigned int color2Ptr, unsigned int factorPtr) {
    Color* color1 = cart_get_pointer(color1Ptr, sizeof(Color));
    Color* color2 = cart_get_pointer(color2Ptr, sizeof(Color));
    Color out = ColorLerp(*color1, *color2, factorPtr);
    free((void*)color1);
    free((void*)color2);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetColor(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int hexValuePtr) {
    Color out = GetColor(hexValuePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetPixelColor(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int srcPtrPtr, unsigned int formatPtr) {
    void ** srcPtr = cart_get_pointer(srcPtrPtr, sizeof(void *));
    Color out = GetPixelColor(srcPtr, formatPtr);
    free((void*)srcPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_SetPixelColor(wasm_exec_env_t exec_env, unsigned int dstPtrPtr, unsigned int colorPtr, unsigned int formatPtr) {
    void ** dstPtr = cart_get_pointer(dstPtrPtr, sizeof(void *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    SetPixelColor(dstPtr, *color, formatPtr);
    free((void*)dstPtr);
    free((void*)color);
}

unsigned int host_GetPixelDataSize(wasm_exec_env_t exec_env, unsigned int widthPtr, unsigned int heightPtr, unsigned int formatPtr) {
    return GetPixelDataSize(widthPtr, heightPtr, formatPtr);
}

void host_GetFontDefault(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Font out = GetFontDefault();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadFont(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Font out = LoadFont(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadFontEx(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr, unsigned int fontSizePtr, unsigned int codepointsPtr, unsigned int codepointCountPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(int *));
    Font out = LoadFontEx(fileName, fontSizePtr, codepoints, codepointCountPtr);
    free((void*)fileName);
    free((void*)codepoints);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadFontFromImage(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr, unsigned int keyPtr, unsigned int firstCharPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Color* key = cart_get_pointer(keyPtr, sizeof(Color));
    Font out = LoadFontFromImage(*image, *key, firstCharPtr);
    free((void*)image);
    free((void*)key);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadFontFromMemory(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileTypePtr, unsigned int fileDataPtr, unsigned int dataSizePtr, unsigned int fontSizePtr, unsigned int codepointsPtr, unsigned int codepointCountPtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(int *));
    Font out = LoadFontFromMemory(fileType, fileData, dataSizePtr, fontSizePtr, codepoints, codepointCountPtr);
    free((void*)fileType);
    free((void*)fileData);
    free((void*)codepoints);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsFontValid(wasm_exec_env_t exec_env, unsigned int fontPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    return IsFontValid(*font);
    free((void*)font);
}

void host_LoadFontData(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileDataPtr, unsigned int dataSizePtr, unsigned int fontSizePtr, unsigned int codepointsPtr, unsigned int codepointCountPtr, unsigned int typePtr) {
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(int *));
    GlyphInfo * out = LoadFontData(fileData, dataSizePtr, fontSizePtr, codepoints, codepointCountPtr, typePtr);
    free((void*)fileData);
    free((void*)codepoints);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenImageFontAtlas(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int glyphsPtr, unsigned int glyphRecsPtr, unsigned int glyphCountPtr, unsigned int fontSizePtr, unsigned int paddingPtr, unsigned int packMethodPtr) {
    const GlyphInfo ** glyphs = cart_get_pointer(glyphsPtr, sizeof(const GlyphInfo *));
    Rectangle *** glyphRecs = cart_get_pointer(glyphRecsPtr, sizeof(Rectangle **));
    Image out = GenImageFontAtlas(glyphs, glyphRecs, glyphCountPtr, fontSizePtr, paddingPtr, packMethodPtr);
    free((void*)glyphs);
    free((void*)glyphRecs);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadFontData(wasm_exec_env_t exec_env, unsigned int glyphsPtr, unsigned int glyphCountPtr) {
    GlyphInfo ** glyphs = cart_get_pointer(glyphsPtr, sizeof(GlyphInfo *));
    UnloadFontData(glyphs, glyphCountPtr);
    free((void*)glyphs);
}

void host_UnloadFont(wasm_exec_env_t exec_env, unsigned int fontPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    UnloadFont(*font);
    free((void*)font);
}

unsigned int host_ExportFontAsCode(wasm_exec_env_t exec_env, unsigned int fontPtr, unsigned int fileNamePtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportFontAsCode(*font, fileName);
    free((void*)font);
    free((void*)fileName);
}

void host_DrawFPS(wasm_exec_env_t exec_env, unsigned int posXPtr, unsigned int posYPtr) {
    DrawFPS(posXPtr, posYPtr);
}

void host_DrawText(wasm_exec_env_t exec_env, unsigned int textPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int fontSizePtr, unsigned int colorPtr) {
    const char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawText(text, posXPtr, posYPtr, fontSizePtr, *color);
    free((void*)text);
    free((void*)color);
}

void host_DrawTextEx(wasm_exec_env_t exec_env, unsigned int fontPtr, unsigned int textPtr, unsigned int positionPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* text = cart_get_string(textPtr);
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextEx(*font, text, *position, fontSizePtr, spacingPtr, *tint);
    free((void*)font);
    free((void*)text);
    free((void*)position);
    free((void*)tint);
}

void host_DrawTextPro(wasm_exec_env_t exec_env, unsigned int fontPtr, unsigned int textPtr, unsigned int positionPtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* text = cart_get_string(textPtr);
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Vector2* origin = cart_get_pointer(originPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextPro(*font, text, *position, *origin, rotationPtr, fontSizePtr, spacingPtr, *tint);
    free((void*)font);
    free((void*)text);
    free((void*)position);
    free((void*)origin);
    free((void*)tint);
}

void host_DrawTextCodepoint(wasm_exec_env_t exec_env, unsigned int fontPtr, unsigned int codepointPtr, unsigned int positionPtr, unsigned int fontSizePtr, unsigned int tintPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextCodepoint(*font, codepointPtr, *position, fontSizePtr, *tint);
    free((void*)font);
    free((void*)position);
    free((void*)tint);
}

void host_DrawTextCodepoints(wasm_exec_env_t exec_env, unsigned int fontPtr, unsigned int codepointsPtr, unsigned int codepointCountPtr, unsigned int positionPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(const int *));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextCodepoints(*font, codepoints, codepointCountPtr, *position, fontSizePtr, spacingPtr, *tint);
    free((void*)font);
    free((void*)codepoints);
    free((void*)position);
    free((void*)tint);
}

void host_SetTextLineSpacing(wasm_exec_env_t exec_env, unsigned int spacingPtr) {
    SetTextLineSpacing(spacingPtr);
}

unsigned int host_MeasureText(wasm_exec_env_t exec_env, unsigned int textPtr, unsigned int fontSizePtr) {
    const char* text = cart_get_string(textPtr);
    return MeasureText(text, fontSizePtr);
    free((void*)text);
}

void host_MeasureTextEx(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fontPtr, unsigned int textPtr, unsigned int fontSizePtr, unsigned int spacingPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* text = cart_get_string(textPtr);
    Vector2 out = MeasureTextEx(*font, text, fontSizePtr, spacingPtr);
    free((void*)font);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_GetGlyphIndex(wasm_exec_env_t exec_env, unsigned int fontPtr, unsigned int codepointPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    return GetGlyphIndex(*font, codepointPtr);
    free((void*)font);
}

void host_GetGlyphInfo(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fontPtr, unsigned int codepointPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    GlyphInfo out = GetGlyphInfo(*font, codepointPtr);
    free((void*)font);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetGlyphAtlasRec(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fontPtr, unsigned int codepointPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    Rectangle out = GetGlyphAtlasRec(*font, codepointPtr);
    free((void*)font);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadUTF8(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int codepointsPtr, unsigned int lengthPtr) {
    const int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(const int *));
    char * out = LoadUTF8(codepoints, lengthPtr);
    free((void*)codepoints);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadUTF8(wasm_exec_env_t exec_env, unsigned int textPtr) {
    char ** text = cart_get_pointer(textPtr, sizeof(char *));
    UnloadUTF8(text);
    free((void*)text);
}

void host_LoadCodepoints(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr, unsigned int countPtr) {
    const char* text = cart_get_string(textPtr);
    int ** count = cart_get_pointer(countPtr, sizeof(int *));
    int * out = LoadCodepoints(text, count);
    free((void*)text);
    free((void*)count);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadCodepoints(wasm_exec_env_t exec_env, unsigned int codepointsPtr) {
    int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(int *));
    UnloadCodepoints(codepoints);
    free((void*)codepoints);
}

unsigned int host_GetCodepointCount(wasm_exec_env_t exec_env, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    return GetCodepointCount(text);
    free((void*)text);
}

unsigned int host_GetCodepoint(wasm_exec_env_t exec_env, unsigned int textPtr, unsigned int codepointSizePtr) {
    const char* text = cart_get_string(textPtr);
    int ** codepointSize = cart_get_pointer(codepointSizePtr, sizeof(int *));
    return GetCodepoint(text, codepointSize);
    free((void*)text);
    free((void*)codepointSize);
}

unsigned int host_GetCodepointNext(wasm_exec_env_t exec_env, unsigned int textPtr, unsigned int codepointSizePtr) {
    const char* text = cart_get_string(textPtr);
    int ** codepointSize = cart_get_pointer(codepointSizePtr, sizeof(int *));
    return GetCodepointNext(text, codepointSize);
    free((void*)text);
    free((void*)codepointSize);
}

unsigned int host_GetCodepointPrevious(wasm_exec_env_t exec_env, unsigned int textPtr, unsigned int codepointSizePtr) {
    const char* text = cart_get_string(textPtr);
    int ** codepointSize = cart_get_pointer(codepointSizePtr, sizeof(int *));
    return GetCodepointPrevious(text, codepointSize);
    free((void*)text);
    free((void*)codepointSize);
}

void host_CodepointToUTF8(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int codepointPtr, unsigned int utf8SizePtr) {
    int ** utf8Size = cart_get_pointer(utf8SizePtr, sizeof(int *));
    const char * out = CodepointToUTF8(codepointPtr, utf8Size);
    free((void*)utf8Size);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_TextCopy(wasm_exec_env_t exec_env, unsigned int dstPtr, unsigned int srcPtr) {
    char ** dst = cart_get_pointer(dstPtr, sizeof(char *));
    const char* src = cart_get_string(srcPtr);
    return TextCopy(dst, src);
    free((void*)dst);
    free((void*)src);
}

unsigned int host_TextIsEqual(wasm_exec_env_t exec_env, unsigned int text1Ptr, unsigned int text2Ptr) {
    const char* text1 = cart_get_string(text1Ptr);
    const char* text2 = cart_get_string(text2Ptr);
    return TextIsEqual(text1, text2);
    free((void*)text1);
    free((void*)text2);
}

unsigned int host_TextLength(wasm_exec_env_t exec_env, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    return TextLength(text);
    free((void*)text);
}

void host_TextSubtext(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr, unsigned int positionPtr, unsigned int lengthPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextSubtext(text, positionPtr, lengthPtr);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextReplace(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr, unsigned int replacePtr, unsigned int byPtr) {
    const char* text = cart_get_string(textPtr);
    const char* replace = cart_get_string(replacePtr);
    const char* by = cart_get_string(byPtr);
    char * out = TextReplace(text, replace, by);
    free((void*)text);
    free((void*)replace);
    free((void*)by);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextInsert(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr, unsigned int insertPtr, unsigned int positionPtr) {
    const char* text = cart_get_string(textPtr);
    const char* insert = cart_get_string(insertPtr);
    char * out = TextInsert(text, insert, positionPtr);
    free((void*)text);
    free((void*)insert);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextJoin(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textListPtr, unsigned int countPtr, unsigned int delimiterPtr) {
    const char *** textList = cart_get_pointer(textListPtr, sizeof(const char **));
    const char* delimiter = cart_get_string(delimiterPtr);
    const char * out = TextJoin(textList, countPtr, delimiter);
    free((void*)textList);
    free((void*)delimiter);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextSplit(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr, unsigned int delimiterPtr, unsigned int countPtr) {
    const char* text = cart_get_string(textPtr);
    int ** count = cart_get_pointer(countPtr, sizeof(int *));
    const char ** out = TextSplit(text, delimiterPtr, count);
    free((void*)text);
    free((void*)count);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextAppend(wasm_exec_env_t exec_env, unsigned int textPtr, unsigned int appendPtr, unsigned int positionPtr) {
    char ** text = cart_get_pointer(textPtr, sizeof(char *));
    const char* append = cart_get_string(appendPtr);
    int ** position = cart_get_pointer(positionPtr, sizeof(int *));
    TextAppend(text, append, position);
    free((void*)text);
    free((void*)append);
    free((void*)position);
}

unsigned int host_TextFindIndex(wasm_exec_env_t exec_env, unsigned int textPtr, unsigned int findPtr) {
    const char* text = cart_get_string(textPtr);
    const char* find = cart_get_string(findPtr);
    return TextFindIndex(text, find);
    free((void*)text);
    free((void*)find);
}

void host_TextToUpper(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToUpper(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextToLower(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToLower(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextToPascal(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToPascal(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextToSnake(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToSnake(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_TextToCamel(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToCamel(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_TextToInteger(wasm_exec_env_t exec_env, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    return TextToInteger(text);
    free((void*)text);
}

unsigned int host_TextToFloat(wasm_exec_env_t exec_env, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    return TextToFloat(text);
    free((void*)text);
}

void host_DrawLine3D(wasm_exec_env_t exec_env, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLine3D(*startPos, *endPos, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

void host_DrawPoint3D(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPoint3D(*position, *color);
    free((void*)position);
    free((void*)color);
}

void host_DrawCircle3D(wasm_exec_env_t exec_env, unsigned int centerPtr, unsigned int radiusPtr, unsigned int rotationAxisPtr, unsigned int rotationAnglePtr, unsigned int colorPtr) {
    Vector3* center = cart_get_pointer(centerPtr, sizeof(Vector3));
    Vector3* rotationAxis = cart_get_pointer(rotationAxisPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircle3D(*center, radiusPtr, *rotationAxis, rotationAnglePtr, *color);
    free((void*)center);
    free((void*)rotationAxis);
    free((void*)color);
}

void host_DrawTriangle3D(wasm_exec_env_t exec_env, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
    Vector3* v1 = cart_get_pointer(v1Ptr, sizeof(Vector3));
    Vector3* v2 = cart_get_pointer(v2Ptr, sizeof(Vector3));
    Vector3* v3 = cart_get_pointer(v3Ptr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangle3D(*v1, *v2, *v3, *color);
    free((void*)v1);
    free((void*)v2);
    free((void*)v3);
    free((void*)color);
}

void host_DrawTriangleStrip3D(wasm_exec_env_t exec_env, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    const Vector3 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector3 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangleStrip3D(points, pointCountPtr, *color);
    free((void*)points);
    free((void*)color);
}

void host_DrawCube(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int lengthPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCube(*position, widthPtr, heightPtr, lengthPtr, *color);
    free((void*)position);
    free((void*)color);
}

void host_DrawCubeV(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector3* size = cart_get_pointer(sizePtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCubeV(*position, *size, *color);
    free((void*)position);
    free((void*)size);
    free((void*)color);
}

void host_DrawCubeWires(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int lengthPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCubeWires(*position, widthPtr, heightPtr, lengthPtr, *color);
    free((void*)position);
    free((void*)color);
}

void host_DrawCubeWiresV(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector3* size = cart_get_pointer(sizePtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCubeWiresV(*position, *size, *color);
    free((void*)position);
    free((void*)size);
    free((void*)color);
}

void host_DrawSphere(wasm_exec_env_t exec_env, unsigned int centerPosPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Vector3* centerPos = cart_get_pointer(centerPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSphere(*centerPos, radiusPtr, *color);
    free((void*)centerPos);
    free((void*)color);
}

void host_DrawSphereEx(wasm_exec_env_t exec_env, unsigned int centerPosPtr, unsigned int radiusPtr, unsigned int ringsPtr, unsigned int slicesPtr, unsigned int colorPtr) {
    Vector3* centerPos = cart_get_pointer(centerPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSphereEx(*centerPos, radiusPtr, ringsPtr, slicesPtr, *color);
    free((void*)centerPos);
    free((void*)color);
}

void host_DrawSphereWires(wasm_exec_env_t exec_env, unsigned int centerPosPtr, unsigned int radiusPtr, unsigned int ringsPtr, unsigned int slicesPtr, unsigned int colorPtr) {
    Vector3* centerPos = cart_get_pointer(centerPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSphereWires(*centerPos, radiusPtr, ringsPtr, slicesPtr, *color);
    free((void*)centerPos);
    free((void*)color);
}

void host_DrawCylinder(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int radiusTopPtr, unsigned int radiusBottomPtr, unsigned int heightPtr, unsigned int slicesPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCylinder(*position, radiusTopPtr, radiusBottomPtr, heightPtr, slicesPtr, *color);
    free((void*)position);
    free((void*)color);
}

void host_DrawCylinderEx(wasm_exec_env_t exec_env, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int startRadiusPtr, unsigned int endRadiusPtr, unsigned int sidesPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCylinderEx(*startPos, *endPos, startRadiusPtr, endRadiusPtr, sidesPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

void host_DrawCylinderWires(wasm_exec_env_t exec_env, unsigned int positionPtr, unsigned int radiusTopPtr, unsigned int radiusBottomPtr, unsigned int heightPtr, unsigned int slicesPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCylinderWires(*position, radiusTopPtr, radiusBottomPtr, heightPtr, slicesPtr, *color);
    free((void*)position);
    free((void*)color);
}

void host_DrawCylinderWiresEx(wasm_exec_env_t exec_env, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int startRadiusPtr, unsigned int endRadiusPtr, unsigned int sidesPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCylinderWiresEx(*startPos, *endPos, startRadiusPtr, endRadiusPtr, sidesPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

void host_DrawCapsule(wasm_exec_env_t exec_env, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int radiusPtr, unsigned int slicesPtr, unsigned int ringsPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCapsule(*startPos, *endPos, radiusPtr, slicesPtr, ringsPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

void host_DrawCapsuleWires(wasm_exec_env_t exec_env, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int radiusPtr, unsigned int slicesPtr, unsigned int ringsPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCapsuleWires(*startPos, *endPos, radiusPtr, slicesPtr, ringsPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

void host_DrawPlane(wasm_exec_env_t exec_env, unsigned int centerPosPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Vector3* centerPos = cart_get_pointer(centerPosPtr, sizeof(Vector3));
    Vector2* size = cart_get_pointer(sizePtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPlane(*centerPos, *size, *color);
    free((void*)centerPos);
    free((void*)size);
    free((void*)color);
}

void host_DrawRay(wasm_exec_env_t exec_env, unsigned int rayPtr, unsigned int colorPtr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRay(*ray, *color);
    free((void*)ray);
    free((void*)color);
}

void host_DrawGrid(wasm_exec_env_t exec_env, unsigned int slicesPtr, unsigned int spacingPtr) {
    DrawGrid(slicesPtr, spacingPtr);
}

void host_LoadModel(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Model out = LoadModel(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadModelFromMesh(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int meshPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    Model out = LoadModelFromMesh(*mesh);
    free((void*)mesh);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsModelValid(wasm_exec_env_t exec_env, unsigned int modelPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    return IsModelValid(*model);
    free((void*)model);
}

void host_UnloadModel(wasm_exec_env_t exec_env, unsigned int modelPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    UnloadModel(*model);
    free((void*)model);
}

void host_GetModelBoundingBox(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int modelPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    BoundingBox out = GetModelBoundingBox(*model);
    free((void*)model);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_DrawModel(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int positionPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModel(*model, *position, scalePtr, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)tint);
}

void host_DrawModelEx(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int positionPtr, unsigned int rotationAxisPtr, unsigned int rotationAnglePtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector3* rotationAxis = cart_get_pointer(rotationAxisPtr, sizeof(Vector3));
    Vector3* scale = cart_get_pointer(scalePtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModelEx(*model, *position, *rotationAxis, rotationAnglePtr, *scale, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)rotationAxis);
    free((void*)scale);
    free((void*)tint);
}

void host_DrawModelWires(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int positionPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModelWires(*model, *position, scalePtr, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)tint);
}

void host_DrawModelWiresEx(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int positionPtr, unsigned int rotationAxisPtr, unsigned int rotationAnglePtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector3* rotationAxis = cart_get_pointer(rotationAxisPtr, sizeof(Vector3));
    Vector3* scale = cart_get_pointer(scalePtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModelWiresEx(*model, *position, *rotationAxis, rotationAnglePtr, *scale, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)rotationAxis);
    free((void*)scale);
    free((void*)tint);
}

void host_DrawModelPoints(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int positionPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModelPoints(*model, *position, scalePtr, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)tint);
}

void host_DrawModelPointsEx(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int positionPtr, unsigned int rotationAxisPtr, unsigned int rotationAnglePtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector3* rotationAxis = cart_get_pointer(rotationAxisPtr, sizeof(Vector3));
    Vector3* scale = cart_get_pointer(scalePtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModelPointsEx(*model, *position, *rotationAxis, rotationAnglePtr, *scale, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)rotationAxis);
    free((void*)scale);
    free((void*)tint);
}

void host_DrawBoundingBox(wasm_exec_env_t exec_env, unsigned int boxPtr, unsigned int colorPtr) {
    BoundingBox* box = cart_get_pointer(boxPtr, sizeof(BoundingBox));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawBoundingBox(*box, *color);
    free((void*)box);
    free((void*)color);
}

void host_DrawBillboard(wasm_exec_env_t exec_env, unsigned int cameraPtr, unsigned int texturePtr, unsigned int positionPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawBillboard(*camera, *texture, *position, scalePtr, *tint);
    free((void*)camera);
    free((void*)texture);
    free((void*)position);
    free((void*)tint);
}

void host_DrawBillboardRec(wasm_exec_env_t exec_env, unsigned int cameraPtr, unsigned int texturePtr, unsigned int sourcePtr, unsigned int positionPtr, unsigned int sizePtr, unsigned int tintPtr) {
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Rectangle* source = cart_get_pointer(sourcePtr, sizeof(Rectangle));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector2* size = cart_get_pointer(sizePtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawBillboardRec(*camera, *texture, *source, *position, *size, *tint);
    free((void*)camera);
    free((void*)texture);
    free((void*)source);
    free((void*)position);
    free((void*)size);
    free((void*)tint);
}

void host_DrawBillboardPro(wasm_exec_env_t exec_env, unsigned int cameraPtr, unsigned int texturePtr, unsigned int sourcePtr, unsigned int positionPtr, unsigned int upPtr, unsigned int sizePtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int tintPtr) {
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Rectangle* source = cart_get_pointer(sourcePtr, sizeof(Rectangle));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector3* up = cart_get_pointer(upPtr, sizeof(Vector3));
    Vector2* size = cart_get_pointer(sizePtr, sizeof(Vector2));
    Vector2* origin = cart_get_pointer(originPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawBillboardPro(*camera, *texture, *source, *position, *up, *size, *origin, rotationPtr, *tint);
    free((void*)camera);
    free((void*)texture);
    free((void*)source);
    free((void*)position);
    free((void*)up);
    free((void*)size);
    free((void*)origin);
    free((void*)tint);
}

void host_UploadMesh(wasm_exec_env_t exec_env, unsigned int meshPtr, unsigned int dynamicPtr) {
    Mesh ** mesh = cart_get_pointer(meshPtr, sizeof(Mesh *));
    UploadMesh(mesh, dynamicPtr);
    free((void*)mesh);
}

void host_UpdateMeshBuffer(wasm_exec_env_t exec_env, unsigned int meshPtr, unsigned int indexPtr, unsigned int dataPtr, unsigned int dataSizePtr, unsigned int offsetPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    const void ** data = cart_get_pointer(dataPtr, sizeof(const void *));
    UpdateMeshBuffer(*mesh, indexPtr, data, dataSizePtr, offsetPtr);
    free((void*)mesh);
    free((void*)data);
}

void host_UnloadMesh(wasm_exec_env_t exec_env, unsigned int meshPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    UnloadMesh(*mesh);
    free((void*)mesh);
}

void host_DrawMesh(wasm_exec_env_t exec_env, unsigned int meshPtr, unsigned int materialPtr, unsigned int transformPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    Material* material = cart_get_pointer(materialPtr, sizeof(Material));
    Matrix* transform = cart_get_pointer(transformPtr, sizeof(Matrix));
    DrawMesh(*mesh, *material, *transform);
    free((void*)mesh);
    free((void*)material);
    free((void*)transform);
}

void host_DrawMeshInstanced(wasm_exec_env_t exec_env, unsigned int meshPtr, unsigned int materialPtr, unsigned int transformsPtr, unsigned int instancesPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    Material* material = cart_get_pointer(materialPtr, sizeof(Material));
    const Matrix ** transforms = cart_get_pointer(transformsPtr, sizeof(const Matrix *));
    DrawMeshInstanced(*mesh, *material, transforms, instancesPtr);
    free((void*)mesh);
    free((void*)material);
    free((void*)transforms);
}

void host_GetMeshBoundingBox(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int meshPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    BoundingBox out = GetMeshBoundingBox(*mesh);
    free((void*)mesh);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshTangents(wasm_exec_env_t exec_env, unsigned int meshPtr) {
    Mesh ** mesh = cart_get_pointer(meshPtr, sizeof(Mesh *));
    GenMeshTangents(mesh);
    free((void*)mesh);
}

unsigned int host_ExportMesh(wasm_exec_env_t exec_env, unsigned int meshPtr, unsigned int fileNamePtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportMesh(*mesh, fileName);
    free((void*)mesh);
    free((void*)fileName);
}

unsigned int host_ExportMeshAsCode(wasm_exec_env_t exec_env, unsigned int meshPtr, unsigned int fileNamePtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportMeshAsCode(*mesh, fileName);
    free((void*)mesh);
    free((void*)fileName);
}

void host_GenMeshPoly(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int sidesPtr, unsigned int radiusPtr) {
    Mesh out = GenMeshPoly(sidesPtr, radiusPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshPlane(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int lengthPtr, unsigned int resXPtr, unsigned int resZPtr) {
    Mesh out = GenMeshPlane(widthPtr, lengthPtr, resXPtr, resZPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshCube(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int lengthPtr) {
    Mesh out = GenMeshCube(widthPtr, heightPtr, lengthPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshSphere(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int radiusPtr, unsigned int ringsPtr, unsigned int slicesPtr) {
    Mesh out = GenMeshSphere(radiusPtr, ringsPtr, slicesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshHemiSphere(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int radiusPtr, unsigned int ringsPtr, unsigned int slicesPtr) {
    Mesh out = GenMeshHemiSphere(radiusPtr, ringsPtr, slicesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshCylinder(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int radiusPtr, unsigned int heightPtr, unsigned int slicesPtr) {
    Mesh out = GenMeshCylinder(radiusPtr, heightPtr, slicesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshCone(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int radiusPtr, unsigned int heightPtr, unsigned int slicesPtr) {
    Mesh out = GenMeshCone(radiusPtr, heightPtr, slicesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshTorus(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int radiusPtr, unsigned int sizePtr, unsigned int radSegPtr, unsigned int sidesPtr) {
    Mesh out = GenMeshTorus(radiusPtr, sizePtr, radSegPtr, sidesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshKnot(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int radiusPtr, unsigned int sizePtr, unsigned int radSegPtr, unsigned int sidesPtr) {
    Mesh out = GenMeshKnot(radiusPtr, sizePtr, radSegPtr, sidesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshHeightmap(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int heightmapPtr, unsigned int sizePtr) {
    Image* heightmap = cart_get_pointer(heightmapPtr, sizeof(Image));
    Vector3* size = cart_get_pointer(sizePtr, sizeof(Vector3));
    Mesh out = GenMeshHeightmap(*heightmap, *size);
    free((void*)heightmap);
    free((void*)size);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GenMeshCubicmap(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int cubicmapPtr, unsigned int cubeSizePtr) {
    Image* cubicmap = cart_get_pointer(cubicmapPtr, sizeof(Image));
    Vector3* cubeSize = cart_get_pointer(cubeSizePtr, sizeof(Vector3));
    Mesh out = GenMeshCubicmap(*cubicmap, *cubeSize);
    free((void*)cubicmap);
    free((void*)cubeSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadMaterials(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr, unsigned int materialCountPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** materialCount = cart_get_pointer(materialCountPtr, sizeof(int *));
    Material * out = LoadMaterials(fileName, materialCount);
    free((void*)fileName);
    free((void*)materialCount);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadMaterialDefault(wasm_exec_env_t exec_env, unsigned int outPtr) {
    Material out = LoadMaterialDefault();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsMaterialValid(wasm_exec_env_t exec_env, unsigned int materialPtr) {
    Material* material = cart_get_pointer(materialPtr, sizeof(Material));
    return IsMaterialValid(*material);
    free((void*)material);
}

void host_UnloadMaterial(wasm_exec_env_t exec_env, unsigned int materialPtr) {
    Material* material = cart_get_pointer(materialPtr, sizeof(Material));
    UnloadMaterial(*material);
    free((void*)material);
}

void host_SetMaterialTexture(wasm_exec_env_t exec_env, unsigned int materialPtr, unsigned int mapTypePtr, unsigned int texturePtr) {
    Material ** material = cart_get_pointer(materialPtr, sizeof(Material *));
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    SetMaterialTexture(material, mapTypePtr, *texture);
    free((void*)material);
    free((void*)texture);
}

void host_SetModelMeshMaterial(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int meshIdPtr, unsigned int materialIdPtr) {
    Model ** model = cart_get_pointer(modelPtr, sizeof(Model *));
    SetModelMeshMaterial(model, meshIdPtr, materialIdPtr);
    free((void*)model);
}

void host_LoadModelAnimations(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr, unsigned int animCountPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** animCount = cart_get_pointer(animCountPtr, sizeof(int *));
    ModelAnimation * out = LoadModelAnimations(fileName, animCount);
    free((void*)fileName);
    free((void*)animCount);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UpdateModelAnimation(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int animPtr, unsigned int framePtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    ModelAnimation* anim = cart_get_pointer(animPtr, sizeof(ModelAnimation));
    UpdateModelAnimation(*model, *anim, framePtr);
    free((void*)model);
    free((void*)anim);
}

void host_UpdateModelAnimationBones(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int animPtr, unsigned int framePtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    ModelAnimation* anim = cart_get_pointer(animPtr, sizeof(ModelAnimation));
    UpdateModelAnimationBones(*model, *anim, framePtr);
    free((void*)model);
    free((void*)anim);
}

void host_UnloadModelAnimation(wasm_exec_env_t exec_env, unsigned int animPtr) {
    ModelAnimation* anim = cart_get_pointer(animPtr, sizeof(ModelAnimation));
    UnloadModelAnimation(*anim);
    free((void*)anim);
}

void host_UnloadModelAnimations(wasm_exec_env_t exec_env, unsigned int animationsPtr, unsigned int animCountPtr) {
    ModelAnimation ** animations = cart_get_pointer(animationsPtr, sizeof(ModelAnimation *));
    UnloadModelAnimations(animations, animCountPtr);
    free((void*)animations);
}

unsigned int host_IsModelAnimationValid(wasm_exec_env_t exec_env, unsigned int modelPtr, unsigned int animPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    ModelAnimation* anim = cart_get_pointer(animPtr, sizeof(ModelAnimation));
    return IsModelAnimationValid(*model, *anim);
    free((void*)model);
    free((void*)anim);
}

unsigned int host_CheckCollisionSpheres(wasm_exec_env_t exec_env, unsigned int center1Ptr, unsigned int radius1Ptr, unsigned int center2Ptr, unsigned int radius2Ptr) {
    Vector3* center1 = cart_get_pointer(center1Ptr, sizeof(Vector3));
    Vector3* center2 = cart_get_pointer(center2Ptr, sizeof(Vector3));
    return CheckCollisionSpheres(*center1, radius1Ptr, *center2, radius2Ptr);
    free((void*)center1);
    free((void*)center2);
}

unsigned int host_CheckCollisionBoxes(wasm_exec_env_t exec_env, unsigned int box1Ptr, unsigned int box2Ptr) {
    BoundingBox* box1 = cart_get_pointer(box1Ptr, sizeof(BoundingBox));
    BoundingBox* box2 = cart_get_pointer(box2Ptr, sizeof(BoundingBox));
    return CheckCollisionBoxes(*box1, *box2);
    free((void*)box1);
    free((void*)box2);
}

unsigned int host_CheckCollisionBoxSphere(wasm_exec_env_t exec_env, unsigned int boxPtr, unsigned int centerPtr, unsigned int radiusPtr) {
    BoundingBox* box = cart_get_pointer(boxPtr, sizeof(BoundingBox));
    Vector3* center = cart_get_pointer(centerPtr, sizeof(Vector3));
    return CheckCollisionBoxSphere(*box, *center, radiusPtr);
    free((void*)box);
    free((void*)center);
}

void host_GetRayCollisionSphere(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int rayPtr, unsigned int centerPtr, unsigned int radiusPtr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    Vector3* center = cart_get_pointer(centerPtr, sizeof(Vector3));
    RayCollision out = GetRayCollisionSphere(*ray, *center, radiusPtr);
    free((void*)ray);
    free((void*)center);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetRayCollisionBox(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int rayPtr, unsigned int boxPtr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    BoundingBox* box = cart_get_pointer(boxPtr, sizeof(BoundingBox));
    RayCollision out = GetRayCollisionBox(*ray, *box);
    free((void*)ray);
    free((void*)box);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetRayCollisionMesh(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int rayPtr, unsigned int meshPtr, unsigned int transformPtr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    Matrix* transform = cart_get_pointer(transformPtr, sizeof(Matrix));
    RayCollision out = GetRayCollisionMesh(*ray, *mesh, *transform);
    free((void*)ray);
    free((void*)mesh);
    free((void*)transform);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetRayCollisionTriangle(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int rayPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    Vector3* p1 = cart_get_pointer(p1Ptr, sizeof(Vector3));
    Vector3* p2 = cart_get_pointer(p2Ptr, sizeof(Vector3));
    Vector3* p3 = cart_get_pointer(p3Ptr, sizeof(Vector3));
    RayCollision out = GetRayCollisionTriangle(*ray, *p1, *p2, *p3);
    free((void*)ray);
    free((void*)p1);
    free((void*)p2);
    free((void*)p3);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_GetRayCollisionQuad(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int rayPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    Vector3* p1 = cart_get_pointer(p1Ptr, sizeof(Vector3));
    Vector3* p2 = cart_get_pointer(p2Ptr, sizeof(Vector3));
    Vector3* p3 = cart_get_pointer(p3Ptr, sizeof(Vector3));
    Vector3* p4 = cart_get_pointer(p4Ptr, sizeof(Vector3));
    RayCollision out = GetRayCollisionQuad(*ray, *p1, *p2, *p3, *p4);
    free((void*)ray);
    free((void*)p1);
    free((void*)p2);
    free((void*)p3);
    free((void*)p4);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_InitAudioDevice(wasm_exec_env_t exec_env) {
    InitAudioDevice();
}

void host_CloseAudioDevice(wasm_exec_env_t exec_env) {
    CloseAudioDevice();
}

unsigned int host_IsAudioDeviceReady(wasm_exec_env_t exec_env) {
    return IsAudioDeviceReady();
}

void host_SetMasterVolume(wasm_exec_env_t exec_env, unsigned int volumePtr) {
    SetMasterVolume(volumePtr);
}

unsigned int host_GetMasterVolume(wasm_exec_env_t exec_env) {
    return GetMasterVolume();
}

void host_LoadWave(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Wave out = LoadWave(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadWaveFromMemory(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileTypePtr, unsigned int fileDataPtr, unsigned int dataSizePtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    Wave out = LoadWaveFromMemory(fileType, fileData, dataSizePtr);
    free((void*)fileType);
    free((void*)fileData);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsWaveValid(wasm_exec_env_t exec_env, unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    return IsWaveValid(*wave);
    free((void*)wave);
}

void host_LoadSound(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Sound out = LoadSound(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadSoundFromWave(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    Sound out = LoadSoundFromWave(*wave);
    free((void*)wave);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadSoundAlias(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int sourcePtr) {
    Sound* source = cart_get_pointer(sourcePtr, sizeof(Sound));
    Sound out = LoadSoundAlias(*source);
    free((void*)source);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsSoundValid(wasm_exec_env_t exec_env, unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    return IsSoundValid(*sound);
    free((void*)sound);
}

void host_UpdateSound(wasm_exec_env_t exec_env, unsigned int soundPtr, unsigned int dataPtr, unsigned int sampleCountPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    const void ** data = cart_get_pointer(dataPtr, sizeof(const void *));
    UpdateSound(*sound, data, sampleCountPtr);
    free((void*)sound);
    free((void*)data);
}

void host_UnloadWave(wasm_exec_env_t exec_env, unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    UnloadWave(*wave);
    free((void*)wave);
}

void host_UnloadSound(wasm_exec_env_t exec_env, unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    UnloadSound(*sound);
    free((void*)sound);
}

void host_UnloadSoundAlias(wasm_exec_env_t exec_env, unsigned int aliasPtr) {
    Sound* alias = cart_get_pointer(aliasPtr, sizeof(Sound));
    UnloadSoundAlias(*alias);
    free((void*)alias);
}

unsigned int host_ExportWave(wasm_exec_env_t exec_env, unsigned int wavePtr, unsigned int fileNamePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportWave(*wave, fileName);
    free((void*)wave);
    free((void*)fileName);
}

unsigned int host_ExportWaveAsCode(wasm_exec_env_t exec_env, unsigned int wavePtr, unsigned int fileNamePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportWaveAsCode(*wave, fileName);
    free((void*)wave);
    free((void*)fileName);
}

void host_PlaySound(wasm_exec_env_t exec_env, unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    PlaySound(*sound);
    free((void*)sound);
}

void host_StopSound(wasm_exec_env_t exec_env, unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    StopSound(*sound);
    free((void*)sound);
}

void host_PauseSound(wasm_exec_env_t exec_env, unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    PauseSound(*sound);
    free((void*)sound);
}

void host_ResumeSound(wasm_exec_env_t exec_env, unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    ResumeSound(*sound);
    free((void*)sound);
}

unsigned int host_IsSoundPlaying(wasm_exec_env_t exec_env, unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    return IsSoundPlaying(*sound);
    free((void*)sound);
}

void host_SetSoundVolume(wasm_exec_env_t exec_env, unsigned int soundPtr, unsigned int volumePtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    SetSoundVolume(*sound, volumePtr);
    free((void*)sound);
}

void host_SetSoundPitch(wasm_exec_env_t exec_env, unsigned int soundPtr, unsigned int pitchPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    SetSoundPitch(*sound, pitchPtr);
    free((void*)sound);
}

void host_SetSoundPan(wasm_exec_env_t exec_env, unsigned int soundPtr, unsigned int panPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    SetSoundPan(*sound, panPtr);
    free((void*)sound);
}

void host_WaveCopy(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    Wave out = WaveCopy(*wave);
    free((void*)wave);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_WaveCrop(wasm_exec_env_t exec_env, unsigned int wavePtr, unsigned int initFramePtr, unsigned int finalFramePtr) {
    Wave ** wave = cart_get_pointer(wavePtr, sizeof(Wave *));
    WaveCrop(wave, initFramePtr, finalFramePtr);
    free((void*)wave);
}

void host_WaveFormat(wasm_exec_env_t exec_env, unsigned int wavePtr, unsigned int sampleRatePtr, unsigned int sampleSizePtr, unsigned int channelsPtr) {
    Wave ** wave = cart_get_pointer(wavePtr, sizeof(Wave *));
    WaveFormat(wave, sampleRatePtr, sampleSizePtr, channelsPtr);
    free((void*)wave);
}

void host_LoadWaveSamples(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    float * out = LoadWaveSamples(*wave);
    free((void*)wave);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_UnloadWaveSamples(wasm_exec_env_t exec_env, unsigned int samplesPtr) {
    float ** samples = cart_get_pointer(samplesPtr, sizeof(float *));
    UnloadWaveSamples(samples);
    free((void*)samples);
}

void host_LoadMusicStream(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Music out = LoadMusicStream(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

void host_LoadMusicStreamFromMemory(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileTypePtr, unsigned int dataPtr, unsigned int dataSizePtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    Music out = LoadMusicStreamFromMemory(fileType, data, dataSizePtr);
    free((void*)fileType);
    free((void*)data);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsMusicValid(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    return IsMusicValid(*music);
    free((void*)music);
}

void host_UnloadMusicStream(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    UnloadMusicStream(*music);
    free((void*)music);
}

void host_PlayMusicStream(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    PlayMusicStream(*music);
    free((void*)music);
}

unsigned int host_IsMusicStreamPlaying(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    return IsMusicStreamPlaying(*music);
    free((void*)music);
}

void host_UpdateMusicStream(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    UpdateMusicStream(*music);
    free((void*)music);
}

void host_StopMusicStream(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    StopMusicStream(*music);
    free((void*)music);
}

void host_PauseMusicStream(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    PauseMusicStream(*music);
    free((void*)music);
}

void host_ResumeMusicStream(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    ResumeMusicStream(*music);
    free((void*)music);
}

void host_SeekMusicStream(wasm_exec_env_t exec_env, unsigned int musicPtr, unsigned int positionPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    SeekMusicStream(*music, positionPtr);
    free((void*)music);
}

void host_SetMusicVolume(wasm_exec_env_t exec_env, unsigned int musicPtr, unsigned int volumePtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    SetMusicVolume(*music, volumePtr);
    free((void*)music);
}

void host_SetMusicPitch(wasm_exec_env_t exec_env, unsigned int musicPtr, unsigned int pitchPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    SetMusicPitch(*music, pitchPtr);
    free((void*)music);
}

void host_SetMusicPan(wasm_exec_env_t exec_env, unsigned int musicPtr, unsigned int panPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    SetMusicPan(*music, panPtr);
    free((void*)music);
}

unsigned int host_GetMusicTimeLength(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    return GetMusicTimeLength(*music);
    free((void*)music);
}

unsigned int host_GetMusicTimePlayed(wasm_exec_env_t exec_env, unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    return GetMusicTimePlayed(*music);
    free((void*)music);
}

void host_LoadAudioStream(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int sampleRatePtr, unsigned int sampleSizePtr, unsigned int channelsPtr) {
    AudioStream out = LoadAudioStream(sampleRatePtr, sampleSizePtr, channelsPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

unsigned int host_IsAudioStreamValid(wasm_exec_env_t exec_env, unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    return IsAudioStreamValid(*stream);
    free((void*)stream);
}

void host_UnloadAudioStream(wasm_exec_env_t exec_env, unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    UnloadAudioStream(*stream);
    free((void*)stream);
}

void host_UpdateAudioStream(wasm_exec_env_t exec_env, unsigned int streamPtr, unsigned int dataPtr, unsigned int frameCountPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    const void ** data = cart_get_pointer(dataPtr, sizeof(const void *));
    UpdateAudioStream(*stream, data, frameCountPtr);
    free((void*)stream);
    free((void*)data);
}

unsigned int host_IsAudioStreamProcessed(wasm_exec_env_t exec_env, unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    return IsAudioStreamProcessed(*stream);
    free((void*)stream);
}

void host_PlayAudioStream(wasm_exec_env_t exec_env, unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    PlayAudioStream(*stream);
    free((void*)stream);
}

void host_PauseAudioStream(wasm_exec_env_t exec_env, unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    PauseAudioStream(*stream);
    free((void*)stream);
}

void host_ResumeAudioStream(wasm_exec_env_t exec_env, unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    ResumeAudioStream(*stream);
    free((void*)stream);
}

unsigned int host_IsAudioStreamPlaying(wasm_exec_env_t exec_env, unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    return IsAudioStreamPlaying(*stream);
    free((void*)stream);
}

void host_StopAudioStream(wasm_exec_env_t exec_env, unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    StopAudioStream(*stream);
    free((void*)stream);
}

void host_SetAudioStreamVolume(wasm_exec_env_t exec_env, unsigned int streamPtr, unsigned int volumePtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    SetAudioStreamVolume(*stream, volumePtr);
    free((void*)stream);
}

void host_SetAudioStreamPitch(wasm_exec_env_t exec_env, unsigned int streamPtr, unsigned int pitchPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    SetAudioStreamPitch(*stream, pitchPtr);
    free((void*)stream);
}

void host_SetAudioStreamPan(wasm_exec_env_t exec_env, unsigned int streamPtr, unsigned int panPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    SetAudioStreamPan(*stream, panPtr);
    free((void*)stream);
}

void host_SetAudioStreamBufferSizeDefault(wasm_exec_env_t exec_env, unsigned int sizePtr) {
    SetAudioStreamBufferSizeDefault(sizePtr);
}

void host_SetAudioStreamCallback(wasm_exec_env_t exec_env, unsigned int streamPtr, unsigned int callbackPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    AudioCallback* callback = cart_get_pointer(callbackPtr, sizeof(AudioCallback));
    SetAudioStreamCallback(*stream, *callback);
    free((void*)stream);
    free((void*)callback);
}

void host_AttachAudioStreamProcessor(wasm_exec_env_t exec_env, unsigned int streamPtr, unsigned int processorPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    AudioCallback* processor = cart_get_pointer(processorPtr, sizeof(AudioCallback));
    AttachAudioStreamProcessor(*stream, *processor);
    free((void*)stream);
    free((void*)processor);
}

void host_DetachAudioStreamProcessor(wasm_exec_env_t exec_env, unsigned int streamPtr, unsigned int processorPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    AudioCallback* processor = cart_get_pointer(processorPtr, sizeof(AudioCallback));
    DetachAudioStreamProcessor(*stream, *processor);
    free((void*)stream);
    free((void*)processor);
}

void host_AttachAudioMixedProcessor(wasm_exec_env_t exec_env, unsigned int processorPtr) {
    AudioCallback* processor = cart_get_pointer(processorPtr, sizeof(AudioCallback));
    AttachAudioMixedProcessor(*processor);
    free((void*)processor);
}

void host_DetachAudioMixedProcessor(wasm_exec_env_t exec_env, unsigned int processorPtr) {
    AudioCallback* processor = cart_get_pointer(processorPtr, sizeof(AudioCallback));
    DetachAudioMixedProcessor(*processor);
    free((void*)processor);
}


static NativeSymbol native_symbols[] = {
  {"InitWindow", host_InitWindow, NULL},
  {"CloseWindow", host_CloseWindow, NULL},
  {"WindowShouldClose", host_WindowShouldClose, NULL},
  {"IsWindowReady", host_IsWindowReady, NULL},
  {"IsWindowFullscreen", host_IsWindowFullscreen, NULL},
  {"IsWindowHidden", host_IsWindowHidden, NULL},
  {"IsWindowMinimized", host_IsWindowMinimized, NULL},
  {"IsWindowMaximized", host_IsWindowMaximized, NULL},
  {"IsWindowFocused", host_IsWindowFocused, NULL},
  {"IsWindowResized", host_IsWindowResized, NULL},
  {"IsWindowState", host_IsWindowState, NULL},
  {"SetWindowState", host_SetWindowState, NULL},
  {"ClearWindowState", host_ClearWindowState, NULL},
  {"ToggleFullscreen", host_ToggleFullscreen, NULL},
  {"ToggleBorderlessWindowed", host_ToggleBorderlessWindowed, NULL},
  {"MaximizeWindow", host_MaximizeWindow, NULL},
  {"MinimizeWindow", host_MinimizeWindow, NULL},
  {"RestoreWindow", host_RestoreWindow, NULL},
  {"SetWindowIcon", host_SetWindowIcon, NULL},
  {"SetWindowIcons", host_SetWindowIcons, NULL},
  {"SetWindowTitle", host_SetWindowTitle, NULL},
  {"SetWindowPosition", host_SetWindowPosition, NULL},
  {"SetWindowMonitor", host_SetWindowMonitor, NULL},
  {"SetWindowMinSize", host_SetWindowMinSize, NULL},
  {"SetWindowMaxSize", host_SetWindowMaxSize, NULL},
  {"SetWindowSize", host_SetWindowSize, NULL},
  {"SetWindowOpacity", host_SetWindowOpacity, NULL},
  {"SetWindowFocused", host_SetWindowFocused, NULL},
  {"GetWindowHandle", host_GetWindowHandle, NULL},
  {"GetScreenWidth", host_GetScreenWidth, NULL},
  {"GetScreenHeight", host_GetScreenHeight, NULL},
  {"GetRenderWidth", host_GetRenderWidth, NULL},
  {"GetRenderHeight", host_GetRenderHeight, NULL},
  {"GetMonitorCount", host_GetMonitorCount, NULL},
  {"GetCurrentMonitor", host_GetCurrentMonitor, NULL},
  {"GetMonitorPosition", host_GetMonitorPosition, NULL},
  {"GetMonitorWidth", host_GetMonitorWidth, NULL},
  {"GetMonitorHeight", host_GetMonitorHeight, NULL},
  {"GetMonitorPhysicalWidth", host_GetMonitorPhysicalWidth, NULL},
  {"GetMonitorPhysicalHeight", host_GetMonitorPhysicalHeight, NULL},
  {"GetMonitorRefreshRate", host_GetMonitorRefreshRate, NULL},
  {"GetWindowPosition", host_GetWindowPosition, NULL},
  {"GetWindowScaleDPI", host_GetWindowScaleDPI, NULL},
  {"GetMonitorName", host_GetMonitorName, NULL},
  {"SetClipboardText", host_SetClipboardText, NULL},
  {"GetClipboardText", host_GetClipboardText, NULL},
  {"EnableEventWaiting", host_EnableEventWaiting, NULL},
  {"DisableEventWaiting", host_DisableEventWaiting, NULL},
  {"ShowCursor", host_ShowCursor, NULL},
  {"HideCursor", host_HideCursor, NULL},
  {"IsCursorHidden", host_IsCursorHidden, NULL},
  {"EnableCursor", host_EnableCursor, NULL},
  {"DisableCursor", host_DisableCursor, NULL},
  {"IsCursorOnScreen", host_IsCursorOnScreen, NULL},
  {"ClearBackground", host_ClearBackground, NULL},
  {"BeginDrawing", host_BeginDrawing, NULL},
  {"EndDrawing", host_EndDrawing, NULL},
  {"BeginMode2D", host_BeginMode2D, NULL},
  {"EndMode2D", host_EndMode2D, NULL},
  {"BeginMode3D", host_BeginMode3D, NULL},
  {"EndMode3D", host_EndMode3D, NULL},
  {"BeginTextureMode", host_BeginTextureMode, NULL},
  {"EndTextureMode", host_EndTextureMode, NULL},
  {"BeginShaderMode", host_BeginShaderMode, NULL},
  {"EndShaderMode", host_EndShaderMode, NULL},
  {"BeginBlendMode", host_BeginBlendMode, NULL},
  {"EndBlendMode", host_EndBlendMode, NULL},
  {"BeginScissorMode", host_BeginScissorMode, NULL},
  {"EndScissorMode", host_EndScissorMode, NULL},
  {"BeginVrStereoMode", host_BeginVrStereoMode, NULL},
  {"EndVrStereoMode", host_EndVrStereoMode, NULL},
  {"LoadVrStereoConfig", host_LoadVrStereoConfig, NULL},
  {"UnloadVrStereoConfig", host_UnloadVrStereoConfig, NULL},
  {"LoadShader", host_LoadShader, NULL},
  {"LoadShaderFromMemory", host_LoadShaderFromMemory, NULL},
  {"IsShaderValid", host_IsShaderValid, NULL},
  {"GetShaderLocation", host_GetShaderLocation, NULL},
  {"GetShaderLocationAttrib", host_GetShaderLocationAttrib, NULL},
  {"SetShaderValue", host_SetShaderValue, NULL},
  {"SetShaderValueV", host_SetShaderValueV, NULL},
  {"SetShaderValueMatrix", host_SetShaderValueMatrix, NULL},
  {"SetShaderValueTexture", host_SetShaderValueTexture, NULL},
  {"UnloadShader", host_UnloadShader, NULL},
  {"GetScreenToWorldRay", host_GetScreenToWorldRay, NULL},
  {"GetScreenToWorldRayEx", host_GetScreenToWorldRayEx, NULL},
  {"GetWorldToScreen", host_GetWorldToScreen, NULL},
  {"GetWorldToScreenEx", host_GetWorldToScreenEx, NULL},
  {"GetWorldToScreen2D", host_GetWorldToScreen2D, NULL},
  {"GetScreenToWorld2D", host_GetScreenToWorld2D, NULL},
  {"GetCameraMatrix", host_GetCameraMatrix, NULL},
  {"GetCameraMatrix2D", host_GetCameraMatrix2D, NULL},
  {"SetTargetFPS", host_SetTargetFPS, NULL},
  {"GetFrameTime", host_GetFrameTime, NULL},
  {"GetTime", host_GetTime, NULL},
  {"GetFPS", host_GetFPS, NULL},
  {"SwapScreenBuffer", host_SwapScreenBuffer, NULL},
  {"PollInputEvents", host_PollInputEvents, NULL},
  {"WaitTime", host_WaitTime, NULL},
  {"SetRandomSeed", host_SetRandomSeed, NULL},
  {"GetRandomValue", host_GetRandomValue, NULL},
  {"LoadRandomSequence", host_LoadRandomSequence, NULL},
  {"UnloadRandomSequence", host_UnloadRandomSequence, NULL},
  {"TakeScreenshot", host_TakeScreenshot, NULL},
  {"SetConfigFlags", host_SetConfigFlags, NULL},
  {"OpenURL", host_OpenURL, NULL},
  {"SetTraceLogLevel", host_SetTraceLogLevel, NULL},
  {"MemAlloc", host_MemAlloc, NULL},
  {"MemRealloc", host_MemRealloc, NULL},
  {"MemFree", host_MemFree, NULL},
  {"SetTraceLogCallback", host_SetTraceLogCallback, NULL},
  {"SetLoadFileDataCallback", host_SetLoadFileDataCallback, NULL},
  {"SetSaveFileDataCallback", host_SetSaveFileDataCallback, NULL},
  {"SetLoadFileTextCallback", host_SetLoadFileTextCallback, NULL},
  {"SetSaveFileTextCallback", host_SetSaveFileTextCallback, NULL},
  {"LoadFileData", host_LoadFileData, NULL},
  {"UnloadFileData", host_UnloadFileData, NULL},
  {"SaveFileData", host_SaveFileData, NULL},
  {"ExportDataAsCode", host_ExportDataAsCode, NULL},
  {"LoadFileText", host_LoadFileText, NULL},
  {"UnloadFileText", host_UnloadFileText, NULL},
  {"SaveFileText", host_SaveFileText, NULL},
  {"FileExists", host_FileExists, NULL},
  {"DirectoryExists", host_DirectoryExists, NULL},
  {"IsFileExtension", host_IsFileExtension, NULL},
  {"GetFileLength", host_GetFileLength, NULL},
  {"GetFileExtension", host_GetFileExtension, NULL},
  {"GetFileName", host_GetFileName, NULL},
  {"GetFileNameWithoutExt", host_GetFileNameWithoutExt, NULL},
  {"GetDirectoryPath", host_GetDirectoryPath, NULL},
  {"GetPrevDirectoryPath", host_GetPrevDirectoryPath, NULL},
  {"GetWorkingDirectory", host_GetWorkingDirectory, NULL},
  {"GetApplicationDirectory", host_GetApplicationDirectory, NULL},
  {"MakeDirectory", host_MakeDirectory, NULL},
  {"ChangeDirectory", host_ChangeDirectory, NULL},
  {"IsPathFile", host_IsPathFile, NULL},
  {"IsFileNameValid", host_IsFileNameValid, NULL},
  {"LoadDirectoryFiles", host_LoadDirectoryFiles, NULL},
  {"LoadDirectoryFilesEx", host_LoadDirectoryFilesEx, NULL},
  {"UnloadDirectoryFiles", host_UnloadDirectoryFiles, NULL},
  {"IsFileDropped", host_IsFileDropped, NULL},
  {"LoadDroppedFiles", host_LoadDroppedFiles, NULL},
  {"UnloadDroppedFiles", host_UnloadDroppedFiles, NULL},
  {"GetFileModTime", host_GetFileModTime, NULL},
  {"CompressData", host_CompressData, NULL},
  {"DecompressData", host_DecompressData, NULL},
  {"EncodeDataBase64", host_EncodeDataBase64, NULL},
  {"DecodeDataBase64", host_DecodeDataBase64, NULL},
  {"ComputeCRC32", host_ComputeCRC32, NULL},
  {"ComputeMD5", host_ComputeMD5, NULL},
  {"ComputeSHA1", host_ComputeSHA1, NULL},
  {"LoadAutomationEventList", host_LoadAutomationEventList, NULL},
  {"UnloadAutomationEventList", host_UnloadAutomationEventList, NULL},
  {"ExportAutomationEventList", host_ExportAutomationEventList, NULL},
  {"SetAutomationEventList", host_SetAutomationEventList, NULL},
  {"SetAutomationEventBaseFrame", host_SetAutomationEventBaseFrame, NULL},
  {"StartAutomationEventRecording", host_StartAutomationEventRecording, NULL},
  {"StopAutomationEventRecording", host_StopAutomationEventRecording, NULL},
  {"PlayAutomationEvent", host_PlayAutomationEvent, NULL},
  {"IsKeyPressed", host_IsKeyPressed, NULL},
  {"IsKeyPressedRepeat", host_IsKeyPressedRepeat, NULL},
  {"IsKeyDown", host_IsKeyDown, NULL},
  {"IsKeyReleased", host_IsKeyReleased, NULL},
  {"IsKeyUp", host_IsKeyUp, NULL},
  {"GetKeyPressed", host_GetKeyPressed, NULL},
  {"GetCharPressed", host_GetCharPressed, NULL},
  {"SetExitKey", host_SetExitKey, NULL},
  {"IsGamepadAvailable", host_IsGamepadAvailable, NULL},
  {"GetGamepadName", host_GetGamepadName, NULL},
  {"IsGamepadButtonPressed", host_IsGamepadButtonPressed, NULL},
  {"IsGamepadButtonDown", host_IsGamepadButtonDown, NULL},
  {"IsGamepadButtonReleased", host_IsGamepadButtonReleased, NULL},
  {"IsGamepadButtonUp", host_IsGamepadButtonUp, NULL},
  {"GetGamepadButtonPressed", host_GetGamepadButtonPressed, NULL},
  {"GetGamepadAxisCount", host_GetGamepadAxisCount, NULL},
  {"GetGamepadAxisMovement", host_GetGamepadAxisMovement, NULL},
  {"SetGamepadMappings", host_SetGamepadMappings, NULL},
  {"SetGamepadVibration", host_SetGamepadVibration, NULL},
  {"IsMouseButtonPressed", host_IsMouseButtonPressed, NULL},
  {"IsMouseButtonDown", host_IsMouseButtonDown, NULL},
  {"IsMouseButtonReleased", host_IsMouseButtonReleased, NULL},
  {"IsMouseButtonUp", host_IsMouseButtonUp, NULL},
  {"GetMouseX", host_GetMouseX, NULL},
  {"GetMouseY", host_GetMouseY, NULL},
  {"GetMousePosition", host_GetMousePosition, NULL},
  {"GetMouseDelta", host_GetMouseDelta, NULL},
  {"SetMousePosition", host_SetMousePosition, NULL},
  {"SetMouseOffset", host_SetMouseOffset, NULL},
  {"SetMouseScale", host_SetMouseScale, NULL},
  {"GetMouseWheelMove", host_GetMouseWheelMove, NULL},
  {"GetMouseWheelMoveV", host_GetMouseWheelMoveV, NULL},
  {"SetMouseCursor", host_SetMouseCursor, NULL},
  {"GetTouchX", host_GetTouchX, NULL},
  {"GetTouchY", host_GetTouchY, NULL},
  {"GetTouchPosition", host_GetTouchPosition, NULL},
  {"GetTouchPointId", host_GetTouchPointId, NULL},
  {"GetTouchPointCount", host_GetTouchPointCount, NULL},
  {"SetGesturesEnabled", host_SetGesturesEnabled, NULL},
  {"IsGestureDetected", host_IsGestureDetected, NULL},
  {"GetGestureDetected", host_GetGestureDetected, NULL},
  {"GetGestureHoldDuration", host_GetGestureHoldDuration, NULL},
  {"GetGestureDragVector", host_GetGestureDragVector, NULL},
  {"GetGestureDragAngle", host_GetGestureDragAngle, NULL},
  {"GetGesturePinchVector", host_GetGesturePinchVector, NULL},
  {"GetGesturePinchAngle", host_GetGesturePinchAngle, NULL},
  {"UpdateCamera", host_UpdateCamera, NULL},
  {"UpdateCameraPro", host_UpdateCameraPro, NULL},
  {"SetShapesTexture", host_SetShapesTexture, NULL},
  {"GetShapesTexture", host_GetShapesTexture, NULL},
  {"GetShapesTextureRectangle", host_GetShapesTextureRectangle, NULL},
  {"DrawPixel", host_DrawPixel, NULL},
  {"DrawPixelV", host_DrawPixelV, NULL},
  {"DrawLine", host_DrawLine, NULL},
  {"DrawLineV", host_DrawLineV, NULL},
  {"DrawLineEx", host_DrawLineEx, NULL},
  {"DrawLineStrip", host_DrawLineStrip, NULL},
  {"DrawLineBezier", host_DrawLineBezier, NULL},
  {"DrawCircle", host_DrawCircle, NULL},
  {"DrawCircleSector", host_DrawCircleSector, NULL},
  {"DrawCircleSectorLines", host_DrawCircleSectorLines, NULL},
  {"DrawCircleGradient", host_DrawCircleGradient, NULL},
  {"DrawCircleV", host_DrawCircleV, NULL},
  {"DrawCircleLines", host_DrawCircleLines, NULL},
  {"DrawCircleLinesV", host_DrawCircleLinesV, NULL},
  {"DrawEllipse", host_DrawEllipse, NULL},
  {"DrawEllipseLines", host_DrawEllipseLines, NULL},
  {"DrawRing", host_DrawRing, NULL},
  {"DrawRingLines", host_DrawRingLines, NULL},
  {"DrawRectangle", host_DrawRectangle, NULL},
  {"DrawRectangleV", host_DrawRectangleV, NULL},
  {"DrawRectangleRec", host_DrawRectangleRec, NULL},
  {"DrawRectanglePro", host_DrawRectanglePro, NULL},
  {"DrawRectangleGradientV", host_DrawRectangleGradientV, NULL},
  {"DrawRectangleGradientH", host_DrawRectangleGradientH, NULL},
  {"DrawRectangleGradientEx", host_DrawRectangleGradientEx, NULL},
  {"DrawRectangleLines", host_DrawRectangleLines, NULL},
  {"DrawRectangleLinesEx", host_DrawRectangleLinesEx, NULL},
  {"DrawRectangleRounded", host_DrawRectangleRounded, NULL},
  {"DrawRectangleRoundedLines", host_DrawRectangleRoundedLines, NULL},
  {"DrawRectangleRoundedLinesEx", host_DrawRectangleRoundedLinesEx, NULL},
  {"DrawTriangle", host_DrawTriangle, NULL},
  {"DrawTriangleLines", host_DrawTriangleLines, NULL},
  {"DrawTriangleFan", host_DrawTriangleFan, NULL},
  {"DrawTriangleStrip", host_DrawTriangleStrip, NULL},
  {"DrawPoly", host_DrawPoly, NULL},
  {"DrawPolyLines", host_DrawPolyLines, NULL},
  {"DrawPolyLinesEx", host_DrawPolyLinesEx, NULL},
  {"DrawSplineLinear", host_DrawSplineLinear, NULL},
  {"DrawSplineBasis", host_DrawSplineBasis, NULL},
  {"DrawSplineCatmullRom", host_DrawSplineCatmullRom, NULL},
  {"DrawSplineBezierQuadratic", host_DrawSplineBezierQuadratic, NULL},
  {"DrawSplineBezierCubic", host_DrawSplineBezierCubic, NULL},
  {"DrawSplineSegmentLinear", host_DrawSplineSegmentLinear, NULL},
  {"DrawSplineSegmentBasis", host_DrawSplineSegmentBasis, NULL},
  {"DrawSplineSegmentCatmullRom", host_DrawSplineSegmentCatmullRom, NULL},
  {"DrawSplineSegmentBezierQuadratic", host_DrawSplineSegmentBezierQuadratic, NULL},
  {"DrawSplineSegmentBezierCubic", host_DrawSplineSegmentBezierCubic, NULL},
  {"GetSplinePointLinear", host_GetSplinePointLinear, NULL},
  {"GetSplinePointBasis", host_GetSplinePointBasis, NULL},
  {"GetSplinePointCatmullRom", host_GetSplinePointCatmullRom, NULL},
  {"GetSplinePointBezierQuad", host_GetSplinePointBezierQuad, NULL},
  {"GetSplinePointBezierCubic", host_GetSplinePointBezierCubic, NULL},
  {"CheckCollisionRecs", host_CheckCollisionRecs, NULL},
  {"CheckCollisionCircles", host_CheckCollisionCircles, NULL},
  {"CheckCollisionCircleRec", host_CheckCollisionCircleRec, NULL},
  {"CheckCollisionCircleLine", host_CheckCollisionCircleLine, NULL},
  {"CheckCollisionPointRec", host_CheckCollisionPointRec, NULL},
  {"CheckCollisionPointCircle", host_CheckCollisionPointCircle, NULL},
  {"CheckCollisionPointTriangle", host_CheckCollisionPointTriangle, NULL},
  {"CheckCollisionPointLine", host_CheckCollisionPointLine, NULL},
  {"CheckCollisionPointPoly", host_CheckCollisionPointPoly, NULL},
  {"CheckCollisionLines", host_CheckCollisionLines, NULL},
  {"GetCollisionRec", host_GetCollisionRec, NULL},
  {"LoadImage", host_LoadImage, NULL},
  {"LoadImageRaw", host_LoadImageRaw, NULL},
  {"LoadImageAnim", host_LoadImageAnim, NULL},
  {"LoadImageAnimFromMemory", host_LoadImageAnimFromMemory, NULL},
  {"LoadImageFromMemory", host_LoadImageFromMemory, NULL},
  {"LoadImageFromTexture", host_LoadImageFromTexture, NULL},
  {"LoadImageFromScreen", host_LoadImageFromScreen, NULL},
  {"IsImageValid", host_IsImageValid, NULL},
  {"UnloadImage", host_UnloadImage, NULL},
  {"ExportImage", host_ExportImage, NULL},
  {"ExportImageToMemory", host_ExportImageToMemory, NULL},
  {"ExportImageAsCode", host_ExportImageAsCode, NULL},
  {"GenImageColor", host_GenImageColor, NULL},
  {"GenImageGradientLinear", host_GenImageGradientLinear, NULL},
  {"GenImageGradientRadial", host_GenImageGradientRadial, NULL},
  {"GenImageGradientSquare", host_GenImageGradientSquare, NULL},
  {"GenImageChecked", host_GenImageChecked, NULL},
  {"GenImageWhiteNoise", host_GenImageWhiteNoise, NULL},
  {"GenImagePerlinNoise", host_GenImagePerlinNoise, NULL},
  {"GenImageCellular", host_GenImageCellular, NULL},
  {"GenImageText", host_GenImageText, NULL},
  {"ImageCopy", host_ImageCopy, NULL},
  {"ImageFromImage", host_ImageFromImage, NULL},
  {"ImageFromChannel", host_ImageFromChannel, NULL},
  {"ImageText", host_ImageText, NULL},
  {"ImageTextEx", host_ImageTextEx, NULL},
  {"ImageFormat", host_ImageFormat, NULL},
  {"ImageToPOT", host_ImageToPOT, NULL},
  {"ImageCrop", host_ImageCrop, NULL},
  {"ImageAlphaCrop", host_ImageAlphaCrop, NULL},
  {"ImageAlphaClear", host_ImageAlphaClear, NULL},
  {"ImageAlphaMask", host_ImageAlphaMask, NULL},
  {"ImageAlphaPremultiply", host_ImageAlphaPremultiply, NULL},
  {"ImageBlurGaussian", host_ImageBlurGaussian, NULL},
  {"ImageKernelConvolution", host_ImageKernelConvolution, NULL},
  {"ImageResize", host_ImageResize, NULL},
  {"ImageResizeNN", host_ImageResizeNN, NULL},
  {"ImageResizeCanvas", host_ImageResizeCanvas, NULL},
  {"ImageMipmaps", host_ImageMipmaps, NULL},
  {"ImageDither", host_ImageDither, NULL},
  {"ImageFlipVertical", host_ImageFlipVertical, NULL},
  {"ImageFlipHorizontal", host_ImageFlipHorizontal, NULL},
  {"ImageRotate", host_ImageRotate, NULL},
  {"ImageRotateCW", host_ImageRotateCW, NULL},
  {"ImageRotateCCW", host_ImageRotateCCW, NULL},
  {"ImageColorTint", host_ImageColorTint, NULL},
  {"ImageColorInvert", host_ImageColorInvert, NULL},
  {"ImageColorGrayscale", host_ImageColorGrayscale, NULL},
  {"ImageColorContrast", host_ImageColorContrast, NULL},
  {"ImageColorBrightness", host_ImageColorBrightness, NULL},
  {"ImageColorReplace", host_ImageColorReplace, NULL},
  {"LoadImageColors", host_LoadImageColors, NULL},
  {"LoadImagePalette", host_LoadImagePalette, NULL},
  {"UnloadImageColors", host_UnloadImageColors, NULL},
  {"UnloadImagePalette", host_UnloadImagePalette, NULL},
  {"GetImageAlphaBorder", host_GetImageAlphaBorder, NULL},
  {"GetImageColor", host_GetImageColor, NULL},
  {"ImageClearBackground", host_ImageClearBackground, NULL},
  {"ImageDrawPixel", host_ImageDrawPixel, NULL},
  {"ImageDrawPixelV", host_ImageDrawPixelV, NULL},
  {"ImageDrawLine", host_ImageDrawLine, NULL},
  {"ImageDrawLineV", host_ImageDrawLineV, NULL},
  {"ImageDrawLineEx", host_ImageDrawLineEx, NULL},
  {"ImageDrawCircle", host_ImageDrawCircle, NULL},
  {"ImageDrawCircleV", host_ImageDrawCircleV, NULL},
  {"ImageDrawCircleLines", host_ImageDrawCircleLines, NULL},
  {"ImageDrawCircleLinesV", host_ImageDrawCircleLinesV, NULL},
  {"ImageDrawRectangle", host_ImageDrawRectangle, NULL},
  {"ImageDrawRectangleV", host_ImageDrawRectangleV, NULL},
  {"ImageDrawRectangleRec", host_ImageDrawRectangleRec, NULL},
  {"ImageDrawRectangleLines", host_ImageDrawRectangleLines, NULL},
  {"ImageDrawTriangle", host_ImageDrawTriangle, NULL},
  {"ImageDrawTriangleEx", host_ImageDrawTriangleEx, NULL},
  {"ImageDrawTriangleLines", host_ImageDrawTriangleLines, NULL},
  {"ImageDrawTriangleFan", host_ImageDrawTriangleFan, NULL},
  {"ImageDrawTriangleStrip", host_ImageDrawTriangleStrip, NULL},
  {"ImageDraw", host_ImageDraw, NULL},
  {"ImageDrawText", host_ImageDrawText, NULL},
  {"ImageDrawTextEx", host_ImageDrawTextEx, NULL},
  {"LoadTexture", host_LoadTexture, NULL},
  {"LoadTextureFromImage", host_LoadTextureFromImage, NULL},
  {"LoadTextureCubemap", host_LoadTextureCubemap, NULL},
  {"LoadRenderTexture", host_LoadRenderTexture, NULL},
  {"IsTextureValid", host_IsTextureValid, NULL},
  {"UnloadTexture", host_UnloadTexture, NULL},
  {"IsRenderTextureValid", host_IsRenderTextureValid, NULL},
  {"UnloadRenderTexture", host_UnloadRenderTexture, NULL},
  {"UpdateTexture", host_UpdateTexture, NULL},
  {"UpdateTextureRec", host_UpdateTextureRec, NULL},
  {"GenTextureMipmaps", host_GenTextureMipmaps, NULL},
  {"SetTextureFilter", host_SetTextureFilter, NULL},
  {"SetTextureWrap", host_SetTextureWrap, NULL},
  {"DrawTexture", host_DrawTexture, NULL},
  {"DrawTextureV", host_DrawTextureV, NULL},
  {"DrawTextureEx", host_DrawTextureEx, NULL},
  {"DrawTextureRec", host_DrawTextureRec, NULL},
  {"DrawTexturePro", host_DrawTexturePro, NULL},
  {"DrawTextureNPatch", host_DrawTextureNPatch, NULL},
  {"ColorIsEqual", host_ColorIsEqual, NULL},
  {"Fade", host_Fade, NULL},
  {"ColorToInt", host_ColorToInt, NULL},
  {"ColorNormalize", host_ColorNormalize, NULL},
  {"ColorFromNormalized", host_ColorFromNormalized, NULL},
  {"ColorToHSV", host_ColorToHSV, NULL},
  {"ColorFromHSV", host_ColorFromHSV, NULL},
  {"ColorTint", host_ColorTint, NULL},
  {"ColorBrightness", host_ColorBrightness, NULL},
  {"ColorContrast", host_ColorContrast, NULL},
  {"ColorAlpha", host_ColorAlpha, NULL},
  {"ColorAlphaBlend", host_ColorAlphaBlend, NULL},
  {"ColorLerp", host_ColorLerp, NULL},
  {"GetColor", host_GetColor, NULL},
  {"GetPixelColor", host_GetPixelColor, NULL},
  {"SetPixelColor", host_SetPixelColor, NULL},
  {"GetPixelDataSize", host_GetPixelDataSize, NULL},
  {"GetFontDefault", host_GetFontDefault, NULL},
  {"LoadFont", host_LoadFont, NULL},
  {"LoadFontEx", host_LoadFontEx, NULL},
  {"LoadFontFromImage", host_LoadFontFromImage, NULL},
  {"LoadFontFromMemory", host_LoadFontFromMemory, NULL},
  {"IsFontValid", host_IsFontValid, NULL},
  {"LoadFontData", host_LoadFontData, NULL},
  {"GenImageFontAtlas", host_GenImageFontAtlas, NULL},
  {"UnloadFontData", host_UnloadFontData, NULL},
  {"UnloadFont", host_UnloadFont, NULL},
  {"ExportFontAsCode", host_ExportFontAsCode, NULL},
  {"DrawFPS", host_DrawFPS, NULL},
  {"DrawText", host_DrawText, NULL},
  {"DrawTextEx", host_DrawTextEx, NULL},
  {"DrawTextPro", host_DrawTextPro, NULL},
  {"DrawTextCodepoint", host_DrawTextCodepoint, NULL},
  {"DrawTextCodepoints", host_DrawTextCodepoints, NULL},
  {"SetTextLineSpacing", host_SetTextLineSpacing, NULL},
  {"MeasureText", host_MeasureText, NULL},
  {"MeasureTextEx", host_MeasureTextEx, NULL},
  {"GetGlyphIndex", host_GetGlyphIndex, NULL},
  {"GetGlyphInfo", host_GetGlyphInfo, NULL},
  {"GetGlyphAtlasRec", host_GetGlyphAtlasRec, NULL},
  {"LoadUTF8", host_LoadUTF8, NULL},
  {"UnloadUTF8", host_UnloadUTF8, NULL},
  {"LoadCodepoints", host_LoadCodepoints, NULL},
  {"UnloadCodepoints", host_UnloadCodepoints, NULL},
  {"GetCodepointCount", host_GetCodepointCount, NULL},
  {"GetCodepoint", host_GetCodepoint, NULL},
  {"GetCodepointNext", host_GetCodepointNext, NULL},
  {"GetCodepointPrevious", host_GetCodepointPrevious, NULL},
  {"CodepointToUTF8", host_CodepointToUTF8, NULL},
  {"TextCopy", host_TextCopy, NULL},
  {"TextIsEqual", host_TextIsEqual, NULL},
  {"TextLength", host_TextLength, NULL},
  {"TextSubtext", host_TextSubtext, NULL},
  {"TextReplace", host_TextReplace, NULL},
  {"TextInsert", host_TextInsert, NULL},
  {"TextJoin", host_TextJoin, NULL},
  {"TextSplit", host_TextSplit, NULL},
  {"TextAppend", host_TextAppend, NULL},
  {"TextFindIndex", host_TextFindIndex, NULL},
  {"TextToUpper", host_TextToUpper, NULL},
  {"TextToLower", host_TextToLower, NULL},
  {"TextToPascal", host_TextToPascal, NULL},
  {"TextToSnake", host_TextToSnake, NULL},
  {"TextToCamel", host_TextToCamel, NULL},
  {"TextToInteger", host_TextToInteger, NULL},
  {"TextToFloat", host_TextToFloat, NULL},
  {"DrawLine3D", host_DrawLine3D, NULL},
  {"DrawPoint3D", host_DrawPoint3D, NULL},
  {"DrawCircle3D", host_DrawCircle3D, NULL},
  {"DrawTriangle3D", host_DrawTriangle3D, NULL},
  {"DrawTriangleStrip3D", host_DrawTriangleStrip3D, NULL},
  {"DrawCube", host_DrawCube, NULL},
  {"DrawCubeV", host_DrawCubeV, NULL},
  {"DrawCubeWires", host_DrawCubeWires, NULL},
  {"DrawCubeWiresV", host_DrawCubeWiresV, NULL},
  {"DrawSphere", host_DrawSphere, NULL},
  {"DrawSphereEx", host_DrawSphereEx, NULL},
  {"DrawSphereWires", host_DrawSphereWires, NULL},
  {"DrawCylinder", host_DrawCylinder, NULL},
  {"DrawCylinderEx", host_DrawCylinderEx, NULL},
  {"DrawCylinderWires", host_DrawCylinderWires, NULL},
  {"DrawCylinderWiresEx", host_DrawCylinderWiresEx, NULL},
  {"DrawCapsule", host_DrawCapsule, NULL},
  {"DrawCapsuleWires", host_DrawCapsuleWires, NULL},
  {"DrawPlane", host_DrawPlane, NULL},
  {"DrawRay", host_DrawRay, NULL},
  {"DrawGrid", host_DrawGrid, NULL},
  {"LoadModel", host_LoadModel, NULL},
  {"LoadModelFromMesh", host_LoadModelFromMesh, NULL},
  {"IsModelValid", host_IsModelValid, NULL},
  {"UnloadModel", host_UnloadModel, NULL},
  {"GetModelBoundingBox", host_GetModelBoundingBox, NULL},
  {"DrawModel", host_DrawModel, NULL},
  {"DrawModelEx", host_DrawModelEx, NULL},
  {"DrawModelWires", host_DrawModelWires, NULL},
  {"DrawModelWiresEx", host_DrawModelWiresEx, NULL},
  {"DrawModelPoints", host_DrawModelPoints, NULL},
  {"DrawModelPointsEx", host_DrawModelPointsEx, NULL},
  {"DrawBoundingBox", host_DrawBoundingBox, NULL},
  {"DrawBillboard", host_DrawBillboard, NULL},
  {"DrawBillboardRec", host_DrawBillboardRec, NULL},
  {"DrawBillboardPro", host_DrawBillboardPro, NULL},
  {"UploadMesh", host_UploadMesh, NULL},
  {"UpdateMeshBuffer", host_UpdateMeshBuffer, NULL},
  {"UnloadMesh", host_UnloadMesh, NULL},
  {"DrawMesh", host_DrawMesh, NULL},
  {"DrawMeshInstanced", host_DrawMeshInstanced, NULL},
  {"GetMeshBoundingBox", host_GetMeshBoundingBox, NULL},
  {"GenMeshTangents", host_GenMeshTangents, NULL},
  {"ExportMesh", host_ExportMesh, NULL},
  {"ExportMeshAsCode", host_ExportMeshAsCode, NULL},
  {"GenMeshPoly", host_GenMeshPoly, NULL},
  {"GenMeshPlane", host_GenMeshPlane, NULL},
  {"GenMeshCube", host_GenMeshCube, NULL},
  {"GenMeshSphere", host_GenMeshSphere, NULL},
  {"GenMeshHemiSphere", host_GenMeshHemiSphere, NULL},
  {"GenMeshCylinder", host_GenMeshCylinder, NULL},
  {"GenMeshCone", host_GenMeshCone, NULL},
  {"GenMeshTorus", host_GenMeshTorus, NULL},
  {"GenMeshKnot", host_GenMeshKnot, NULL},
  {"GenMeshHeightmap", host_GenMeshHeightmap, NULL},
  {"GenMeshCubicmap", host_GenMeshCubicmap, NULL},
  {"LoadMaterials", host_LoadMaterials, NULL},
  {"LoadMaterialDefault", host_LoadMaterialDefault, NULL},
  {"IsMaterialValid", host_IsMaterialValid, NULL},
  {"UnloadMaterial", host_UnloadMaterial, NULL},
  {"SetMaterialTexture", host_SetMaterialTexture, NULL},
  {"SetModelMeshMaterial", host_SetModelMeshMaterial, NULL},
  {"LoadModelAnimations", host_LoadModelAnimations, NULL},
  {"UpdateModelAnimation", host_UpdateModelAnimation, NULL},
  {"UpdateModelAnimationBones", host_UpdateModelAnimationBones, NULL},
  {"UnloadModelAnimation", host_UnloadModelAnimation, NULL},
  {"UnloadModelAnimations", host_UnloadModelAnimations, NULL},
  {"IsModelAnimationValid", host_IsModelAnimationValid, NULL},
  {"CheckCollisionSpheres", host_CheckCollisionSpheres, NULL},
  {"CheckCollisionBoxes", host_CheckCollisionBoxes, NULL},
  {"CheckCollisionBoxSphere", host_CheckCollisionBoxSphere, NULL},
  {"GetRayCollisionSphere", host_GetRayCollisionSphere, NULL},
  {"GetRayCollisionBox", host_GetRayCollisionBox, NULL},
  {"GetRayCollisionMesh", host_GetRayCollisionMesh, NULL},
  {"GetRayCollisionTriangle", host_GetRayCollisionTriangle, NULL},
  {"GetRayCollisionQuad", host_GetRayCollisionQuad, NULL},
  {"InitAudioDevice", host_InitAudioDevice, NULL},
  {"CloseAudioDevice", host_CloseAudioDevice, NULL},
  {"IsAudioDeviceReady", host_IsAudioDeviceReady, NULL},
  {"SetMasterVolume", host_SetMasterVolume, NULL},
  {"GetMasterVolume", host_GetMasterVolume, NULL},
  {"LoadWave", host_LoadWave, NULL},
  {"LoadWaveFromMemory", host_LoadWaveFromMemory, NULL},
  {"IsWaveValid", host_IsWaveValid, NULL},
  {"LoadSound", host_LoadSound, NULL},
  {"LoadSoundFromWave", host_LoadSoundFromWave, NULL},
  {"LoadSoundAlias", host_LoadSoundAlias, NULL},
  {"IsSoundValid", host_IsSoundValid, NULL},
  {"UpdateSound", host_UpdateSound, NULL},
  {"UnloadWave", host_UnloadWave, NULL},
  {"UnloadSound", host_UnloadSound, NULL},
  {"UnloadSoundAlias", host_UnloadSoundAlias, NULL},
  {"ExportWave", host_ExportWave, NULL},
  {"ExportWaveAsCode", host_ExportWaveAsCode, NULL},
  {"PlaySound", host_PlaySound, NULL},
  {"StopSound", host_StopSound, NULL},
  {"PauseSound", host_PauseSound, NULL},
  {"ResumeSound", host_ResumeSound, NULL},
  {"IsSoundPlaying", host_IsSoundPlaying, NULL},
  {"SetSoundVolume", host_SetSoundVolume, NULL},
  {"SetSoundPitch", host_SetSoundPitch, NULL},
  {"SetSoundPan", host_SetSoundPan, NULL},
  {"WaveCopy", host_WaveCopy, NULL},
  {"WaveCrop", host_WaveCrop, NULL},
  {"WaveFormat", host_WaveFormat, NULL},
  {"LoadWaveSamples", host_LoadWaveSamples, NULL},
  {"UnloadWaveSamples", host_UnloadWaveSamples, NULL},
  {"LoadMusicStream", host_LoadMusicStream, NULL},
  {"LoadMusicStreamFromMemory", host_LoadMusicStreamFromMemory, NULL},
  {"IsMusicValid", host_IsMusicValid, NULL},
  {"UnloadMusicStream", host_UnloadMusicStream, NULL},
  {"PlayMusicStream", host_PlayMusicStream, NULL},
  {"IsMusicStreamPlaying", host_IsMusicStreamPlaying, NULL},
  {"UpdateMusicStream", host_UpdateMusicStream, NULL},
  {"StopMusicStream", host_StopMusicStream, NULL},
  {"PauseMusicStream", host_PauseMusicStream, NULL},
  {"ResumeMusicStream", host_ResumeMusicStream, NULL},
  {"SeekMusicStream", host_SeekMusicStream, NULL},
  {"SetMusicVolume", host_SetMusicVolume, NULL},
  {"SetMusicPitch", host_SetMusicPitch, NULL},
  {"SetMusicPan", host_SetMusicPan, NULL},
  {"GetMusicTimeLength", host_GetMusicTimeLength, NULL},
  {"GetMusicTimePlayed", host_GetMusicTimePlayed, NULL},
  {"LoadAudioStream", host_LoadAudioStream, NULL},
  {"IsAudioStreamValid", host_IsAudioStreamValid, NULL},
  {"UnloadAudioStream", host_UnloadAudioStream, NULL},
  {"UpdateAudioStream", host_UpdateAudioStream, NULL},
  {"IsAudioStreamProcessed", host_IsAudioStreamProcessed, NULL},
  {"PlayAudioStream", host_PlayAudioStream, NULL},
  {"PauseAudioStream", host_PauseAudioStream, NULL},
  {"ResumeAudioStream", host_ResumeAudioStream, NULL},
  {"IsAudioStreamPlaying", host_IsAudioStreamPlaying, NULL},
  {"StopAudioStream", host_StopAudioStream, NULL},
  {"SetAudioStreamVolume", host_SetAudioStreamVolume, NULL},
  {"SetAudioStreamPitch", host_SetAudioStreamPitch, NULL},
  {"SetAudioStreamPan", host_SetAudioStreamPan, NULL},
  {"SetAudioStreamBufferSizeDefault", host_SetAudioStreamBufferSizeDefault, NULL},
  {"SetAudioStreamCallback", host_SetAudioStreamCallback, NULL},
  {"AttachAudioStreamProcessor", host_AttachAudioStreamProcessor, NULL},
  {"DetachAudioStreamProcessor", host_DetachAudioStreamProcessor, NULL},
  {"AttachAudioMixedProcessor", host_AttachAudioMixedProcessor, NULL},
  {"DetachAudioMixedProcessor", host_DetachAudioMixedProcessor, NULL}
};


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
