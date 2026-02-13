#include <stdint.h>

/* --- UEFI BASE DEFINITIONS --- */
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
typedef struct { uint32_t Data1; uint16_t Data2; uint16_t Data3; uint8_t Data4[8]; } EFI_GUID;

#define EFI_SUCCESS 0
#define EFI_ERROR(s) ((int64_t)(s) < 0)
#define NULL 0
#define TRUE 1
#define FALSE 0

/* --- SYSTEM RESET TYPES --- */
typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

struct _EFI_SYSTEM_TABLE;
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

/* --- PROTOCOLS --- */
typedef struct {
    int32_t MaxMode;
    int32_t Mode;
    int32_t Attribute;
    int32_t CursorColumn;
    int32_t CursorRow;
    BOOLEAN CursorVisible;
} EFI_SIMPLE_TEXT_OUTPUT_MODE;

typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN ExtendedVerification);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_OUTPUT_STRING)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_QUERY_MODE)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber, UINTN *Columns, UINTN *Rows);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_MODE)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Attribute);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_CURSOR_POSITION)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Column, UINTN Row);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_ENABLE_CURSOR)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN Visible);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET Reset;
    EFI_TEXT_OUTPUT_STRING OutputString;
    void *TestString;
    EFI_TEXT_QUERY_MODE QueryMode;
    EFI_TEXT_SET_MODE SetMode;
    EFI_TEXT_SET_ATTRIBUTE SetAttribute;
    EFI_TEXT_CLEAR_SCREEN ClearScreen;
    EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
    EFI_TEXT_ENABLE_CURSOR EnableCursor;
    EFI_SIMPLE_TEXT_OUTPUT_MODE *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {
    uint16_t ScanCode;
    uint16_t UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS (EFIAPI *EFI_INPUT_RESET)(struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, BOOLEAN ExtendedVerification);
typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY)(struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, EFI_INPUT_KEY *Key);

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET Reset;
    EFI_INPUT_READ_KEY ReadKeyStroke;
    void *WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

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
typedef EFI_STATUS (EFIAPI *EFI_FILE_OPEN)(struct _EFI_FILE_PROTOCOL *This, struct _EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, uint64_t OpenMode, uint64_t Attributes);
typedef EFI_STATUS (EFIAPI *EFI_FILE_CLOSE)(struct _EFI_FILE_PROTOCOL *This);
typedef EFI_STATUS (EFIAPI *EFI_FILE_DELETE)(struct _EFI_FILE_PROTOCOL *This);
typedef EFI_STATUS (EFIAPI *EFI_FILE_READ)(struct _EFI_FILE_PROTOCOL *This, UINTN *BufferSize, void *Buffer);
typedef EFI_STATUS (EFIAPI *EFI_FILE_WRITE)(struct _EFI_FILE_PROTOCOL *This, UINTN *BufferSize, void *Buffer);
typedef EFI_STATUS (EFIAPI *EFI_FILE_GET_INFO)(struct _EFI_FILE_PROTOCOL *This, EFI_GUID *InformationType, UINTN *BufferSize, void *Buffer);

typedef struct _EFI_FILE_PROTOCOL {
    uint64_t Revision;
    EFI_FILE_OPEN Open;
    EFI_FILE_CLOSE Close;
    EFI_FILE_DELETE Delete;
    EFI_FILE_READ Read;
    EFI_FILE_WRITE Write;
    void *GetPosition;
    void *SetPosition;
    EFI_FILE_GET_INFO GetInfo;
} EFI_FILE_PROTOCOL;

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    uint64_t Revision;
    EFI_STATUS (EFIAPI *OpenVolume)(struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This, EFI_FILE_PROTOCOL **Root);
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

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

