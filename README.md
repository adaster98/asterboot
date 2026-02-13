# Asterboot

Asterboot is an extremely lightweight UEFI bootloader with one job... to boot kernels. <br>
**13.5KB compiled**. No libraries. No bloat. *Pure **suckless** nature.* <br>
<sub> Root partition encryption is supported as the initramfs handles decryption, full disk encryption **not** supported. </sub>

<img width="6472" height="3635" alt="image" src="https://github.com/user-attachments/assets/365f6685-9cdb-458b-af3f-b58466a520f3" />


## Usage

#### Entering the menu:
Timeout > 0: Countdown timer is shown. Any key interrupts timer.<br>
Timeout = 0: Boots default entry instantly. To interrupt and access the menu, Hold **SPACEBAR**.<br>
<sub> I recommend the "Instant Boot" mode for daily use and speed. </sub>

#### Using the menu:
**Arrow keys** to select a slot. **Enter** to boot.<br>
**D** to set a default slot graphically.

## Build or Download binaryAnd that also means 

Use the **precompiled binary** under releases, or you can **build from source** using the commands below:
```shell
git clone https://github.com/adaster98/asterboot
cd asterboot
clang -target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -Wl,-entry:efi_main -fuse-ld=lld -Wl,-subsystem:efi_application -Wl,-nodefaultlib -o asterbootx64.efi main.c
```

## Setup & Configure

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
## Optional Secureboot:
If you wish to utilize **secureboot**, you must generate a keypair and certificate, and use it to sign the bootloader and your kernel.<br>
You will need `openssl`, `sbsigntools` and `sbsigntools`.
<sub> **Note:** It may be possible to use a microsoft signed SHIM instead, but this is untested and probably won't work. </sub>

#### 1. Create a workspace and generate the keys:
```shell
mkdir keys
cd keys

# 1. Platform Key (PK) - The Master Key
openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Asterboot PK/" -keyout PK.key -out PK.crt -days 3650 -nodes -sha256

# 2. Key Exchange Key (KEK) - Used to update signatures
openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Asterboot KEK/" -keyout KEK.key -out KEK.crt -days 3650 -nodes -sha256

# 3. Signature Database Key (db) - Used to sign your bootloader/kernel
openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Asterboot db/" -keyout db.key -out db.crt -days 3650 -nodes -sha256
```

#### 2. Prepare keys for BIOS Enrollment:
```shell
# 1. Convert Certificates to ESL format
cert-to-efi-sig-list -g "$(uuidgen)" PK.crt PK.esl
cert-to-efi-sig-list -g "$(uuidgen)" KEK.crt KEK.esl
cert-to-efi-sig-list -g "$(uuidgen)" db.crt db.esl

# 2. Sign the ESLs to create .auth files
# PK signs itself. PK signs KEK. KEK signs db.

# Sign PK with itself
sign-efi-sig-list -k PK.key -c PK.crt PK PK.esl PK.auth

# Sign KEK with PK
sign-efi-sig-list -k PK.key -c PK.crt KEK KEK.esl KEK.auth

# Sign db with KEK
sign-efi-sig-list -k KEK.key -c KEK.crt db db.esl db.auth
```

#### 3. Sign the EFI files:
```shell
# Sign Asterboot
sbsign --key db.key --cert db.crt --output asterbootx64-signed.efi asterbootx64.efi

# Sign your kernel. Repeat for every kernel you want to boot
sbsign --key db.key --cert db.crt --output myKernel-signed.efi myKernel.efi
```

#### 4. Enroll in BIOS:
Transfer your db, KEK and PK to a flash drive and enroll them in your BIOS under the secure-boot menu.<br>
**NOTE:** Enroll PK last and this ends setup mode.
