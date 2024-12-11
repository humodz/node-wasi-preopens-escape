This repository showcases how to circumvent the "filesystem sandbox" in node:wasi.

The `WASI` class has a `preopens` parameter that describes what files the WASM program has access to. Usually, accessing files outside of that results in a "file not found error", but it's possible to circumvent that if a symlink replaces the file at a very precise moment.

This relies on an outside process to happen and it's not really a problem in practice, but it means that node:wasi isn't fully compliant with the [WASI filesystem specification](https://github.com/WebAssembly/wasi-filesystem/blob/main/path-resolution.md)

> Importantly, the sandboxing is designed to be implementable even in the presence of outside processes accessing the same filesystem, including renaming, unlinking, and creating new files and directories.

## Instructions

1. (optional) build hello.c into hello.wasm

2. open a terminal and run `./swapper.sh`

3. open another terminal and run `node main.js | grep OUTSIDE`

## Result

hello.wasm is able to open ./outside.txt

```
$ node main.js | grep OUTSIDE
(node:426476) ExperimentalWarning: WASI is an experimental feature and might change at any time
(Use `node --trace-warnings ...` to show where the warning was created)
Data: !!!! OUTSIDE - THIS FILE SHOULD NOT BE READABLE BY HELLO.WASM
Data: !!!! OUTSIDE - THIS FILE SHOULD NOT BE READABLE BY HELLO.WASM
Data: !!!! OUTSIDE - THIS FILE SHOULD NOT BE READABLE BY HELLO.WASM
```

## Expected result

WASM programs using WASI should be not able to open symlinks pointing outside the preopens directory.

```
$ ln -sf ../outside.txt preopens/inside.txt
$ cat preopens/inside.txt
<contents of outside.txt ...>

$ node main.js
Error: no such file or directory
```

## Why this happens

1. [uvwasi__resolve_path](https://github.com/nodejs/uvwasi/blob/main/src/uvwasi.c#L2082C9-L2082C29) is invoked, it sees that `preopens/inside.txt` is an actual file inside the preopens dir, so the sandbox check succeeds and the WASM program is allowed to access the file.

2. The swapper script replaces `preopens/inside.txt` with a symlink to `outside.txt`

3. [uv_fs_open](https://github.com/nodejs/uvwasi/blob/main/src/uvwasi.c#L2093C7-L2093C17) is called with `preopens/inside.txt` and opens `outside.txt`, which WASI is not supposed to allow.

## Consequences

If you run malicious code using node:wasi, it can potentially open files outside the preopens dir.