typedef EFI_STATUS (EFIAPI *EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol, void *Registration, void **Interface);
typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL)(int PoolType, UINTN Size, void **Buffer);
typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL)(void *Buffer);
typedef EFI_STATUS (EFIAPI *EFI_HANDLE_PROTOCOL)(EFI_HANDLE Handle, EFI_GUID *Protocol, void **Interface);
typedef EFI_STATUS (EFIAPI *EFI_LOAD_IMAGE)(BOOLEAN BootPolicy, EFI_HANDLE ParentImageHandle, void *DevicePath, void *SourceBuffer, UINTN SourceSize, EFI_HANDLE *ImageHandle);
typedef EFI_STATUS (EFIAPI *EFI_START_IMAGE)(EFI_HANDLE ImageHandle, UINTN *ExitDataSize, CHAR16 **ExitData);
typedef EFI_STATUS (EFIAPI *EFI_WAIT_FOR_EVENT)(UINTN NumberOfEvents, EFI_EVENT *Event, UINTN *Index);
typedef EFI_STATUS (EFIAPI *EFI_STALL)(UINTN Microseconds);
typedef EFI_STATUS (EFIAPI *EFI_SET_WATCHDOG_TIMER)(UINTN Timeout, uint64_t WatchdogCode, UINTN DataSize, CHAR16 *WatchdogData);
typedef EFI_STATUS (EFIAPI *EFI_RESET_SYSTEM)(EFI_RESET_TYPE ResetType, EFI_STATUS ResetStatus, UINTN DataSize, void *ResetData);
typedef EFI_STATUS (EFIAPI *EFI_GET_VARIABLE)(CHAR16 *VariableName, EFI_GUID *VendorGuid, UINT32 *Attributes, UINTN *DataSize, void *Data);
typedef EFI_STATUS (EFIAPI *EFI_SET_VARIABLE)(CHAR16 *VariableName, EFI_GUID *VendorGuid, UINT32 Attributes, UINTN DataSize, void *Data);

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

static const EFI_GUID EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID = { 0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b} };
static const EFI_GUID EFI_FILE_INFO_ID = { 0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b} };
static const EFI_GUID EFI_LOADED_IMAGE_PROTOCOL_GUID = { 0x5B1B31A1, 0x9562, 0x11d2, {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B} };
static const EFI_GUID EFI_GLOBAL_VARIABLE = { 0x8BE4DF61, 0x93CA, 0x11D2, {0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C} };

EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES *BS;
EFI_RUNTIME_SERVICES *RT;

#define CAST_STR(s) ((CHAR16*)(s))
#define MAX_ENTRIES 32
#define MAX_VISIBLE 16
CHAR16 *ActiveRoot = NULL;

typedef struct {
    CHAR16 FileName[64];
    CHAR16 Title[64];
    CHAR16 Version[32];
    CHAR16 Kernel[128];
    CHAR16 Initrd[128];
    CHAR16 Params[512];
} BootEntry;

typedef struct {
    UINTN Timeout;
    CHAR16 DefaultSlot[64];
} GlobalConfig;

BootEntry Entries[MAX_ENTRIES];
UINTN EntryCount = 0;
GlobalConfig Config;
UINTN SelectedIndex = 0;
UINTN WindowStart = 0;
CHAR16 DebugLog[2048] = {0};

#define EFI_BLACK 0x00
#define EFI_BLUE  0x01
#define EFI_GREEN 0x02
#define EFI_CYAN  0x03
#define EFI_RED   0x04
#define EFI_MAGENTA 0x05
#define EFI_BROWN 0x06
#define EFI_LIGHTGRAY 0x07
#define EFI_DARKGRAY 0x08
#define EFI_LIGHTBLUE 0x09
#define EFI_LIGHTGREEN 0x0A
#define EFI_LIGHTCYAN 0x0B
#define EFI_LIGHTRED 0x0C
#define EFI_LIGHTMAGENTA 0x0D
#define EFI_YELLOW 0x0E
#define EFI_WHITE 0x0F

#define BG_BLACK 0x00
#define BG_LIGHTGRAY 0x70
#define BG_MAGENTA 0x50

