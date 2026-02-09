# Asterboot

This is a **basic bitch** bootloader.<br>
It dont support no god damn secure boot, encryption or non-efistub entries. <br>
It has one job... boot my linux kernels. And it does it well. <br>
**13KB compiled**. No libraries. No bloat. Pure **suckless** nature. <br>

# Build

Build with: 
```
clang -target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -Wl,-entry:efi_main -fuse-ld=lld -Wl,-subsystem:efi_application -Wl,-nodefaultlib -o asterbootx64.efi main.c
```

# Info

Create the following file structure in your **root efi partition**:

```
/
├── EFI/
│   └── asterboot/
│       └── asterbootx64.efi
├── asterboot/
│   ├── asterboot.conf
│   └── slots/
│       ├── entry1.conf
│       └── entry2.conf
├── kernel1.efi
└── kernel2.efi
```

Main config file (asterboot.conf):
```
TIMEOUT=5
DEFAULT=entry1.conf
```

Slot config example (entry1.conf): 
```
TITLE=Gentoo Linux
VERSION=6.18.9-gentoo-minimal
KERNEL=\vmlinuz-6.18.9-gentoo-minimal.efi
PARAMS=root=PARTUUID=5b282577-32e2-42c0-9c01-d6a21160accb rw quiet
```

Create entry in motheboard NVRAM:
```
sudo efibootmgr --create --disk /dev/nvme0n1 --part 1 --label "Asterboot" --loader "\EFI\asterboot\asterbootx64.efi"
```
