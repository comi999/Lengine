#include "IO/IOSystem.hpp"
#include "IO/FileSystem/PlatformFileSystem.hpp"
#if IS_EDITOR
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <Editor/Editor.hpp>
#include <Application/Application.hpp>

Engine* Editor::GetEngine() const { return m_Application ? m_Application->GetEngine() : nullptr; }
Client* Editor::GetClient() const { return m_Application ? m_Application->GetClient() : nullptr; }

struct EditorInfo
{
	GLFWwindow* Window = nullptr;
    ImGuiIO* IO = nullptr;
    bool ShowDemoWindow = true;
    bool ShowAnotherWindow = false;
	ImVec4 ClearColour = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

struct EditorWindowContext
{
	const char* Name = "";
};

struct EditorWindow
{
	EditorWindow( EditorWindowContext& a_Context )
	{
		
	}
};

struct EditorToolbar : EditorWindow
{
	EditorToolbar( EditorWindowContext& a_Context )
		: EditorWindow( a_Context )
	{
		a_Context.Name = "Toolbar";
	}
};

struct EditorContentBrowser : EditorWindow
{
	EditorContentBrowser( EditorWindowContext& a_Context )
		: EditorWindow( a_Context )
	{
		a_Context.Name = "Content Browser";
	}
};

struct EditorSceneViewer : EditorWindow
{
	EditorSceneViewer( EditorWindowContext& a_Context )
		: EditorWindow( a_Context )
	{
		a_Context.Name = "Scene Viewer";
	}
};

struct EditorInspector : EditorWindow
{
	EditorInspector( EditorWindowContext& a_Context )
		: EditorWindow( a_Context )
	{
		a_Context.Name = "Inspector";
	}
};

struct EditorViewport : EditorWindow
{
	EditorViewport( EditorWindowContext& a_Context )
		: EditorWindow( a_Context )
	{
		a_Context.Name = "Viewport";
	}
};

void Editor::OnCreate()
{
    m_Info = std::make_shared< EditorInfo >();

    //glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    m_Info->Window = glfwCreateWindow( 1280, 720, GetApplication()->GetName().c_str(), nullptr, nullptr);
    if ( m_Info->Window == nullptr )
        return;
    
    glfwMakeContextCurrent( m_Info->Window );
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_Info->IO = &ImGui::GetIO();
    m_Info->IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    m_Info->IO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL( m_Info->Window, true );
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
#define IMGUI_CONFIG_PATH PATH_CONFIGS "/imgui.ini"
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    PlatformFileSystem::CreateDirectories( PATH_CONFIGS );
    io.IniFilename = IMGUI_CONFIG_PATH;
}

void Editor::OnDestroy()
{
	// Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow( m_Info->Window );
    glfwTerminate();
    m_Info.reset();
}

//--------------------------------------------------------------
void dockingReset1()
{
    if ( ImGui::BeginMainMenuBar() )
	{
		if ( ImGui::BeginMenu( "Add Actor" ) )
		{

			if ( ImGui::MenuItem( "Light" ) )
			{
				//NewActor = new ALight();
			}

			if ( ImGui::MenuItem( "Static Mesh" ) )
			{
				
			}

			if ( ImGui::MenuItem( "Camera" ) )
			{
				
			}


			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

    ImGuiWindowFlags Window_Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowPos( ImVec2( 0, 20 ) );
	ImGui::SetNextWindowSize( ImVec2( 1280, 720 ) );
	if ( ImGui::Begin( "Editor", nullptr, Window_Flags ) )
	{
		ImGuiID idWind = ImGui::GetID( "Editor" );

		ImGui::DockSpace( idWind, ImVec2( 0, 0 ) );
	}
	ImGui::End();

	ImGui::Begin("inspector");
        if ( ImGui::TreeNode("Stats") )
        {
            ImGui::Text("Val %i", 3);
	        ImGui::TreePop();
        }
    ImGui::End();

	ImGui::Begin("inspector2");
        if ( ImGui::TreeNode("Stats") )
        {
            ImGui::Text("Val %i", 3);
	        ImGui::TreePop();
        }
    ImGui::End();


    







	//ImGuiViewport* viewport = ImGui::GetMainViewport();


	//ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//ImGuiID dockspace_id = ImGui::GetID("DockSpace");


	//static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    

	//ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
	//ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
	//ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);


	//// split the dockspace into 2 nodes --
	//// DockBuilderSplitNode takes in the following args in the following order
	////   window ID to split, direction, fraction (between 0 and 1),
	//// the final two setting let's us choose which id we want (which ever one we DON'T set as NULL,
	//// will be returned by the function)
	//// out_id_at_dir is the id of the node in the direction we specified earlier,
	//// out_id_at_opposite_dir is in the opposite direction
	//auto dock_id_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2f, nullptr, &dockspace_id);
	//auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
	//auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
	//auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.15f, nullptr, &dockspace_id);
	////auto dock_id_left2 = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.2f, nullptr, &dock_id_left);
	////auto dock_id_down2 = ImGui::DockBuilderSplitNode(dock_id_down, ImGuiDir_Right, 0.15f, nullptr, &dock_id_down);


	//// we now dock our windows into the docking node we made above


	//ImGui::DockBuilderDockWindow("Window 1", dock_id_top);
	//ImGui::DockBuilderDockWindow("Window 2", dock_id_right);
	//ImGui::DockBuilderDockWindow("Window 3", dock_id_left);
	//ImGui::DockBuilderDockWindow("Window 4", dock_id_down);
	//ImGui::DockBuilderDockWindow("Window 0", dock_id_top);


	//ImGui::DockBuilderFinish(dockspace_id);
}

void Editor::OnUpdate( const float a_DeltaTime )
{
    if ( glfwWindowShouldClose( m_Info->Window ) )
    {
	    m_Application->Quit();
        return;
    }

	// Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();
    
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    dockingReset1();

    //ImGui::Begin("Hi");
    //ImGui::End();
    
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   /* if ( m_Info->ShowDemoWindow )
        ImGui::ShowDemoWindow( &m_Info->ShowDemoWindow );*/
    
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;
    //
    //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
    //
    //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //    ImGui::Checkbox("Demo Window", &m_Info->ShowDemoWindow);      // Edit bools storing our window open/close state
    //    ImGui::Checkbox("Another Window", &m_Info->ShowAnotherWindow);
    //
    //    ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );            // Edit 1 float using a slider from 0.0f to 1.0f
    //    ImGui::ColorEdit3( "clear color", ( float* )&m_Info->ClearColour ); // Edit 3 floats representing a color
    //
    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);
    //
    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_Info->IO->Framerate, m_Info->IO->Framerate);
    //    ImGui::End();
    //}
    
    // 3. Show another simple window.
    //if ( m_Info->ShowAnotherWindow )
    //{
    //    ImGui::Begin( "Another Window", &m_Info->ShowAnotherWindow );   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //    ImGui::Text("Hello from another window!");
    //    if (ImGui::Button("Close Me"))
    //        m_Info->ShowAnotherWindow = false;
    //    ImGui::End();
    //}
    
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize( m_Info->Window, &display_w, &display_h );
    glViewport(0, 0, display_w, display_h);
    glClearColor( m_Info->ClearColour.x * m_Info->ClearColour.w, m_Info->ClearColour.y * m_Info->ClearColour.w, m_Info->ClearColour.z * m_Info->ClearColour.w, m_Info->ClearColour.w );
    glClear( GL_COLOR_BUFFER_BIT );
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    glfwSwapBuffers( m_Info->Window );
}
#endif // IS_EDITOR