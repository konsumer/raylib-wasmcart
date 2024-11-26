#!/bin/bash

# This will build a cart (zip file with main.wasm)

# buildcart.sh "fullpath/to/whatever.null0" "fullpath/to/whatever.wasm" "fullpath/to/assets"

CART="${1}"
WASM=$(basename "${2}")
WASM_DIR=$(dirname "${2}")
FILES_DIR="${3}"
WEB_DIR="${4}"

printf "\nBuild Cart:\n"
printf "  CART: %s\n" "${CART}"
printf "  WASM: %s\n" "${WASM}"
printf "  WASM_DIR: %s\n" "${WASM_DIR}"
printf "  FILES_DIR: %s\n" "${FILES_DIR}"
printf "  WEB_DIR: %s\n" "${WEB_DIR}"

cp "${WASM_DIR}/${WASM}" "${FILES_DIR}/main.wasm"
cd "${FILES_DIR}"

shopt -s extglob

files=$(ls -c1 **/!(*.c|*.h|.DS_Store|__*) !(*.c|*.h|.DS_Store|__*) 2>/dev/null)
printf "\nFiles added:\n"
printf "  %s\n" $files

zip -rq "${CART}" main.wasm **/!(*.c|*.h|.DS_Store|__*) !(*.c|*.h|.DS_Store|__*)
shopt -u extglob

rm "${FILES_DIR}/main.wasm"

cp "${CART}" "${WEB_DIR}"
