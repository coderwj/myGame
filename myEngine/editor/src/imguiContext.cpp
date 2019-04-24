#include "imguiContext.h"

#include "bgfx/embedded_shader.h"
#include "bx/allocator.h"
#include "bx/math.h"
#include "bx/timer.h"


#include "vs_ocornut_imgui.bin.h"
#include "fs_ocornut_imgui.bin.h"
#include "vs_imgui_image.bin.h"
#include "fs_imgui_image.bin.h"

#include "roboto_regular.ttf.h"


namespace myGame {
    
    static bx::DefaultAllocator allocator;
    static const float FONT_SIZE = 18.f;
    
    static void* memAlloc(size_t _size, void* _userData)
    {
        BX_UNUSED(_userData);
        return BX_ALLOC(&allocator, _size);
    }
    
    static void memFree(void* _ptr, void* _userData)
    {
        BX_UNUSED(_userData);
        BX_FREE(&allocator, _ptr);
    }
    
    static const bgfx::EmbeddedShader s_embeddedShaders[] =
    {
        BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
        BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
        BGFX_EMBEDDED_SHADER(vs_imgui_image),
        BGFX_EMBEDDED_SHADER(fs_imgui_image),
        BGFX_EMBEDDED_SHADER_END()
    };
    
    static bool checkAvailTransientBuffers(uint32_t _numVertices, const bgfx::VertexDecl& _decl, uint32_t _numIndices)
    {
        return _numVertices == bgfx::getAvailTransientVertexBuffer(_numVertices, _decl)
        && (0 == _numIndices || _numIndices == bgfx::getAvailTransientIndexBuffer(_numIndices));
    }
    
    void MyImguiContext::create()
    {
        m_lastScroll = 0;
        m_last = bx::getHPCounter();
        
        ImGui::SetAllocatorFunctions(memAlloc, memFree, NULL);
        
        m_imgui = ImGui::CreateContext();
        
        setupStyle(true);
        
        bgfx::RendererType::Enum type = bgfx::getRendererType();
        m_program = bgfx::createProgram(
                                        bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui")
                                        , bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui")
                                        , true
                                        );
        
        u_imageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
        m_imageProgram = bgfx::createProgram(
                                             bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image")
                                             , bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image")
                                             , true
                                             );
        
        m_decl
        .begin()
        .add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
        .end();
        
        s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);
        
        uint8_t* data;
        int32_t width;
        int32_t height;
        
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromMemoryTTF( (void*)s_robotoRegularTtf,     sizeof(s_robotoRegularTtf), FONT_SIZE);
        
        io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
        
        m_texture = bgfx::createTexture2D(
                                          (uint16_t)width
                                          , (uint16_t)height
                                          , false
                                          , 1
                                          , bgfx::TextureFormat::BGRA8
                                          , 0
                                          , bgfx::copy(data, width*height*4)
                                          );
    }
    
    
    void MyImguiContext::destroy()
    {
        ImGui::DestroyContext(m_imgui);
        
        bgfx::destroy(s_tex);
        bgfx::destroy(m_texture);
        
        bgfx::destroy(u_imageLodEnabled);
        bgfx::destroy(m_imageProgram);
        bgfx::destroy(m_program);
        
        m_allocator = NULL;
    }
    
    void MyImguiContext::beginFrame()
    {
        const int64_t now = bx::getHPCounter();
        const int64_t frameTime = now - m_last;
        m_last = now;
        const double freq = double(bx::getHPFrequency() );
        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = float(frameTime/freq);

        ImGui::NewFrame();
    }
    
    
    void MyImguiContext::render(ImDrawData* _drawData)
    {
        bgfx::setViewName(m_viewId, "ImGui");
        bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);
        
        const bgfx::Caps* caps = bgfx::getCaps();

        float ortho[16];
        bx::mtxOrtho(ortho, 0.f, m_width, m_height, 0.f, 0.f, 1000.0f, 0.0f, caps->homogeneousDepth);
        bgfx::setViewTransform(m_viewId, NULL, ortho);
        bgfx::setViewRect(m_viewId, 0, 0, uint16_t(m_width), uint16_t(m_height) );
        
        // Render command lists
        for (int32_t ii = 0, num = _drawData->CmdListsCount; ii < num; ++ii)
        {
            bgfx::TransientVertexBuffer tvb;
            bgfx::TransientIndexBuffer tib;
            
            const ImDrawList* drawList = _drawData->CmdLists[ii];
            uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
            uint32_t numIndices  = (uint32_t)drawList->IdxBuffer.size();
            
            if (!checkAvailTransientBuffers(numVertices, m_decl, numIndices) )
            {
                // not enough space in transient buffer just quit drawing the rest...
                break;
            }
            
            bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_decl);
            bgfx::allocTransientIndexBuffer(&tib, numIndices);
            
            ImDrawVert* verts = (ImDrawVert*)tvb.data;
            bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert) );
            
            ImDrawIdx* indices = (ImDrawIdx*)tib.data;
            bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx) );
            
            uint32_t offset = 0;
            for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
            {
                if (cmd->UserCallback)
                {
                    cmd->UserCallback(drawList, cmd);
                }
                else if (0 != cmd->ElemCount)
                {
                    uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;
                    
                    bgfx::TextureHandle th = m_texture;
                    bgfx::ProgramHandle program = m_program;
                    
                    if (NULL != cmd->TextureId)
                    {
                        union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; } texture = { cmd->TextureId };
                        state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
                        ? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
                        : BGFX_STATE_NONE
                        ;
                        th = texture.s.handle;
                        if (0 != texture.s.mip)
                        {
                            const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
                            bgfx::setUniform(u_imageLodEnabled, lodEnabled);
                            program = m_imageProgram;
                        }
                    }
                    else
                    {
                        state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
                    }
                    
                    const uint16_t xx = uint16_t(bx::max(cmd->ClipRect.x, 0.0f) );
                    const uint16_t yy = uint16_t(bx::max(cmd->ClipRect.y, 0.0f) );
                    bgfx::setScissor(xx, yy
                                     , uint16_t(bx::min(cmd->ClipRect.z, 65535.0f)-xx)
                                     , uint16_t(bx::min(cmd->ClipRect.w, 65535.0f)-yy)
                                     );
                    
                    bgfx::setState(state);
                    bgfx::setTexture(0, s_tex, th);
                    bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
                    bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
                    bgfx::submit(m_viewId, program);
                }
                
                offset += cmd->ElemCount;
            }
        }
    }
    
    void MyImguiContext::endFrame()
    {
        ImGui::Render();
        render(ImGui::GetDrawData());
    }
    
    
    void MyImguiContext::setupStyle(bool _dark)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.WindowBorderSize = 0.0f;
        if (_dark)
        {
            ImGui::StyleColorsDark(&style);
        }
        else
        {
            ImGui::StyleColorsLight(&style);
        }
    }

    
} // namespace myGame
