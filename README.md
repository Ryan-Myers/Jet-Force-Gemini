# Jet Force Gemini

A repository exploring a decompilation of Jet Force Gemini.

This uses the Kiosk demo ROM as it was discovered to have many symbols, which will make things a little easier.

This is super minimal and likely won't become a full decomp project. It does the bare minimum to confirm the symbols found in the kiosk decomp are real, and can match known functions in the Diddy Kong Racing decompilation which this engine is based on.

Grab tools

```sh
git submodule update --init --recursive
```

Drop in `kiosk` as `baserom.kiosk.z64` (sha1sum: `f00f7c7fb085d0df57dcb649793aced5be4e8562`)

```sh
make dependencies
make extract
make
```

