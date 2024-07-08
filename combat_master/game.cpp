#include "game.h"

namespace game
{
	static uint64_t projectDll = 0;

	uint64_t getDllBase()
	{
		if (!projectDll)
		{
			auto dllBase = (uint64_t)GetModuleHandleW(L"project.dll");;
			projectDll = dllBase;
		}

		return projectDll;
	}

	void isInGame(bool& value)
	{
		uint64_t address = *(uint64_t*)(projectDll + Offset::weaponClass);
		if (!address)
		{
			value = false;
			return;
		}

		address = *(uint64_t*)(address + 0xB8);
		if (address) 
		{
			address = *(uint64_t*)(address + 0x8);
			address ? value = true : value = false;
		}
		else { value = false; }
	}

	void toggleInfiniteAmmo(bool value)
	{
		static bool flag = false;
		
		DWORD old;
		BYTE patchBytes[5]{ 0x90,0x90,0x90,0x90,0x90 };
		BYTE originalBytes[5]{ 0x42, 0x89, 0x7C, 0xC2, 0x20 }; // mov [rdx + r8*8+20], edi
		PVOID patchAddress = (PVOID)(game::getDllBase() + Offset::infiniteWeaponPatch);

		if (value && flag)
		{
			// We need to change protections or else game crashes
			VirtualProtect(patchAddress, sizeof(originalBytes), PAGE_EXECUTE_READWRITE, &old);
			memcpy(patchAddress, patchBytes, sizeof(originalBytes));
			VirtualProtect(patchAddress, sizeof(originalBytes), old, &old);
			flag = false;
		}
		else if (!value && !flag)
		{
			VirtualProtect(patchAddress, sizeof(originalBytes), PAGE_EXECUTE_READWRITE, &old);
			memcpy(patchAddress, originalBytes, sizeof(originalBytes));
			VirtualProtect(patchAddress, sizeof(originalBytes), old, &old);
			flag = true;
		}
	}

	void toggleInfiniteMag(bool value)
	{
		auto base = projectDll + Offset::weaponClass;
		auto address = memory::readChain(base, Offset::weaponClassChain);

		if (address > 0 && value)
			*(uint64_t*)(address + Offset::weaponMagazine) = 0x1FF; // Maximum Amount of ammo game allows
	}

	uint64_t readChain(uint64_t start, std::vector<uint32_t> offsets)
	{
		uint64_t result = *(uint64_t*)start;
		for (auto offset : offsets)
			result = *(uint64_t*)(result + offset);
		return result;
	}

	void toggleInfiniteGrenades(bool value)
	{
		auto base = projectDll + Offset::weaponClass;
		auto classAddress = readChain(base, Offset::weaponClassChain);

		if (classAddress > 0 && value)
			*(uint64_t*)(classAddress + Offset::weaponGrenades) = 0x1F; // Maximum Amount of grenades game allows
	}

	void setWorldFov(float value)
	{
		auto base = projectDll + Offset::settingsClass;
		auto classAddress = readChain(base, Offset::settingsClassChain);

		if (classAddress > 0 && value > 0 && value < 180)
			*(float*)(classAddress + Offset::settingsWorldFov) = value;
	}

	void setHandsFov(float value)
	{
		/*
		v57 = sub_3E54A80(0i64, v48, v50);
        if ( !v57 )
            sub_52B1C0(v59, v58, v60);
        v61 = *(float *)(v57 + 0x524);

		----------------------------------------

		v17 = sub_3E54A80(0i64, a2, a3);
		v168 = *(_QWORD *)(v17 + 0x168);
		v44 = v168;

		v62 = *(_QWORD *)(v44 + 0x20);
		if ( !v62 )
			sub_52B1C0(v59, v58, v60);
		v63 = *(_QWORD *)(v62 + 0x48);
		if ( !v63 )
			sub_52B1C0(v59, v58, v60);
		*(float *)(v63 + 0x24) = Settings_hand_field_of_view;
		*/

		auto base = projectDll + Offset::settingsClass;
		auto classAddress = readChain(base, Offset::settingsClassChain);

		if (classAddress > 0 && value > 0 && value < 180)
			*(float*)(classAddress + Offset::settingsHandsFov) = value;
	}
}