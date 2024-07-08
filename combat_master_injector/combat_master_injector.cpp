#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>

void fatalError(std::string message)
{
	printf("[ERROR] %s\n", message.c_str());
	Sleep(5000); // Give 5 seconds to read message
	ExitProcess(0);
}

DWORD getPid(const char* processName)
{
	DWORD result = 0;
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!handle || handle == INVALID_HANDLE_VALUE)
		return 0;

	PROCESSENTRY32 pe32{ sizeof(PROCESSENTRY32) };
	Process32First(handle, &pe32);

	while(Process32Next(handle, &pe32))
		if (!strcmp(processName, pe32.szExeFile))
		{
			result = pe32.th32ProcessID;
			break;
		}

	CloseHandle(handle);
	return result;
}

int main()
{
	DWORD processID = 0;
	std::string dllPath = (std::filesystem::current_path() / "combat_master.dll").string();

	SetConsoleTitleA("Injector");
	printf("Searching for game process.\n");

	do { 
		processID = getPid("CombatMaster.exe");
	} while (!processID);

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (!processHandle || processHandle == INVALID_HANDLE_VALUE)
	{
		auto lastError = GetLastError();
		printf("GetLastError(): 0x%d\n", lastError);
		fatalError("Cannot obtain handle to process?");
		return 1;
	}

	auto protectType = PAGE_EXECUTE_READWRITE;
	auto allocType = MEM_COMMIT | MEM_RESERVE;
	auto allocSize = dllPath.length();

	PVOID pathAllocation = VirtualAllocEx(processHandle, 0, allocSize, allocType, protectType);
	if (!pathAllocation)
	{
		auto lastError = GetLastError();
		printf("GetLastError(): 0x%d\n", lastError);
		fatalError("Failed to create allocation for DLL path string");
		return 1;
	}

	// Write the path of the DLL inside of the process where we allocated memory
	// We write the path of our DLL inside of the process so when we call "LoadLibraryA"
	// Inside of the process, it will have the address of the string we are referencing
	// Which is our DLL.
	SIZE_T bytesWritten = 0;
	if (!WriteProcessMemory(processHandle, pathAllocation, dllPath.c_str(), allocSize, &bytesWritten) ||
		bytesWritten != allocSize)
	{
		auto lastError = GetLastError();
		printf("bytesWritten: 0x%llx\nallocSize: 0x%llx\nGetLastError(): 0x%d\n", bytesWritten, allocSize, lastError);
		fatalError("Writing process to memory was not successful.");
		return 1;
	}

	auto kernel32 = GetModuleHandleA("kernel32.dll");
	auto loadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32, "LoadLibraryA");

	PVOID remoteThread = CreateRemoteThread(processHandle, 0, 0, loadLibrary, pathAllocation, 0, 0);
	if (!remoteThread)
	{
		auto lastError = GetLastError();
		printf("GetLastError(): 0x%d\n", lastError);
		printf("loadLibrary: 0x%p\n", loadLibrary);
		printf("remoteThread: 0x%p\n", remoteThread);
		fatalError("Calling LoadLibraryA failed.");
		return 1;
	}

	printf("Waiting for module to execute entryPoint.\n");

	do {
		Sleep(1000);
	} while (WaitForSingleObject(remoteThread, INFINITE));
	
	// Cleanup 

	CloseHandle(remoteThread);
	VirtualFreeEx(processHandle, pathAllocation, allocSize, MEM_FREE | MEM_RELEASE);
	CloseHandle(processHandle);

	printf("Injection Successful\n");
	Sleep(2500);

	return 0;
}