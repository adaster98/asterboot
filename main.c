#include <stdint.h>

#define EFIAPI __attribute__((ms_abi))

typedef uint64_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef void *EFI_EVENT;
typedef uint64_t UINTN;
typedef uint16_t CHAR16;
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef int16_t INT16;
typedef uint8_t BOOLEAN;

typedef struct {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
} EFI_GUID;

#define EFI_SUCCESS 0
#define EFI_ERROR(s) ((int64_t)(s) < 0)
#define NULL 0
#define TRUE 1
#define FALSE 0

/* Reset types - cold boot seems to work best */
typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

/* Forward declarations because EFI is a mess of circular dependencies */
struct _EFI_SYSTEM_TABLE;
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

/* ------------------------------------------------------------------
 * PROTOCOL DEFINITIONS
 * ------------------------------------------------------------------ */

/* Text output */
typedef struct {
    int32_t MaxMode;
    int32_t Mode;
    int32_t Attribute;
    int32_t CursorColumn;
    int32_t CursorRow;
    BOOLEAN CursorVisible;
} EFI_SIMPLE_TEXT_OUTPUT_MODE;

/* Function pointer typedefs */
typedef EFI_STATUS(EFIAPI *EFI_TEXT_RESET)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    BOOLEAN ExtendedVerification);

typedef EFI_STATUS(EFIAPI *EFI_TEXT_OUTPUT_STRING)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);

typedef EFI_STATUS(EFIAPI *EFI_TEXT_QUERY_MODE)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber,
    UINTN *Columns, UINTN *Rows);

typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_MODE)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber);

typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Attribute);

typedef EFI_STATUS(EFIAPI *EFI_TEXT_CLEAR_SCREEN)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);

typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_CURSOR_POSITION)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Column,
    UINTN Row);

typedef EFI_STATUS(EFIAPI *EFI_TEXT_ENABLE_CURSOR)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN Visible);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET Reset;
    EFI_TEXT_OUTPUT_STRING OutputString;
    void *TestString; /* what even is this */
    EFI_TEXT_QUERY_MODE QueryMode;
    EFI_TEXT_SET_MODE SetMode;
    EFI_TEXT_SET_ATTRIBUTE SetAttribute;
    EFI_TEXT_CLEAR_SCREEN ClearScreen;
    EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
    EFI_TEXT_ENABLE_CURSOR EnableCursor;
    EFI_SIMPLE_TEXT_OUTPUT_MODE *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

/* Input */
typedef struct {
    uint16_t ScanCode;
    uint16_t UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS(EFIAPI *EFI_INPUT_RESET)(
    struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
    BOOLEAN ExtendedVerification);

typedef EFI_STATUS(EFIAPI *EFI_INPUT_READ_KEY)(
    struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, EFI_INPUT_KEY *Key);

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET Reset;
    EFI_INPUT_READ_KEY ReadKeyStroke;
    void *WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

/* File system protocols */
#define EFI_FILE_MODE_READ	0x0000000000000001
#define EFI_FILE_MODE_WRITE	0x0000000000000002
#define EFI_FILE_MODE_CREATE	0x8000000000000000
#define EFI_FILE_DIRECTORY	0x0000000000000010

/* Unnessesarily complicated time struct */
typedef struct {
    UINT16 Year;
    UINT8 Month;
    UINT8 Day;
    UINT8 Hour;
    UINT8 Minute;
    UINT8 Second;
    UINT8 Pad1;
    UINT32 Nanosecond;
    INT16 TimeZone;
    UINT8 Daylight;
    UINT8 Pad2;
} EFI_TIME;

/* File info */
typedef struct {
    uint64_t Size;
    uint64_t FileSize;
    uint64_t PhysicalSize;
    EFI_TIME CreateTime;
    EFI_TIME LastAccessTime;
    EFI_TIME ModificationTime;
    uint64_t Attribute;
    CHAR16 FileName[256];
} EFI_FILE_INFO;

struct _EFI_FILE_PROTOCOL;
typedef EFI_STATUS(EFIAPI *EFI_FILE_OPEN)(
    struct _EFI_FILE_PROTOCOL *This, struct _EFI_FILE_PROTOCOL **NewHandle,
    CHAR16 *FileName, uint64_t OpenMode, uint64_t Attributes);

typedef EFI_STATUS(EFIAPI *EFI_FILE_CLOSE)(struct _EFI_FILE_PROTOCOL *This);

typedef EFI_STATUS(EFIAPI *EFI_FILE_DELETE)(struct _EFI_FILE_PROTOCOL *This);

typedef EFI_STATUS(EFIAPI *EFI_FILE_READ)(struct _EFI_FILE_PROTOCOL *This,
                                          UINTN *BufferSize, void *Buffer);

typedef EFI_STATUS(EFIAPI *EFI_FILE_WRITE)(struct _EFI_FILE_PROTOCOL *This,
                                           UINTN *BufferSize, void *Buffer);

typedef EFI_STATUS(EFIAPI *EFI_FILE_GET_INFO)(struct _EFI_FILE_PROTOCOL *This,
                                              EFI_GUID *InformationType,
                                              UINTN *BufferSize, void *Buffer);

typedef struct _EFI_FILE_PROTOCOL {
    uint64_t Revision;
    EFI_FILE_OPEN Open;
    EFI_FILE_CLOSE Close;
    EFI_FILE_DELETE Delete;
    EFI_FILE_READ Read;
    EFI_FILE_WRITE Write;
    void *GetPosition; /* Not implemented */
    void *SetPosition;
    EFI_FILE_GET_INFO GetInfo;
} EFI_FILE_PROTOCOL;

/* Simple FS */
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    uint64_t Revision;
    EFI_STATUS(EFIAPI *OpenVolume)
    (struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
     EFI_FILE_PROTOCOL **Root);
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

/* Image loading protocols */
typedef struct {
    uint32_t Revision;
    EFI_HANDLE ParentHandle;
    struct _EFI_SYSTEM_TABLE *SystemTable;
    EFI_HANDLE DeviceHandle;
    void *FilePath;
    void *Reserved;
    uint32_t LoadOptionsSize;
    void *LoadOptions;
    void *ImageBase;
    uint64_t ImageSize;
} EFI_LOADED_IMAGE_PROTOCOL;

/* Boot services */
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol,
                                                void *Registration,
                                                void **Interface);

typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_POOL)(int PoolType, UINTN Size,
                                              void **Buffer);

typedef EFI_STATUS(EFIAPI *EFI_FREE_POOL)(void *Buffer);

typedef EFI_STATUS(EFIAPI *EFI_HANDLE_PROTOCOL)(EFI_HANDLE Handle,
                                                EFI_GUID *Protocol,
                                                void **Interface);

typedef EFI_STATUS(EFIAPI *EFI_LOAD_IMAGE)(BOOLEAN BootPolicy,
                                           EFI_HANDLE ParentImageHandle,
                                           void *DevicePath, void *SourceBuffer,
                                           UINTN SourceSize,
                                           EFI_HANDLE *ImageHandle);

typedef EFI_STATUS(EFIAPI *EFI_START_IMAGE)(EFI_HANDLE ImageHandle,
                                            UINTN *ExitDataSize,
                                            CHAR16 **ExitData);

typedef EFI_STATUS(EFIAPI *EFI_WAIT_FOR_EVENT)(UINTN NumberOfEvents,
                                               EFI_EVENT *Event, UINTN *Index);

typedef EFI_STATUS(EFIAPI *EFI_STALL)(UINTN Microseconds);

typedef EFI_STATUS(EFIAPI *EFI_SET_WATCHDOG_TIMER)(UINTN Timeout,
                                                   uint64_t WatchdogCode,
                                                   UINTN DataSize,
                                                   CHAR16 *WatchdogData);

typedef EFI_STATUS(EFIAPI *EFI_RESET_SYSTEM)(EFI_RESET_TYPE ResetType,
                                             EFI_STATUS ResetStatus,
                                             UINTN DataSize, void *ResetData);

typedef EFI_STATUS(EFIAPI *EFI_GET_VARIABLE)(CHAR16 *VariableName,
                                             EFI_GUID *VendorGuid,
                                             UINT32 *Attributes,
                                             UINTN *DataSize, void *Data);

typedef EFI_STATUS(EFIAPI *EFI_SET_VARIABLE)(CHAR16 *VariableName,
                                             EFI_GUID *VendorGuid,
                                             UINT32 Attributes, UINTN DataSize,
                                             void *Data);

/* Massive struct - thank god there is only a few of these */
typedef struct {
    char Header[24];
    void *RaiseTPL;
    void *RestoreTPL;
    void *AllocatePages;
    void *FreePages;
    void *GetMemoryMap;
    EFI_ALLOCATE_POOL AllocatePool;
    EFI_FREE_POOL FreePool;
    void *CreateEvent;
    void *SetTimer;
    EFI_WAIT_FOR_EVENT WaitForEvent;
    void *SignalEvent;
    void *CloseEvent;
    void *CheckEvent;
    void *InstallProtocolInterface;
    void *ReinstallProtocolInterface;
    void *UninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL HandleProtocol;
    void *Reserved;
    void *RegisterProtocolNotify;
    void *LocateHandle;
    void *LocateDevicePath;
    void *InstallConfigurationTable;
    EFI_LOAD_IMAGE LoadImage;
    EFI_START_IMAGE StartImage;
    void *Exit;
    void *UnloadImage;
    void *ExitBootServices;
    void *GetNextMonotonicCount;
    EFI_STALL Stall;
    EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;
    void *ConnectController;
    void *DisconnectController;
    void *OpenProtocol;
    void *CloseProtocol;
    void *OpenProtocolInformation;
    void *ProtocolsPerHandle;
    void *LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL LocateProtocol;
} EFI_BOOT_SERVICES;

