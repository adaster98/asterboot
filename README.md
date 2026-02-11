# Asterboot

Asterboot is an extremely lightweight bootloader with one job... to boot kernels. <br>
It doesn't support secure boot, encryption* or non-efistub entries. <br>
**13KB compiled**. No libraries. No bloat. *Pure **suckless** nature.* <br>
<sub> *root partition encryption is supported as the initramfs handles decryption </sub>

<img width="6472" height="3635" alt="image" src="https://github.com/user-attachments/assets/365f6685-9cdb-458b-af3f-b58466a520f3" />


# Usage

#### Entering the menu:
Timeout > 0: Countdown timer is shown. Any key interrupts timer.<br>
Timeout = 0: Boots default entry instantly. To interrupt and access the menu, Hold **SPACEBAR**.<br>
<sub> I recommend the "Instant Boot" mode for daily use and speed. </sub>

#### Using the menu:
**Arrow keys** to select a slot. **Enter** to boot.<br>
**D** to set a default slot graphically.

# Build or Download binary

Use the **precompiled binary** under releases, or you can **build from source** using the commands below:
```shell
git clone https://github.com/adaster98/asterboot
cd asterboot
clang -target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -Wl,-entry:efi_main -fuse-ld=lld -Wl,-subsystem:efi_application -Wl,-nodefaultlib -o asterbootx64.efi main.c
```

# Setup & Configure

#### 1. Create the following file structure in your **root efi partition**

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

#### 2. Example config file (asterboot.conf)
```ini
TIMEOUT=5
DEFAULT=entry1.conf
```
####  3. Example slot config (entry1.conf)
```ini
TITLE=Gentoo Linux
VERSION=6.18.9
KERNEL=\kernel1.efi
PARAMS=root=PARTUUID=XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX rw quiet
```
If you use initramfs, add this key:
`INITRD=\initramfs1.img`<br>
For LUKS encryption, make sure to add `rd.luks.uuid=<UUID>` or `luks.uuid=<UUID>` to your PARAMS depending on which you use.

#### 4. Create entry in motherboard NVRAM so its selectable as a boot option
```shell
sudo efibootmgr --create --disk /dev/nvme0n1 --part 1 --label "Asterboot" --loader "\EFI\asterboot\asterbootx64.efi"
```
