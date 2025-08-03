#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "Vector.h"
#include "../../DMALibrary/Memory/Memory.h"
#include <string>
#include <algorithm>
#include "Globals.h"

int main() {
    // Initialize memory(Dma)
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
    auto size = mem.GetBaseSize("GameHunt.dll");

    while (true)
    {
        auto handle = mem.CreateScatterHandle();

        mem.AddScatterReadRequest(handle, base + SSystemGlobalEnvironmentoffset, &SSystemGlobalEnvironment, sizeof(SSystemGlobalEnvironment));
        mem.ExecuteReadScatter(handle);
        mem.AddScatterReadRequest(handle, SSystemGlobalEnvironment + EntitySystemOffset, &EntitySystem, sizeof(EntitySystem));
        mem.AddScatterReadRequest(handle, SSystemGlobalEnvironment + Psystem, &pSystem, sizeof(pSystem));
        mem.ExecuteReadScatter(handle);

        // This is only 4 bytes and no negatives
        mem.AddScatterReadRequest(handle, EntitySystem + ObjectCountOffset, &NumberOfObjects, sizeof(NumberOfObjects));
        mem.ExecuteReadScatter(handle);
        std::cout << "NumberOfObjects: " << NumberOfObjects << std::endl;

        uint64_t EntityList = EntitySystem + EntityListOffset;
        /* This code is not needed for applying chams its only needed for reading the player cam stuff!
        D3DXMATRIX m_renderViewMatrix = mem.Read<D3DXMATRIX>(pSystem + 0x928 + 0x230);
        Vector3 cameraPos = mem.Read<Vector3>(pSystem + 0x928 + 0x2F0);
        D3DXMATRIX m_renderProjectionMatrix = mem.Read<D3DXMATRIX>(pSystem + 0x928 + 0x270);
        */

        // Iterate through the numberOfObjects
        for (unsigned int i = 0; i < NumberOfObjects; ++i) {
            mem.AddScatterReadRequest(handle, EntityList + i * sizeof(uint64_t), &entityBase, sizeof(entityBase));
            mem.ExecuteReadScatter(handle);
            if (entityBase == NULL) {
                continue;
            }

            // Get name of entity
            entityNameStruct entityName;
            mem.AddScatterReadRequest(handle, entityBase + 0x10, &entityNamePtr, sizeof(entityNamePtr));
            mem.ExecuteReadScatter(handle);
            mem.AddScatterReadRequest(handle, entityNamePtr, &entityName, sizeof(entityName));
            mem.ExecuteReadScatter(handle);
            entityName.name[99] = '\0';

            // Get class name of entity
            entityNameStruct entityClassName;
            mem.AddScatterReadRequest(handle, entityBase + 0x18, &entityClassPtr, sizeof(entityClassPtr));
            mem.ExecuteReadScatter(handle);
            mem.AddScatterReadRequest(handle, entityClassPtr + 0x10, &entityClassPtr, sizeof(entityClassPtr));
            mem.ExecuteReadScatter(handle);
            mem.AddScatterReadRequest(handle, entityClassPtr, &entityClassName, sizeof(entityClassName));
            mem.ExecuteReadScatter(handle);
            entityClassName.name[99] = '\0';

            // Follow the new chams offset path: class + 0xa8 -> 0x0 -> + 0xa0 -> + 0x130
            mem.AddScatterReadRequest(handle, entityBase + 0xA8, &slotsPtr, sizeof(slotsPtr));
            mem.ExecuteReadScatter(handle);
            if (slotsPtr == 0) {
                continue;
            }
            mem.AddScatterReadRequest(handle, slotsPtr + 0x0, &slotPtr, sizeof(slotPtr));
            mem.ExecuteReadScatter(handle);
            if (slotPtr == 0) {
                continue;
            }
            mem.AddScatterReadRequest(handle, slotPtr + 0xA0, &renderNodePtr, sizeof(renderNodePtr));
            mem.ExecuteReadScatter(handle);
            if (renderNodePtr == 0) {
                continue;
            }

            // Calculate the chams color offset (renderNodePtr + 0x130)
            uintptr_t chamsColorPtr = renderNodePtr + 0x130;

            // Store entities in separate vectors based on their class names and apply chams
            if (strstr(entityClassName.name, "HunterBasic") != NULL) {
                hunterBases.push_back(entityBase);
                std::cout << "Found HunterBasic  [" << hunterBases.size() << "]" << std::endl;

                // Apply chams using the new offset
                /*
				mem.AddScatterWriteRequest(handle, renderNodePtr + 0x10, &allmap, sizeof(uint32_t));
				mem.AddScatterWriteRequest(handle, renderNodePtr + 0x38, &maxdistance, sizeof(uint32_t));*/
                mem.AddScatterWriteRequest(handle, chamsColorPtr, &bigBodyColor, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityClassName.name, "immolator_elite") != NULL) {
                immolatorBases.push_back(entityBase);
                std::cout << "Found immolator_elite  [" << immolatorBases.size() << "]" << std::endl;

                mem.AddScatterWriteRequest(handle, chamsColorPtr, &bigBodyColor, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityClassName.name, "target_butcher") != NULL) {
                Target_ButcherBases.push_back(entityBase);
                std::cout << "Found target_butcher  [" << Target_ButcherBases.size() << "]" << std::endl;

                mem.AddScatterWriteRequest(handle, chamsColorPtr, &bigBodyColor, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityClassName.name, "target_spider") != NULL) {
                Spider_targetBase.push_back(entityBase);
                std::cout << "Found target_spider  [" << Spider_targetBase.size() << "]" << std::endl;

                mem.AddScatterWriteRequest(handle, chamsColorPtr, &bigBodyColor, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityClassName.name, "target_scrapbeak") != NULL) {
                Target_ScrapbeakBase.push_back(entityBase);
                std::cout << "Found target_scrapbeak  [" << Target_ScrapbeakBase.size() << "]" << std::endl;

                mem.AddScatterWriteRequest(handle, chamsColorPtr, &bigBodyColor, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityClassName.name, "target_assassin") != NULL) {
                Target_AssassinBase.push_back(entityBase);
                std::cout << "Found target_assassin  [" << Target_AssassinBase.size() << "]" << std::endl;

                mem.AddScatterWriteRequest(handle, chamsColorPtr, &bigBodyColor, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityClassName.name, "target_rotjaw") != NULL) {
                // Add rotjaw vector if not already in globals
                std::cout << "Found target_rotjaw" << std::endl;

                mem.AddScatterWriteRequest(handle, chamsColorPtr, &bigBodyColor, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityName.name, "cash_register_golden") != NULL) {
                cash_register_golden.push_back(entityBase);
                std::cout << "Found cash_register_golden  [" << cash_register_golden.size() << "]" << std::endl;

                mem.AddScatterWriteRequest(handle, chamsColorPtr, &bigBodyColor, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            /*
            else if (strstr(entityClassName.name, "2r") != NULL && strcmp(entityName.name, "") == 0 || (entityClassName.name, "1r") != NULL && strcmp(entityName.name, "") == 0) {
                mem.AddScatterWriteRequest(handle, chamsColorPtr, &RedFull, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }*/
        }
        mem.CloseScatterHandle(handle);
        std::cout << "Press Enter to Repeat loop";
        std::cin.get();
        system("CLS");
        // Clear all entity vectors before each loop iteration
        hunterBases.clear();
        immolatorBases.clear();
        Target_ButcherBases.clear();
        Spider_targetBase.clear();
        Target_ScrapbeakBase.clear();
        Target_AssassinBase.clear();
        cash_register_golden.clear();
    }
}