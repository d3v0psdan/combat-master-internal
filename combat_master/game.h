#pragma once
#include "includes.h"
#include "memory.h"

namespace game 
{
	using ByteVector = std::vector<unsigned int>;

	namespace Offset {
		constexpr auto infiniteWeaponPatch = 0x8BDD90; // mov [rdx + r8*8+20], edi
		constexpr auto weaponClass = 0x04CD0100;
		inline ByteVector weaponClassChain{ 0xB8, 0x8, 0x20, 0x68, 0x10, 0xA0 };
		constexpr auto weaponMagazine = 0x2C0;
		constexpr auto weaponGrenades = 0x248;
		constexpr auto settingsClass = 0x4CF4BF8;
		inline ByteVector settingsClassChain{ 0xB8, 0x0, 0x58 };
		constexpr auto settingsWorldFov = 0x50C;
		constexpr auto settingsHandsFov = 0x524;
	}

	uint64_t getDllBase();
	void isInGame(bool& value);
	void toggleInfiniteAmmo(bool value);
	void toggleInfiniteMag(bool value);
	void toggleInfiniteGrenades(bool value);
	void setWorldFov(float value);
	void setHandsFov(float value);
}