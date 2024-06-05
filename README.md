```
  ______                                       __  __                             ______    ______  
 /      \                                     /  |/  |                           /      \  /      \ 
/$$$$$$  | __    __   ______    ______    ____$$ |$$/   ______   _______        /$$$$$$  |/$$$$$$  |
$$ | _$$/ /  |  /  | /      \  /      \  /    $$ |/  | /      \ /       \       $$ |  $$ |$$ \__$$/ 
$$ |/    |$$ |  $$ | $$$$$$  |/$$$$$$  |/$$$$$$$ |$$ | $$$$$$  |$$$$$$$  |      $$ |  $$ |$$      \ 
$$ |$$$$ |$$ |  $$ | /    $$ |$$ |  $$/ $$ |  $$ |$$ | /    $$ |$$ |  $$ |      $$ |  $$ | $$$$$$  |
$$ \__$$ |$$ \__$$ |/$$$$$$$ |$$ |      $$ \__$$ |$$ |/$$$$$$$ |$$ |  $$ |      $$ \__$$ |/  \__$$ |
$$    $$/ $$    $$/ $$    $$ |$$ |      $$    $$ |$$ |$$    $$ |$$ |  $$ |      $$    $$/ $$    $$/ 
 $$$$$$/   $$$$$$/   $$$$$$$/ $$/        $$$$$$$/ $$/  $$$$$$$/ $$/   $$/        $$$$$$/   $$$$$$/

```

This OS was developed by **Nathan Hornby**.

## Current Status

Guardian OS currently requires an IDT (Interrupt Descriptor Table). Despite multiple efforts, it is not functioning correctly at this time.

### Important Note
⚠️ Guardian OS will boot without an IDT; however, any functionality requiring interrupts will be non-operational.

### Key Features
- **64-bit Architecture**: Guardian OS is designed for x64 bit systems.
- **Built off [Limine C++](https://github.com/limine-bootloader/limine-cpp-template), including [Level-5 Paging](https://en.wikipedia.org/wiki/Intel_5-level_paging).**
- **Library Dependency**: It utilizes [libhydrix](https://github.com/AzureianGH/libhydrix), and the necessary version is included.
