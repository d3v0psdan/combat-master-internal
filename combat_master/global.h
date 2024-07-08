#pragma once

namespace global
{
	inline bool presentInitialized{ false };
	inline bool menuOpen{ false };
	
	inline bool infiniteAmmo{ false };
	inline bool infiniteGrenades{ false };
	inline bool infiniteMag{ false };
	inline bool modifyWorldFov{ false };
	inline bool modifyHandsFov{ false };
	inline float worldFov{ 90 };
	inline float handsFov{ 80 };

	inline Present originalPresent;
	inline HWND gameWindow = nullptr;
	inline WNDPROC originalWndProc{ nullptr };
	inline ID3D11Device* d3d11Device{ nullptr };
	inline ID3D11DeviceContext* d3d11Context{ nullptr };
	inline ID3D11RenderTargetView* d3d11RenderTarget{ nullptr };
}