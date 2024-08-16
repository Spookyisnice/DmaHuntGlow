#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "Vector.h"
#include "../../DMALibrary/Memory/Memory.h"
#include <string>
#include <algorithm>

// All variables from this Project
uint64_t SSystemGlobEnv = 0x26782f8; // Update this as needed
uint64_t EntitySystemOffset = 0xC0;
uint64_t Psystem = 0x90;
uint64_t ObjectCountOffset = 0x40092;
uint64_t EntityListOffset = 0x40078;
uint64_t PosOffset = 0x134;
uint64_t RenderNodeColorOffset = 0x3C;
uint64_t RenderNodePointer = 0x0;

uint64_t renderNodePtr;

bool run = true;
Vector3 ImmolatorBossPos;
// Apply additional commands before writing the glow color
uint64_t allmap = 0x80018;
float maxdistance = 5000;

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
    char pad_01[0x28]; // 0x00(0x28)
    unsigned __int64 rnd_flags; // 0x28(0x08)
    char pad_02[0xc]; // 0x30(0xc)
    unsigned int silhouettes_param; // 0x3c(0x04)
};

struct entityNameStruct {
    char name[100];
};




const char* hunter = "HunterBasic";


int main() {

    std::vector<uint64_t> entityBases;

    uint64_t HunterBasic;
    uint64_t immolator_el;
    uint64_t Target_Butcher;

    // Initialize memory
    if (!mem.Init("HuntGame.exe", true, true)) {
        std::cout << "Failed to initialize DMA" << std::endl;
        return 1;
    }

    if (!mem.FixCr3())
        std::cout << "Failed to fix CR3" << std::endl;
    else
        std::cout << "CR3 fixed" << std::endl;

    std::cout << "DMA initialized" << std::endl;

    uintptr_t base = mem.GetBaseDaddy("GameHunt.dll");

    uint64_t SSystemGlobalEnvironment = mem.Read<uint64_t>(base + SSystemGlobEnv);
    uint64_t EntitySystem = mem.Read<uint64_t>(SSystemGlobalEnvironment + EntitySystemOffset);
    uint64_t pSystem = mem.Read<uint64_t>(SSystemGlobalEnvironment + Psystem);

    // This is only 4 bytes and no negatives
    uint16_t NumberOfObjects = mem.Read<uint16_t>(EntitySystem + ObjectCountOffset);
    std::cout << "NumberOfObjects: " << NumberOfObjects << std::endl;

    uint64_t EntityList = EntitySystem + EntityListOffset;

    D3DXMATRIX m_renderViewMatrix = mem.Read<D3DXMATRIX>(pSystem + 0x928 + 0x230);
    Vector3 cameraPos = mem.Read<Vector3>(pSystem + 0x928 + 0x2F0);
    D3DXMATRIX m_renderProjectionMatrix = mem.Read<D3DXMATRIX>(pSystem + 0x928 + 0x270);
         
    // Counter for the number of HunterBasic entities found
    int hunterCount = 0;
    int Target_ButcherCount = 0;
    int immolator_eliteCount = 0;


    bool loopCompleted = false;


    std::vector<uint64_t> hunterBases;
    std::vector<uint64_t> immolatorBases;
    std::vector<uint64_t> butcherBases;

    // Iterate through the numberOfObjects
    for (unsigned int i = 0; i < NumberOfObjects; ++i) {
        uint64_t entityBase = mem.Read<uintptr_t>(EntityList + i * sizeof(uint64_t));
        if (entityBase == NULL) {
            continue;
        }

        // Get name of entity
        uintptr_t entityNamePtr = mem.Read<uintptr_t>(entityBase + 0x10);
        entityNameStruct entityName = mem.Read<entityNameStruct>(entityNamePtr);
        entityName.name[99] = '\0';

        // Store entities in separate vectors based on their type
        if (strstr(entityName.name, "HunterBasic") != NULL) {
            hunterBases.push_back(entityBase);
            std::cout << "Found HunterBasic  [" << hunterBases.size() << "]" << std::endl;
        }
        else if (strstr(entityName.name, "immolator_el") != NULL) {
            immolatorBases.push_back(entityBase);
            std::cout << "Found immolator_elite  [" << immolatorBases.size() << "]" << std::endl;
        }
        else if (strstr(entityName.name, "Target_Butcher") != NULL) {
            butcherBases.push_back(entityBase);
            std::cout << "Found Target_Butcher  [" << butcherBases.size() << "]" << std::endl;
        }
    }

    // Accessing Immolator positions
    if (!immolatorBases.empty()) {
        for (size_t i = 0; i < immolatorBases.size(); ++i) {
            Vector3 immolatorPos = mem.Read<Vector3>(immolatorBases[i] + PosOffset);
            std::cout << "Immolator " << i + 1 << " Position - X: " << immolatorPos.x << " Y: " << immolatorPos.y << " Z: " << immolatorPos.z << std::endl;
        }
    }




    loopCompleted = true;

    while (loopCompleted)
    {


        // Accessing HunterBasic positions
        if (!hunterBases.empty()) {
            for (size_t i = 0; i < hunterBases.size(); ++i) {
                Vector3 hunterPos = mem.Read<Vector3>(hunterBases[i] + PosOffset);
                std::cout << "Hunter " << i + 1 << " Position - X: " << hunterPos.x << " Y: " << hunterPos.y << " Z: " << hunterPos.z << std::endl;
            }
        }
        // Accessing Immolator positions
        if (!immolatorBases.empty()) {
            for (size_t i = 0; i < immolatorBases.size(); ++i) {
                Vector3 immolatorPos = mem.Read<Vector3>(immolatorBases[i] + PosOffset);
                std::cout << "Immolator " << i + 1 << " Position - X: " << immolatorPos.x << " Y: " << immolatorPos.y << " Z: " << immolatorPos.z << std::endl;
            }
        }

    }



    // Wait for user input before exiting
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}