/* Runtime services */
typedef struct {
    char Header[24];
    void *GetTime;
    void *SetTime;
    void *GetWakeupTime;
    void *SetWakeupTime;
    void *SetVirtualAddressMap;
    void *ConvertPointer;
    EFI_GET_VARIABLE GetVariable;
    void *GetNextVariableName;
    EFI_SET_VARIABLE SetVariable;
    void *GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM ResetSystem;
    void *UpdateCapsule;
    void *QueryCapsuleCapabilities;
    void *QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

/* System table */
typedef struct _EFI_SYSTEM_TABLE {
    char Header[24];
    void *FirmwareVendor;
    uint32_t FirmwareRevision;
    void *ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    void *ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    void *StandardErrorHandle;
    void *StdErr;
    EFI_RUNTIME_SERVICES *RuntimeServices;
    EFI_BOOT_SERVICES *BootServices;
} EFI_SYSTEM_TABLE;

/* ------------------------------------------------------------------
 * GUIDS - I HATE these so very much
 * ------------------------------------------------------------------ */
static const EFI_GUID EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID = {
    0x0964e5b22, 0x6459, 0x11d2,
    {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}
};

static const EFI_GUID EFI_FILE_INFO_ID = {
    0x09576e92, 0x6d3f, 0x11d2,
    {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}
};

static const EFI_GUID EFI_LOADED_IMAGE_PROTOCOL_GUID = {
    0x5B1B31A1, 0x9562, 0x11d2,
    {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
};

static const EFI_GUID EFI_GLOBAL_VARIABLE = {
    0x8BE4DF61, 0x93CA, 0x11D2,
    {0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C}
};

/* ------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------ */
EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES *BS;
EFI_RUNTIME_SERVICES *RT;

#define CAST_STR(s) ((CHAR16 *)(s))
#define LEN(a) (sizeof(a) / sizeof((a)[0]))
#define MAX_ENTRIES 32
#define MAX_VISIBLE 16
CHAR16 *ActiveRoot = NULL; /* This gets set by DetectRoot */

/* Boot entry */
typedef struct {
    CHAR16 FileName[64];
    CHAR16 Title[64];
    CHAR16 Version[32];
    CHAR16 Kernel[128];
    CHAR16 Initrd[128];
    CHAR16 Params[512];
} BootEntry;

/* Global config */
typedef struct {
    UINTN Timeout;
    CHAR16 DefaultSlot[64];
} GlobalConfig;

BootEntry Entries[MAX_ENTRIES];
UINTN EntryCount = 0;
GlobalConfig Config;
UINTN SelectedIndex = 0;
UINTN WindowStart = 0; /* Tracks scroll position */
EFI_LOADED_IMAGE_PROTOCOL *LoadedImage; /* Set in efi_main */
EFI_FILE_PROTOCOL *Root; /* Volume asterboot was loaded from */

/* Colours */
#define EFI_BLACK	0x00
#define EFI_BLUE	0x01
#define EFI_GREEN	0x02
#define EFI_CYAN	0x03
#define EFI_RED		0x04
#define EFI_MAGENTA	0x05
#define EFI_BROWN	0x06
#define EFI_LIGHTGRAY	0x07
#define EFI_DARKGRAY	0x08
#define EFI_LIGHTBLUE	0x09
#define EFI_LIGHTGREEN	0x0A
#define EFI_LIGHTCYAN	0x0B
#define EFI_LIGHTRED	0x0C
#define EFI_LIGHTMAGENTA 0x0D
#define EFI_YELLOW	0x0E
#define EFI_WHITE	0x0F

#define BG_BLACK	0x00
#define BG_LIGHTGRAY	0x70
#define BG_MAGENTA	0x50

/* ------------------------------------------------------------------
 * STRING HELPERS - because EFI doesn't provide any
 * ------------------------------------------------------------------ */

/* Simple strlen */
UINTN StrLen(CHAR16 *Str)
{
    UINTN L = 0;
    while (*Str++)
        L++;
    return L;
}

/* Copy string, cutting at Max - 1 chars so it always fits */
void StrCpy(CHAR16 *D, CHAR16 *S, UINTN Max)
{
    while (--Max && *S)
        *D++ = *S++;
    *D = 0;
}

/* Compare strings - returns 0 if equal */
int StrCmp(CHAR16 *A, CHAR16 *B)
{
    while (*A && *B) {
        if (*A != *B)
            return *A - *B;
        A++;
        B++;
    }
    return *A - *B;
}

/* Concatenate */
void StrCat(CHAR16 *Dest, CHAR16 *Src)
{
    while (*Dest)
        Dest++;
    while (*Src)
        *Dest++ = *Src++;
    *Dest = 0;
}

/* ASCII versions for config parsing */

/* Widen ASCII into a CHAR16 buffer, cutting at Max - 1 chars */
void AsciiToStr(CHAR16 *D, const char *S, UINTN Max)
{
    while (--Max && *S)
        *D++ = (CHAR16)(UINT8)*S++;
    *D = 0;
}

/* Convert ASCII to integer */
UINTN AsciiToInt(const char *Str)
{
    UINTN N = 0;
    while (*Str >= '0' && *Str <= '9') {
        N = N * 10 + (*Str - '0');
        Str++;
    }
    return N;
}

/* Cut the next line out of *P (dropping any \r) and move *P past it */
char *NextLine(char **P)
{
    char *L = *P, *E = L;

    if (!*L)
        return NULL;
    while (*E && *E != '\n')
        E++;
    *P = *E ? E + 1 : E;
    *E = 0;
    if (E > L && E[-1] == '\r')
        E[-1] = 0;
    return L;
}

/* ------------------------------------------------------------------
 * Resolution Optimizer - 4K screens make text tiny
 * This picks a readable one. I want about 100 columns
 * ------------------------------------------------------------------ */
void SetBestResolution(void)
{
    UINTN ModeCount = ST->ConOut->Mode->MaxMode;
    UINTN BestMode = 0;
    UINTN TargetCols = 100;
    UINTN BestDiff = 9999;
    UINTN i;

    for (i = 0; i < ModeCount; i++) {
        UINTN C, R;

        if (ST->ConOut->QueryMode(ST->ConOut, i, &C, &R) == 0) {
            if (C >= 80) {
                UINTN Diff = (C > TargetCols) ?
                (C - TargetCols) :
                (TargetCols - C);
                if (Diff < BestDiff) {
                    BestDiff = Diff;
                    BestMode = i;
                }
            }
        }
    }
    ST->ConOut->SetMode(ST->ConOut, BestMode);
    ST->ConOut->ClearScreen(ST->ConOut);
}

/* ------------------------------------------------------------------
 * SYSTEM UTILITIES - reset and firmware setup
 * ------------------------------------------------------------------ */
void Reboot(void)
{
    RT->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
}

/* Ask firmware to show setup menu on next boot */
void EnterSetup(void)
{
    uint64_t OsInd = 0;
    UINTN Sz = sizeof(OsInd);

    /* Keep any bits already set, then add EFI_OS_INDICATIONS_BOOT_TO_FW_UI */
    RT->GetVariable(CAST_STR(L"OsIndications"),
                    (EFI_GUID *)&EFI_GLOBAL_VARIABLE, NULL, &Sz, &OsInd);
    OsInd |= 0x0000000000000001;
    /* Request firmware setup on next boot via standard global variable */
    RT->SetVariable(CAST_STR(L"OsIndications"),
                    (EFI_GUID *)&EFI_GLOBAL_VARIABLE,
                    0x00000007, /* Attributes: NonVolatile | BootServices | Runtime */
                    sizeof(uint64_t), &OsInd);
    Reboot();
}

/* ------------------------------------------------------------------
 * FILE I/O
 * ------------------------------------------------------------------ */
/* Path is relative to Dir unless it starts with \ */
EFI_STATUS ReadFile(EFI_FILE_PROTOCOL *Dir, CHAR16 *Path, char **Buffer,
                    UINTN *Size)
{
    EFI_FILE_PROTOCOL *File;
    EFI_FILE_INFO Info; /* Room for a 255 char name, enough for FAT */
    UINTN InfoSize = sizeof(Info);
    EFI_STATUS Status = Dir->Open(Dir, &File, Path, EFI_FILE_MODE_READ, 0);

    if (EFI_ERROR(Status))
        return Status;

    /* File info holds the size */
    Status = File->GetInfo(File, (EFI_GUID *)&EFI_FILE_INFO_ID, &InfoSize,
                           &Info);
    if (!EFI_ERROR(Status)) {
        *Size = Info.FileSize;
        Status = BS->AllocatePool(2, *Size + 1, (void **)Buffer);
    }
    if (!EFI_ERROR(Status)) {
        Status = File->Read(File, Size, *Buffer);
        if (EFI_ERROR(Status))
            BS->FreePool(*Buffer);
        else
            (*Buffer)[*Size] = 0; /* Null terminate */
    }
    File->Close(File);
    return Status;
}

/* ------------------------------------------------------------------
 * CONFIG PARSER - I HATE I HATE I HATE I HATE
 * ------------------------------------------------------------------ */
/* Return the value after KEY= if line starts with key (ignoring whitespace), else NULL */
char *GetVal(char *Line, const char *Key)
{
    while (*Line == ' ' || *Line == '\t')
        Line++;
    while (*Key)
        if (*Line++ != *Key++)
            return NULL;
    while (*Line == ' ' || *Line == '\t' || *Line == '=')
        Line++;
    return Line;
}

/* Parse a slot file into a BootEntry, values too long get cut to fit */
void ParseSlot(CHAR16 *Name, char *Buffer)
{
    BootEntry *E = &Entries[EntryCount++];
    char *Line, *V;

    StrCpy(E->FileName, Name, LEN(E->FileName));
    StrCpy(E->Title, Name, LEN(E->Title)); /* Default title is filename */

    while ((Line = NextLine(&Buffer))) {
        if ((V = GetVal(Line, "TITLE")))
            AsciiToStr(E->Title, V, LEN(E->Title));
        else if ((V = GetVal(Line, "VERSION")))
            AsciiToStr(E->Version, V, LEN(E->Version));
        else if ((V = GetVal(Line, "KERNEL")))
            AsciiToStr(E->Kernel, V, LEN(E->Kernel));
        else if ((V = GetVal(Line, "INITRD")))
            AsciiToStr(E->Initrd, V, LEN(E->Initrd));
        else if ((V = GetVal(Line, "PARAMS")))
            AsciiToStr(E->Params, V, LEN(E->Params));
    }
}

/* Find where asterboot is installed - either \asterboot or \EFI\asterboot */
void DetectRoot(void)
{
    EFI_FILE_PROTOCOL *Test;

    /* Check root directory, then EFI directory fallback */
    if (!EFI_ERROR(Root->Open(Root, &Test, CAST_STR(L"\\asterboot"),
                              EFI_FILE_MODE_READ, 0)))
        ActiveRoot = CAST_STR(L"\\asterboot");
    else if (!EFI_ERROR(Root->Open(Root, &Test, CAST_STR(L"\\EFI\\asterboot"),
                                   EFI_FILE_MODE_READ, 0)))
        ActiveRoot = CAST_STR(L"\\EFI\\asterboot");
    else
        return;
    Test->Close(Test);
}

/* Load all configs and slot files */
void LoadAll(void)
{
    CHAR16 Path[128];
    char *Buf, *P, *Line, *V;
    UINTN Sz;
    EFI_FILE_PROTOCOL *Dir;
    EFI_FILE_INFO Info;
    UINTN InfoSz;

    if (!ActiveRoot)
        return;

    /* First, load the main config file */
    StrCpy(Path, ActiveRoot, LEN(Path));
    StrCat(Path, CAST_STR(L"\\asterboot.conf"));

    if (!EFI_ERROR(ReadFile(Root, Path, &Buf, &Sz))) {
        P = Buf;
        while ((Line = NextLine(&P))) {
            if ((V = GetVal(Line, "TIMEOUT")))
                Config.Timeout = AsciiToInt(V);
            else if ((V = GetVal(Line, "DEFAULT")))
                AsciiToStr(Config.DefaultSlot, V, LEN(Config.DefaultSlot));
        }
        BS->FreePool(Buf);
    } else {
        /* No config file - use defaults */
        Config.Timeout = 5;
    }
    /* UI and WriteDefault only handle two digits */
    if (Config.Timeout > 99)
        Config.Timeout = 99;

    /* Now scan the slots directory */
    StrCpy(Path, ActiveRoot, LEN(Path));
    StrCat(Path, CAST_STR(L"\\slots"));

    if (EFI_ERROR(Root->Open(Root, &Dir, Path, EFI_FILE_MODE_READ, 0)))
        return;

    while (EntryCount < MAX_ENTRIES) {
        InfoSz = sizeof(Info);
        if (EFI_ERROR(Dir->Read(Dir, &InfoSz, &Info)) || InfoSz == 0)
            break;
        /* Skip dotfiles, . and .. and any folders */
        if (Info.FileName[0] == '.' || (Info.Attribute & EFI_FILE_DIRECTORY))
            continue;

        /* Name is opened relative to the slots folder */
        if (!EFI_ERROR(ReadFile(Dir, Info.FileName, &Buf, &Sz))) {
            ParseSlot(Info.FileName, Buf);
            BS->FreePool(Buf);
        }
    }
    Dir->Close(Dir);
}

/* Save the current selection as default */
void WriteDefault(void)
{
    CHAR16 CfgPath[128];
    EFI_FILE_PROTOCOL *File;
    char Buffer[128];
    char *P = Buffer;
    char *T;
    CHAR16 *S;
    UINTN Len;

    if (!ActiveRoot)
        return;

    StrCpy(CfgPath, ActiveRoot, LEN(CfgPath));
    StrCat(CfgPath, CAST_STR(L"\\asterboot.conf"));

    /* Open never truncates, so delete the old file first or its tail lingers */
    if (!EFI_ERROR(Root->Open(Root, &File, CfgPath,
                              EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0)))
        File->Delete(File); /* Also closes the handle */

    if (EFI_ERROR(Root->Open(Root, &File, CfgPath,
                             EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ |
                             EFI_FILE_MODE_WRITE,
                             0)))
        return;

    /* Build config file content */
    T = "TIMEOUT=";
    while (*T)
        *P++ = *T++;
    if (Config.Timeout > 9)
        *P++ = '0' + (Config.Timeout / 10);
    *P++ = '0' + (Config.Timeout % 10);
    *P++ = '\n';
    T = "DEFAULT=";
    while (*T)
        *P++ = *T++;
    S = Entries[SelectedIndex].FileName;
    while (*S)
        *P++ = (char)(*S++);
    *P++ = '\n';
    *P = 0;

    Len = (UINTN)(P - Buffer);
    if (!EFI_ERROR(File->Write(File, &Len, Buffer)))
        StrCpy(Config.DefaultSlot, Entries[SelectedIndex].FileName,
               LEN(Config.DefaultSlot));
    File->Close(File);
}

/* ------------------------------------------------------------------
 * UI DRAWING - perdyyy
 * ------------------------------------------------------------------ */
void PrintAt(UINTN Col, UINTN Row, UINTN Attr, CHAR16 *Str)
{
    ST->ConOut->SetAttribute(ST->ConOut, Attr);
    ST->ConOut->SetCursorPosition(ST->ConOut, Col, Row);
    ST->ConOut->OutputString(ST->ConOut, Str);
}

/* Draw the main menu */
void DrawMenu(void)
{
    UINTN Cols, Rows;
    UINTN TitleH = 5;
    UINTN HeaderH = 2;
    UINTN VisibleCount;
    UINTN MenuH;
    UINTN TotalContentH;
    UINTN StartY;
    UINTN CX;
    UINTN R;
    UINTN HWidth = 60;
    UINTN HX;
    UINTN k;
    UINTN End;
    UINTN i;

    /* Scroll so the selection is always inside the visible window */
    if (SelectedIndex < WindowStart)
        WindowStart = SelectedIndex;
    if (SelectedIndex >= WindowStart + MAX_VISIBLE)
        WindowStart = SelectedIndex - MAX_VISIBLE + 1;

    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGRAY | BG_BLACK);
    ST->ConOut->ClearScreen(ST->ConOut);

    /* Get Screen Dimensions */
    ST->ConOut->QueryMode(ST->ConOut, ST->ConOut->Mode->Mode, &Cols, &Rows);

    /* Calculate Vertical Centering */
    VisibleCount = (EntryCount < MAX_VISIBLE) ? EntryCount : MAX_VISIBLE;
    MenuH = VisibleCount;
    TotalContentH = TitleH + HeaderH + MenuH;

    StartY = (Rows > TotalContentH) ? (Rows - TotalContentH) / 2 : 1;
    CX = (Cols > 55) ? (Cols - 55) / 2 : 0;

    /* Title */
    R = StartY;
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK,
            CAST_STR(L"    _    ____ _____ _____ ____  ____   ___   ___ _____ "));
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK,
            CAST_STR(L"   / \\  / ___|_   _| ____|  _ \\| __ ) / _ \\ / _ \\_   _|"));
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK,
            CAST_STR(L"  / _ \\ \\___ \\ | | |  _| | |_) |  _ \\| | | | | | || |  "));
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK,
            CAST_STR(L" / ___ \\ ___) || | | |___|  _ <| |_) | |_| | |_| || |  "));
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK,
            CAST_STR(L"/_/   \\_\\____/ |_| |_____|_| \\_\\____/ \\___/ \\___/ |_|  "));
    R++;

    /* Header bar */
    HX = (Cols > HWidth) ? (Cols - HWidth) / 2 : 0;

    ST->ConOut->SetAttribute(ST->ConOut, BG_LIGHTGRAY | EFI_BLACK);
    ST->ConOut->SetCursorPosition(ST->ConOut, HX, R);
    for (k = 0; k < HWidth; k++)
        ST->ConOut->OutputString(ST->ConOut, CAST_STR(L" "));

    PrintAt(HX + 2, R, BG_LIGHTGRAY | EFI_BLACK, CAST_STR(L"SLOTS:"));

    /* Scroll indicators */
    if (EntryCount > MAX_VISIBLE) {
        if (WindowStart > 0)
            PrintAt(HX + HWidth - 3, R, BG_LIGHTGRAY | EFI_BLACK,
                    CAST_STR(L"↑"));
        if (WindowStart + MAX_VISIBLE < EntryCount)
            PrintAt(HX + HWidth - 2, R, BG_LIGHTGRAY | EFI_BLACK,
                    CAST_STR(L"↓"));
    }
    R++;

    if (EntryCount == 0) {
        PrintAt(CX, R++, EFI_LIGHTRED | BG_BLACK,
                CAST_STR(L"NO BOOT ENTRIES FOUND!"));
    }

    /* Loop through only the visible window of entries */
    End = WindowStart + MAX_VISIBLE;
    if (End > EntryCount)
        End = EntryCount;

    for (i = WindowStart; i < End; i++) {
        BOOLEAN IsSel = (i == SelectedIndex);
        BOOLEAN IsDef =
        (StrCmp(Entries[i].FileName, Config.DefaultSlot) == 0);
        UINTN TextAttr = IsSel ? (EFI_LIGHTMAGENTA | BG_BLACK) :
        (EFI_LIGHTGRAY | BG_BLACK);
        UINTN RelR = R + (i - WindowStart);
        CHAR16 Line[128]; /* Title (63) + Version (31) always fit */

        if (IsDef)
            PrintAt(HX + 2, RelR, EFI_LIGHTGREEN | BG_BLACK,
                    CAST_STR(L"D"));
        if (IsSel)
            PrintAt(HX + 6, RelR, EFI_LIGHTMAGENTA | BG_BLACK,
                    CAST_STR(L">"));

        /* "Title (Version)", or just the title when there's no version */
        StrCpy(Line, Entries[i].Title, LEN(Line));
        if (Entries[i].Version[0]) {
            StrCat(Line, CAST_STR(L" ("));
            StrCat(Line, Entries[i].Version);
            StrCat(Line, CAST_STR(L")"));
        }
        PrintAt(HX + 10, RelR, TextAttr, Line);
    }
}