/* --- HELPERS --- */
UINTN StrLen(CHAR16 *Str) { UINTN L=0; while(*Str++) L++; return L; }
void StrCpy(CHAR16 *D, CHAR16 *S) { while(*S) *D++ = *S++; *D=0; }
int StrCmp(CHAR16 *A, CHAR16 *B) { while(*A && *B) { if (*A!=*B) return *A-*B; A++; B++; } return *A-*B; }

void StrCat(CHAR16 *Dest, CHAR16 *Src) {
    while (*Dest) Dest++;
    while (*Src) *Dest++ = *Src++;
    *Dest = 0;
}

UINTN AsciiStrLen(const char *Str) { UINTN L=0; while(*Str++) L++; return L; }
int AsciiStrnCmp(const char *A, const char *B, UINTN N) { while(N>0 && *A && *B) { if(*A!=*B) return *A-*B; A++; B++; N--; } if(N==0) return 0; return *A-*B; }
UINTN AsciiToInt(const char *Str) { UINTN N=0; while(*Str>='0' && *Str<='9') { N=N*10+(*Str-'0'); Str++; } return N; }
char* AsciiFindNextLine(char *S) { while(*S) { if(*S=='\n') return S; S++; } return NULL; }

/* --- RESOLUTION OPTIMISER --- */
// I found that max res was too hard to read so this finds the mode closest to ~100-120 cols.
void SetBestResolution() {
    UINTN ModeCount = ST->ConOut->Mode->MaxMode;
    UINTN BestMode = 0;
    UINTN TargetCols = 100;
    UINTN BestDiff = 9999;
    for (UINTN i = 0; i < ModeCount; i++) {
        UINTN C, R;
        if (ST->ConOut->QueryMode(ST->ConOut, i, &C, &R) == 0) {
            if (C >= 80) {
                UINTN Diff = (C > TargetCols) ? (C - TargetCols) : (TargetCols - C);
                if (Diff < BestDiff) { BestDiff = Diff; BestMode = i; }
            }
        }
    }
    ST->ConOut->SetMode(ST->ConOut, BestMode);
    ST->ConOut->ClearScreen(ST->ConOut);
}

/* --- SYSTEM UTILS --- */
void Reboot() {
    RT->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
}

void EnterSetup() {
    uint64_t OsInd = 0x0000000000000001;
    RT->SetVariable(CAST_STR(L"OsIndications"), (EFI_GUID*)&EFI_GLOBAL_VARIABLE,
                    0x00000007, // NV | BS | RT
                    sizeof(uint64_t), &OsInd);
    Reboot();
}

/* --- FILE I/O --- */
EFI_STATUS ReadFile(EFI_FILE_PROTOCOL *Root, CHAR16 *Path, char **Buffer, UINTN *Size) {
    EFI_FILE_PROTOCOL *File;
    EFI_STATUS Status = Root->Open(Root, &File, Path, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) return Status;
    EFI_FILE_INFO *Info;
    UINTN InfoSize = 0;
    File->GetInfo(File, (EFI_GUID*)&EFI_FILE_INFO_ID, &InfoSize, NULL);
    BS->AllocatePool(2, InfoSize, (void**)&Info);
    File->GetInfo(File, (EFI_GUID*)&EFI_FILE_INFO_ID, &InfoSize, Info);
    *Size = Info->FileSize;
    BS->FreePool(Info);
    BS->AllocatePool(2, *Size + 1, (void**)Buffer);
    Status = File->Read(File, Size, *Buffer);
    (*Buffer)[*Size] = 0;
    File->Close(File);
    return Status;
}

/* --- PARSER --- */
int KeyMatch(char *Line, const char *Key) {
    while (*Line == ' ' || *Line == '\t') Line++;
    UINTN KeyLen = AsciiStrLen(Key);
    if (AsciiStrnCmp(Line, Key, KeyLen) == 0) return 1;
    return 0;
}

