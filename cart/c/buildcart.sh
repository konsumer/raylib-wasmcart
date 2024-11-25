#!/bin/bash

# This will build a cart (zip file with main.wasm)

# buildcart.sh "fullpath/to/whatever.null0" "fullpath/to/whatever.wasm" "fullpath/to/assets"

CART="${1}"
WASM=$(basename "${2}")
WASM_DIR=$(dirname "${2}")
FILES_DIR="${3}"
WEB_DIR="${4}"

echo "Build Cart:"
echo "  CART: ${CART}"
echo "  WASM: ${WASM}"
echo "  WASM_DIR: ${WASM_DIR}"
echo "  FILES_DIR: ${FILES_DIR}"
echo "  WEB_DIR: ${WEB_DIR}"

cp "${WASM_DIR}/${WASM}" "${FILES_DIR}/main.wasm"
cd "${FILES_DIR}"

shopt -s extglob
ls -c1 main.wasm **/!(*.c|*.h|.DS_Store|__*) !(*.c|*.h|.DS_Store|__*)
zip -rq "${CART}" main.wasm **/!(*.c|*.h|.DS_Store|__*) !(*.c|*.h|.DS_Store|__*)
shopt -u extglob

rm "${FILES_DIR}/main.wasm"

cp "${CART}" "${WEB_DIR}"