/* Bottom status bar, separate so the countdown can redraw just this line */
void DrawStatus(UINTN Timer)
{
    UINTN Cols, Rows;
    UINTN k;
    CHAR16 Status[128];
    CHAR16 *T = Status;
    CHAR16 *Msg;

    ST->ConOut->QueryMode(ST->ConOut, ST->ConOut->Mode->Mode, &Cols, &Rows);

    /* Divider sized to the screen so it never wraps */
    for (k = 0; k < Cols && k < LEN(Status) - 1; k++)
        Status[k] = '_';
    Status[k] = 0;
    PrintAt(0, Rows - 2, EFI_DARKGRAY | BG_BLACK, Status);

    Msg = (Timer > 0) ?
    CAST_STR(L" Booting in ") :
    CAST_STR(L" UP/DOWN: Select | ENTER: Boot | D: Default | U: BIOS | ESC: Reboot");
    while (*Msg)
        *T++ = *Msg++;
    if (Timer > 0) {
        /* Fixed width so the text doesn't shift going from 10 to 9 */
        if (Config.Timeout > 9)
            *T++ = (Timer > 9) ? '0' + (Timer / 10) : ' ';
        *T++ = '0' + (Timer % 10);
        Msg = CAST_STR(L" sec... | ANY KEY: Interrupt");
        while (*Msg)
            *T++ = *Msg++;
    }
    *T = 0;

    k = StrLen(Status);
    PrintAt((Cols > k) ? (Cols - k) / 2 : 0, Rows - 1,
            EFI_LIGHTCYAN | BG_BLACK, Status);
}

