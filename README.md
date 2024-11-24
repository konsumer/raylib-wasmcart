The idea here is the full raylib API, but you make "carts" which are zip files that include your game-code compiled to wasm, and any assets you need.

This will allow you to make very small games that are easily distributable, in any language you like.

```
# setup tools
npm i

# run the watching web-server, build on changes
npm start

# run the build/server in docker (less host-tooling needed)
npm run start:docker

# make a native game, run with ./build/my_raylib_game
npm run build:native

# compile web-version
npm run build:web
```
