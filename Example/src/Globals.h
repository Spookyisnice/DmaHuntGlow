#pragma once
// All variables from this Project
uint64_t EntitySystemOffset = 0xC0;
uint64_t Psystem = 0x90;
uint64_t ObjectCountOffset = 0x40092;
uint64_t EntityListOffset = 0x40078;
uint64_t PosOffset = 0x134;
uint64_t EntityArray = 0x400A0;
uint64_t renderNodePtr;
uint64_t SSystemGlobalEnvironmentoffset = 0x20C7318; //this changes every update it gets posted on unknowncheats https://www.unknowncheats.me/forum/other-fps-games/350352-hunt-showdown-45.html


uint64_t SSystemGlobalEnvironment;
uint64_t pSystem;
uint64_t EntitySystem;
uint16_t NumberOfObjects;
uintptr_t entityNamePtr;
uintptr_t entityClassPtr;
uintptr_t slotsPtr;
uintptr_t slotPtr;
uint64_t entityBase;
uint32_t value1 = 0x80018;


uintptr_t IGameFramework = 0xD8;
uintptr_t pGame = 0x700;
uintptr_t pGameClientNub = 0x48;
uintptr_t pGameClientChannel = 0x20;
uintptr_t localPlayerIdx = 0x24;

// Counter for the number of HunterBasic entities found
int hunterCount = 0;
int Target_ButcherCount = 0;
int immolator_eliteCount = 0;


bool run = true;

std::vector<uint64_t> hunterBases;
std::vector<uint64_t> immolatorBases;
std::vector<uint64_t> Target_ButcherBases;
std::vector<uint64_t> Target_AssassinBase;
std::vector<uint64_t> Spider_targetBase;
std::vector<uint64_t> Target_ScrapbeakBase;
std::vector<uint64_t> cash_register_golden;
std::vector<uint64_t> cash_register;
std::vector<uint64_t> localplayerbase;





std::vector<uint64_t> BulletBase;
std::vector<uint64_t> ZombieBase;




enum colorType : uint32_t {
    Red = 0xFF0000FF,
    Green = 0x00FF00FF,
    Blue = 0x0000FFFF,
    Cyan = 0x00FFFFFF,
    Orange = 0xFFA500FF,
    Yellow = 0xFFFF00FF,
    White = 0xFFFFFFFF,
    RedFilled = 0xFF000000,
    BlueFilled = 0x0000FF00,
    YellowFilled = 0xFFFF0000,
    OrangeFilled = 0xFFA50000,
    CyanFilled = 0x00FFFF00,
    WhiteFilled = 0xFFFFFF00
};

struct renderNode {
    char pad_01[0x0010]; // 0x00(0x28)
    unsigned __int64 rnd_flags; // 0x28(0x08)
    char pad_02[0x002C]; // 0x30(0xc)
    unsigned int silhouettes_param; // 0x3c(0x04)
};

struct entityNameStruct {
    char name[100];
};


uint32_t value2 = colorType::CyanFilled;
uint32_t value3 = colorType::RedFilled;