/* ------------------------------------------------------------------
 * BOOT - the main event                                                *
 * ------------------------------------------------------------------ */
void Boot(EFI_HANDLE ImageHandle)
{
    BootEntry *E = &Entries[SelectedIndex];
    void *KernelBuffer;
    UINTN KernelSize;
    EFI_HANDLE KernelHandle;
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL *KernelLoaded;
    CHAR16 CmdLine[LEN(E->Params) + 8 + LEN(E->Initrd)]; /* Params + " initrd=" + Initrd */

    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->OutputString(ST->ConOut, CAST_STR(L"Loading Kernel...\r\n"));

    /* Load kernel into memory */
    if (EFI_ERROR(ReadFile(Root, E->Kernel, (char **)&KernelBuffer,
                           &KernelSize))) {
        ST->ConOut->OutputString(ST->ConOut,
                                 CAST_STR(L"Error: Kernel not found.\r\n"));
        ST->ConOut->OutputString(ST->ConOut, E->Kernel);
        BS->Stall(5000000);
        return;
    }

    /* Create image handle, firmware copies the image so the buffer can go */
    Status = BS->LoadImage(FALSE, ImageHandle, NULL, KernelBuffer,
                           KernelSize, &KernelHandle);
    BS->FreePool(KernelBuffer);
    if (EFI_ERROR(Status)) {
        ST->ConOut->OutputString(ST->ConOut,
                                 CAST_STR(L"LoadImage Error\r\n"));
        BS->Stall(3000000);
        return;
    }

    BS->HandleProtocol(KernelHandle,
                       (EFI_GUID *)&EFI_LOADED_IMAGE_PROTOCOL_GUID,
                       (void **)&KernelLoaded);

    /* This was a pain in the ass to figure out
     * Set device handle manually, because when loaded direct to ram, the firmware device handle is NULL */
    KernelLoaded->DeviceHandle = LoadedImage->DeviceHandle;

    /* Build command line with initrd if present */
    StrCpy(CmdLine, E->Params, LEN(CmdLine));
    if (E->Initrd[0]) {
        StrCat(CmdLine, CAST_STR(L" initrd="));
        StrCat(CmdLine, E->Initrd);
    }

    KernelLoaded->LoadOptionsSize = (StrLen(CmdLine) + 1) * 2;
    KernelLoaded->LoadOptions = CmdLine;

    /* Hand off to kernel, a freeze on this line means the kernel hung */
    ST->ConOut->OutputString(ST->ConOut, CAST_STR(L"Starting kernel...\r\n"));
    BS->StartImage(KernelHandle, NULL, NULL);

    /* Error handling */
    ST->ConOut->OutputString(ST->ConOut, CAST_STR(L"Kernel Returned\r\n"));
    BS->Stall(5000000);
}

