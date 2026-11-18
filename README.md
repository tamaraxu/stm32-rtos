# stm32-rtos

A from-scratch RTOS for the NUCLEO-F446RE. No HAL, no CubeMX.

## Layout
- `linker.ld`: **you write this** (flash at 0x08000000, 512K; SRAM at 0x20000000, 128K)
- `src/`: **you write these**: startup/vector table, `main.c`, drivers, kernel (`.c`, `.s`, `.S`, subfolders OK)
- `include/`: your headers (already on the include path)
- `Makefile`, `openocd.cfg`, `gdb/init.gdb`: build and debug tooling

## Commands
| Command | What it does |
|---|---|
| `make` | build `build/firmware.elf/.bin/.hex`, show flash/RAM usage |
| `make flash` | program the board over ST-LINK and reset it |
| `make debug` | start the OpenOCD GDB server (keep it running) |
| `make gdb` | second terminal: connect, load, break at `Reset_Handler` |
| `make disasm` | full disassembly to `build/firmware.lst` |
| `make clean` | delete `build/` |
| `make OPT=-O2` | build with a different optimization level |

## Notes
- Linked with `-nostartfiles`: nothing runs before your `Reset_Handler`. Copying `.data` and zeroing `.bss` is your job.
- FPU flags are hard-float (`fpv4-sp-d16`). Enable the FPU (CPACR) in startup before any float code runs, or you'll get a UsageFault.
- `build/firmware.map` shows where every symbol landed. Use it to check your linker script.
