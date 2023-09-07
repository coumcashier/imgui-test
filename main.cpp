// Dear ImGui: standalone example application for SDL3 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <string>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#include "my_widgets.h"
#include "colors.h"


#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

// This example doesn't compile with Emscripten yet! Awaiting SDL3 support.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif


ImVec4 RGBAtoIV4(int r, int g, int b, int a) {
    float newr = r / 255;
    float newg = g / 255;
    float newb = b / 255;
    float newa = a;
    return ImVec4(newr, newg, newb, newa);
}


void AddUnderLine(ImColor col_)
{
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    min.y = max.y;
    ImGui::GetWindowDrawList()->AddLine(
        min, max, col_, 1.0f);
}




// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0)
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Enable native IME.
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL3+OpenGL3 example", 500, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_ShowWindow(window);

    // Setup Dear//  ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
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

    ImFontConfig config;
    config.OversampleH = 4;
    config.OversampleV = 4;
    io.Fonts->AddFontDefault();
    auto font =          io.Fonts->AddFontFromFileTTF("data/font/Roboto/Roboto-Regular.ttf", 14.0f, &config);
    ImFont* font_h1 =    io.Fonts->AddFontFromFileTTF("data/font/Roboto/Roboto-Regular.ttf", 32.0f, &config);
    ImFont* font_input = io.Fonts->AddFontFromFileTTF("data/font/Roboto/Roboto-Regular.ttf", 16.0f, &config);
    IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // style no border
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 0.0f;
    //style.Colors[ImGuiCol_Border] = ImVec4();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = col_primary_selected;
    style.Colors[ImGuiCol_FrameBgHovered] =ImVec4(1.0f, 1.0f, 1.0f, 1.0f) ;
    style.Colors[ImGuiCol_ButtonHovered] = col_button_hover;
    style.Colors[ImGuiCol_ButtonActive] = col_button_active;
    style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    io.FontGlobalScale = 2;
    style.ScaleAllSizes(2);
    auto global_scale = 2;
    

    // Main loop
    bool done = false;
    auto text = std::string();
    text = "Placeholder text here";
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
	SDL_Event event;
    //while (!done && SDL_WaitEvent(&event))
    while (!done )
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

         // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4 * global_scale);
            ImGui::PushFont(font);
            ImGui::PushStyleColor(ImGuiCol_Border, border_color);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1 * global_scale);
            static float f = 0.0f;
            static int counter = 0;

            auto isOpen = true;
            ImGui::Begin("Hello, world!",  &isOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoDecoration );                          // Create a window called "Hello, world!" and append into it.
            auto size = ImVec2( io.DisplaySize.x, io.DisplaySize.y);
            ImGui::SetWindowSize(size);
            ImGui::SetWindowPos(ImVec2(0, 0));

            ImGui::PushFont(font_h1);
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::PopFont();

            auto window_size = ImGui::GetContentRegionAvail();
            ImGui::PushItemWidth(window_size.x);
            ImGui::PushFont(font_input);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 15) * global_scale);
            ImGui::Text("Username");               // Display some text (you can use a format strings too)
            ImGui::InputText("##username", &text, 0, NULL,  NULL);
            ImGui::Text("Password");               // Display some text (you can use a format strings too)
            ImGui::InputText("##password", &text, 0, NULL,  NULL);
            ImGui::PopStyleVar();
            ImGui::PopFont();
            ImGui::PopItemWidth();

            ImGui::CheckboxEx("Remember me", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Text("Register");
            AddUnderLine(ImVec4(0,0,0,1));

            ImGui::PushStyleColor(ImGuiCol_Button, col_button_bg);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(24, 12) * global_scale);
            if (ImGui::MyButtonEx("Login", ImVec2(window_size.x,0), ImGuiButtonFlags_None))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();

            //ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::PopStyleVar();
            ImGui::PopFont();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::End();
        }

  
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