char* GetVal(char *Line, const char *Key) {
    while (*Line == ' ' || *Line == '\t') Line++;
    Line += AsciiStrLen(Key);
    while (*Line == ' ' || *Line == '\t' || *Line == '=') Line++;
    return Line;
}

void ParseSlot(CHAR16 *Name, char *Buffer) {
    BootEntry *E = &Entries[EntryCount++];
    StrCpy(E->FileName, Name);
    StrCpy(E->Title, Name);

    char *Line = Buffer;
    char *Next;
    while (Line && *Line) {
        Next = AsciiFindNextLine(Line);
        if (Next) *Next = 0;
        int Len = AsciiStrLen(Line);
        if (Len > 0 && Line[Len-1] == '\r') Line[Len-1] = 0;

        if (KeyMatch(Line, "TITLE")) {
            char *V = GetVal(Line, "TITLE");
            for(int i=0; V[i]; i++) E->Title[i]=(CHAR16)V[i]; E->Title[AsciiStrLen(V)]=0;
        }
        else if (KeyMatch(Line, "VERSION")) {
            char *V = GetVal(Line, "VERSION");
            for(int i=0; V[i]; i++) E->Version[i]=(CHAR16)V[i]; E->Version[AsciiStrLen(V)]=0;
        }
        else if (KeyMatch(Line, "KERNEL")) {
            char *V = GetVal(Line, "KERNEL");
            for(int i=0; V[i]; i++) E->Kernel[i]=(CHAR16)V[i]; E->Kernel[AsciiStrLen(V)]=0;
        }
        else if (KeyMatch(Line, "INITRD")) {
            char *V = GetVal(Line, "INITRD");
            for(int i=0; V[i]; i++) E->Initrd[i]=(CHAR16)V[i]; E->Initrd[AsciiStrLen(V)]=0;
        }
        else if (KeyMatch(Line, "PARAMS")) {
            char *V = GetVal(Line, "PARAMS");
            for(int i=0; V[i]; i++) E->Params[i]=(CHAR16)V[i]; E->Params[AsciiStrLen(V)]=0;
        }

        if (Next) Line = Next + 1; else break;
    }
}

void DetectRoot(EFI_FILE_PROTOCOL *Vol) {
    EFI_FILE_PROTOCOL *Test;
    StrCpy(DebugLog, CAST_STR(L"DEBUG LOG:\n"));
    if (!EFI_ERROR(Vol->Open(Vol, &Test, CAST_STR(L"\\asterboot"), EFI_FILE_MODE_READ, 0))) {
        ActiveRoot = CAST_STR(L"\\asterboot");
        StrCat(DebugLog, CAST_STR(L"Found Root: \\asterboot\n"));
        Test->Close(Test);
        return;
    }
    if (!EFI_ERROR(Vol->Open(Vol, &Test, CAST_STR(L"\\EFI\\asterboot"), EFI_FILE_MODE_READ, 0))) {
        ActiveRoot = CAST_STR(L"\\EFI\\asterboot");
        StrCat(DebugLog, CAST_STR(L"Found Root: \\EFI\\asterboot\n"));
        Test->Close(Test);
        return;
    }
    StrCat(DebugLog, CAST_STR(L"ERR: Root not found!\n"));
}

