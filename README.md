## Playground for a Gameboy Advance app

To build, install [devkitPro](https://devkitpro.org/wiki/Getting_Started) and
get its gba-dev package, and set `$DEVKITPRO` to the install root. This should
be sufficient to get a CMake build going, which will produce a `.gba` file.

To debug, launch `mgba -g build/untitled.gba`, which starts a GDB server.
Connect to it with:

```
gdb build/untitled.elf
(gdb) target remote localhost:2345
```

Some IDEs, like CLion and Visual Studio Code, support remote debugging with a
nice UI.

To view runtime print statements, go to Tools -> Logs in the mGBA menu bar.
