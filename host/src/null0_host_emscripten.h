#pragma once
#include <emscripten.h>
#include <string.h>

// allocate cart-memory from host C
EM_JS(unsigned int, cart_malloc, (int size), {
  return Module.cart.malloc(size);
});

// free cart-memory from host C
EM_JS(void, cart_free, (unsigned int ptr), {
  Module.cart.free(ptr);
});

// copy a pointer from cart to host
EM_JS(void*, cart_get_pointer, (unsigned int cartPtr, unsigned int len), {
  const out = Module._malloc(len);
  const cartMem = new Uint8Array(Module.cart.memory.buffer).slice(cartPtr, cartPtr + len);
  Module.HEAPU8.set(cartMem, out);
  return out;
});

// copy a pointer from host to cart
EM_JS(unsigned int, cart_set_pointer, (void* hostPtr, unsigned int len, unsigned int cartPtr), {
  cartPtr = cartPtr || Module.cart.malloc(len);
  new Uint8Array(Module.cart.memory.buffer).set(Module.HEAPU8.slice(hostPtr, hostPtr + len), cartPtr);
  return cartPtr;
});

// copy a string from cart to host
EM_JS(char*, cart_get_string, (unsigned int cartPtr), {
  const cartMem = new Uint8Array(Module.cart.memory.buffer.slice(cartPtr));
  const len = cartMem.findIndex(b => b === 0);
  if (len !== -1) {
    return cart_get_pointer(cartPtr, len + 1);
  }
});

// copy a string from host to cart
EM_JS(unsigned int, cart_set_string, (char* hostPtr), {
  const hostMem = Module.HEAPU8.slice(hostPtr);
  const len = hostMem.findIndex(b => b === 0);
  if (len !== -1) {
    return cart_set_pointer(hostPtr, len + 1);
  }
});

EMSCRIPTEN_KEEPALIVE void host_InitWindow(unsigned int widthPtr, unsigned int heightPtr, unsigned int titlePtr) {
    const char* title = cart_get_string(titlePtr);
    InitWindow(widthPtr, heightPtr, title);
    free((void*)title);
}

