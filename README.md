# Jet Force Gemini

A repository exploring a decompilation of Jet Force Gemini.

This uses the US ROM by default, but it will also support the kiosk ROM if so desired. Work on PAL and JPN is so far minimal to not started.

This game is a heavily modified Diddy Kong Racing engine, and thus this repository will steal from there when it can.

Grab tools

```sh
git submodule update --init --recursive
```

Install Dependencies
```sh
sudo apt install build-essential pkg-config git python3 wget python3-pip binutils-mips-linux-gnu python3-venv
```

Drop in `us` into the `baseroms` folder as `baserom.us.z64` (sha1sum: `493ced9008dbe932d6e91179b68e8630cf23a023`)
Drop in `kiosk` into the `baseroms` folder as `baserom.kiosk.z64` (sha1sum: `f00f7c7fb085d0df57dcb649793aced5be4e8562`)

```sh
make setup
make extract
make
```