void LoadAll(EFI_FILE_PROTOCOL *Vol) {
    if (!ActiveRoot) return;
    CHAR16 CfgPath[128];
    StrCpy(CfgPath, ActiveRoot);
    StrCat(CfgPath, CAST_STR(L"\\asterboot.conf"));
    char *Buf; UINTN Sz;
    if (!EFI_ERROR(ReadFile(Vol, CfgPath, &Buf, &Sz))) {
        char *L = Buf, *Nx;
        while(L && *L) {
            Nx = AsciiFindNextLine(L); if(Nx) *Nx=0;
            if (KeyMatch(L, "TIMEOUT")) Config.Timeout=AsciiToInt(GetVal(L, "TIMEOUT"));
            else if (KeyMatch(L, "DEFAULT")) {
                char *V=GetVal(L, "DEFAULT"); if(V[AsciiStrLen(V)-1]=='\r') V[AsciiStrLen(V)-1]=0;
                for(int i=0; V[i]; i++) Config.DefaultSlot[i]=(CHAR16)V[i];
                Config.DefaultSlot[AsciiStrLen(V)]=0;
            }
            if(Nx) L=Nx+1; else break;
        }
        BS->FreePool(Buf);
    } else {
        Config.Timeout = 5;
        StrCat(DebugLog, CAST_STR(L"WARN: No config file\n"));
    }

    CHAR16 SlotPath[128];
    StrCpy(SlotPath, ActiveRoot);
    StrCat(SlotPath, CAST_STR(L"\\slots"));
    EFI_FILE_PROTOCOL *Dir;
    if (EFI_ERROR(Vol->Open(Vol, &Dir, SlotPath, EFI_FILE_MODE_READ, 0))) {
        StrCat(DebugLog, CAST_STR(L"ERR: Slots dir missing\n"));
        return;
    }
    UINTN InfoSz = 2048;
    EFI_FILE_INFO *Info;
    BS->AllocatePool(2, InfoSz, (void**)&Info);
    StrCat(DebugLog, CAST_STR(L"Scanning Slots:\n"));
    while(1) {
        InfoSz = 2048;
        if (EFI_ERROR(Dir->Read(Dir, &InfoSz, Info)) || InfoSz==0) break;
        if (Info->FileName[0] == '.') continue;
        StrCat(DebugLog, CAST_STR(L" - "));
        StrCat(DebugLog, Info->FileName);
        CHAR16 FullPath[256];
        StrCpy(FullPath, SlotPath);
        StrCat(FullPath, CAST_STR(L"\\"));
        StrCat(FullPath, Info->FileName);
        if (!EFI_ERROR(ReadFile(Vol, FullPath, &Buf, &Sz))) {
            ParseSlot(Info->FileName, Buf);
            BS->FreePool(Buf);
            StrCat(DebugLog, CAST_STR(L" [OK]\n"));
        } else {
            StrCat(DebugLog, CAST_STR(L" [READ FAIL]\n"));
        }
    }
    Dir->Close(Dir);
    BS->FreePool(Info);
}

void WriteDefault(EFI_FILE_PROTOCOL *Vol) {
    if (!ActiveRoot) return;
    CHAR16 CfgPath[128];
    StrCpy(CfgPath, ActiveRoot);
    StrCat(CfgPath, CAST_STR(L"\\asterboot.conf"));
    EFI_FILE_PROTOCOL *File;
    if (EFI_ERROR(Vol->Open(Vol, &File, CfgPath, EFI_FILE_MODE_CREATE|EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE, 0))) return;
    char Buffer[128]; char *P=Buffer;
    char *T="TIMEOUT="; while(*T) *P++=*T++;
    if(Config.Timeout>9) *P++='0'+(Config.Timeout/10); *P++='0'+(Config.Timeout%10); *P++='\n';
    T="DEFAULT="; while(*T) *P++=*T++;
    CHAR16 *S=Entries[SelectedIndex].FileName; while(*S) *P++=(char)(*S++); *P++='\n'; *P=0;
    UINTN Len = (UINTN)(P-Buffer);
    File->Write(File, &Len, Buffer);
    File->Close(File);
    StrCpy(Config.DefaultSlot, Entries[SelectedIndex].FileName);
}

/* --- UI --- */
void PrintAt(UINTN Col, UINTN Row, UINTN Attr, CHAR16 *Str) {
    ST->ConOut->SetAttribute(ST->ConOut, Attr);
    ST->ConOut->SetCursorPosition(ST->ConOut, Col, Row);
    ST->ConOut->OutputString(ST->ConOut, Str);
}