EMSCRIPTEN_KEEPALIVE void host_CloseWindow() {
    CloseWindow();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_WindowShouldClose() {
    return WindowShouldClose();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWindowReady() {
    return IsWindowReady();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWindowFullscreen() {
    return IsWindowFullscreen();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWindowHidden() {
    return IsWindowHidden();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWindowMinimized() {
    return IsWindowMinimized();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWindowMaximized() {
    return IsWindowMaximized();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWindowFocused() {
    return IsWindowFocused();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWindowResized() {
    return IsWindowResized();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWindowState(unsigned int flagPtr) {
    return IsWindowState(flagPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowState(unsigned int flagsPtr) {
    SetWindowState(flagsPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ClearWindowState(unsigned int flagsPtr) {
    ClearWindowState(flagsPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ToggleFullscreen() {
    ToggleFullscreen();
}

EMSCRIPTEN_KEEPALIVE void host_ToggleBorderlessWindowed() {
    ToggleBorderlessWindowed();
}

EMSCRIPTEN_KEEPALIVE void host_MaximizeWindow() {
    MaximizeWindow();
}

EMSCRIPTEN_KEEPALIVE void host_MinimizeWindow() {
    MinimizeWindow();
}

EMSCRIPTEN_KEEPALIVE void host_RestoreWindow() {
    RestoreWindow();
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowIcon(unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    SetWindowIcon(*image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowIcons(unsigned int imagesPtr, unsigned int countPtr) {
    Image ** images = cart_get_pointer(imagesPtr, sizeof(Image *));
    SetWindowIcons(images, countPtr);
    free((void*)images);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowTitle(unsigned int titlePtr) {
    const char* title = cart_get_string(titlePtr);
    SetWindowTitle(title);
    free((void*)title);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowPosition(unsigned int xPtr, unsigned int yPtr) {
    SetWindowPosition(xPtr, yPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowMonitor(unsigned int monitorPtr) {
    SetWindowMonitor(monitorPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowMinSize(unsigned int widthPtr, unsigned int heightPtr) {
    SetWindowMinSize(widthPtr, heightPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowMaxSize(unsigned int widthPtr, unsigned int heightPtr) {
    SetWindowMaxSize(widthPtr, heightPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowSize(unsigned int widthPtr, unsigned int heightPtr) {
    SetWindowSize(widthPtr, heightPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowOpacity(unsigned int opacityPtr) {
    SetWindowOpacity(opacityPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowFocused() {
    SetWindowFocused();
}

EMSCRIPTEN_KEEPALIVE void host_GetWindowHandle(unsigned int outPtr) {
    void * out = GetWindowHandle();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetScreenWidth() {
    return GetScreenWidth();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetScreenHeight() {
    return GetScreenHeight();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetRenderWidth() {
    return GetRenderWidth();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetRenderHeight() {
    return GetRenderHeight();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMonitorCount() {
    return GetMonitorCount();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetCurrentMonitor() {
    return GetCurrentMonitor();
}

EMSCRIPTEN_KEEPALIVE void host_GetMonitorPosition(unsigned int outPtr, unsigned int monitorPtr) {
    Vector2 out = GetMonitorPosition(monitorPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMonitorWidth(unsigned int monitorPtr) {
    return GetMonitorWidth(monitorPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMonitorHeight(unsigned int monitorPtr) {
    return GetMonitorHeight(monitorPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMonitorPhysicalWidth(unsigned int monitorPtr) {
    return GetMonitorPhysicalWidth(monitorPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMonitorPhysicalHeight(unsigned int monitorPtr) {
    return GetMonitorPhysicalHeight(monitorPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMonitorRefreshRate(unsigned int monitorPtr) {
    return GetMonitorRefreshRate(monitorPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetWindowPosition(unsigned int outPtr) {
    Vector2 out = GetWindowPosition();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetWindowScaleDPI(unsigned int outPtr) {
    Vector2 out = GetWindowScaleDPI();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetMonitorName(unsigned int outPtr, unsigned int monitorPtr) {
    const char * out = GetMonitorName(monitorPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetClipboardText(unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    SetClipboardText(text);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE void host_GetClipboardText(unsigned int outPtr) {
    const char * out = GetClipboardText();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_EnableEventWaiting() {
    EnableEventWaiting();
}

EMSCRIPTEN_KEEPALIVE void host_DisableEventWaiting() {
    DisableEventWaiting();
}

EMSCRIPTEN_KEEPALIVE void host_ShowCursor() {
    ShowCursor();
}

EMSCRIPTEN_KEEPALIVE void host_HideCursor() {
    HideCursor();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsCursorHidden() {
    return IsCursorHidden();
}

EMSCRIPTEN_KEEPALIVE void host_EnableCursor() {
    EnableCursor();
}

EMSCRIPTEN_KEEPALIVE void host_DisableCursor() {
    DisableCursor();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsCursorOnScreen() {
    return IsCursorOnScreen();
}

EMSCRIPTEN_KEEPALIVE void host_ClearBackground(unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ClearBackground(*color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_BeginDrawing() {
    BeginDrawing();
}

EMSCRIPTEN_KEEPALIVE void host_EndDrawing() {
    EndDrawing();
}

EMSCRIPTEN_KEEPALIVE void host_BeginMode2D(unsigned int cameraPtr) {
    Camera2D* camera = cart_get_pointer(cameraPtr, sizeof(Camera2D));
    BeginMode2D(*camera);
    free((void*)camera);
}

EMSCRIPTEN_KEEPALIVE void host_EndMode2D() {
    EndMode2D();
}

EMSCRIPTEN_KEEPALIVE void host_BeginMode3D(unsigned int cameraPtr) {
    Camera3D* camera = cart_get_pointer(cameraPtr, sizeof(Camera3D));
    BeginMode3D(*camera);
    free((void*)camera);
}

EMSCRIPTEN_KEEPALIVE void host_EndMode3D() {
    EndMode3D();
}

EMSCRIPTEN_KEEPALIVE void host_BeginTextureMode(unsigned int targetPtr) {
    RenderTexture2D* target = cart_get_pointer(targetPtr, sizeof(RenderTexture2D));
    BeginTextureMode(*target);
    free((void*)target);
}

EMSCRIPTEN_KEEPALIVE void host_EndTextureMode() {
    EndTextureMode();
}

EMSCRIPTEN_KEEPALIVE void host_BeginShaderMode(unsigned int shaderPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    BeginShaderMode(*shader);
    free((void*)shader);
}

EMSCRIPTEN_KEEPALIVE void host_EndShaderMode() {
    EndShaderMode();
}

EMSCRIPTEN_KEEPALIVE void host_BeginBlendMode(unsigned int modePtr) {
    BeginBlendMode(modePtr);
}

EMSCRIPTEN_KEEPALIVE void host_EndBlendMode() {
    EndBlendMode();
}

EMSCRIPTEN_KEEPALIVE void host_BeginScissorMode(unsigned int xPtr, unsigned int yPtr, unsigned int widthPtr, unsigned int heightPtr) {
    BeginScissorMode(xPtr, yPtr, widthPtr, heightPtr);
}

EMSCRIPTEN_KEEPALIVE void host_EndScissorMode() {
    EndScissorMode();
}

EMSCRIPTEN_KEEPALIVE void host_BeginVrStereoMode(unsigned int configPtr) {
    VrStereoConfig* config = cart_get_pointer(configPtr, sizeof(VrStereoConfig));
    BeginVrStereoMode(*config);
    free((void*)config);
}

EMSCRIPTEN_KEEPALIVE void host_EndVrStereoMode() {
    EndVrStereoMode();
}

EMSCRIPTEN_KEEPALIVE void host_LoadVrStereoConfig(unsigned int outPtr, unsigned int devicePtr) {
    VrDeviceInfo* device = cart_get_pointer(devicePtr, sizeof(VrDeviceInfo));
    VrStereoConfig out = LoadVrStereoConfig(*device);
    free((void*)device);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadVrStereoConfig(unsigned int configPtr) {
    VrStereoConfig* config = cart_get_pointer(configPtr, sizeof(VrStereoConfig));
    UnloadVrStereoConfig(*config);
    free((void*)config);
}

EMSCRIPTEN_KEEPALIVE void host_LoadShader(unsigned int outPtr, unsigned int vsFileNamePtr, unsigned int fsFileNamePtr) {
    const char* vsFileName = cart_get_string(vsFileNamePtr);
    const char* fsFileName = cart_get_string(fsFileNamePtr);
    Shader out = LoadShader(vsFileName, fsFileName);
    free((void*)vsFileName);
    free((void*)fsFileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadShaderFromMemory(unsigned int outPtr, unsigned int vsCodePtr, unsigned int fsCodePtr) {
    const char* vsCode = cart_get_string(vsCodePtr);
    const char* fsCode = cart_get_string(fsCodePtr);
    Shader out = LoadShaderFromMemory(vsCode, fsCode);
    free((void*)vsCode);
    free((void*)fsCode);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsShaderValid(unsigned int shaderPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    return IsShaderValid(*shader);
    free((void*)shader);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetShaderLocation(unsigned int shaderPtr, unsigned int uniformNamePtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    const char* uniformName = cart_get_string(uniformNamePtr);
    return GetShaderLocation(*shader, uniformName);
    free((void*)shader);
    free((void*)uniformName);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetShaderLocationAttrib(unsigned int shaderPtr, unsigned int attribNamePtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    const char* attribName = cart_get_string(attribNamePtr);
    return GetShaderLocationAttrib(*shader, attribName);
    free((void*)shader);
    free((void*)attribName);
}

EMSCRIPTEN_KEEPALIVE void host_SetShaderValue(unsigned int shaderPtr, unsigned int locIndexPtr, unsigned int valuePtr, unsigned int uniformTypePtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    const void ** value = cart_get_pointer(valuePtr, sizeof(const void *));
    SetShaderValue(*shader, locIndexPtr, value, uniformTypePtr);
    free((void*)shader);
    free((void*)value);
}

EMSCRIPTEN_KEEPALIVE void host_SetShaderValueV(unsigned int shaderPtr, unsigned int locIndexPtr, unsigned int valuePtr, unsigned int uniformTypePtr, unsigned int countPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    const void ** value = cart_get_pointer(valuePtr, sizeof(const void *));
    SetShaderValueV(*shader, locIndexPtr, value, uniformTypePtr, countPtr);
    free((void*)shader);
    free((void*)value);
}

EMSCRIPTEN_KEEPALIVE void host_SetShaderValueMatrix(unsigned int shaderPtr, unsigned int locIndexPtr, unsigned int matPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    Matrix* mat = cart_get_pointer(matPtr, sizeof(Matrix));
    SetShaderValueMatrix(*shader, locIndexPtr, *mat);
    free((void*)shader);
    free((void*)mat);
}

EMSCRIPTEN_KEEPALIVE void host_SetShaderValueTexture(unsigned int shaderPtr, unsigned int locIndexPtr, unsigned int texturePtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    SetShaderValueTexture(*shader, locIndexPtr, *texture);
    free((void*)shader);
    free((void*)texture);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadShader(unsigned int shaderPtr) {
    Shader* shader = cart_get_pointer(shaderPtr, sizeof(Shader));
    UnloadShader(*shader);
    free((void*)shader);
}

EMSCRIPTEN_KEEPALIVE void host_GetScreenToWorldRay(unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Ray out = GetScreenToWorldRay(*position, *camera);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetScreenToWorldRayEx(unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr, unsigned int widthPtr, unsigned int heightPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Ray out = GetScreenToWorldRayEx(*position, *camera, widthPtr, heightPtr);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetWorldToScreen(unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Vector2 out = GetWorldToScreen(*position, *camera);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetWorldToScreenEx(unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr, unsigned int widthPtr, unsigned int heightPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Vector2 out = GetWorldToScreenEx(*position, *camera, widthPtr, heightPtr);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetWorldToScreen2D(unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Camera2D* camera = cart_get_pointer(cameraPtr, sizeof(Camera2D));
    Vector2 out = GetWorldToScreen2D(*position, *camera);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetScreenToWorld2D(unsigned int outPtr, unsigned int positionPtr, unsigned int cameraPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Camera2D* camera = cart_get_pointer(cameraPtr, sizeof(Camera2D));
    Vector2 out = GetScreenToWorld2D(*position, *camera);
    free((void*)position);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetCameraMatrix(unsigned int outPtr, unsigned int cameraPtr) {
    Camera* camera = cart_get_pointer(cameraPtr, sizeof(Camera));
    Matrix out = GetCameraMatrix(*camera);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetCameraMatrix2D(unsigned int outPtr, unsigned int cameraPtr) {
    Camera2D* camera = cart_get_pointer(cameraPtr, sizeof(Camera2D));
    Matrix out = GetCameraMatrix2D(*camera);
    free((void*)camera);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetTargetFPS(unsigned int fpsPtr) {
    SetTargetFPS(fpsPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetFrameTime() {
    return GetFrameTime();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetTime() {
    return GetTime();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetFPS() {
    return GetFPS();
}

EMSCRIPTEN_KEEPALIVE void host_SwapScreenBuffer() {
    SwapScreenBuffer();
}

EMSCRIPTEN_KEEPALIVE void host_PollInputEvents() {
    PollInputEvents();
}

EMSCRIPTEN_KEEPALIVE void host_WaitTime(unsigned int secondsPtr) {
    WaitTime(secondsPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetRandomSeed(unsigned int seedPtr) {
    SetRandomSeed(seedPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetRandomValue(unsigned int minPtr, unsigned int maxPtr) {
    return GetRandomValue(minPtr, maxPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadRandomSequence(unsigned int outPtr, unsigned int countPtr, unsigned int minPtr, unsigned int maxPtr) {
    int * out = LoadRandomSequence(countPtr, minPtr, maxPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadRandomSequence(unsigned int sequencePtr) {
    int ** sequence = cart_get_pointer(sequencePtr, sizeof(int *));
    UnloadRandomSequence(sequence);
    free((void*)sequence);
}

EMSCRIPTEN_KEEPALIVE void host_TakeScreenshot(unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    TakeScreenshot(fileName);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_SetConfigFlags(unsigned int flagsPtr) {
    SetConfigFlags(flagsPtr);
}

EMSCRIPTEN_KEEPALIVE void host_OpenURL(unsigned int urlPtr) {
    const char* url = cart_get_string(urlPtr);
    OpenURL(url);
    free((void*)url);
}

EMSCRIPTEN_KEEPALIVE void host_SetTraceLogLevel(unsigned int logLevelPtr) {
    SetTraceLogLevel(logLevelPtr);
}

EMSCRIPTEN_KEEPALIVE void host_MemAlloc(unsigned int outPtr, unsigned int sizePtr) {
    void * out = MemAlloc(sizePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_MemRealloc(unsigned int outPtr, unsigned int ptrPtr, unsigned int sizePtr) {
    void ** ptr = cart_get_pointer(ptrPtr, sizeof(void *));
    void * out = MemRealloc(ptr, sizePtr);
    free((void*)ptr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_MemFree(unsigned int ptrPtr) {
    void ** ptr = cart_get_pointer(ptrPtr, sizeof(void *));
    MemFree(ptr);
    free((void*)ptr);
}

EMSCRIPTEN_KEEPALIVE void host_SetTraceLogCallback(unsigned int callbackPtr) {
    TraceLogCallback* callback = cart_get_pointer(callbackPtr, sizeof(TraceLogCallback));
    SetTraceLogCallback(*callback);
    free((void*)callback);
}

EMSCRIPTEN_KEEPALIVE void host_SetLoadFileDataCallback(unsigned int callbackPtr) {
    LoadFileDataCallback* callback = cart_get_pointer(callbackPtr, sizeof(LoadFileDataCallback));
    SetLoadFileDataCallback(*callback);
    free((void*)callback);
}

EMSCRIPTEN_KEEPALIVE void host_SetSaveFileDataCallback(unsigned int callbackPtr) {
    SaveFileDataCallback* callback = cart_get_pointer(callbackPtr, sizeof(SaveFileDataCallback));
    SetSaveFileDataCallback(*callback);
    free((void*)callback);
}

EMSCRIPTEN_KEEPALIVE void host_SetLoadFileTextCallback(unsigned int callbackPtr) {
    LoadFileTextCallback* callback = cart_get_pointer(callbackPtr, sizeof(LoadFileTextCallback));
    SetLoadFileTextCallback(*callback);
    free((void*)callback);
}

EMSCRIPTEN_KEEPALIVE void host_SetSaveFileTextCallback(unsigned int callbackPtr) {
    SaveFileTextCallback* callback = cart_get_pointer(callbackPtr, sizeof(SaveFileTextCallback));
    SetSaveFileTextCallback(*callback);
    free((void*)callback);
}

EMSCRIPTEN_KEEPALIVE void host_LoadFileData(unsigned int outPtr, unsigned int fileNamePtr, unsigned int dataSizePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** dataSize = cart_get_pointer(dataSizePtr, sizeof(int *));
    unsigned char * out = LoadFileData(fileName, dataSize);
    free((void*)fileName);
    free((void*)dataSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadFileData(unsigned int dataPtr) {
    unsigned char ** data = cart_get_pointer(dataPtr, sizeof(unsigned char *));
    UnloadFileData(data);
    free((void*)data);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_SaveFileData(unsigned int fileNamePtr, unsigned int dataPtr, unsigned int dataSizePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    void ** data = cart_get_pointer(dataPtr, sizeof(void *));
    return SaveFileData(fileName, data, dataSizePtr);
    free((void*)fileName);
    free((void*)data);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportDataAsCode(unsigned int dataPtr, unsigned int dataSizePtr, unsigned int fileNamePtr) {
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportDataAsCode(data, dataSizePtr, fileName);
    free((void*)data);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_LoadFileText(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    char * out = LoadFileText(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadFileText(unsigned int textPtr) {
    char ** text = cart_get_pointer(textPtr, sizeof(char *));
    UnloadFileText(text);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_SaveFileText(unsigned int fileNamePtr, unsigned int textPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    char ** text = cart_get_pointer(textPtr, sizeof(char *));
    return SaveFileText(fileName, text);
    free((void*)fileName);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_FileExists(unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    return FileExistsFS(fileName);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_DirectoryExists(unsigned int dirPathPtr) {
    const char* dirPath = cart_get_string(dirPathPtr);
    return DirectoryExistsFS(dirPath);
    free((void*)dirPath);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsFileExtension(unsigned int fileNamePtr, unsigned int extPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    const char* ext = cart_get_string(extPtr);
    return IsFileExtension(fileName, ext);
    free((void*)fileName);
    free((void*)ext);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetFileLength(unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    return GetFileLengthFS(fileName);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_GetFileExtension(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    const char * out = GetFileExtension(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetFileName(unsigned int outPtr, unsigned int filePathPtr) {
    const char* filePath = cart_get_string(filePathPtr);
    const char * out = GetFileName(filePath);
    free((void*)filePath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetFileNameWithoutExt(unsigned int outPtr, unsigned int filePathPtr) {
    const char* filePath = cart_get_string(filePathPtr);
    const char * out = GetFileNameWithoutExt(filePath);
    free((void*)filePath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetDirectoryPath(unsigned int outPtr, unsigned int filePathPtr) {
    const char* filePath = cart_get_string(filePathPtr);
    const char * out = GetDirectoryPath(filePath);
    free((void*)filePath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetPrevDirectoryPath(unsigned int outPtr, unsigned int dirPathPtr) {
    const char* dirPath = cart_get_string(dirPathPtr);
    const char * out = GetPrevDirectoryPath(dirPath);
    free((void*)dirPath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetWorkingDirectory(unsigned int outPtr) {
    const char * out = GetWorkingDirectory();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetApplicationDirectory(unsigned int outPtr) {
    const char * out = GetApplicationDirectory();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_MakeDirectory(unsigned int dirPathPtr) {
    const char* dirPath = cart_get_string(dirPathPtr);
    return MakeDirectory(dirPath);
    free((void*)dirPath);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ChangeDirectory(unsigned int dirPtr) {
    const char* dir = cart_get_string(dirPtr);
    return ChangeDirectoryFS(dir);
    free((void*)dir);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsPathFile(unsigned int pathPtr) {
    const char* path = cart_get_string(pathPtr);
    return IsPathFileFS(path);
    free((void*)path);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsFileNameValid(unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    return IsFileNameValid(fileName);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_LoadDirectoryFiles(unsigned int outPtr, unsigned int dirPathPtr) {
    const char* dirPath = cart_get_string(dirPathPtr);
    FilePathList out = LoadDirectoryFilesFS(dirPath);
    free((void*)dirPath);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadDirectoryFilesEx(unsigned int outPtr, unsigned int basePathPtr, unsigned int filterPtr, unsigned int scanSubdirsPtr) {
    const char* basePath = cart_get_string(basePathPtr);
    const char* filter = cart_get_string(filterPtr);
    FilePathList out = LoadDirectoryFilesExFS(basePath, filter, scanSubdirsPtr);
    free((void*)basePath);
    free((void*)filter);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadDirectoryFiles(unsigned int filesPtr) {
    FilePathList* files = cart_get_pointer(filesPtr, sizeof(FilePathList));
    UnloadDirectoryFiles(*files);
    free((void*)files);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsFileDropped() {
    return IsFileDropped();
}

EMSCRIPTEN_KEEPALIVE void host_LoadDroppedFiles(unsigned int outPtr) {
    FilePathList out = LoadDroppedFiles();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadDroppedFiles(unsigned int filesPtr) {
    FilePathList* files = cart_get_pointer(filesPtr, sizeof(FilePathList));
    UnloadDroppedFiles(*files);
    free((void*)files);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetFileModTime(unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    return GetFileModTimeFS(fileName);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_CompressData(unsigned int outPtr, unsigned int dataPtr, unsigned int dataSizePtr, unsigned int compDataSizePtr) {
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    int ** compDataSize = cart_get_pointer(compDataSizePtr, sizeof(int *));
    unsigned char * out = CompressData(data, dataSizePtr, compDataSize);
    free((void*)data);
    free((void*)compDataSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_DecompressData(unsigned int outPtr, unsigned int compDataPtr, unsigned int compDataSizePtr, unsigned int dataSizePtr) {
    const unsigned char ** compData = cart_get_pointer(compDataPtr, sizeof(const unsigned char *));
    int ** dataSize = cart_get_pointer(dataSizePtr, sizeof(int *));
    unsigned char * out = DecompressData(compData, compDataSizePtr, dataSize);
    free((void*)compData);
    free((void*)dataSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_EncodeDataBase64(unsigned int outPtr, unsigned int dataPtr, unsigned int dataSizePtr, unsigned int outputSizePtr) {
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    int ** outputSize = cart_get_pointer(outputSizePtr, sizeof(int *));
    char * out = EncodeDataBase64(data, dataSizePtr, outputSize);
    free((void*)data);
    free((void*)outputSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_DecodeDataBase64(unsigned int outPtr, unsigned int dataPtr, unsigned int outputSizePtr) {
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    int ** outputSize = cart_get_pointer(outputSizePtr, sizeof(int *));
    unsigned char * out = DecodeDataBase64(data, outputSize);
    free((void*)data);
    free((void*)outputSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ComputeCRC32(unsigned int dataPtr, unsigned int dataSizePtr) {
    unsigned char ** data = cart_get_pointer(dataPtr, sizeof(unsigned char *));
    return ComputeCRC32(data, dataSizePtr);
    free((void*)data);
}

EMSCRIPTEN_KEEPALIVE void host_ComputeMD5(unsigned int outPtr, unsigned int dataPtr, unsigned int dataSizePtr) {
    unsigned char ** data = cart_get_pointer(dataPtr, sizeof(unsigned char *));
    unsigned int * out = ComputeMD5(data, dataSizePtr);
    free((void*)data);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ComputeSHA1(unsigned int outPtr, unsigned int dataPtr, unsigned int dataSizePtr) {
    unsigned char ** data = cart_get_pointer(dataPtr, sizeof(unsigned char *));
    unsigned int * out = ComputeSHA1(data, dataSizePtr);
    free((void*)data);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadAutomationEventList(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    AutomationEventList out = LoadAutomationEventList(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadAutomationEventList(unsigned int listPtr) {
    AutomationEventList* list = cart_get_pointer(listPtr, sizeof(AutomationEventList));
    UnloadAutomationEventList(*list);
    free((void*)list);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportAutomationEventList(unsigned int listPtr, unsigned int fileNamePtr) {
    AutomationEventList* list = cart_get_pointer(listPtr, sizeof(AutomationEventList));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportAutomationEventList(*list, fileName);
    free((void*)list);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_SetAutomationEventList(unsigned int listPtr) {
    AutomationEventList ** list = cart_get_pointer(listPtr, sizeof(AutomationEventList *));
    SetAutomationEventList(list);
    free((void*)list);
}

EMSCRIPTEN_KEEPALIVE void host_SetAutomationEventBaseFrame(unsigned int framePtr) {
    SetAutomationEventBaseFrame(framePtr);
}

EMSCRIPTEN_KEEPALIVE void host_StartAutomationEventRecording() {
    StartAutomationEventRecording();
}

EMSCRIPTEN_KEEPALIVE void host_StopAutomationEventRecording() {
    StopAutomationEventRecording();
}

EMSCRIPTEN_KEEPALIVE void host_PlayAutomationEvent(unsigned int eventPtr) {
    AutomationEvent* event = cart_get_pointer(eventPtr, sizeof(AutomationEvent));
    PlayAutomationEvent(*event);
    free((void*)event);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsKeyPressed(unsigned int keyPtr) {
    return IsKeyPressed(keyPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsKeyPressedRepeat(unsigned int keyPtr) {
    return IsKeyPressedRepeat(keyPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsKeyDown(unsigned int keyPtr) {
    return IsKeyDown(keyPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsKeyReleased(unsigned int keyPtr) {
    return IsKeyReleased(keyPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsKeyUp(unsigned int keyPtr) {
    return IsKeyUp(keyPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetKeyPressed() {
    return GetKeyPressed();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetCharPressed() {
    return GetCharPressed();
}

EMSCRIPTEN_KEEPALIVE void host_SetExitKey(unsigned int keyPtr) {
    SetExitKey(keyPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsGamepadAvailable(unsigned int gamepadPtr) {
    return IsGamepadAvailable(gamepadPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetGamepadName(unsigned int outPtr, unsigned int gamepadPtr) {
    const char * out = GetGamepadName(gamepadPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsGamepadButtonPressed(unsigned int gamepadPtr, unsigned int buttonPtr) {
    return IsGamepadButtonPressed(gamepadPtr, buttonPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsGamepadButtonDown(unsigned int gamepadPtr, unsigned int buttonPtr) {
    return IsGamepadButtonDown(gamepadPtr, buttonPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsGamepadButtonReleased(unsigned int gamepadPtr, unsigned int buttonPtr) {
    return IsGamepadButtonReleased(gamepadPtr, buttonPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsGamepadButtonUp(unsigned int gamepadPtr, unsigned int buttonPtr) {
    return IsGamepadButtonUp(gamepadPtr, buttonPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetGamepadButtonPressed() {
    return GetGamepadButtonPressed();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetGamepadAxisCount(unsigned int gamepadPtr) {
    return GetGamepadAxisCount(gamepadPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetGamepadAxisMovement(unsigned int gamepadPtr, unsigned int axisPtr) {
    return GetGamepadAxisMovement(gamepadPtr, axisPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_SetGamepadMappings(unsigned int mappingsPtr) {
    const char* mappings = cart_get_string(mappingsPtr);
    return SetGamepadMappings(mappings);
    free((void*)mappings);
}

EMSCRIPTEN_KEEPALIVE void host_SetGamepadVibration(unsigned int gamepadPtr, unsigned int leftMotorPtr, unsigned int rightMotorPtr, unsigned int durationPtr) {
    SetGamepadVibration(gamepadPtr, leftMotorPtr, rightMotorPtr, durationPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsMouseButtonPressed(unsigned int buttonPtr) {
    return IsMouseButtonPressed(buttonPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsMouseButtonDown(unsigned int buttonPtr) {
    return IsMouseButtonDown(buttonPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsMouseButtonReleased(unsigned int buttonPtr) {
    return IsMouseButtonReleased(buttonPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsMouseButtonUp(unsigned int buttonPtr) {
    return IsMouseButtonUp(buttonPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMouseX() {
    return GetMouseX();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMouseY() {
    return GetMouseY();
}

EMSCRIPTEN_KEEPALIVE void host_GetMousePosition(unsigned int outPtr) {
    Vector2 out = GetMousePosition();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetMouseDelta(unsigned int outPtr) {
    Vector2 out = GetMouseDelta();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetMousePosition(unsigned int xPtr, unsigned int yPtr) {
    SetMousePosition(xPtr, yPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetMouseOffset(unsigned int offsetXPtr, unsigned int offsetYPtr) {
    SetMouseOffset(offsetXPtr, offsetYPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetMouseScale(unsigned int scaleXPtr, unsigned int scaleYPtr) {
    SetMouseScale(scaleXPtr, scaleYPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMouseWheelMove() {
    return GetMouseWheelMove();
}

EMSCRIPTEN_KEEPALIVE void host_GetMouseWheelMoveV(unsigned int outPtr) {
    Vector2 out = GetMouseWheelMoveV();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetMouseCursor(unsigned int cursorPtr) {
    SetMouseCursor(cursorPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetTouchX() {
    return GetTouchX();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetTouchY() {
    return GetTouchY();
}

EMSCRIPTEN_KEEPALIVE void host_GetTouchPosition(unsigned int outPtr, unsigned int indexPtr) {
    Vector2 out = GetTouchPosition(indexPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetTouchPointId(unsigned int indexPtr) {
    return GetTouchPointId(indexPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetTouchPointCount() {
    return GetTouchPointCount();
}

EMSCRIPTEN_KEEPALIVE void host_SetGesturesEnabled(unsigned int flagsPtr) {
    SetGesturesEnabled(flagsPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsGestureDetected(unsigned int gesturePtr) {
    return IsGestureDetected(gesturePtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetGestureDetected() {
    return GetGestureDetected();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetGestureHoldDuration() {
    return GetGestureHoldDuration();
}

EMSCRIPTEN_KEEPALIVE void host_GetGestureDragVector(unsigned int outPtr) {
    Vector2 out = GetGestureDragVector();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetGestureDragAngle() {
    return GetGestureDragAngle();
}

EMSCRIPTEN_KEEPALIVE void host_GetGesturePinchVector(unsigned int outPtr) {
    Vector2 out = GetGesturePinchVector();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetGesturePinchAngle() {
    return GetGesturePinchAngle();
}

EMSCRIPTEN_KEEPALIVE void host_UpdateCamera(unsigned int cameraPtr, unsigned int modePtr) {
    Camera ** camera = cart_get_pointer(cameraPtr, sizeof(Camera *));
    UpdateCamera(camera, modePtr);
    free((void*)camera);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateCameraPro(unsigned int cameraPtr, unsigned int movementPtr, unsigned int rotationPtr, unsigned int zoomPtr) {
    Camera ** camera = cart_get_pointer(cameraPtr, sizeof(Camera *));
    Vector3* movement = cart_get_pointer(movementPtr, sizeof(Vector3));
    Vector3* rotation = cart_get_pointer(rotationPtr, sizeof(Vector3));
    UpdateCameraPro(camera, *movement, *rotation, zoomPtr);
    free((void*)camera);
    free((void*)movement);
    free((void*)rotation);
}

EMSCRIPTEN_KEEPALIVE void host_SetShapesTexture(unsigned int texturePtr, unsigned int sourcePtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Rectangle* source = cart_get_pointer(sourcePtr, sizeof(Rectangle));
    SetShapesTexture(*texture, *source);
    free((void*)texture);
    free((void*)source);
}

EMSCRIPTEN_KEEPALIVE void host_GetShapesTexture(unsigned int outPtr) {
    Texture2D out = GetShapesTexture();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetShapesTextureRectangle(unsigned int outPtr) {
    Rectangle out = GetShapesTextureRectangle();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_DrawPixel(unsigned int posXPtr, unsigned int posYPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPixel(posXPtr, posYPtr, *color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawPixelV(unsigned int positionPtr, unsigned int colorPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPixelV(*position, *color);
    free((void*)position);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawLine(unsigned int startPosXPtr, unsigned int startPosYPtr, unsigned int endPosXPtr, unsigned int endPosYPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLine(startPosXPtr, startPosYPtr, endPosXPtr, endPosYPtr, *color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawLineV(unsigned int startPosPtr, unsigned int endPosPtr, unsigned int colorPtr) {
    Vector2* startPos = cart_get_pointer(startPosPtr, sizeof(Vector2));
    Vector2* endPos = cart_get_pointer(endPosPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLineV(*startPos, *endPos, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawLineEx(unsigned int startPosPtr, unsigned int endPosPtr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* startPos = cart_get_pointer(startPosPtr, sizeof(Vector2));
    Vector2* endPos = cart_get_pointer(endPosPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLineEx(*startPos, *endPos, thickPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawLineStrip(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLineStrip(points, pointCountPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawLineBezier(unsigned int startPosPtr, unsigned int endPosPtr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* startPos = cart_get_pointer(startPosPtr, sizeof(Vector2));
    Vector2* endPos = cart_get_pointer(endPosPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLineBezier(*startPos, *endPos, thickPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCircle(unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircle(centerXPtr, centerYPtr, radiusPtr, *color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCircleSector(unsigned int centerPtr, unsigned int radiusPtr, unsigned int startAnglePtr, unsigned int endAnglePtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleSector(*center, radiusPtr, startAnglePtr, endAnglePtr, segmentsPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCircleSectorLines(unsigned int centerPtr, unsigned int radiusPtr, unsigned int startAnglePtr, unsigned int endAnglePtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleSectorLines(*center, radiusPtr, startAnglePtr, endAnglePtr, segmentsPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCircleGradient(unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int innerPtr, unsigned int outerPtr) {
    Color* inner = cart_get_pointer(innerPtr, sizeof(Color));
    Color* outer = cart_get_pointer(outerPtr, sizeof(Color));
    DrawCircleGradient(centerXPtr, centerYPtr, radiusPtr, *inner, *outer);
    free((void*)inner);
    free((void*)outer);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCircleV(unsigned int centerPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleV(*center, radiusPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCircleLines(unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleLines(centerXPtr, centerYPtr, radiusPtr, *color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCircleLinesV(unsigned int centerPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircleLinesV(*center, radiusPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawEllipse(unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusHPtr, unsigned int radiusVPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawEllipse(centerXPtr, centerYPtr, radiusHPtr, radiusVPtr, *color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawEllipseLines(unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusHPtr, unsigned int radiusVPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawEllipseLines(centerXPtr, centerYPtr, radiusHPtr, radiusVPtr, *color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRing(unsigned int centerPtr, unsigned int innerRadiusPtr, unsigned int outerRadiusPtr, unsigned int startAnglePtr, unsigned int endAnglePtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRing(*center, innerRadiusPtr, outerRadiusPtr, startAnglePtr, endAnglePtr, segmentsPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRingLines(unsigned int centerPtr, unsigned int innerRadiusPtr, unsigned int outerRadiusPtr, unsigned int startAnglePtr, unsigned int endAnglePtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRingLines(*center, innerRadiusPtr, outerRadiusPtr, startAnglePtr, endAnglePtr, segmentsPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangle(unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangle(posXPtr, posYPtr, widthPtr, heightPtr, *color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleV(unsigned int positionPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Vector2* size = cart_get_pointer(sizePtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleV(*position, *size, *color);
    free((void*)position);
    free((void*)size);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleRec(unsigned int recPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleRec(*rec, *color);
    free((void*)rec);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectanglePro(unsigned int recPtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Vector2* origin = cart_get_pointer(originPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectanglePro(*rec, *origin, rotationPtr, *color);
    free((void*)rec);
    free((void*)origin);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleGradientV(unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int topPtr, unsigned int bottomPtr) {
    Color* top = cart_get_pointer(topPtr, sizeof(Color));
    Color* bottom = cart_get_pointer(bottomPtr, sizeof(Color));
    DrawRectangleGradientV(posXPtr, posYPtr, widthPtr, heightPtr, *top, *bottom);
    free((void*)top);
    free((void*)bottom);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleGradientH(unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int leftPtr, unsigned int rightPtr) {
    Color* left = cart_get_pointer(leftPtr, sizeof(Color));
    Color* right = cart_get_pointer(rightPtr, sizeof(Color));
    DrawRectangleGradientH(posXPtr, posYPtr, widthPtr, heightPtr, *left, *right);
    free((void*)left);
    free((void*)right);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleGradientEx(unsigned int recPtr, unsigned int topLeftPtr, unsigned int bottomLeftPtr, unsigned int topRightPtr, unsigned int bottomRightPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleLines(unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleLines(posXPtr, posYPtr, widthPtr, heightPtr, *color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleLinesEx(unsigned int recPtr, unsigned int lineThickPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleLinesEx(*rec, lineThickPtr, *color);
    free((void*)rec);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleRounded(unsigned int recPtr, unsigned int roundnessPtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleRounded(*rec, roundnessPtr, segmentsPtr, *color);
    free((void*)rec);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleRoundedLines(unsigned int recPtr, unsigned int roundnessPtr, unsigned int segmentsPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleRoundedLines(*rec, roundnessPtr, segmentsPtr, *color);
    free((void*)rec);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRectangleRoundedLinesEx(unsigned int recPtr, unsigned int roundnessPtr, unsigned int segmentsPtr, unsigned int lineThickPtr, unsigned int colorPtr) {
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRectangleRoundedLinesEx(*rec, roundnessPtr, segmentsPtr, lineThickPtr, *color);
    free((void*)rec);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTriangle(unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawTriangleLines(unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawTriangleFan(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangleFan(points, pointCountPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTriangleStrip(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangleStrip(points, pointCountPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawPoly(unsigned int centerPtr, unsigned int sidesPtr, unsigned int radiusPtr, unsigned int rotationPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPoly(*center, sidesPtr, radiusPtr, rotationPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawPolyLines(unsigned int centerPtr, unsigned int sidesPtr, unsigned int radiusPtr, unsigned int rotationPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPolyLines(*center, sidesPtr, radiusPtr, rotationPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawPolyLinesEx(unsigned int centerPtr, unsigned int sidesPtr, unsigned int radiusPtr, unsigned int rotationPtr, unsigned int lineThickPtr, unsigned int colorPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPolyLinesEx(*center, sidesPtr, radiusPtr, rotationPtr, lineThickPtr, *color);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSplineLinear(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineLinear(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSplineBasis(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineBasis(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSplineCatmullRom(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineCatmullRom(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSplineBezierQuadratic(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineBezierQuadratic(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSplineBezierCubic(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int thickPtr, unsigned int colorPtr) {
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineBezierCubic(points, pointCountPtr, thickPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSplineSegmentLinear(unsigned int p1Ptr, unsigned int p2Ptr, unsigned int thickPtr, unsigned int colorPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSplineSegmentLinear(*p1, *p2, thickPtr, *color);
    free((void*)p1);
    free((void*)p2);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSplineSegmentBasis(unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr, unsigned int thickPtr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawSplineSegmentCatmullRom(unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr, unsigned int thickPtr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawSplineSegmentBezierQuadratic(unsigned int p1Ptr, unsigned int c2Ptr, unsigned int p3Ptr, unsigned int thickPtr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawSplineSegmentBezierCubic(unsigned int p1Ptr, unsigned int c2Ptr, unsigned int c3Ptr, unsigned int p4Ptr, unsigned int thickPtr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_GetSplinePointLinear(unsigned int outPtr, unsigned int startPosPtr, unsigned int endPosPtr, unsigned int tPtr) {
    Vector2* startPos = cart_get_pointer(startPosPtr, sizeof(Vector2));
    Vector2* endPos = cart_get_pointer(endPosPtr, sizeof(Vector2));
    Vector2 out = GetSplinePointLinear(*startPos, *endPos, tPtr);
    free((void*)startPos);
    free((void*)endPos);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetSplinePointBasis(unsigned int outPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr, unsigned int tPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_GetSplinePointCatmullRom(unsigned int outPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr, unsigned int tPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_GetSplinePointBezierQuad(unsigned int outPtr, unsigned int p1Ptr, unsigned int c2Ptr, unsigned int p3Ptr, unsigned int tPtr) {
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* c2 = cart_get_pointer(c2Ptr, sizeof(Vector2));
    Vector2* p3 = cart_get_pointer(p3Ptr, sizeof(Vector2));
    Vector2 out = GetSplinePointBezierQuad(*p1, *c2, *p3, tPtr);
    free((void*)p1);
    free((void*)c2);
    free((void*)p3);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetSplinePointBezierCubic(unsigned int outPtr, unsigned int p1Ptr, unsigned int c2Ptr, unsigned int c3Ptr, unsigned int p4Ptr, unsigned int tPtr) {
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

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionRecs(unsigned int rec1Ptr, unsigned int rec2Ptr) {
    Rectangle* rec1 = cart_get_pointer(rec1Ptr, sizeof(Rectangle));
    Rectangle* rec2 = cart_get_pointer(rec2Ptr, sizeof(Rectangle));
    return CheckCollisionRecs(*rec1, *rec2);
    free((void*)rec1);
    free((void*)rec2);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionCircles(unsigned int center1Ptr, unsigned int radius1Ptr, unsigned int center2Ptr, unsigned int radius2Ptr) {
    Vector2* center1 = cart_get_pointer(center1Ptr, sizeof(Vector2));
    Vector2* center2 = cart_get_pointer(center2Ptr, sizeof(Vector2));
    return CheckCollisionCircles(*center1, radius1Ptr, *center2, radius2Ptr);
    free((void*)center1);
    free((void*)center2);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionCircleRec(unsigned int centerPtr, unsigned int radiusPtr, unsigned int recPtr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    return CheckCollisionCircleRec(*center, radiusPtr, *rec);
    free((void*)center);
    free((void*)rec);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionCircleLine(unsigned int centerPtr, unsigned int radiusPtr, unsigned int p1Ptr, unsigned int p2Ptr) {
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    return CheckCollisionCircleLine(*center, radiusPtr, *p1, *p2);
    free((void*)center);
    free((void*)p1);
    free((void*)p2);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionPointRec(unsigned int pointPtr, unsigned int recPtr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    return CheckCollisionPointRec(*point, *rec);
    free((void*)point);
    free((void*)rec);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionPointCircle(unsigned int pointPtr, unsigned int centerPtr, unsigned int radiusPtr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    return CheckCollisionPointCircle(*point, *center, radiusPtr);
    free((void*)point);
    free((void*)center);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionPointTriangle(unsigned int pointPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr) {
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

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionPointLine(unsigned int pointPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int thresholdPtr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    Vector2* p1 = cart_get_pointer(p1Ptr, sizeof(Vector2));
    Vector2* p2 = cart_get_pointer(p2Ptr, sizeof(Vector2));
    return CheckCollisionPointLine(*point, *p1, *p2, thresholdPtr);
    free((void*)point);
    free((void*)p1);
    free((void*)p2);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionPointPoly(unsigned int pointPtr, unsigned int pointsPtr, unsigned int pointCountPtr) {
    Vector2* point = cart_get_pointer(pointPtr, sizeof(Vector2));
    const Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector2 *));
    return CheckCollisionPointPoly(*point, points, pointCountPtr);
    free((void*)point);
    free((void*)points);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionLines(unsigned int startPos1Ptr, unsigned int endPos1Ptr, unsigned int startPos2Ptr, unsigned int endPos2Ptr, unsigned int collisionPointPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_GetCollisionRec(unsigned int outPtr, unsigned int rec1Ptr, unsigned int rec2Ptr) {
    Rectangle* rec1 = cart_get_pointer(rec1Ptr, sizeof(Rectangle));
    Rectangle* rec2 = cart_get_pointer(rec2Ptr, sizeof(Rectangle));
    Rectangle out = GetCollisionRec(*rec1, *rec2);
    free((void*)rec1);
    free((void*)rec2);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImage(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Image out = LoadImage(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImageRaw(unsigned int outPtr, unsigned int fileNamePtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int formatPtr, unsigned int headerSizePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Image out = LoadImageRaw(fileName, widthPtr, heightPtr, formatPtr, headerSizePtr);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImageAnim(unsigned int outPtr, unsigned int fileNamePtr, unsigned int framesPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** frames = cart_get_pointer(framesPtr, sizeof(int *));
    Image out = LoadImageAnim(fileName, frames);
    free((void*)fileName);
    free((void*)frames);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImageAnimFromMemory(unsigned int outPtr, unsigned int fileTypePtr, unsigned int fileDataPtr, unsigned int dataSizePtr, unsigned int framesPtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    int ** frames = cart_get_pointer(framesPtr, sizeof(int *));
    Image out = LoadImageAnimFromMemory(fileType, fileData, dataSizePtr, frames);
    free((void*)fileType);
    free((void*)fileData);
    free((void*)frames);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImageFromMemory(unsigned int outPtr, unsigned int fileTypePtr, unsigned int fileDataPtr, unsigned int dataSizePtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    Image out = LoadImageFromMemory(fileType, fileData, dataSizePtr);
    free((void*)fileType);
    free((void*)fileData);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImageFromTexture(unsigned int outPtr, unsigned int texturePtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Image out = LoadImageFromTexture(*texture);
    free((void*)texture);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImageFromScreen(unsigned int outPtr) {
    Image out = LoadImageFromScreen();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsImageValid(unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    return IsImageValid(*image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadImage(unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    UnloadImage(*image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportImage(unsigned int imagePtr, unsigned int fileNamePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportImage(*image, fileName);
    free((void*)image);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_ExportImageToMemory(unsigned int outPtr, unsigned int imagePtr, unsigned int fileTypePtr, unsigned int fileSizePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    const char* fileType = cart_get_string(fileTypePtr);
    int ** fileSize = cart_get_pointer(fileSizePtr, sizeof(int *));
    unsigned char * out = ExportImageToMemory(*image, fileType, fileSize);
    free((void*)image);
    free((void*)fileType);
    free((void*)fileSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportImageAsCode(unsigned int imagePtr, unsigned int fileNamePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportImageAsCode(*image, fileName);
    free((void*)image);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageColor(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Image out = GenImageColor(widthPtr, heightPtr, *color);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageGradientLinear(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int directionPtr, unsigned int startPtr, unsigned int endPtr) {
    Color* start = cart_get_pointer(startPtr, sizeof(Color));
    Color* end = cart_get_pointer(endPtr, sizeof(Color));
    Image out = GenImageGradientLinear(widthPtr, heightPtr, directionPtr, *start, *end);
    free((void*)start);
    free((void*)end);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageGradientRadial(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int densityPtr, unsigned int innerPtr, unsigned int outerPtr) {
    Color* inner = cart_get_pointer(innerPtr, sizeof(Color));
    Color* outer = cart_get_pointer(outerPtr, sizeof(Color));
    Image out = GenImageGradientRadial(widthPtr, heightPtr, densityPtr, *inner, *outer);
    free((void*)inner);
    free((void*)outer);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageGradientSquare(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int densityPtr, unsigned int innerPtr, unsigned int outerPtr) {
    Color* inner = cart_get_pointer(innerPtr, sizeof(Color));
    Color* outer = cart_get_pointer(outerPtr, sizeof(Color));
    Image out = GenImageGradientSquare(widthPtr, heightPtr, densityPtr, *inner, *outer);
    free((void*)inner);
    free((void*)outer);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageChecked(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int checksXPtr, unsigned int checksYPtr, unsigned int col1Ptr, unsigned int col2Ptr) {
    Color* col1 = cart_get_pointer(col1Ptr, sizeof(Color));
    Color* col2 = cart_get_pointer(col2Ptr, sizeof(Color));
    Image out = GenImageChecked(widthPtr, heightPtr, checksXPtr, checksYPtr, *col1, *col2);
    free((void*)col1);
    free((void*)col2);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageWhiteNoise(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int factorPtr) {
    Image out = GenImageWhiteNoise(widthPtr, heightPtr, factorPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImagePerlinNoise(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int offsetXPtr, unsigned int offsetYPtr, unsigned int scalePtr) {
    Image out = GenImagePerlinNoise(widthPtr, heightPtr, offsetXPtr, offsetYPtr, scalePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageCellular(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int tileSizePtr) {
    Image out = GenImageCellular(widthPtr, heightPtr, tileSizePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageText(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    Image out = GenImageText(widthPtr, heightPtr, text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ImageCopy(unsigned int outPtr, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Image out = ImageCopy(*image);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ImageFromImage(unsigned int outPtr, unsigned int imagePtr, unsigned int recPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Image out = ImageFromImage(*image, *rec);
    free((void*)image);
    free((void*)rec);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ImageFromChannel(unsigned int outPtr, unsigned int imagePtr, unsigned int selectedChannelPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Image out = ImageFromChannel(*image, selectedChannelPtr);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ImageText(unsigned int outPtr, unsigned int textPtr, unsigned int fontSizePtr, unsigned int colorPtr) {
    const char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Image out = ImageText(text, fontSizePtr, *color);
    free((void*)text);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ImageTextEx(unsigned int outPtr, unsigned int fontPtr, unsigned int textPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* text = cart_get_string(textPtr);
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    Image out = ImageTextEx(*font, text, fontSizePtr, spacingPtr, *tint);
    free((void*)font);
    free((void*)text);
    free((void*)tint);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ImageFormat(unsigned int imagePtr, unsigned int newFormatPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageFormat(image, newFormatPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageToPOT(unsigned int imagePtr, unsigned int fillPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* fill = cart_get_pointer(fillPtr, sizeof(Color));
    ImageToPOT(image, *fill);
    free((void*)image);
    free((void*)fill);
}

EMSCRIPTEN_KEEPALIVE void host_ImageCrop(unsigned int imagePtr, unsigned int cropPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Rectangle* crop = cart_get_pointer(cropPtr, sizeof(Rectangle));
    ImageCrop(image, *crop);
    free((void*)image);
    free((void*)crop);
}

EMSCRIPTEN_KEEPALIVE void host_ImageAlphaCrop(unsigned int imagePtr, unsigned int thresholdPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageAlphaCrop(image, thresholdPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageAlphaClear(unsigned int imagePtr, unsigned int colorPtr, unsigned int thresholdPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageAlphaClear(image, *color, thresholdPtr);
    free((void*)image);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageAlphaMask(unsigned int imagePtr, unsigned int alphaMaskPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Image* alphaMask = cart_get_pointer(alphaMaskPtr, sizeof(Image));
    ImageAlphaMask(image, *alphaMask);
    free((void*)image);
    free((void*)alphaMask);
}

EMSCRIPTEN_KEEPALIVE void host_ImageAlphaPremultiply(unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageAlphaPremultiply(image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageBlurGaussian(unsigned int imagePtr, unsigned int blurSizePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageBlurGaussian(image, blurSizePtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageKernelConvolution(unsigned int imagePtr, unsigned int kernelPtr, unsigned int kernelSizePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    const float ** kernel = cart_get_pointer(kernelPtr, sizeof(const float *));
    ImageKernelConvolution(image, kernel, kernelSizePtr);
    free((void*)image);
    free((void*)kernel);
}

EMSCRIPTEN_KEEPALIVE void host_ImageResize(unsigned int imagePtr, unsigned int newWidthPtr, unsigned int newHeightPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageResize(image, newWidthPtr, newHeightPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageResizeNN(unsigned int imagePtr, unsigned int newWidthPtr, unsigned int newHeightPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageResizeNN(image, newWidthPtr, newHeightPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageResizeCanvas(unsigned int imagePtr, unsigned int newWidthPtr, unsigned int newHeightPtr, unsigned int offsetXPtr, unsigned int offsetYPtr, unsigned int fillPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* fill = cart_get_pointer(fillPtr, sizeof(Color));
    ImageResizeCanvas(image, newWidthPtr, newHeightPtr, offsetXPtr, offsetYPtr, *fill);
    free((void*)image);
    free((void*)fill);
}

EMSCRIPTEN_KEEPALIVE void host_ImageMipmaps(unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageMipmaps(image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDither(unsigned int imagePtr, unsigned int rBppPtr, unsigned int gBppPtr, unsigned int bBppPtr, unsigned int aBppPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageDither(image, rBppPtr, gBppPtr, bBppPtr, aBppPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageFlipVertical(unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageFlipVertical(image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageFlipHorizontal(unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageFlipHorizontal(image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageRotate(unsigned int imagePtr, unsigned int degreesPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageRotate(image, degreesPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageRotateCW(unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageRotateCW(image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageRotateCCW(unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageRotateCCW(image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageColorTint(unsigned int imagePtr, unsigned int colorPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageColorTint(image, *color);
    free((void*)image);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageColorInvert(unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageColorInvert(image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageColorGrayscale(unsigned int imagePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageColorGrayscale(image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageColorContrast(unsigned int imagePtr, unsigned int contrastPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageColorContrast(image, contrastPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageColorBrightness(unsigned int imagePtr, unsigned int brightnessPtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    ImageColorBrightness(image, brightnessPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_ImageColorReplace(unsigned int imagePtr, unsigned int colorPtr, unsigned int replacePtr) {
    Image ** image = cart_get_pointer(imagePtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color* replace = cart_get_pointer(replacePtr, sizeof(Color));
    ImageColorReplace(image, *color, *replace);
    free((void*)image);
    free((void*)color);
    free((void*)replace);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImageColors(unsigned int outPtr, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Color * out = LoadImageColors(*image);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImagePalette(unsigned int outPtr, unsigned int imagePtr, unsigned int maxPaletteSizePtr, unsigned int colorCountPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    int ** colorCount = cart_get_pointer(colorCountPtr, sizeof(int *));
    Color * out = LoadImagePalette(*image, maxPaletteSizePtr, colorCount);
    free((void*)image);
    free((void*)colorCount);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadImageColors(unsigned int colorsPtr) {
    Color ** colors = cart_get_pointer(colorsPtr, sizeof(Color *));
    UnloadImageColors(colors);
    free((void*)colors);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadImagePalette(unsigned int colorsPtr) {
    Color ** colors = cart_get_pointer(colorsPtr, sizeof(Color *));
    UnloadImagePalette(colors);
    free((void*)colors);
}

EMSCRIPTEN_KEEPALIVE void host_GetImageAlphaBorder(unsigned int outPtr, unsigned int imagePtr, unsigned int thresholdPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Rectangle out = GetImageAlphaBorder(*image, thresholdPtr);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetImageColor(unsigned int outPtr, unsigned int imagePtr, unsigned int xPtr, unsigned int yPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Color out = GetImageColor(*image, xPtr, yPtr);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ImageClearBackground(unsigned int dstPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageClearBackground(dst, *color);
    free((void*)dst);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawPixel(unsigned int dstPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawPixel(dst, posXPtr, posYPtr, *color);
    free((void*)dst);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawPixelV(unsigned int dstPtr, unsigned int positionPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawPixelV(dst, *position, *color);
    free((void*)dst);
    free((void*)position);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawLine(unsigned int dstPtr, unsigned int startPosXPtr, unsigned int startPosYPtr, unsigned int endPosXPtr, unsigned int endPosYPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawLine(dst, startPosXPtr, startPosYPtr, endPosXPtr, endPosYPtr, *color);
    free((void*)dst);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawLineV(unsigned int dstPtr, unsigned int startPtr, unsigned int endPtr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_ImageDrawLineEx(unsigned int dstPtr, unsigned int startPtr, unsigned int endPtr, unsigned int thickPtr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_ImageDrawCircle(unsigned int dstPtr, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawCircle(dst, centerXPtr, centerYPtr, radiusPtr, *color);
    free((void*)dst);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawCircleV(unsigned int dstPtr, unsigned int centerPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawCircleV(dst, *center, radiusPtr, *color);
    free((void*)dst);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawCircleLines(unsigned int dstPtr, unsigned int centerXPtr, unsigned int centerYPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawCircleLines(dst, centerXPtr, centerYPtr, radiusPtr, *color);
    free((void*)dst);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawCircleLinesV(unsigned int dstPtr, unsigned int centerPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2* center = cart_get_pointer(centerPtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawCircleLinesV(dst, *center, radiusPtr, *color);
    free((void*)dst);
    free((void*)center);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawRectangle(unsigned int dstPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawRectangle(dst, posXPtr, posYPtr, widthPtr, heightPtr, *color);
    free((void*)dst);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawRectangleV(unsigned int dstPtr, unsigned int positionPtr, unsigned int sizePtr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_ImageDrawRectangleRec(unsigned int dstPtr, unsigned int recPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawRectangleRec(dst, *rec, *color);
    free((void*)dst);
    free((void*)rec);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawRectangleLines(unsigned int dstPtr, unsigned int recPtr, unsigned int thickPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawRectangleLines(dst, *rec, thickPtr, *color);
    free((void*)dst);
    free((void*)rec);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawTriangle(unsigned int dstPtr, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_ImageDrawTriangleEx(unsigned int dstPtr, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int c1Ptr, unsigned int c2Ptr, unsigned int c3Ptr) {
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

EMSCRIPTEN_KEEPALIVE void host_ImageDrawTriangleLines(unsigned int dstPtr, unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_ImageDrawTriangleFan(unsigned int dstPtr, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawTriangleFan(dst, points, pointCountPtr, *color);
    free((void*)dst);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawTriangleStrip(unsigned int dstPtr, unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    Vector2 ** points = cart_get_pointer(pointsPtr, sizeof(Vector2 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawTriangleStrip(dst, points, pointCountPtr, *color);
    free((void*)dst);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDraw(unsigned int dstPtr, unsigned int srcPtr, unsigned int srcRecPtr, unsigned int dstRecPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_ImageDrawText(unsigned int dstPtr, unsigned int textPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int fontSizePtr, unsigned int colorPtr) {
    Image ** dst = cart_get_pointer(dstPtr, sizeof(Image *));
    const char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ImageDrawText(dst, text, posXPtr, posYPtr, fontSizePtr, *color);
    free((void*)dst);
    free((void*)text);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ImageDrawTextEx(unsigned int dstPtr, unsigned int fontPtr, unsigned int textPtr, unsigned int positionPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_LoadTexture(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Texture2D out = LoadTexture(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadTextureFromImage(unsigned int outPtr, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Texture2D out = LoadTextureFromImage(*image);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadTextureCubemap(unsigned int outPtr, unsigned int imagePtr, unsigned int layoutPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    TextureCubemap out = LoadTextureCubemap(*image, layoutPtr);
    free((void*)image);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadRenderTexture(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr) {
    RenderTexture2D out = LoadRenderTexture(widthPtr, heightPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsTextureValid(unsigned int texturePtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    return IsTextureValid(*texture);
    free((void*)texture);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadTexture(unsigned int texturePtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    UnloadTexture(*texture);
    free((void*)texture);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsRenderTextureValid(unsigned int targetPtr) {
    RenderTexture2D* target = cart_get_pointer(targetPtr, sizeof(RenderTexture2D));
    return IsRenderTextureValid(*target);
    free((void*)target);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadRenderTexture(unsigned int targetPtr) {
    RenderTexture2D* target = cart_get_pointer(targetPtr, sizeof(RenderTexture2D));
    UnloadRenderTexture(*target);
    free((void*)target);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateTexture(unsigned int texturePtr, unsigned int pixelsPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    const void ** pixels = cart_get_pointer(pixelsPtr, sizeof(const void *));
    UpdateTexture(*texture, pixels);
    free((void*)texture);
    free((void*)pixels);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateTextureRec(unsigned int texturePtr, unsigned int recPtr, unsigned int pixelsPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Rectangle* rec = cart_get_pointer(recPtr, sizeof(Rectangle));
    const void ** pixels = cart_get_pointer(pixelsPtr, sizeof(const void *));
    UpdateTextureRec(*texture, *rec, pixels);
    free((void*)texture);
    free((void*)rec);
    free((void*)pixels);
}

EMSCRIPTEN_KEEPALIVE void host_GenTextureMipmaps(unsigned int texturePtr) {
    Texture2D ** texture = cart_get_pointer(texturePtr, sizeof(Texture2D *));
    GenTextureMipmaps(texture);
    free((void*)texture);
}

EMSCRIPTEN_KEEPALIVE void host_SetTextureFilter(unsigned int texturePtr, unsigned int filterPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    SetTextureFilter(*texture, filterPtr);
    free((void*)texture);
}

EMSCRIPTEN_KEEPALIVE void host_SetTextureWrap(unsigned int texturePtr, unsigned int wrapPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    SetTextureWrap(*texture, wrapPtr);
    free((void*)texture);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTexture(unsigned int texturePtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTexture(*texture, posXPtr, posYPtr, *tint);
    free((void*)texture);
    free((void*)tint);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTextureV(unsigned int texturePtr, unsigned int positionPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextureV(*texture, *position, *tint);
    free((void*)texture);
    free((void*)position);
    free((void*)tint);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTextureEx(unsigned int texturePtr, unsigned int positionPtr, unsigned int rotationPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextureEx(*texture, *position, rotationPtr, scalePtr, *tint);
    free((void*)texture);
    free((void*)position);
    free((void*)tint);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTextureRec(unsigned int texturePtr, unsigned int sourcePtr, unsigned int positionPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawTexturePro(unsigned int texturePtr, unsigned int sourcePtr, unsigned int destPtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawTextureNPatch(unsigned int texturePtr, unsigned int nPatchInfoPtr, unsigned int destPtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE unsigned int host_ColorIsEqual(unsigned int col1Ptr, unsigned int col2Ptr) {
    Color* col1 = cart_get_pointer(col1Ptr, sizeof(Color));
    Color* col2 = cart_get_pointer(col2Ptr, sizeof(Color));
    return ColorIsEqual(*col1, *col2);
    free((void*)col1);
    free((void*)col2);
}

EMSCRIPTEN_KEEPALIVE void host_Fade(unsigned int outPtr, unsigned int colorPtr, unsigned int alphaPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color out = Fade(*color, alphaPtr);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ColorToInt(unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    return ColorToInt(*color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_ColorNormalize(unsigned int outPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Vector4 out = ColorNormalize(*color);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorFromNormalized(unsigned int outPtr, unsigned int normalizedPtr) {
    Vector4* normalized = cart_get_pointer(normalizedPtr, sizeof(Vector4));
    Color out = ColorFromNormalized(*normalized);
    free((void*)normalized);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorToHSV(unsigned int outPtr, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Vector3 out = ColorToHSV(*color);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorFromHSV(unsigned int outPtr, unsigned int huePtr, unsigned int saturationPtr, unsigned int valuePtr) {
    Color out = ColorFromHSV(huePtr, saturationPtr, valuePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorTint(unsigned int outPtr, unsigned int colorPtr, unsigned int tintPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    Color out = ColorTint(*color, *tint);
    free((void*)color);
    free((void*)tint);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorBrightness(unsigned int outPtr, unsigned int colorPtr, unsigned int factorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color out = ColorBrightness(*color, factorPtr);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorContrast(unsigned int outPtr, unsigned int colorPtr, unsigned int contrastPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color out = ColorContrast(*color, contrastPtr);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorAlpha(unsigned int outPtr, unsigned int colorPtr, unsigned int alphaPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    Color out = ColorAlpha(*color, alphaPtr);
    free((void*)color);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorAlphaBlend(unsigned int outPtr, unsigned int dstPtr, unsigned int srcPtr, unsigned int tintPtr) {
    Color* dst = cart_get_pointer(dstPtr, sizeof(Color));
    Color* src = cart_get_pointer(srcPtr, sizeof(Color));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    Color out = ColorAlphaBlend(*dst, *src, *tint);
    free((void*)dst);
    free((void*)src);
    free((void*)tint);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ColorLerp(unsigned int outPtr, unsigned int color1Ptr, unsigned int color2Ptr, unsigned int factorPtr) {
    Color* color1 = cart_get_pointer(color1Ptr, sizeof(Color));
    Color* color2 = cart_get_pointer(color2Ptr, sizeof(Color));
    Color out = ColorLerp(*color1, *color2, factorPtr);
    free((void*)color1);
    free((void*)color2);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetColor(unsigned int outPtr, unsigned int hexValuePtr) {
    Color out = GetColor(hexValuePtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetPixelColor(unsigned int outPtr, unsigned int srcPtrPtr, unsigned int formatPtr) {
    void ** srcPtr = cart_get_pointer(srcPtrPtr, sizeof(void *));
    Color out = GetPixelColor(srcPtr, formatPtr);
    free((void*)srcPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetPixelColor(unsigned int dstPtrPtr, unsigned int colorPtr, unsigned int formatPtr) {
    void ** dstPtr = cart_get_pointer(dstPtrPtr, sizeof(void *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    SetPixelColor(dstPtr, *color, formatPtr);
    free((void*)dstPtr);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetPixelDataSize(unsigned int widthPtr, unsigned int heightPtr, unsigned int formatPtr) {
    return GetPixelDataSize(widthPtr, heightPtr, formatPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetFontDefault(unsigned int outPtr) {
    Font out = GetFontDefault();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadFont(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Font out = LoadFont(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadFontEx(unsigned int outPtr, unsigned int fileNamePtr, unsigned int fontSizePtr, unsigned int codepointsPtr, unsigned int codepointCountPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(int *));
    Font out = LoadFontEx(fileName, fontSizePtr, codepoints, codepointCountPtr);
    free((void*)fileName);
    free((void*)codepoints);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadFontFromImage(unsigned int outPtr, unsigned int imagePtr, unsigned int keyPtr, unsigned int firstCharPtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Color* key = cart_get_pointer(keyPtr, sizeof(Color));
    Font out = LoadFontFromImage(*image, *key, firstCharPtr);
    free((void*)image);
    free((void*)key);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadFontFromMemory(unsigned int outPtr, unsigned int fileTypePtr, unsigned int fileDataPtr, unsigned int dataSizePtr, unsigned int fontSizePtr, unsigned int codepointsPtr, unsigned int codepointCountPtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(int *));
    Font out = LoadFontFromMemory(fileType, fileData, dataSizePtr, fontSizePtr, codepoints, codepointCountPtr);
    free((void*)fileType);
    free((void*)fileData);
    free((void*)codepoints);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsFontValid(unsigned int fontPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    return IsFontValid(*font);
    free((void*)font);
}

EMSCRIPTEN_KEEPALIVE void host_LoadFontData(unsigned int outPtr, unsigned int fileDataPtr, unsigned int dataSizePtr, unsigned int fontSizePtr, unsigned int codepointsPtr, unsigned int codepointCountPtr, unsigned int typePtr) {
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(int *));
    GlyphInfo * out = LoadFontData(fileData, dataSizePtr, fontSizePtr, codepoints, codepointCountPtr, typePtr);
    free((void*)fileData);
    free((void*)codepoints);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenImageFontAtlas(unsigned int outPtr, unsigned int glyphsPtr, unsigned int glyphRecsPtr, unsigned int glyphCountPtr, unsigned int fontSizePtr, unsigned int paddingPtr, unsigned int packMethodPtr) {
    const GlyphInfo ** glyphs = cart_get_pointer(glyphsPtr, sizeof(const GlyphInfo *));
    Rectangle *** glyphRecs = cart_get_pointer(glyphRecsPtr, sizeof(Rectangle **));
    Image out = GenImageFontAtlas(glyphs, glyphRecs, glyphCountPtr, fontSizePtr, paddingPtr, packMethodPtr);
    free((void*)glyphs);
    free((void*)glyphRecs);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadFontData(unsigned int glyphsPtr, unsigned int glyphCountPtr) {
    GlyphInfo ** glyphs = cart_get_pointer(glyphsPtr, sizeof(GlyphInfo *));
    UnloadFontData(glyphs, glyphCountPtr);
    free((void*)glyphs);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadFont(unsigned int fontPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    UnloadFont(*font);
    free((void*)font);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportFontAsCode(unsigned int fontPtr, unsigned int fileNamePtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportFontAsCode(*font, fileName);
    free((void*)font);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_DrawFPS(unsigned int posXPtr, unsigned int posYPtr) {
    DrawFPS(posXPtr, posYPtr);
}

EMSCRIPTEN_KEEPALIVE void host_DrawText(unsigned int textPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int fontSizePtr, unsigned int colorPtr) {
    const char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawText(text, posXPtr, posYPtr, fontSizePtr, *color);
    free((void*)text);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTextEx(unsigned int fontPtr, unsigned int textPtr, unsigned int positionPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawTextPro(unsigned int fontPtr, unsigned int textPtr, unsigned int positionPtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawTextCodepoint(unsigned int fontPtr, unsigned int codepointPtr, unsigned int positionPtr, unsigned int fontSizePtr, unsigned int tintPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    Vector2* position = cart_get_pointer(positionPtr, sizeof(Vector2));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTextCodepoint(*font, codepointPtr, *position, fontSizePtr, *tint);
    free((void*)font);
    free((void*)position);
    free((void*)tint);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTextCodepoints(unsigned int fontPtr, unsigned int codepointsPtr, unsigned int codepointCountPtr, unsigned int positionPtr, unsigned int fontSizePtr, unsigned int spacingPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_SetTextLineSpacing(unsigned int spacingPtr) {
    SetTextLineSpacing(spacingPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_MeasureText(unsigned int textPtr, unsigned int fontSizePtr) {
    const char* text = cart_get_string(textPtr);
    return MeasureText(text, fontSizePtr);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE void host_MeasureTextEx(unsigned int outPtr, unsigned int fontPtr, unsigned int textPtr, unsigned int fontSizePtr, unsigned int spacingPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    const char* text = cart_get_string(textPtr);
    Vector2 out = MeasureTextEx(*font, text, fontSizePtr, spacingPtr);
    free((void*)font);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetGlyphIndex(unsigned int fontPtr, unsigned int codepointPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    return GetGlyphIndex(*font, codepointPtr);
    free((void*)font);
}

EMSCRIPTEN_KEEPALIVE void host_GetGlyphInfo(unsigned int outPtr, unsigned int fontPtr, unsigned int codepointPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    GlyphInfo out = GetGlyphInfo(*font, codepointPtr);
    free((void*)font);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetGlyphAtlasRec(unsigned int outPtr, unsigned int fontPtr, unsigned int codepointPtr) {
    Font* font = cart_get_pointer(fontPtr, sizeof(Font));
    Rectangle out = GetGlyphAtlasRec(*font, codepointPtr);
    free((void*)font);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadUTF8(unsigned int outPtr, unsigned int codepointsPtr, unsigned int lengthPtr) {
    const int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(const int *));
    char * out = LoadUTF8(codepoints, lengthPtr);
    free((void*)codepoints);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadUTF8(unsigned int textPtr) {
    char ** text = cart_get_pointer(textPtr, sizeof(char *));
    UnloadUTF8(text);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE void host_LoadCodepoints(unsigned int outPtr, unsigned int textPtr, unsigned int countPtr) {
    const char* text = cart_get_string(textPtr);
    int ** count = cart_get_pointer(countPtr, sizeof(int *));
    int * out = LoadCodepoints(text, count);
    free((void*)text);
    free((void*)count);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadCodepoints(unsigned int codepointsPtr) {
    int ** codepoints = cart_get_pointer(codepointsPtr, sizeof(int *));
    UnloadCodepoints(codepoints);
    free((void*)codepoints);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetCodepointCount(unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    return GetCodepointCount(text);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetCodepoint(unsigned int textPtr, unsigned int codepointSizePtr) {
    const char* text = cart_get_string(textPtr);
    int ** codepointSize = cart_get_pointer(codepointSizePtr, sizeof(int *));
    return GetCodepoint(text, codepointSize);
    free((void*)text);
    free((void*)codepointSize);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetCodepointNext(unsigned int textPtr, unsigned int codepointSizePtr) {
    const char* text = cart_get_string(textPtr);
    int ** codepointSize = cart_get_pointer(codepointSizePtr, sizeof(int *));
    return GetCodepointNext(text, codepointSize);
    free((void*)text);
    free((void*)codepointSize);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetCodepointPrevious(unsigned int textPtr, unsigned int codepointSizePtr) {
    const char* text = cart_get_string(textPtr);
    int ** codepointSize = cart_get_pointer(codepointSizePtr, sizeof(int *));
    return GetCodepointPrevious(text, codepointSize);
    free((void*)text);
    free((void*)codepointSize);
}

EMSCRIPTEN_KEEPALIVE void host_CodepointToUTF8(unsigned int outPtr, unsigned int codepointPtr, unsigned int utf8SizePtr) {
    int ** utf8Size = cart_get_pointer(utf8SizePtr, sizeof(int *));
    const char * out = CodepointToUTF8(codepointPtr, utf8Size);
    free((void*)utf8Size);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_TextCopy(unsigned int dstPtr, unsigned int srcPtr) {
    char ** dst = cart_get_pointer(dstPtr, sizeof(char *));
    const char* src = cart_get_string(srcPtr);
    return TextCopy(dst, src);
    free((void*)dst);
    free((void*)src);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_TextIsEqual(unsigned int text1Ptr, unsigned int text2Ptr) {
    const char* text1 = cart_get_string(text1Ptr);
    const char* text2 = cart_get_string(text2Ptr);
    return TextIsEqual(text1, text2);
    free((void*)text1);
    free((void*)text2);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_TextLength(unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    return TextLength(text);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE void host_TextSubtext(unsigned int outPtr, unsigned int textPtr, unsigned int positionPtr, unsigned int lengthPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextSubtext(text, positionPtr, lengthPtr);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextReplace(unsigned int outPtr, unsigned int textPtr, unsigned int replacePtr, unsigned int byPtr) {
    const char* text = cart_get_string(textPtr);
    const char* replace = cart_get_string(replacePtr);
    const char* by = cart_get_string(byPtr);
    char * out = TextReplace(text, replace, by);
    free((void*)text);
    free((void*)replace);
    free((void*)by);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextInsert(unsigned int outPtr, unsigned int textPtr, unsigned int insertPtr, unsigned int positionPtr) {
    const char* text = cart_get_string(textPtr);
    const char* insert = cart_get_string(insertPtr);
    char * out = TextInsert(text, insert, positionPtr);
    free((void*)text);
    free((void*)insert);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextJoin(unsigned int outPtr, unsigned int textListPtr, unsigned int countPtr, unsigned int delimiterPtr) {
    const char *** textList = cart_get_pointer(textListPtr, sizeof(const char **));
    const char* delimiter = cart_get_string(delimiterPtr);
    const char * out = TextJoin(textList, countPtr, delimiter);
    free((void*)textList);
    free((void*)delimiter);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextSplit(unsigned int outPtr, unsigned int textPtr, unsigned int delimiterPtr, unsigned int countPtr) {
    const char* text = cart_get_string(textPtr);
    int ** count = cart_get_pointer(countPtr, sizeof(int *));
    const char ** out = TextSplit(text, delimiterPtr, count);
    free((void*)text);
    free((void*)count);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextAppend(unsigned int textPtr, unsigned int appendPtr, unsigned int positionPtr) {
    char ** text = cart_get_pointer(textPtr, sizeof(char *));
    const char* append = cart_get_string(appendPtr);
    int ** position = cart_get_pointer(positionPtr, sizeof(int *));
    TextAppend(text, append, position);
    free((void*)text);
    free((void*)append);
    free((void*)position);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_TextFindIndex(unsigned int textPtr, unsigned int findPtr) {
    const char* text = cart_get_string(textPtr);
    const char* find = cart_get_string(findPtr);
    return TextFindIndex(text, find);
    free((void*)text);
    free((void*)find);
}

EMSCRIPTEN_KEEPALIVE void host_TextToUpper(unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToUpper(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextToLower(unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToLower(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextToPascal(unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToPascal(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextToSnake(unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToSnake(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_TextToCamel(unsigned int outPtr, unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    const char * out = TextToCamel(text);
    free((void*)text);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_TextToInteger(unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    return TextToInteger(text);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_TextToFloat(unsigned int textPtr) {
    const char* text = cart_get_string(textPtr);
    return TextToFloat(text);
    free((void*)text);
}

EMSCRIPTEN_KEEPALIVE void host_DrawLine3D(unsigned int startPosPtr, unsigned int endPosPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawLine3D(*startPos, *endPos, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawPoint3D(unsigned int positionPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPoint3D(*position, *color);
    free((void*)position);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCircle3D(unsigned int centerPtr, unsigned int radiusPtr, unsigned int rotationAxisPtr, unsigned int rotationAnglePtr, unsigned int colorPtr) {
    Vector3* center = cart_get_pointer(centerPtr, sizeof(Vector3));
    Vector3* rotationAxis = cart_get_pointer(rotationAxisPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCircle3D(*center, radiusPtr, *rotationAxis, rotationAnglePtr, *color);
    free((void*)center);
    free((void*)rotationAxis);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTriangle3D(unsigned int v1Ptr, unsigned int v2Ptr, unsigned int v3Ptr, unsigned int colorPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawTriangleStrip3D(unsigned int pointsPtr, unsigned int pointCountPtr, unsigned int colorPtr) {
    const Vector3 ** points = cart_get_pointer(pointsPtr, sizeof(const Vector3 *));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawTriangleStrip3D(points, pointCountPtr, *color);
    free((void*)points);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCube(unsigned int positionPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int lengthPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCube(*position, widthPtr, heightPtr, lengthPtr, *color);
    free((void*)position);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCubeV(unsigned int positionPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector3* size = cart_get_pointer(sizePtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCubeV(*position, *size, *color);
    free((void*)position);
    free((void*)size);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCubeWires(unsigned int positionPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int lengthPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCubeWires(*position, widthPtr, heightPtr, lengthPtr, *color);
    free((void*)position);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCubeWiresV(unsigned int positionPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Vector3* size = cart_get_pointer(sizePtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCubeWiresV(*position, *size, *color);
    free((void*)position);
    free((void*)size);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSphere(unsigned int centerPosPtr, unsigned int radiusPtr, unsigned int colorPtr) {
    Vector3* centerPos = cart_get_pointer(centerPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSphere(*centerPos, radiusPtr, *color);
    free((void*)centerPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSphereEx(unsigned int centerPosPtr, unsigned int radiusPtr, unsigned int ringsPtr, unsigned int slicesPtr, unsigned int colorPtr) {
    Vector3* centerPos = cart_get_pointer(centerPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSphereEx(*centerPos, radiusPtr, ringsPtr, slicesPtr, *color);
    free((void*)centerPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawSphereWires(unsigned int centerPosPtr, unsigned int radiusPtr, unsigned int ringsPtr, unsigned int slicesPtr, unsigned int colorPtr) {
    Vector3* centerPos = cart_get_pointer(centerPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawSphereWires(*centerPos, radiusPtr, ringsPtr, slicesPtr, *color);
    free((void*)centerPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCylinder(unsigned int positionPtr, unsigned int radiusTopPtr, unsigned int radiusBottomPtr, unsigned int heightPtr, unsigned int slicesPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCylinder(*position, radiusTopPtr, radiusBottomPtr, heightPtr, slicesPtr, *color);
    free((void*)position);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCylinderEx(unsigned int startPosPtr, unsigned int endPosPtr, unsigned int startRadiusPtr, unsigned int endRadiusPtr, unsigned int sidesPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCylinderEx(*startPos, *endPos, startRadiusPtr, endRadiusPtr, sidesPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCylinderWires(unsigned int positionPtr, unsigned int radiusTopPtr, unsigned int radiusBottomPtr, unsigned int heightPtr, unsigned int slicesPtr, unsigned int colorPtr) {
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCylinderWires(*position, radiusTopPtr, radiusBottomPtr, heightPtr, slicesPtr, *color);
    free((void*)position);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCylinderWiresEx(unsigned int startPosPtr, unsigned int endPosPtr, unsigned int startRadiusPtr, unsigned int endRadiusPtr, unsigned int sidesPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCylinderWiresEx(*startPos, *endPos, startRadiusPtr, endRadiusPtr, sidesPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCapsule(unsigned int startPosPtr, unsigned int endPosPtr, unsigned int radiusPtr, unsigned int slicesPtr, unsigned int ringsPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCapsule(*startPos, *endPos, radiusPtr, slicesPtr, ringsPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawCapsuleWires(unsigned int startPosPtr, unsigned int endPosPtr, unsigned int radiusPtr, unsigned int slicesPtr, unsigned int ringsPtr, unsigned int colorPtr) {
    Vector3* startPos = cart_get_pointer(startPosPtr, sizeof(Vector3));
    Vector3* endPos = cart_get_pointer(endPosPtr, sizeof(Vector3));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawCapsuleWires(*startPos, *endPos, radiusPtr, slicesPtr, ringsPtr, *color);
    free((void*)startPos);
    free((void*)endPos);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawPlane(unsigned int centerPosPtr, unsigned int sizePtr, unsigned int colorPtr) {
    Vector3* centerPos = cart_get_pointer(centerPosPtr, sizeof(Vector3));
    Vector2* size = cart_get_pointer(sizePtr, sizeof(Vector2));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawPlane(*centerPos, *size, *color);
    free((void*)centerPos);
    free((void*)size);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawRay(unsigned int rayPtr, unsigned int colorPtr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawRay(*ray, *color);
    free((void*)ray);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawGrid(unsigned int slicesPtr, unsigned int spacingPtr) {
    DrawGrid(slicesPtr, spacingPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadModel(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Model out = LoadModel(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadModelFromMesh(unsigned int outPtr, unsigned int meshPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    Model out = LoadModelFromMesh(*mesh);
    free((void*)mesh);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsModelValid(unsigned int modelPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    return IsModelValid(*model);
    free((void*)model);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadModel(unsigned int modelPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    UnloadModel(*model);
    free((void*)model);
}

EMSCRIPTEN_KEEPALIVE void host_GetModelBoundingBox(unsigned int outPtr, unsigned int modelPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    BoundingBox out = GetModelBoundingBox(*model);
    free((void*)model);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_DrawModel(unsigned int modelPtr, unsigned int positionPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModel(*model, *position, scalePtr, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)tint);
}

EMSCRIPTEN_KEEPALIVE void host_DrawModelEx(unsigned int modelPtr, unsigned int positionPtr, unsigned int rotationAxisPtr, unsigned int rotationAnglePtr, unsigned int scalePtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawModelWires(unsigned int modelPtr, unsigned int positionPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModelWires(*model, *position, scalePtr, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)tint);
}

EMSCRIPTEN_KEEPALIVE void host_DrawModelWiresEx(unsigned int modelPtr, unsigned int positionPtr, unsigned int rotationAxisPtr, unsigned int rotationAnglePtr, unsigned int scalePtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawModelPoints(unsigned int modelPtr, unsigned int positionPtr, unsigned int scalePtr, unsigned int tintPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    Vector3* position = cart_get_pointer(positionPtr, sizeof(Vector3));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawModelPoints(*model, *position, scalePtr, *tint);
    free((void*)model);
    free((void*)position);
    free((void*)tint);
}

EMSCRIPTEN_KEEPALIVE void host_DrawModelPointsEx(unsigned int modelPtr, unsigned int positionPtr, unsigned int rotationAxisPtr, unsigned int rotationAnglePtr, unsigned int scalePtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawBoundingBox(unsigned int boxPtr, unsigned int colorPtr) {
    BoundingBox* box = cart_get_pointer(boxPtr, sizeof(BoundingBox));
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawBoundingBox(*box, *color);
    free((void*)box);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawBillboard(unsigned int cameraPtr, unsigned int texturePtr, unsigned int positionPtr, unsigned int scalePtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawBillboardRec(unsigned int cameraPtr, unsigned int texturePtr, unsigned int sourcePtr, unsigned int positionPtr, unsigned int sizePtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_DrawBillboardPro(unsigned int cameraPtr, unsigned int texturePtr, unsigned int sourcePtr, unsigned int positionPtr, unsigned int upPtr, unsigned int sizePtr, unsigned int originPtr, unsigned int rotationPtr, unsigned int tintPtr) {
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

EMSCRIPTEN_KEEPALIVE void host_UploadMesh(unsigned int meshPtr, unsigned int dynamicPtr) {
    Mesh ** mesh = cart_get_pointer(meshPtr, sizeof(Mesh *));
    UploadMesh(mesh, dynamicPtr);
    free((void*)mesh);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateMeshBuffer(unsigned int meshPtr, unsigned int indexPtr, unsigned int dataPtr, unsigned int dataSizePtr, unsigned int offsetPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    const void ** data = cart_get_pointer(dataPtr, sizeof(const void *));
    UpdateMeshBuffer(*mesh, indexPtr, data, dataSizePtr, offsetPtr);
    free((void*)mesh);
    free((void*)data);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadMesh(unsigned int meshPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    UnloadMesh(*mesh);
    free((void*)mesh);
}

EMSCRIPTEN_KEEPALIVE void host_DrawMesh(unsigned int meshPtr, unsigned int materialPtr, unsigned int transformPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    Material* material = cart_get_pointer(materialPtr, sizeof(Material));
    Matrix* transform = cart_get_pointer(transformPtr, sizeof(Matrix));
    DrawMesh(*mesh, *material, *transform);
    free((void*)mesh);
    free((void*)material);
    free((void*)transform);
}

EMSCRIPTEN_KEEPALIVE void host_DrawMeshInstanced(unsigned int meshPtr, unsigned int materialPtr, unsigned int transformsPtr, unsigned int instancesPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    Material* material = cart_get_pointer(materialPtr, sizeof(Material));
    const Matrix ** transforms = cart_get_pointer(transformsPtr, sizeof(const Matrix *));
    DrawMeshInstanced(*mesh, *material, transforms, instancesPtr);
    free((void*)mesh);
    free((void*)material);
    free((void*)transforms);
}

EMSCRIPTEN_KEEPALIVE void host_GetMeshBoundingBox(unsigned int outPtr, unsigned int meshPtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    BoundingBox out = GetMeshBoundingBox(*mesh);
    free((void*)mesh);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshTangents(unsigned int meshPtr) {
    Mesh ** mesh = cart_get_pointer(meshPtr, sizeof(Mesh *));
    GenMeshTangents(mesh);
    free((void*)mesh);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportMesh(unsigned int meshPtr, unsigned int fileNamePtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportMesh(*mesh, fileName);
    free((void*)mesh);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportMeshAsCode(unsigned int meshPtr, unsigned int fileNamePtr) {
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportMeshAsCode(*mesh, fileName);
    free((void*)mesh);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshPoly(unsigned int outPtr, unsigned int sidesPtr, unsigned int radiusPtr) {
    Mesh out = GenMeshPoly(sidesPtr, radiusPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshPlane(unsigned int outPtr, unsigned int widthPtr, unsigned int lengthPtr, unsigned int resXPtr, unsigned int resZPtr) {
    Mesh out = GenMeshPlane(widthPtr, lengthPtr, resXPtr, resZPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshCube(unsigned int outPtr, unsigned int widthPtr, unsigned int heightPtr, unsigned int lengthPtr) {
    Mesh out = GenMeshCube(widthPtr, heightPtr, lengthPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshSphere(unsigned int outPtr, unsigned int radiusPtr, unsigned int ringsPtr, unsigned int slicesPtr) {
    Mesh out = GenMeshSphere(radiusPtr, ringsPtr, slicesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshHemiSphere(unsigned int outPtr, unsigned int radiusPtr, unsigned int ringsPtr, unsigned int slicesPtr) {
    Mesh out = GenMeshHemiSphere(radiusPtr, ringsPtr, slicesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshCylinder(unsigned int outPtr, unsigned int radiusPtr, unsigned int heightPtr, unsigned int slicesPtr) {
    Mesh out = GenMeshCylinder(radiusPtr, heightPtr, slicesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshCone(unsigned int outPtr, unsigned int radiusPtr, unsigned int heightPtr, unsigned int slicesPtr) {
    Mesh out = GenMeshCone(radiusPtr, heightPtr, slicesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshTorus(unsigned int outPtr, unsigned int radiusPtr, unsigned int sizePtr, unsigned int radSegPtr, unsigned int sidesPtr) {
    Mesh out = GenMeshTorus(radiusPtr, sizePtr, radSegPtr, sidesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshKnot(unsigned int outPtr, unsigned int radiusPtr, unsigned int sizePtr, unsigned int radSegPtr, unsigned int sidesPtr) {
    Mesh out = GenMeshKnot(radiusPtr, sizePtr, radSegPtr, sidesPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshHeightmap(unsigned int outPtr, unsigned int heightmapPtr, unsigned int sizePtr) {
    Image* heightmap = cart_get_pointer(heightmapPtr, sizeof(Image));
    Vector3* size = cart_get_pointer(sizePtr, sizeof(Vector3));
    Mesh out = GenMeshHeightmap(*heightmap, *size);
    free((void*)heightmap);
    free((void*)size);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GenMeshCubicmap(unsigned int outPtr, unsigned int cubicmapPtr, unsigned int cubeSizePtr) {
    Image* cubicmap = cart_get_pointer(cubicmapPtr, sizeof(Image));
    Vector3* cubeSize = cart_get_pointer(cubeSizePtr, sizeof(Vector3));
    Mesh out = GenMeshCubicmap(*cubicmap, *cubeSize);
    free((void*)cubicmap);
    free((void*)cubeSize);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadMaterials(unsigned int outPtr, unsigned int fileNamePtr, unsigned int materialCountPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** materialCount = cart_get_pointer(materialCountPtr, sizeof(int *));
    Material * out = LoadMaterials(fileName, materialCount);
    free((void*)fileName);
    free((void*)materialCount);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadMaterialDefault(unsigned int outPtr) {
    Material out = LoadMaterialDefault();
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsMaterialValid(unsigned int materialPtr) {
    Material* material = cart_get_pointer(materialPtr, sizeof(Material));
    return IsMaterialValid(*material);
    free((void*)material);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadMaterial(unsigned int materialPtr) {
    Material* material = cart_get_pointer(materialPtr, sizeof(Material));
    UnloadMaterial(*material);
    free((void*)material);
}

EMSCRIPTEN_KEEPALIVE void host_SetMaterialTexture(unsigned int materialPtr, unsigned int mapTypePtr, unsigned int texturePtr) {
    Material ** material = cart_get_pointer(materialPtr, sizeof(Material *));
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    SetMaterialTexture(material, mapTypePtr, *texture);
    free((void*)material);
    free((void*)texture);
}

EMSCRIPTEN_KEEPALIVE void host_SetModelMeshMaterial(unsigned int modelPtr, unsigned int meshIdPtr, unsigned int materialIdPtr) {
    Model ** model = cart_get_pointer(modelPtr, sizeof(Model *));
    SetModelMeshMaterial(model, meshIdPtr, materialIdPtr);
    free((void*)model);
}

EMSCRIPTEN_KEEPALIVE void host_LoadModelAnimations(unsigned int outPtr, unsigned int fileNamePtr, unsigned int animCountPtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    int ** animCount = cart_get_pointer(animCountPtr, sizeof(int *));
    ModelAnimation * out = LoadModelAnimations(fileName, animCount);
    free((void*)fileName);
    free((void*)animCount);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateModelAnimation(unsigned int modelPtr, unsigned int animPtr, unsigned int framePtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    ModelAnimation* anim = cart_get_pointer(animPtr, sizeof(ModelAnimation));
    UpdateModelAnimation(*model, *anim, framePtr);
    free((void*)model);
    free((void*)anim);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateModelAnimationBones(unsigned int modelPtr, unsigned int animPtr, unsigned int framePtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    ModelAnimation* anim = cart_get_pointer(animPtr, sizeof(ModelAnimation));
    UpdateModelAnimationBones(*model, *anim, framePtr);
    free((void*)model);
    free((void*)anim);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadModelAnimation(unsigned int animPtr) {
    ModelAnimation* anim = cart_get_pointer(animPtr, sizeof(ModelAnimation));
    UnloadModelAnimation(*anim);
    free((void*)anim);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadModelAnimations(unsigned int animationsPtr, unsigned int animCountPtr) {
    ModelAnimation ** animations = cart_get_pointer(animationsPtr, sizeof(ModelAnimation *));
    UnloadModelAnimations(animations, animCountPtr);
    free((void*)animations);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsModelAnimationValid(unsigned int modelPtr, unsigned int animPtr) {
    Model* model = cart_get_pointer(modelPtr, sizeof(Model));
    ModelAnimation* anim = cart_get_pointer(animPtr, sizeof(ModelAnimation));
    return IsModelAnimationValid(*model, *anim);
    free((void*)model);
    free((void*)anim);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionSpheres(unsigned int center1Ptr, unsigned int radius1Ptr, unsigned int center2Ptr, unsigned int radius2Ptr) {
    Vector3* center1 = cart_get_pointer(center1Ptr, sizeof(Vector3));
    Vector3* center2 = cart_get_pointer(center2Ptr, sizeof(Vector3));
    return CheckCollisionSpheres(*center1, radius1Ptr, *center2, radius2Ptr);
    free((void*)center1);
    free((void*)center2);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionBoxes(unsigned int box1Ptr, unsigned int box2Ptr) {
    BoundingBox* box1 = cart_get_pointer(box1Ptr, sizeof(BoundingBox));
    BoundingBox* box2 = cart_get_pointer(box2Ptr, sizeof(BoundingBox));
    return CheckCollisionBoxes(*box1, *box2);
    free((void*)box1);
    free((void*)box2);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_CheckCollisionBoxSphere(unsigned int boxPtr, unsigned int centerPtr, unsigned int radiusPtr) {
    BoundingBox* box = cart_get_pointer(boxPtr, sizeof(BoundingBox));
    Vector3* center = cart_get_pointer(centerPtr, sizeof(Vector3));
    return CheckCollisionBoxSphere(*box, *center, radiusPtr);
    free((void*)box);
    free((void*)center);
}

EMSCRIPTEN_KEEPALIVE void host_GetRayCollisionSphere(unsigned int outPtr, unsigned int rayPtr, unsigned int centerPtr, unsigned int radiusPtr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    Vector3* center = cart_get_pointer(centerPtr, sizeof(Vector3));
    RayCollision out = GetRayCollisionSphere(*ray, *center, radiusPtr);
    free((void*)ray);
    free((void*)center);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetRayCollisionBox(unsigned int outPtr, unsigned int rayPtr, unsigned int boxPtr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    BoundingBox* box = cart_get_pointer(boxPtr, sizeof(BoundingBox));
    RayCollision out = GetRayCollisionBox(*ray, *box);
    free((void*)ray);
    free((void*)box);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetRayCollisionMesh(unsigned int outPtr, unsigned int rayPtr, unsigned int meshPtr, unsigned int transformPtr) {
    Ray* ray = cart_get_pointer(rayPtr, sizeof(Ray));
    Mesh* mesh = cart_get_pointer(meshPtr, sizeof(Mesh));
    Matrix* transform = cart_get_pointer(transformPtr, sizeof(Matrix));
    RayCollision out = GetRayCollisionMesh(*ray, *mesh, *transform);
    free((void*)ray);
    free((void*)mesh);
    free((void*)transform);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_GetRayCollisionTriangle(unsigned int outPtr, unsigned int rayPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr) {
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

EMSCRIPTEN_KEEPALIVE void host_GetRayCollisionQuad(unsigned int outPtr, unsigned int rayPtr, unsigned int p1Ptr, unsigned int p2Ptr, unsigned int p3Ptr, unsigned int p4Ptr) {
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

EMSCRIPTEN_KEEPALIVE void host_InitAudioDevice() {
    InitAudioDevice();
}

EMSCRIPTEN_KEEPALIVE void host_CloseAudioDevice() {
    CloseAudioDevice();
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsAudioDeviceReady() {
    return IsAudioDeviceReady();
}

EMSCRIPTEN_KEEPALIVE void host_SetMasterVolume(unsigned int volumePtr) {
    SetMasterVolume(volumePtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMasterVolume() {
    return GetMasterVolume();
}

EMSCRIPTEN_KEEPALIVE void host_LoadWave(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Wave out = LoadWave(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadWaveFromMemory(unsigned int outPtr, unsigned int fileTypePtr, unsigned int fileDataPtr, unsigned int dataSizePtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** fileData = cart_get_pointer(fileDataPtr, sizeof(const unsigned char *));
    Wave out = LoadWaveFromMemory(fileType, fileData, dataSizePtr);
    free((void*)fileType);
    free((void*)fileData);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsWaveValid(unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    return IsWaveValid(*wave);
    free((void*)wave);
}

EMSCRIPTEN_KEEPALIVE void host_LoadSound(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Sound out = LoadSound(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadSoundFromWave(unsigned int outPtr, unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    Sound out = LoadSoundFromWave(*wave);
    free((void*)wave);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadSoundAlias(unsigned int outPtr, unsigned int sourcePtr) {
    Sound* source = cart_get_pointer(sourcePtr, sizeof(Sound));
    Sound out = LoadSoundAlias(*source);
    free((void*)source);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsSoundValid(unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    return IsSoundValid(*sound);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateSound(unsigned int soundPtr, unsigned int dataPtr, unsigned int sampleCountPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    const void ** data = cart_get_pointer(dataPtr, sizeof(const void *));
    UpdateSound(*sound, data, sampleCountPtr);
    free((void*)sound);
    free((void*)data);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadWave(unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    UnloadWave(*wave);
    free((void*)wave);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadSound(unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    UnloadSound(*sound);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadSoundAlias(unsigned int aliasPtr) {
    Sound* alias = cart_get_pointer(aliasPtr, sizeof(Sound));
    UnloadSoundAlias(*alias);
    free((void*)alias);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportWave(unsigned int wavePtr, unsigned int fileNamePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportWave(*wave, fileName);
    free((void*)wave);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_ExportWaveAsCode(unsigned int wavePtr, unsigned int fileNamePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    const char* fileName = cart_get_string(fileNamePtr);
    return ExportWaveAsCode(*wave, fileName);
    free((void*)wave);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_PlaySound(unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    PlaySound(*sound);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_StopSound(unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    StopSound(*sound);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_PauseSound(unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    PauseSound(*sound);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_ResumeSound(unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    ResumeSound(*sound);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsSoundPlaying(unsigned int soundPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    return IsSoundPlaying(*sound);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_SetSoundVolume(unsigned int soundPtr, unsigned int volumePtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    SetSoundVolume(*sound, volumePtr);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_SetSoundPitch(unsigned int soundPtr, unsigned int pitchPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    SetSoundPitch(*sound, pitchPtr);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_SetSoundPan(unsigned int soundPtr, unsigned int panPtr) {
    Sound* sound = cart_get_pointer(soundPtr, sizeof(Sound));
    SetSoundPan(*sound, panPtr);
    free((void*)sound);
}

EMSCRIPTEN_KEEPALIVE void host_WaveCopy(unsigned int outPtr, unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    Wave out = WaveCopy(*wave);
    free((void*)wave);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_WaveCrop(unsigned int wavePtr, unsigned int initFramePtr, unsigned int finalFramePtr) {
    Wave ** wave = cart_get_pointer(wavePtr, sizeof(Wave *));
    WaveCrop(wave, initFramePtr, finalFramePtr);
    free((void*)wave);
}

EMSCRIPTEN_KEEPALIVE void host_WaveFormat(unsigned int wavePtr, unsigned int sampleRatePtr, unsigned int sampleSizePtr, unsigned int channelsPtr) {
    Wave ** wave = cart_get_pointer(wavePtr, sizeof(Wave *));
    WaveFormat(wave, sampleRatePtr, sampleSizePtr, channelsPtr);
    free((void*)wave);
}

EMSCRIPTEN_KEEPALIVE void host_LoadWaveSamples(unsigned int outPtr, unsigned int wavePtr) {
    Wave* wave = cart_get_pointer(wavePtr, sizeof(Wave));
    float * out = LoadWaveSamples(*wave);
    free((void*)wave);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadWaveSamples(unsigned int samplesPtr) {
    float ** samples = cart_get_pointer(samplesPtr, sizeof(float *));
    UnloadWaveSamples(samples);
    free((void*)samples);
}

EMSCRIPTEN_KEEPALIVE void host_LoadMusicStream(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Music out = LoadMusicStream(fileName);
    free((void*)fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE void host_LoadMusicStreamFromMemory(unsigned int outPtr, unsigned int fileTypePtr, unsigned int dataPtr, unsigned int dataSizePtr) {
    const char* fileType = cart_get_string(fileTypePtr);
    const unsigned char ** data = cart_get_pointer(dataPtr, sizeof(const unsigned char *));
    Music out = LoadMusicStreamFromMemory(fileType, data, dataSizePtr);
    free((void*)fileType);
    free((void*)data);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsMusicValid(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    return IsMusicValid(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadMusicStream(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    UnloadMusicStream(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_PlayMusicStream(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    PlayMusicStream(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsMusicStreamPlaying(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    return IsMusicStreamPlaying(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateMusicStream(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    UpdateMusicStream(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_StopMusicStream(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    StopMusicStream(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_PauseMusicStream(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    PauseMusicStream(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_ResumeMusicStream(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    ResumeMusicStream(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_SeekMusicStream(unsigned int musicPtr, unsigned int positionPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    SeekMusicStream(*music, positionPtr);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_SetMusicVolume(unsigned int musicPtr, unsigned int volumePtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    SetMusicVolume(*music, volumePtr);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_SetMusicPitch(unsigned int musicPtr, unsigned int pitchPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    SetMusicPitch(*music, pitchPtr);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_SetMusicPan(unsigned int musicPtr, unsigned int panPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    SetMusicPan(*music, panPtr);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMusicTimeLength(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    return GetMusicTimeLength(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_GetMusicTimePlayed(unsigned int musicPtr) {
    Music* music = cart_get_pointer(musicPtr, sizeof(Music));
    return GetMusicTimePlayed(*music);
    free((void*)music);
}

EMSCRIPTEN_KEEPALIVE void host_LoadAudioStream(unsigned int outPtr, unsigned int sampleRatePtr, unsigned int sampleSizePtr, unsigned int channelsPtr) {
    AudioStream out = LoadAudioStream(sampleRatePtr, sampleSizePtr, channelsPtr);
    cart_set_pointer(&out, sizeof(out), outPtr);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsAudioStreamValid(unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    return IsAudioStreamValid(*stream);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadAudioStream(unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    UnloadAudioStream(*stream);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_UpdateAudioStream(unsigned int streamPtr, unsigned int dataPtr, unsigned int frameCountPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    const void ** data = cart_get_pointer(dataPtr, sizeof(const void *));
    UpdateAudioStream(*stream, data, frameCountPtr);
    free((void*)stream);
    free((void*)data);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsAudioStreamProcessed(unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    return IsAudioStreamProcessed(*stream);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_PlayAudioStream(unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    PlayAudioStream(*stream);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_PauseAudioStream(unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    PauseAudioStream(*stream);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_ResumeAudioStream(unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    ResumeAudioStream(*stream);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_IsAudioStreamPlaying(unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    return IsAudioStreamPlaying(*stream);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_StopAudioStream(unsigned int streamPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    StopAudioStream(*stream);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_SetAudioStreamVolume(unsigned int streamPtr, unsigned int volumePtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    SetAudioStreamVolume(*stream, volumePtr);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_SetAudioStreamPitch(unsigned int streamPtr, unsigned int pitchPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    SetAudioStreamPitch(*stream, pitchPtr);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_SetAudioStreamPan(unsigned int streamPtr, unsigned int panPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    SetAudioStreamPan(*stream, panPtr);
    free((void*)stream);
}

EMSCRIPTEN_KEEPALIVE void host_SetAudioStreamBufferSizeDefault(unsigned int sizePtr) {
    SetAudioStreamBufferSizeDefault(sizePtr);
}

EMSCRIPTEN_KEEPALIVE void host_SetAudioStreamCallback(unsigned int streamPtr, unsigned int callbackPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    AudioCallback* callback = cart_get_pointer(callbackPtr, sizeof(AudioCallback));
    SetAudioStreamCallback(*stream, *callback);
    free((void*)stream);
    free((void*)callback);
}

EMSCRIPTEN_KEEPALIVE void host_AttachAudioStreamProcessor(unsigned int streamPtr, unsigned int processorPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    AudioCallback* processor = cart_get_pointer(processorPtr, sizeof(AudioCallback));
    AttachAudioStreamProcessor(*stream, *processor);
    free((void*)stream);
    free((void*)processor);
}

EMSCRIPTEN_KEEPALIVE void host_DetachAudioStreamProcessor(unsigned int streamPtr, unsigned int processorPtr) {
    AudioStream* stream = cart_get_pointer(streamPtr, sizeof(AudioStream));
    AudioCallback* processor = cart_get_pointer(processorPtr, sizeof(AudioCallback));
    DetachAudioStreamProcessor(*stream, *processor);
    free((void*)stream);
    free((void*)processor);
}

EMSCRIPTEN_KEEPALIVE void host_AttachAudioMixedProcessor(unsigned int processorPtr) {
    AudioCallback* processor = cart_get_pointer(processorPtr, sizeof(AudioCallback));
    AttachAudioMixedProcessor(*processor);
    free((void*)processor);
}

EMSCRIPTEN_KEEPALIVE void host_DetachAudioMixedProcessor(unsigned int processorPtr) {
    AudioCallback* processor = cart_get_pointer(processorPtr, sizeof(AudioCallback));
    DetachAudioMixedProcessor(*processor);
    free((void*)processor);
}




// called on cart init
EM_ASYNC_JS(void, null0_host_load, (unsigned char* wasmBytesPtr, int wasmBytesLen), {
  if (!Module.cart_wasi) {
    throw new Error("Set cart_wasi");
  }
  if (!Module.CartFS) {
    throw new Error("Set CartFS");
  }

  Module.cart_wasi.fs = new Module.CartFS(Module);
  const wasmBytes = Module.HEAPU8.slice(wasmBytesPtr, wasmBytesPtr + wasmBytesLen);
  const cartImports = {env : {}, wasi_snapshot_preview1 : Module.cart_wasi};
  for (const n of Object.keys(Module)) {
    if (n.startsWith("_host")) {
      cartImports.env[n.replace(/^_host_/, "")] = Module[n];
    }
  }
  const {instance : {exports}} = await WebAssembly.instantiate(wasmBytes, cartImports);
  Module.cart = exports;
  Module.cart_wasi.start(exports);
  Module?.cart?.load && Module.cart.load();
});

// called on cart update
EM_JS(void, null0_host_update, (double timeMs), {
  Module?.cart?.update && Module.cart.update(timeMs);
});

// called on cart unload
EM_JS(void, null0_host_unload, (), {
  Module?.cart?.unload && Module.cart.unload();
});
