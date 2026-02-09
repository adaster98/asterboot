# Asterboot

This is a **basic bitch** bootloader.<br>
It dont support no god damn secure boot, encryption or non-efistub entries. <br>
It has one job... boot my linux kernels. And it does it well. <br>
**13KB compiled**. No libraries. No bloat. Pure **suckless** nature. <br>

# Build

Build with: `clang -target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -Wl,-entry:efi_main -fuse-ld=lld -Wl,-subsystem:efi_application -Wl,-nodefaultlib -o asterbootx64.efi main.c`

# Info

Create the following file structure in your root efi directory:

```
/EFI/asterboot/asterbootx64.efi
/asterboot/asterboot.conf
/asterboot/slots/*.conf
```

Main config file:
```
TIMEOUT=5
DEFAULT=stable.conf
```

Slot config example:
```
TITLE=Gentoo Linux
VERSION=6.18.9-gentoo-minimal
KERNEL=\vmlinuz-6.18.9-gentoo-minimal.efi
PARAMS=root=PARTUUID=5b282577-32e2-42c0-9c01-d6a21160accb rw quiet
```

