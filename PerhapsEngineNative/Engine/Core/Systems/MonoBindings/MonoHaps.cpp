#include "MonoHaps.h"
#include "MonoGui.h"
#include "../../Application.h"


namespace Perhaps
{
	MonoDomain* MonoHaps::mainDomain = nullptr;
	MonoAssembly* MonoHaps::engineAssembly = nullptr;
	MonoObject* MonoHaps::engineObject = nullptr;
	MonoMethod* MonoHaps::updateMethod = nullptr;
	MonoMethod* MonoHaps::guiUpdateMethod = nullptr;

	void MonoHaps::InitializeMono()
	{
		mono_set_dirs("C:\\Program Files (x86)\\Mono\\lib", "C:\\Program Files (x86)\\Mono\\etc");

		const char* options[]
		{
			"--debugger-agent=transport=dt_socket,server=y,suspend=n,timeout=20000,address=127.0.0.1:24040 --debug",
		};
		mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		mono_jit_parse_options(1, (char**)options);

		const char* runtime_version = "v4.0.30319";
		conlog("awaiting managed debugger.");
		mainDomain = mono_jit_init_version("Perhaps.Domain", runtime_version);
		mono_thread_set_main(mono_thread_current());

		//std::string path = std::filesystem::current_path().string();
		//path = path + "\\Build-Debug-Bin\\" + editorDll;
		std::string path = "D:\\Dev\\Dev\\PerhapsEngine\\PerhapsEngineNative\\Build-Debug-Bin\\";

		const char* editorDll = "PerhapsEngineEditor.dll";
		path = path + editorDll;
		conlog(path);

		engineAssembly = mono_domain_assembly_open(mainDomain, path.c_str());

		if (!engineAssembly)
		{
			conlog("Failed to open domain assembly.");
		}
		else
		{
			int argc = 1;
			char* argv[1] = { (char*)"TestAssembly" };

			MonoImage* image = mono_assembly_get_image(engineAssembly);
			MonoClass* engineClass = mono_class_from_name(image, "Perhaps.Engine.Editor", "EditorApplication");
			engineObject = mono_object_new(mainDomain, engineClass);
			MonoMethod* constructorMethod = mono_class_get_method_from_name(engineClass, ".ctor", 1);

			Application* app = Application::GetInstance();
			void* args[1];
			args[0] = &app;

			mono_runtime_invoke(constructorMethod, engineObject, args, NULL);

			updateMethod = mono_class_get_method_from_name(engineClass, "Update", 0);
			guiUpdateMethod = mono_class_get_method_from_name(engineClass, "OnEditorGuiRender", 1);

		}

		EventDispatcher::Subscribe(ImGuiRenderEvent::descriptor, UpdateManagedGui);
	}

	void MonoHaps::UpdateManaged()
	{
		mono_runtime_invoke(updateMethod, engineObject, NULL, NULL);
	}

	void MonoHaps::UpdateManagedGui(const Event& e)
	{
		ImGuiRenderEvent& renderEvent = (ImGuiRenderEvent&)e;

		void* args[1];
		args[0] = &renderEvent.rt;

		mono_runtime_invoke(guiUpdateMethod, engineObject, args, NULL);
	}
}