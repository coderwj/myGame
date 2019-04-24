#pragma once

#include "bgfx/bgfx.h"
#include "imgui.h"

#define IMGUI_MBUT_LEFT   0x01
#define IMGUI_MBUT_RIGHT  0x02
#define IMGUI_MBUT_MIDDLE 0x04
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)


namespace myGame{
    class MyImguiContext
    {
    public:
        bgfx::ViewId m_viewId;
        float m_width;
        float m_height;
        
    public:
        MyImguiContext()
        :m_viewId(1)
        ,m_width(1280.f)
        ,m_height(720.f)
        {
            
        }
        ~MyImguiContext() {}
        void create();
        void destroy();

        void beginFrame();
        void endFrame();
        
        void setupStyle(bool _dark);
        
        void onResize(int width, int height)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
        }
        
        void handleMouseWheel(int scroll_delta)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.MouseWheel = static_cast<float>(scroll_delta);
        }
        
        void handleMouseDown(int x, int y)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos.x = x;
            io.MousePos.y = y;
            io.MouseDown[0] = true;
        }
        
        void handleMouseDragged(int x, int y)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos.x = x;
            io.MousePos.y = y;
        }
        
        void handleMouseUp(int x, int y)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos.x = x;
            io.MousePos.y = y;
            io.MouseDown[0] = false;
        }
        
    private:
        void render(ImDrawData* _drawData);
    private:
        ImGuiContext*       m_imgui;
        bx::AllocatorI*     m_allocator;
        bgfx::VertexDecl    m_decl;
        bgfx::ProgramHandle m_program;
        bgfx::ProgramHandle m_imageProgram;
        bgfx::TextureHandle m_texture;
        bgfx::UniformHandle s_tex;
        bgfx::UniformHandle u_imageLodEnabled;
        int64_t m_last;
        int32_t m_lastScroll;
    };

} // namespace myGame
