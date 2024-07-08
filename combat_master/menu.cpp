#include "menu.h"
#include "game.h"
#include "global.h"

void uiMenu::Styles()
{
	auto& io = ImGui::GetIO();
	io.LogFilename = nullptr;
	io.IniFilename = nullptr;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14.0f);

	auto& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.WindowMinSize = ImVec2(310.0f, 210.0f);

	colors[ImGuiCol_TitleBg] = ImColor(30, 30, 30, 150);
	colors[ImGuiCol_TitleBgActive] = ImColor(30, 30, 30, 150);
	colors[ImGuiCol_TitleBgCollapsed] = ImColor(30, 30, 30, 100);
	colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
	colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 0);
	colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 0);
	colors[ImGuiCol_SliderGrab] = ImColor(249, 149, 79, 255);
	colors[ImGuiCol_SliderGrabActive] = ImColor(249, 149, 79, 255);
	colors[ImGuiCol_CheckMark] = ImColor(249, 149, 79, 255);
	colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30, 255);
	colors[ImGuiCol_FrameBgActive] = ImColor(35, 35, 35, 255);
	colors[ImGuiCol_FrameBgHovered] = ImColor(35, 35, 35, 255);
	colors[ImGuiCol_Border] = ImColor(0, 0, 0, 255);
}

void uiMenu::Render()
{
	ImGui::Begin("d3v0psdan");

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
	ImGui::Checkbox("Infinite Ammo (Doesn't work online)", &global::infiniteAmmo);
	ImGui::Checkbox("Infinite Magazine", &global::infiniteMag);
	ImGui::Checkbox("Infinite Grenades", &global::infiniteGrenades);
	ImGui::Checkbox("Custom world field of view", &global::modifyWorldFov);
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
	if (global::modifyWorldFov)
		ImGui::SliderFloat("##world_slider_float", &global::worldFov, 10.0f, 180.0f, "%.4f World field of view");
	ImGui::Checkbox("Custom hands field of view", &global::modifyHandsFov);
	if (global::modifyWorldFov)
		ImGui::SliderFloat("##hands_slider_float", &global::handsFov, 10.0f, 180.0f, "%.4f Hands field of view");
	ImGui::PopItemWidth();
	ImGui::PopStyleVar();
	ImGui::End();
}

void uiMenu::Background()
{
	auto draw = ImGui::GetBackgroundDrawList();

	bool inGame{false};
	game::isInGame(inGame);

	auto inGameText = (inGame ? "[Active]" : "[Disabled]");
	auto textColor = ImColor(inGame ? ImColor(0, 255, 0, 255).Value : ImColor(255, 0, 0, 255).Value);

	draw->AddText(ImVec2{ 10,10 }, textColor, inGameText);
	if (!inGame) return;

	game::toggleInfiniteAmmo(global::infiniteAmmo);
	game::toggleInfiniteMag(global::infiniteMag);
	game::toggleInfiniteGrenades(global::infiniteGrenades);
	game::setWorldFov(global::worldFov);
	game::setHandsFov(global::handsFov);
}