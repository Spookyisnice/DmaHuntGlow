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

            /*
            char pBuffer[64];

            uintptr_t Entity = mem.Read<uintptr_t>(EntitySystem + 0x40098 + (i * 0x8));

            mem.Read(Entity + 0x10, pBuffer, sizeof(pBuffer));

            // Initialize the buffer and read the name from memory into pBuffer

            if (std::string(pBuffer).find("bullet") != std::string::npos)
            {
                printf("bullet is found.........");
            }*/



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

            // Get entity render node
            mem.AddScatterReadRequest(handle, entityBase + 0xA8, &slotsPtr, sizeof(slotsPtr));
            mem.ExecuteReadScatter(handle);
            if (slotsPtr == 0) {
                continue;
            }
            mem.AddScatterReadRequest(handle, slotsPtr + 0, &slotPtr, sizeof(slotPtr));
            mem.ExecuteReadScatter(handle);
            if (slotPtr == 0) {
                continue;
            }
            mem.AddScatterReadRequest(handle, slotPtr + 0xA0, &renderNodePtr, sizeof(renderNodePtr));
            mem.ExecuteReadScatter(handle);
            if (renderNodePtr == 0) {
                continue;
            }
            renderNode rNode;
            mem.AddScatterReadRequest(handle, renderNodePtr, &rNode, sizeof(rNode));
            mem.ExecuteReadScatter(handle);



            // Store entities in separate vectors based on their type
            if (strstr(entityName.name, "HunterBasic") != NULL) {
                hunterBases.push_back(entityBase);
                //std::cout << "Found HunterBasic  [" << hunterBases.size() << "]" << std::endl;
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x10, &value1, sizeof(uint32_t));
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x2C, &value2, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);

            }
            else if (strstr(entityName.name, "immolator_el") != NULL) {
                immolatorBases.push_back(entityBase);
                std::cout << "Found immolator_elite  [" << immolatorBases.size() << "]" << std::endl;
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x10, &value1, sizeof(uint32_t));
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x2C, &value3, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);

            }
            else if (strstr(entityName.name, "Target_Butcher") != NULL) {
                Target_ButcherBases.push_back(entityBase);
                std::cout << "Found Target_Butcher  [" << Target_ButcherBases.size() << "]" << std::endl;
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x10, &value1, sizeof(uint32_t));
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x2C, &value3, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityName.name, "Target_Assassin") != NULL) {
                Target_AssassinBase.push_back(entityBase);
                std::cout << "Found Target_Assassin  [" << Target_AssassinBase.size() << "]" << std::endl;
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x10, &value1, sizeof(uint32_t));
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x2C, &value3, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityName.name, "Spider_target") != NULL) {
                Spider_targetBase.push_back(entityBase);
                std::cout << "Found Spider_target  [" << Spider_targetBase.size() << "]" << std::endl;
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x10, &value1, sizeof(uint32_t));
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x2C, &value3, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityName.name, "Target_Scrapbeak") != NULL) {
                Target_ScrapbeakBase.push_back(entityBase);
                std::cout << "Found Target_Scrapbeak  [" << Target_ScrapbeakBase.size() << "]" << std::endl;
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x10, &value1, sizeof(uint32_t));
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x2C, &value3, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }
            else if (strstr(entityClassName.name, "2r") != NULL && strcmp(entityName.name, "") == 0 || (entityClassName.name, "1r") != NULL && strcmp(entityName.name, "") == 0) {
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x10, &value1, sizeof(uint32_t));
                mem.AddScatterWriteRequest(handle, renderNodePtr + 0x2C, &value3, sizeof(uint32_t));
                mem.ExecuteWriteScatter(handle);
            }

        }
        mem.CloseScatterHandle(handle);
        std::cout << "Press Enter to Repeat loop";
        std::cin.get();
        system("CLS");
    }

}
