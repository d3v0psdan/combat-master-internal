#pragma once
#include "includes.h"

namespace memory
{
    // This code is also from guidedhacking just re-written for more simplicity 
    // https://guidedhacking.com/threads/finddmaaddy-c-multilevel-pointer-function.6292/
	inline uint64_t readChain(uint64_t start, std::vector<uint32_t> offsets)
	{
		uint64_t result = *(uint64_t*)start;
		for (auto offset : offsets)
			result = *(uint64_t*)(result + offset);
		return result;
	}

    // This was not written by me, this code was just sourced off guidedhacking
    // So I do not take any credit for this code
    inline uint64_t findPattern(const char* dllname, const char* pattern)
    {
        static auto patternToByte = [](const char* pattern)
        {
            auto       bytes = std::vector<int>{};
            const auto start = const_cast<char*>(pattern);
            const auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current)
            {
                if (*current == '?')
                {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                }
                else { bytes.push_back(strtoull(current, &current, 16)); }
            }
            return bytes;
        };

        const auto dllBase = (uint64_t)GetModuleHandleA(dllname);
        const auto dosHeader = (PIMAGE_DOS_HEADER)dllBase;
        const auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t*)dllBase + dosHeader->e_lfanew);

        const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto       patternBytes = patternToByte(pattern);
        const auto scanBytes = reinterpret_cast<uint8_t*>(dllBase);

        const auto s = patternBytes.size();
        const auto d = patternBytes.data();

        for (auto i = 0ul; i < sizeOfImage - s; ++i)
        {
            bool found = true;
            for (auto j = 0ul; j < s; ++j)
            {
                if (scanBytes[i + j] != d[j] && d[j] != -1)
                {
                    found = false;
                    break;
                }
            }
            if (found) { return reinterpret_cast<uint64_t>(&scanBytes[i]); }
        }
        return 0ull;
    }
}