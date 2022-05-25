#include "global.h"
#include "android_data/android_data.h"

bool GetGLError(int line, std::string file) {
    GLenum code = glGetError();
    if (code != GL_NO_ERROR) {
        std::stringstream stream;
        stream << "OpenGL error '" << std::to_string(code) << "' \nAt line: " << line << " \nIn file: " << file << endl;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",stream.str().c_str(),AndroidData::CurrentWindow());
        return true;
    }
    return false;
}

void ImGui::ScrollWhenDraggingOnVoid(const ImVec2& delta, ImGuiMouseButton mouse_button)  {
    ImGuiContext &g = *ImGui::GetCurrentContext();
    ImGuiWindow *window = g.CurrentWindow;
    bool hovered = false;
    bool held = false;
    ImGuiButtonFlags button_flags = (mouse_button == 0) ? ImGuiButtonFlags_MouseButtonLeft
                                                        : (mouse_button == 1)
                                                          ? ImGuiButtonFlags_MouseButtonRight
                                                          : ImGuiButtonFlags_MouseButtonMiddle;
    if (g.HoveredId ==
        0) // If nothing hovered so far in the frame (not same as IsAnyItemHovered()!)
        ImGui::ButtonBehavior(window->Rect(), window->GetID("##scrolldraggingoverlay"),
                              &hovered, &held, button_flags);
    if (held && delta.x != 0.0f)
        ImGui::SetScrollX(window, window->Scroll.x + delta.x);
    if (held && delta.y != 0.0f)
        ImGui::SetScrollY(window, window->Scroll.y + delta.y);
}