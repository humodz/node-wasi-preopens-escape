const { readFile } = require('node:fs/promises');
const { WASI } = require('node:wasi');
const { argv, env } = require('node:process');
const { join } = require('node:path');

(async () => {
  const wasi = new WASI({
    version: 'preview1',
    args: argv,
    env,
    preopens: {
      '/': join(__dirname, 'preopens'),
    },
  });

  const wasm = await WebAssembly.compile(
    await readFile(join(__dirname, 'hello.wasm')),
  );

  const instance = await WebAssembly.instantiate(wasm, wasi.getImportObject());

  wasi.start(instance);
})();