# Used by `make gdb`. Requires `make debug` running in another terminal.
target extended-remote localhost:3333
monitor reset halt
load
# Stop at the first instruction of your reset handler.
# Change or remove this if your handler has a different name.
break Reset_Handler
continue
