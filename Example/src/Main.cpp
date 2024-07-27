#include <iostream>
#include <thread>
#include <chrono>
#include "Vector.h"
#include "../../DMALibrary/Memory/Memory.h"

const uint64_t SYSTEM_GLOBAL_ENVIRONMENT = 0x5EFCF90; // Update this as needed
const uint64_t ENTITY_SYSTEM_OFFSET = 0xA8;
const uint64_t PSYSTEM_OFFSET = 0xC0;
const uint64_t OBJECT_COUNT_OFFSET = 0x4006A;
const uint64_t ENTITY_LIST_OFFSET = 0x40078;
const uint64_t POS_OFFSET = 0x128;
const uint64_t RENDER_NODE_COLOR_OFFSET = 0x3C; // Ensure this is the correct offset for color in the render node

enum colorType : uint32_t {
    Red = 0xFF0000FF,
    Green = 0x00FF00FF,
    Blue = 0x0000FFFF,
    Cyan = 0x00FFFFFF,
    Orange = 0xFFA500FF,
    Yellow = 0xFFFF00FF,
    White = 0xFFFFFFFF,
    RedFilled = 0xFF000000
};

struct renderNode {
    char pad_01[0x28]; // 0x00(0x28)
    uint64_t rnd_flags; // 0x28(0x08)
    char pad_02[0xC]; // 0x30(0xC)
    uint32_t silhouettes_param; // 0x3C(0x04)
};

struct EntityNameStruct {
    char name[100];
};

bool run = true;

int main() {
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

    while (run) {
        uint64_t systemGlobalEnvironment = mem.Read<uint64_t>(base + SYSTEM_GLOBAL_ENVIRONMENT);
        if (systemGlobalEnvironment == 0) {
            continue;
        }

        uint64_t entitySystem = mem.Read<uint64_t>(systemGlobalEnvironment + ENTITY_SYSTEM_OFFSET);
        if (entitySystem == 0) {
            continue;
        }

        uint64_t pSystem = mem.Read<uint64_t>(systemGlobalEnvironment + PSYSTEM_OFFSET);
        if (pSystem == 0) {
            continue;
        }

        uint64_t numberOfObjects = mem.Read<uint64_t>(entitySystem + OBJECT_COUNT_OFFSET);
        uint64_t entityList = entitySystem + ENTITY_LIST_OFFSET;

        for (unsigned int i = 0; i < numberOfObjects; ++i) {
            uint64_t entity = mem.Read<uint64_t>(entityList + i * sizeof(uint64_t));
            if (entity == 0) {
                continue;
            }

            Vector3 enemyPos = mem.Read<Vector3>(entity + POS_OFFSET);

            // Get name of entity
            uintptr_t entityNamePtr = mem.Read<uintptr_t>(entity + 0x10);
            EntityNameStruct entityName = mem.Read<EntityNameStruct>(entityNamePtr);
            entityName.name[99] = '\0';

            // Get class name of entity
            uintptr_t entityClassPtr = mem.Read<uintptr_t>(entity + 0x18);
            entityClassPtr = mem.Read<uintptr_t>(entityClassPtr + 0x10);
            EntityNameStruct entityClassName = mem.Read<EntityNameStruct>(entityClassPtr);
            entityClassName.name[99] = '\0';

            std::cout << "Entity Name: " << entityName.name << ", Class Name: " << entityClassName.name << std::endl;

            // Get entity render node
            uintptr_t slotsPtr = mem.Read<uintptr_t>(entity + 0xA8);
            if (slotsPtr == 0) {
                continue;
            }

            uintptr_t slotPtr = mem.Read<uintptr_t>(slotsPtr + 0);
            if (slotPtr == 0) {
                continue;
            }

            uintptr_t renderNodePtr = mem.Read<uintptr_t>(slotPtr + 0xA8);
            if (renderNodePtr == 0) {
                continue;
            }

                renderNode rNode = mem.Read<renderNode>(renderNodePtr);
            // Filter specific entities by name or class name
            if (strstr(entityName.name, "HunterBasic") != nullptr) {




                // Write the color to the render node
                uint32_t RGBAColor = colorType::RedFilled;
                mem.Write<uint32_t>(renderNodePtr + RENDER_NODE_COLOR_OFFSET, RGBAColor);
                
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for 100 milliseconds
    }

    return 0;
}


/*
        uint64_t allmap = 0x80018;
        float maxdistance = 5000;
        TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x28, &allmap, sizeof(uint64_t)); // change render flag to max distance, allows us to use chams at further distances as long as the model isn't culled.
        TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x3c, &convertedcolour, sizeof(uint32_t));
        TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x48, &maxdistance, sizeof(float));// credit Kiosk
        // printf RenderNodePointer
        */