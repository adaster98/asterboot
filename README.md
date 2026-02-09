# Asterboot

This is a **basic bitch** bootloader.<br>
It dont support no god damn secure boot, encryption* or non-efistub entries. <br>
It has one job... boot my linux kernels. And it does it well. <br>
**13KB compiled**. No libraries. No bloat. Pure **suckless** nature. <br>
<sub> *root partition encryption is supported as the initramfs handles decryption </sub>

<img width="6472" height="3635" alt="image" src="https://github.com/user-attachments/assets/365f6685-9cdb-458b-af3f-b58466a520f3" />


# Usage

If you have the timeout set to 0, the default slot will be automatically booted (around ~180ms) which is basiclly the same speed as a straight up efi boot entry. You can hold **SPACEBAR** to interupt this and access the menu at any time. I recommend this mode.<br>

Use **arrow keys** to select a slot. Press **enter** to boot.<br>

Press **D** to set a default slot graphically.

# Build or Download binary

Use the **precompiled binary** under releases, or you can build from source using the commands below:

**Build from source:**
```
git clone https://github.com/adaster98/asterboot
cd asterboot
clang -target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -Wl,-entry:efi_main -fuse-ld=lld -Wl,-subsystem:efi_application -Wl,-nodefaultlib -o asterbootx64.efi main.c
```

# Setup & Configure

1. Create the following file structure in your **root efi partition**.

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

2. Example config file (asterboot.conf):
```
TIMEOUT=5
DEFAULT=entry1.conf
```

3. Example slot config (entry1.conf): 
```
TITLE=Gentoo Linux
VERSION=6.18.9
KERNEL=\kernel1.efi
PARAMS=root=PARTUUID=XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX rw quiet
```
If you use initramfs, add this key:
`INITRD=\initramfs1.img`<br>
For LUKS encryption, make sure to add `rd.luks.uuid=<UUID>` or `luks.uuid=<UUID>` to your PARAMS depending on which you use.

4. Create entry in motherboard NVRAM so its selectable as a boot option:
```
sudo efibootmgr --create --disk /dev/nvme0n1 --part 1 --label "Asterboot" --loader "\EFI\asterboot\asterbootx64.efi"
```