void DrawMenu(UINTN Timer) {
    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGRAY | BG_BLACK);
    ST->ConOut->ClearScreen(ST->ConOut);

    // Get Screen Dims
    UINTN Cols, Rows;
    ST->ConOut->QueryMode(ST->ConOut, ST->ConOut->Mode->Mode, &Cols, &Rows);

    UINTN TitleH = 5;
    UINTN HeaderH = 2;
    UINTN VisibleCount = (EntryCount < MAX_VISIBLE) ? EntryCount : MAX_VISIBLE;
    UINTN MenuH = VisibleCount;
    UINTN TotalContentH = TitleH + HeaderH + MenuH;

    UINTN StartY = (Rows > TotalContentH) ? (Rows - TotalContentH) / 2 : 1;
    UINTN CX = (Cols > 55) ? (Cols - 55) / 2 : 0;

    // --- TITLE ---
    UINTN R = StartY;
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK, CAST_STR(L"    _    ____ _____ _____ ____  ____   ___   ___ _____ "));
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK, CAST_STR(L"   / \\  / ___|_   _| ____|  _ \\| __ ) / _ \\ / _ \\_   _|"));
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK, CAST_STR(L"  / _ \\ \\___ \\ | | |  _| | |_) |  _ \\| | | | | | || |  "));
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK, CAST_STR(L" / ___ \\ ___) || | | |___|  _ <| |_) | |_| | |_| || |  "));
    PrintAt(CX, R++, EFI_LIGHTMAGENTA | BG_BLACK, CAST_STR(L"/_/   \\_\\____/ |_| |_____|_| \\_\\____/ \\___/ \\___/ |_|  "));
    R++;

    // --- HEADER ---
    UINTN HWidth = 60;
    UINTN HX = (Cols > HWidth) ? (Cols - HWidth) / 2 : 0;
    ST->ConOut->SetAttribute(ST->ConOut, BG_LIGHTGRAY | EFI_BLACK);
    ST->ConOut->SetCursorPosition(ST->ConOut, HX, R);
    for(UINTN k=0; k<HWidth; k++) ST->ConOut->OutputString(ST->ConOut, CAST_STR(L" "));
    PrintAt(HX + 2, R, BG_LIGHTGRAY | EFI_BLACK, CAST_STR(L"SLOTS:"));

    if (EntryCount > MAX_VISIBLE) {
        if (WindowStart > 0) PrintAt(HX+HWidth-3, R, BG_LIGHTGRAY | EFI_BLACK, CAST_STR(L"↑"));
        if (WindowStart + MAX_VISIBLE < EntryCount) PrintAt(HX+HWidth-2, R, BG_LIGHTGRAY | EFI_BLACK, CAST_STR(L"↓"));
    }
    R++;

    // --- ENTRIES (SCROLLABLE) ---
    if (EntryCount == 0) {
        PrintAt(CX, R++, EFI_LIGHTRED | BG_BLACK, CAST_STR(L"NO BOOT ENTRIES FOUND!"));
    }

    UINTN End = WindowStart + MAX_VISIBLE;
    if (End > EntryCount) End = EntryCount;

    for (UINTN i = WindowStart; i < End; i++) {
        BOOLEAN IsSel = (i == SelectedIndex);
        BOOLEAN IsDef = (StrCmp(Entries[i].FileName, Config.DefaultSlot) == 0);
        UINTN TextAttr = IsSel ? (EFI_LIGHTMAGENTA | BG_BLACK) : (EFI_LIGHTGRAY | BG_BLACK);

        UINTN RelR = R + (i - WindowStart);

        if (IsDef) PrintAt(HX+2, RelR, EFI_LIGHTGREEN | BG_BLACK, CAST_STR(L"D"));
        if (IsSel) PrintAt(HX+6, RelR, EFI_LIGHTMAGENTA | BG_BLACK, CAST_STR(L">"));

        ST->ConOut->SetAttribute(ST->ConOut, TextAttr);
        ST->ConOut->SetCursorPosition(ST->ConOut, HX+10, RelR);

        CHAR16 Line[128]; CHAR16 *D=Line, *S=Entries[i].Title;
        while(*S) *D++=*S++; *D++=' '; *D++='(';
        S=Entries[i].Version; while(*S) *D++=*S++; *D++=')'; *D=0;
        ST->ConOut->OutputString(ST->ConOut, Line);
    }

    // --- BOTTOM BAR ---
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | BG_BLACK);
    ST->ConOut->SetCursorPosition(ST->ConOut, 0, Rows-1);

    CHAR16 Status[128]; CHAR16 *T=Status, *Msg=(Timer>0)?CAST_STR(L" Booting in "):CAST_STR(L" UP/DOWN: Select | ENTER: Boot | U: BIOS | ESC: Reboot");
    while(*Msg) *T++=*Msg++;
    if (Timer>0) { if(Timer>9) *T++='0'+(Timer/10); *T++='0'+(Timer%10); Msg=CAST_STR(L" sec... | SPACE: Interrupt"); while(*Msg) *T++=*Msg++; }
    *T=0;

    UINTN SLen = StrLen(Status);
    UINTN SX = (Cols > SLen) ? (Cols - SLen) / 2 : 0;
    PrintAt(0, Rows-2, EFI_DARKGRAY | BG_BLACK, CAST_STR(L"____________________________________________________________________________________________________"));
    PrintAt(SX, Rows-1, EFI_LIGHTCYAN | BG_BLACK, Status);
}

