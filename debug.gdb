symbol-file kernel/bin/kernel
set disassembly-flavor intel
target remote localhost:1234
break _start
break kernel_main
layout asm
continue