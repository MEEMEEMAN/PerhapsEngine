#include "RendererImGui.h"
#include "../../../Application.h"
#include "../../EventSystem/EventSystem.h"
#include "../../EventSystem/EngineEvents.h"
#include "../../Tools/Utils.h"

namespace Perhaps
{
	void RendererImGui::Initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		Window* window = Application::GetInstance()->GetWindow();
		ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);

		const char* glsl_version = "#version 430 core";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void RendererImGui::Render(RenderTexture& gameRender)
	{
		RenderTexture::Unbind();
		Graphics::SetClearColor(Color(0, 0, 0, 0));
		Graphics::Clear(Graphics::ColorMask::COLOR);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuiIO& io = ImGui::GetIO();

		/* ^^^ Fullscreen docking setup  ^^^ */
		static bool p_open = true;
		SetupDockspace(&p_open);

		/* vvv Start rendering app vvv */
		if (p_open)
		{
			ImGuiRenderEvent renderEvent;
			renderEvent.rt = &gameRender;
			EventDispatcher::DispatchEvent(renderEvent);
			ImGui::End();
		}


		/*
		
		static bool styleEditor = false;
		static bool showScene = true;

		static bool buildWindow = false;
		if (ImGui::BeginMenuBar())
		{

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Style Editor"))
				{
					styleEditor = !styleEditor;
				}

				if (ImGui::MenuItem("Scene Window"))
				{
					showScene = !showScene;
				}

				if (ImGui::MenuItem("Build Window"))
				{
					buildWindow = !buildWindow;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (showScene)
		{
			static ImGuiWindowFlags sceneWindowFlags = ImGuiDockNodeFlags_None;
			sceneWindowFlags |= ImGuiWindowFlags_NoCollapse;

			static bool showScene = true;
			if (ImGui::Begin("Game  Scene", &showScene, sceneWindowFlags))
			{
				bool docked = ImGui::IsWindowDocked();
				ImGui::BeginGroup();

				if (ImGui::Button("Enter Play Mode"))
				{

				}

				ImGui::SameLine();

				if (!docked && ImGui::Button("Resize to aspect"))
				{
					glm::vec2 dimensions = gameRender.GetDimensions();
					float aspect = dimensions.x / dimensions.y;
					ImVec2 wsize = ImGui::GetWindowSize();
					if (wsize.x > wsize.y)
					{
						wsize.y = wsize.x / aspect;
					}
					else
					{
						wsize.x = wsize.y * aspect;
					}

					ImGui::SetWindowSize(wsize);
				}

				ImGui::EndGroup();

				if (ImGui::BeginChild("Game Render"))
				{
					ImVec2 wsize = ImGui::GetWindowSize();

					ImGui::Image((ImTextureID)gameRender.GetColorAttachment()->GetId(), wsize, ImVec2(0, 1), ImVec2(1, 0));
					ImGui::EndChild();
				}

				ImGui::End();
			}
		}

		if (styleEditor && ImGui::Begin("Editor Styles"))
		{
			ImGui::ShowStyleEditor();
			ImGui::End();
		}


		if (ImGui::Begin("FileSystem"))
		{
			std::string root = "D:\\Dev\\Dev\\PerhapsEngine\\PerhapsEngineNative\\Build-Debug-Bin";
			ImGui::Text(root.c_str());

			if (ImGui::TreeNode("File view"))
			{
				const int max_columns = 2;

				std::vector<std::string> files =
				{
					"hello.h",
					"hello.cpp",
					"vertex.shader",
					"fragment.shader",
					"shrek.png"
				};
				static int selectedIndex = -1;

				ImGui::Columns(max_columns, NULL, true);
				for (size_t i = 0; i < files.size(); i++)
				{

					ImGui::PushID(i);

					const char* file = files[i].c_str();
					if (ImGui::Selectable(file, selectedIndex == i))
					{
						selectedIndex = i;
						ImGui::CloseCurrentPopup();
					}

					if (ImGui::IsItemClicked(1))
					{
						ImGui::OpenPopup("file_popup");
					}

					if (ImGui::BeginPopup("file_popup"))
					{
						ImGui::MenuItem(file, NULL, false, false);

						if (ImGui::MenuItem("Rename"))
						{

						}

						if (ImGui::MenuItem("Delete"))
						{

						}

						ImGui::EndPopup();
					}

					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("sa", NULL, NULL);
						ImGui::Text(file);

						ImGui::EndDragDropSource();
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("sa"))
						{

						}

						ImGui::EndDragDropTarget();
					}

					ImGui::PopID();

					ImGui::NextColumn();
				}
				ImGui::Columns(1);
				ImGui::TreePop();
			}

			ImGui::End();
		}

		if (ImGui::Begin("Scene Hierarchy"))
		{
			ImGui::End();
		}

		if (ImGui::Begin("Inspector"))
		{
			ImGui::End();
		}
		*/
		/* ^^^ End rendering app ^^^ */

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		ImGui::EndFrame();
	}

	void RendererImGui::SetupDockspace(bool* p_open)
	{
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
	}

	void RendererImGui::Cleanup()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}