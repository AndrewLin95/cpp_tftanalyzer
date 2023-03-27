#include <Windows.h>

#include <iostream>
#include <tchar.h> // _tcscmp
//#include <vector>
// 
#include <TlHelp32.h>
#include <windows.h>

//uintptr_t GetModuleBaseAddress(DWORD procId, wchar_t* modName)
//{
//    //initialize to zero for error checking
//    uintptr_t modBaseAddr = 0;
//
//    //get a handle to a snapshot of all modules
//    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
//
//    //check if it's valid
//    if (hSnap != INVALID_HANDLE_VALUE)
//    {
//        //this struct holds the actual module information
//        MODULEENTRY32 modEntry{};
//
//        //this is required for the function to work
//        modEntry.dwSize = sizeof(modEntry);
//
//        //If a module exists, get it's entry
//        if (Module32First(hSnap, &modEntry))
//        {
//            //loop through the modules
//            do
//            {
//                //compare the module name against ours
//                if (!_wcsicmp(modEntry.szModule, modName))
//                {
//                    //copy the base address and break out of the loop
//                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
//                    break;
//                }
//
//                //each iteration we grab the next module entry
//            } while (Module32Next(hSnap, &modEntry));
//        }
//    }
//
//    //free the handle
//    CloseHandle(hSnap);
//    return modBaseAddr;
//}

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

int main() {

    HWND hGameWindow = FindWindowA(NULL, "Sun Haven");
    if (hGameWindow == NULL) {
        std::cout << "Start the game!" << std::endl;
        return 0;
    };
    DWORD pID = NULL; // ID of our Game

    if (pID != NULL) {
        std::cout << "pID: " << pID << std::endl;
    };

    GetWindowThreadProcessId(hGameWindow, &pID);
    HANDLE processHandle = NULL;
     
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL) { // error handling
        std::cout << "Failed to open process" << std::endl;
        return 0;
    }

    char gameName[] = "Sun Haven.exe";
    DWORD gameBaseAddress = GetModuleBaseAddress(_T(gameName), pID);
    std::cout << "Game Base Address: " << gameBaseAddress << std::endl;



    //DWORD offsetGameToBaseAdress = 0x001F4FC0;
    //std::vector<DWORD> pointsOffsets{ 0x68,0x88,0x08,0x00,0x08,0xA0,0x90,0x10,0xE8 };
    //DWORD baseAddress = NULL;
    ////Get value at gamebase+offset -> store it in baseAddress
    //ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress + offsetGameToBaseAdress), &baseAddress, sizeof(baseAddress), NULL);
    //std::cout << "debugginfo: baseaddress = " << std::hex << baseAddress << std::endl;
    //DWORD pointsAddress = baseAddress; //the Adress we need -> change now while going through offsets
    //for (int i = 0; i < pointsOffsets.size() - 1; i++) // -1 because we dont want the value at the last offset
    //{
    //    ReadProcessMemory(processHandle, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
    //    std::cout << "debugginfo: Value at offset = " << std::hex << pointsAddress << std::endl;
    //}
    //pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1); //Add Last offset -> done!!

    ////"UI"
    //std::cout << "Zuma Deluxe 1.1.0.0 H4CKs" << std::endl;
    //std::cout << "Press Numpad 0 to EXIT" << std::endl;
    //std::cout << "Press Numpad 1 to set Points" << std::endl;
    //while (true) {
    //    Sleep(50);
    //    if (GetAsyncKeyState(VK_NUMPAD0)) { // Exit
    //        return 0;
    //    }
    //    if (GetAsyncKeyState(VK_NUMPAD1)) {//Mouseposition
    //        std::cout << "How many points you want?" << std::endl;
    //        int newPoints = 0;
    //        std::cin >> newPoints;
    //        WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &newPoints, 4, 0);
    //    }
    //}

}