#include "includes.h"
#include "global.h"
#include "game.h"
#include "menu.h"

// This is our input handler where we can handle all of our
// Keyboard inputs and do some other funky things related.
LRESULT inputHandler(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	bool flag = (uMsg == WM_KEYUP && wParam == VK_HOME);
	if (flag) { global::menuOpen ^= 1; }

	if (global::menuOpen)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return TRUE; // Remove this line of code if you want to click behind menu
	}

	return CallWindowProcA(global::originalWndProc, hWnd, uMsg, wParam, lParam);
}

// This is our hook where everything rendering related happens.
HRESULT presentHooked(IDXGISwapChain* swapChain, UINT sync, UINT flag)
{
	if (!global::presentInitialized)
	{
		if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)&global::d3d11Device)))
		{
			global::d3d11Device->GetImmediateContext(&global::d3d11Context);
			
			DXGI_SWAP_CHAIN_DESC swapDesc;
			swapChain->GetDesc(&swapDesc);
			global::gameWindow = swapDesc.OutputWindow;

			ID3D11Texture2D* backBuffer;
			swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
			global::d3d11Device->CreateRenderTargetView(backBuffer, 0, &global::d3d11RenderTarget);
			backBuffer->Release();

			const auto index = GWLP_WNDPROC;
			const auto param = (LPARAM)inputHandler;
			const auto wndproc = (WNDPROC)SetWindowLongPtrA(global::gameWindow, index, param);
			global::originalWndProc = wndproc;
		
			ImGui::CreateContext();
			uiMenu::Styles();

			ImGui_ImplWin32_Init(global::gameWindow);
			ImGui_ImplDX11_Init(global::d3d11Device, global::d3d11Context);

			global::presentInitialized = true;
		}

		else
			return global::originalPresent(swapChain, sync, flag);
	}

	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().WantCaptureMouse = global::menuOpen;
	ImGui::GetIO().WantCaptureKeyboard = global::menuOpen;

	if (global::menuOpen)
		uiMenu::Render();

	uiMenu::Background();
	ImGui::Render();

	global::d3d11Context->OMSetRenderTargets(1, &global::d3d11RenderTarget, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return global::originalPresent(swapChain, sync, flag);
}

DWORD WINAPI MainThread()
{
	bool allocateDebugConsole = true;
	if (allocateDebugConsole)
	{
		// Debug console
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	}

	// Call function to initialize "project.dll" base address
	game::getDllBase();

	const char* moduleName = "GameOverlayRenderer64.dll";
	const char* instructionsPattern = "48 8B 05 ? ? ? ? FF D0 E9 ? ? ? ? 4C 8D 3D";
	auto overlayRenderer = memory::findPattern(moduleName, instructionsPattern);
	// 48 8B 05 63 D6 0B 00 -> mov rax, cs:qword_149BE0

	if (overlayRenderer <= 0)
	{
		MessageBoxA(0, "Failed to find signature.", "Error Hooking Steam", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Once we find the signature we need to "resolve it" because we want
	// to get the value of the qword_xxxxxx.
	overlayRenderer = *(int*)(overlayRenderer + 3) + overlayRenderer + 7;
	Present* steamPresent_orig = (Present*)overlayRenderer;

	global::originalPresent = *steamPresent_orig;
	// Here is the actual hook for the overlay
	// All that's going on is we are simply
	// Exchanging a pointer in memory (steams overlay)
	// And swapping it with our own function
	*steamPresent_orig = presentHooked;

	return TRUE;
}

BOOL WINAPI DllMain(HMODULE module, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		MainThread();
		break;
	}

	return TRUE;
}