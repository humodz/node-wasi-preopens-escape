## Instructions

1. (optional) build hello.c into hello.wasm

2. open a terminal and run `./swapper.sh`

3. open another terminal and run `node main.js | grep OUTSIDE`

## Result

```
$ node main.js | grep OUTSIDE
(node:426476) ExperimentalWarning: WASI is an experimental feature and might change at any time
(Use `node --trace-warnings ...` to show where the warning was created)
Data: !!!! OUTSIDE - THIS FILE SHOULD NOT BE READABLE BY HELLO.WASM
Data: !!!! OUTSIDE - THIS FILE SHOULD NOT BE READABLE BY HELLO.WASM
Data: !!!! OUTSIDE - THIS FILE SHOULD NOT BE READABLE BY HELLO.WASM
```

## Why this happens

1. [uvwasi__resolve_path](https://github.com/nodejs/uvwasi/blob/main/src/uvwasi.c#L2082C9-L2082C29) is invoked, `./preopens/inside.txt` is an actual file inside the preopens dir, so the sandbox check succeeds

2. The swapper script replaces `./preopens/inside.txt` with a symlink to `./outside.txt`

3. [uv_fs_open](https://github.com/nodejs/uvwasi/blob/main/src/uvwasi.c#L2093C7-L2093C17) is called with `./preopens/inside.txt` and opens `./outside.txt`, which WASI is not supposed to allow.