## Playground for a Game Boy Advance app

![CI build status](https://github.com/pmer/gba/actions/workflows/build.yml/badge.svg)

#### Running

Open the nightly build in any Game Boy Advance emulator.

[Download nightly ROM](https://nightly.link/pmer/gba/workflows/build/main/playground.gba.zip)

#### Development

Building can be done on any Windows, macOS, or Linux that runs AMD64. (This is
what has been tested. If other environments work, please share!)

To build, install [devkitPro](https://devkitpro.org/wiki/Getting_Started) and
get its `gba-dev` package, and set `$DEVKITPRO` to the install root. This
should be sufficient to get a CMake build going, which will produce a `.gba`
file.

To debug, launch [mGBA](https://mgba.io/) with the `-g` flag,  as in `mgba -g
build/playground.gba`, which will cause it to start a GDB server and wait for a
connection.  Connect to it with:

```
$ gdb build/playground.elf
(gdb) target remote localhost:2345
```

Some IDEs, like CLion and Visual Studio Code, support remote GDB debugging and
will provide a nice UI while doing so.

To view runtime print statements, go to Tools → Logs in the mGBA menu bar.

![Example image of debugging with CLion](img/debugging.png)