/* ------------------------------------------------------------------
 * ENTRY POINT
 * ------------------------------------------------------------------ */
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FS;
    UINTN i;
    UINTN Timer;
    BOOLEAN Auto = TRUE;
    EFI_INPUT_KEY Key;

    ST = SystemTable;
    BS = SystemTable->BootServices;
    RT = SystemTable->RuntimeServices;

    ST->ConOut->Reset(ST->ConOut, FALSE);
    BS->SetWatchdogTimer(0, 0, 0, NULL); /* Disable watchdog */

    SetBestResolution();
    ST->ConOut->EnableCursor(ST->ConOut, FALSE);

    /* Find the device the bootloader is running on */
    if (EFI_ERROR(BS->HandleProtocol(
        ImageHandle, (EFI_GUID *)&EFI_LOADED_IMAGE_PROTOCOL_GUID,
        (void **)&LoadedImage)))
        return 0;
    if (EFI_ERROR(BS->HandleProtocol(
        LoadedImage->DeviceHandle,
        (EFI_GUID *)&EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID,
        (void **)&FS)))
        return 0;
    if (EFI_ERROR(FS->OpenVolume(FS, &Root)))
        return 0;

    /* Load everything */
    DetectRoot();
    LoadAll();

    /* Set Selection */
    for (i = 0; i < EntryCount; i++) {
        if (StrCmp(Entries[i].FileName, Config.DefaultSlot) == 0)
            SelectedIndex = i;
    }

    Timer = Config.Timeout;

    /* Instant boot, holding SPACE gets the menu instead */
    if (Timer == 0) {
        BS->Stall(100000); /* Brief wait to catch input */
        if (!EFI_ERROR(ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) &&
            Key.UnicodeChar == ' ')
            Auto = FALSE;
    }

    /* Countdown Loop, only the status line redraws each second */
    if (Timer > 0)
        DrawMenu();
    while (Timer > 0) {
        DrawStatus(Timer);
        BS->Stall(1000000);
        if (!EFI_ERROR(ST->ConIn->ReadKeyStroke(ST->ConIn, &Key))) {
            Auto = FALSE;
            break;
        }
        Timer--;
    }
    if (Auto && EntryCount > 0)
        Boot(ImageHandle);

    /* Main UI Loop */
    while (1) {
        UINTN Index;

        DrawMenu();
        DrawStatus(0);
        BS->WaitForEvent(1, (EFI_EVENT *)&ST->ConIn->WaitForKey,
                         &Index);
        /* Some firmware wakes without a readable key, don't reuse the old one */
        if (EFI_ERROR(ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)))
            continue;

        if (Key.ScanCode == 0x17) { /* ESC */
            Reboot();
        } else if (Key.UnicodeChar == 'u' || Key.UnicodeChar == 'U') {
            EnterSetup();
        } else if (EntryCount == 0) {
            continue; /* Nothing to select, boot or save */
        } else if (Key.ScanCode == 0x01) { /* Up, wraps to bottom */
            SelectedIndex = (SelectedIndex + EntryCount - 1) % EntryCount;
        } else if (Key.ScanCode == 0x02) { /* Down, wraps to top */
            SelectedIndex = (SelectedIndex + 1) % EntryCount;
        } else if (Key.UnicodeChar == 0x0D) {
            Boot(ImageHandle);
        } else if (Key.UnicodeChar == 'd' || Key.UnicodeChar == 'D') {
            WriteDefault();
        }
    }
    return EFI_SUCCESS;
}