void Boot(EFI_HANDLE ImageHandle) {
    BootEntry *E = &Entries[SelectedIndex];
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->OutputString(ST->ConOut, CAST_STR(L"Loading Kernel...\r\n"));

    // 1. Identify Self and Disk
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    BS->HandleProtocol(ImageHandle, (EFI_GUID*)&EFI_LOADED_IMAGE_PROTOCOL_GUID, (void**)&LoadedImage);
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FS;
    BS->HandleProtocol(LoadedImage->DeviceHandle, (EFI_GUID*)&EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (void**)&FS);
    EFI_FILE_PROTOCOL *Root;
    FS->OpenVolume(FS, &Root);

    // 2. Load Kernel to RAM
    void *KernelBuffer; UINTN KernelSize;
    if (EFI_ERROR(ReadFile(Root, E->Kernel, (char**)&KernelBuffer, &KernelSize))) {
        ST->ConOut->OutputString(ST->ConOut, CAST_STR(L"Error: Kernel not found.\r\n"));
        ST->ConOut->OutputString(ST->ConOut, E->Kernel);
        BS->Stall(5000000); return;
    }

    // 3. Create the Kernel Image
    EFI_HANDLE KernelHandle;
    EFI_STATUS Status = BS->LoadImage(FALSE, ImageHandle, NULL, KernelBuffer, KernelSize, &KernelHandle);
    if (EFI_ERROR(Status)) { ST->ConOut->OutputString(ST->ConOut, CAST_STR(L"LoadImage Error\r\n")); BS->Stall(3000000); return; }

    EFI_LOADED_IMAGE_PROTOCOL *KernelLoaded;
    BS->HandleProtocol(KernelHandle, (EFI_GUID*)&EFI_LOADED_IMAGE_PROTOCOL_GUID, (void**)&KernelLoaded);
    // Since asterboot loads directly to ram, the firmware gave the kernel a NULL DeviceHandle
    // So DeviceHandle has to be manually copied to the Kernel so it knows where to read initramfs from
    KernelLoaded->DeviceHandle = LoadedImage->DeviceHandle; // Handle Injection

    // 5. Set Command Line
    CHAR16 CmdLine[512]; CHAR16 *D=CmdLine, *S=E->Params;
    while(*S) *D++=*S++;

    // Append Initrd
    if (StrLen(E->Initrd) > 0) {
        CHAR16 *I=CAST_STR(L" initrd="); while(*I) *D++=*I++;
        S=E->Initrd; while(*S) *D++=*S++;
    }
    *D=0;

    KernelLoaded->LoadOptionsSize = (StrLen(CmdLine)+1)*2;
    KernelLoaded->LoadOptions = CmdLine;

    // 6. Start
    Status = BS->StartImage(KernelHandle, NULL, NULL);

    // Failure handling
    ST->ConOut->OutputString(ST->ConOut, CAST_STR(L"Kernel Returned\r\n"));
    BS->Stall(5000000);
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    ST = SystemTable;
    BS = SystemTable->BootServices;
    RT = SystemTable->RuntimeServices;

    ST->ConOut->Reset(ST->ConOut, FALSE);
    BS->SetWatchdogTimer(0, 0, 0, NULL);
    SetBestResolution();
    ST->ConOut->EnableCursor(ST->ConOut, FALSE);

    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    if (EFI_ERROR(BS->HandleProtocol(ImageHandle, (EFI_GUID*)&EFI_LOADED_IMAGE_PROTOCOL_GUID, (void**)&LoadedImage))) return 0;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FS;
    if (EFI_ERROR(BS->HandleProtocol(LoadedImage->DeviceHandle, (EFI_GUID*)&EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (void**)&FS))) return 0;
    EFI_FILE_PROTOCOL *Root;
    FS->OpenVolume(FS, &Root);

    DetectRoot(Root);
    LoadAll(Root);

    for(UINTN i=0; i<EntryCount; i++) {
        if(StrCmp(Entries[i].FileName, Config.DefaultSlot) == 0) SelectedIndex = i;
    }
    if (SelectedIndex >= MAX_VISIBLE) WindowStart = SelectedIndex - MAX_VISIBLE + 1;

    UINTN Timer = Config.Timeout;
    BOOLEAN Auto = TRUE;
    EFI_INPUT_KEY Key;

    if (Timer == 0) {
        BS->Stall(100000);
        if (!EFI_ERROR(ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) && Key.UnicodeChar == ' ') {
            Auto = FALSE; Timer = 0;
        } else {
            Boot(ImageHandle);
        }
    }

    while(Timer > 0 && Auto) {
        DrawMenu(Timer);
        BS->Stall(1000000);
        if (!EFI_ERROR(ST->ConIn->ReadKeyStroke(ST->ConIn, &Key))) { Auto = FALSE; break; }
        Timer--;
    }
    if (Auto && EntryCount > 0) Boot(ImageHandle);

    while(1) {
        DrawMenu(0);
        UINTN Index;
        BS->WaitForEvent(1, (EFI_EVENT*)&ST->ConIn->WaitForKey, &Index);
        ST->ConIn->ReadKeyStroke(ST->ConIn, &Key);

        if (Key.ScanCode == 0x01) {
            if (SelectedIndex > 0) SelectedIndex--; else SelectedIndex = EntryCount-1;
            if (SelectedIndex < WindowStart) WindowStart = SelectedIndex;
            else if (SelectedIndex == EntryCount-1) WindowStart = (EntryCount > MAX_VISIBLE) ? EntryCount - MAX_VISIBLE : 0;
        }
        else if (Key.ScanCode == 0x02) {
            if (SelectedIndex < EntryCount-1) SelectedIndex++; else SelectedIndex = 0;
            if (SelectedIndex >= WindowStart + MAX_VISIBLE) WindowStart++;
            else if (SelectedIndex == 0) WindowStart = 0;
        }
        else if (Key.UnicodeChar == 0x0D && EntryCount > 0) { Boot(ImageHandle); }
        else if (Key.UnicodeChar == 'd' || Key.UnicodeChar == 'D') { WriteDefault(Root); }
        else if (Key.UnicodeChar == 'u' || Key.UnicodeChar == 'U') { EnterSetup(); }
        else if (Key.ScanCode == 0x17) { Reboot(); }
    }
    return EFI_SUCCESS;
}
