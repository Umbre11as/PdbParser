# Lightweight PdbParser
Lightweight library for parsing Microsoft PDB files

## 📄 Example types parsing
```c++
BYTE* pdbBuffer = ReadFile(R"(C:\Symbols\ntkrnlmp.pdb)"); // Your file read function
LightweightPDB::PDB7 pdb(R"(C:\Windows\System32\ntoskrnl.exe)", pdbBuffer);
LightweightPDB::TPIStream tpi = pdb.ReadStream<LightweightPDB::TPIStream>();

// All classes/structs parsing
for (LightweightPDB::Record* record : tpi.Records()) {
    if (LightweightPDB::ClassRecord* classRecord = dynamic_cast<LightweightPDB::ClassRecord*>(record)) {
        printf("%s:\n", classRecord->Name());
        LightweightPDB::FieldListRecord* fieldList = tpi.WithTypeIndex<LightweightPDB::FieldListRecord>(classRecord->FieldListIndex());
        for (LightweightPDB::MemberRecord* memberRecord : fieldList->Members()) {
            printf("    %s = 0x%X\n", memberRecord->Name(), memberRecord->Offset());
        }
    }
}

// Only _EPROCESS
LightweightPDB::ClassRecord* eprocessClass = tpi.FindClass("_EPROCESS");
LightweightPDB::FieldListRecord* fieldList = tpi.WithTypeIndex<LightweightPDB::FieldListRecord>(eprocessClass->FieldListIndex());
for (LightweightPDB::MemberRecord* member : fieldList->Members()) {
    if (strcmp(member->Name(), "ProcessLock") == 0) {
        printf("_EPROCESS->ProcessLock = 0x%X", member->Offset());
    }
}
```

### Output:
```
_FAST_MUTEX:
    Count = 0x0
    Owner = 0x8
    Contention = 0x10
    Event = 0x18
    OldIrql = 0x30
... (other classes)

_EPROCESS->ProcessLock = 0x438
```

## 📄 Example symbols parsing
```c++
LightweightPDB::DBIStream dbi = pdb.ReadStream<LightweightPDB::DBIStream>();
for (const LightweightPDB::FunctionInfo& function : dbi.Functions())
    printf("%s = 0x%X\n", function.Name, function.Offset);
```

### Output:
```
IoVerifyPartitionTable = 0x6E2990
PnpCreateDeviceEventEntry = 0x9240D4
(keep in mind that the names are not demangled)
??_C@_1IC@NLFGEANP@?$AAF?$AAo?$AAr?$AA?5?$AAm?$AAo?$AAr?$AAe?$AA?5?$AAi?$AAn?$AAf?$AAo?$AAr?$AAm@ = 0x40AD0
```

## 🏗️ Build with CMake

### 🌍 Using FetchContent
```cmake
include(FetchContent)

FetchContent_Declare(
    PdbParser
    GIT_REPOSITORY https://github.com/Umbre11as/PdbParser.git
    GIT_TAG master
)
FetchContent_MakeAvailable(PdbParser)
```

### 📂 Using local path
```cmake
set(PdbParser_DIR "/path/to/directory")

find_package(PdbParser REQUIRED)
target_link_libraries(YourProject PRIVATE PdbParser::PdbParser)
```
