#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

typedef HRESULT(__stdcall* Present) (IDXGISwapChain*,UINT,UINT);
typedef LRESULT(__stdcall* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND,UINT,WPARAM,LPARAM);
