# Jet Force Gemini

A repository exploring a decompilation of Jet Force Gemini.

This uses the Kiosk demo ROM as it was discovered to have many symbols, which will make things a little easier.

This does not currently build back to the original ROM. It's just uses splat to help apply the symbols to the ASM to make it easier to explore the assembly.

Grab tools

```sh
git submodule update --init --recursive
```

Drop in `kiosk` as `baserom.kiosk.z64` (sha1sum: `f00f7c7fb085d0df57dcb649793aced5be4e8562`)

```sh
python3 -m pip install -r ./tools/splat/requirements.txt
python3 ./tools/splat/split.py jfg.kiosk.yaml
```

