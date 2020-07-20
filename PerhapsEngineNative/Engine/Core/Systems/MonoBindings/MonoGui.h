#ifndef PERHAPS_MONO_GUI
#define PERHAPS_MONO_GUI
#include "../../PerhapsPch.h"

namespace Perhaps
{

	PAPI bool PBegin(const char* name, bool* open, ImGuiWindowFlags flags)
	{
		return ImGui::Begin(name, open, flags);
	}
	PAPI void PEnd()
	{
		ImGui::End();
	}
	PAPI bool PBeginMenuBar()
	{
		return ImGui::BeginMenuBar();
	}
	PAPI void PEndMenuBar()
	{
		ImGui::EndMenuBar();
	}
	PAPI bool PBeginMenu(const char* name)
	{
		return ImGui::BeginMenu(name);
	}
	PAPI void PEndMenu()
	{
		ImGui::EndMenu();
	}
	PAPI bool PMenuItem(const char* name)
	{
		return ImGui::MenuItem(name);
	}

	PAPI bool PBeginChild(const char* name, ImGuiWindowFlags flags, glm::vec2 size, bool border)
	{
		ImVec2 imSize(size.x, size.y);
		return ImGui::BeginChild(name, imSize, border, flags);
	}
	PAPI void PEndChild()
	{
		ImGui::EndChild();
	}

	PAPI void PImage(Texture2D* tex, const glm::vec2& size, const glm::vec2& uv0, const glm::vec2& uv1)
	{
		ImGui::Image((ImTextureID)tex->GetId(), ImVec2(size.x, size.y), ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y));
	}
	

	PAPI void PGetWindowSize(glm::vec2* dims)
	{
		ImVec2 size = ImGui::GetWindowSize();
		glm::vec2 sizeVec2 = glm::vec2(size.x, size.y);
		*dims = sizeVec2;
	}

	PAPI bool PIsWindowDocked()
	{
		return ImGui::IsWindowDocked();
	}

	PAPI bool PButton(const char* name)
	{
		return ImGui::Button(name);
	}

	PAPI void PSameLine()
	{
		ImGui::SameLine();
	}

	PAPI void PSetWindowSize(glm::vec2 size)
	{
		ImVec2 imSize = ImVec2(size.x, size.y);
		ImGui::SetWindowSize(imSize);
	}

	PAPI void PBeginGroup()
	{
		ImGui::BeginGroup();
	}

	PAPI void PEndGroup()
	{
		ImGui::EndGroup();
	}
}

#endif