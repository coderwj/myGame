#include "Renderer.h"

#include "RenderObject.h"
#include "Camera.h"
#include "Engine.h"
#include "config.h"
#include "HelperFunc.h"

#include "bgfx/bgfx.h"
#include "bx/math.h"

namespace myEngine
{
    Renderer* Renderer::r = NULL;

    Renderer* Renderer::getInstance()
    {
        if (r == NULL)
            r = new Renderer();
        return r;
    }
    void Renderer::destroyInstance() {
        if (r)
        {
            delete(r);
            r = NULL;
        }
    }
    Renderer::Renderer()
    :m_viewport_width(1280)
    ,m_viewport_height(720)
    {

    }
    Renderer::~Renderer()
    {
    }
    bool Renderer::init()
    {
        bgfx::Init init;
        //init.type = bgfx::RendererType::OpenGLES;
        //init.type = bgfx::RendererType::Count;
        init.type = bgfx::RendererType::OpenGL;
        init.vendorId = BGFX_PCI_ID_NONE; //auto select
        init.resolution.width = m_viewport_width;
        init.resolution.height = m_viewport_height;
        init.resolution.reset = BGFX_RESET_VSYNC;
        bgfx::init(init);

        //uint32_t flag = BGFX_DEBUG_WIREFRAME | BGFX_DEBUG_TEXT | BGFX_DEBUG_PROFILER | BGFX_DEBUG_STATS;
        uint32_t flag = BGFX_DEBUG_TEXT | BGFX_DEBUG_PROFILER;
        bgfx::setDebug(flag);

        clear();
        bgfx::frame();

        return true;
    }
    
    void Renderer::render()
    {
        clear();
        //bgfx::touch(0);
        bgfx::setState(BGFX_STATE_DEFAULT);

        myEngine::Camera* _camera = myEngine::Engine::getInstance()->getMaincCamera();
        const Matrix4& _view = _camera->GetViewMatrix();
        const Matrix4& _projection = _camera->GetProjectMatrix();
        bgfx::setViewTransform(0, static_cast<const void*>(&_view), static_cast<const void*>(&_projection));

        bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(m_viewport_width), static_cast<uint16_t>(m_viewport_height));

        for (const RenderObject* r : m_RenderObjects)
        {
            r->draw();
        }
        m_RenderObjects.clear();

        bgfx::frame();
    }
    void Renderer::setViewPort(int width, int height)
    {
        m_viewport_width = width;
        m_viewport_height = height;
        //bgfx::setViewRect(0, 0, 0, width, height);
    }
    void Renderer::clearDepth() const
{
        bgfx::setViewClear(0, BGFX_CLEAR_DEPTH, 0xaaaaaaff, 1.0f, 0);
    }
    void Renderer::clearColor() const
{
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0xaaaaaaff, 1.0f, 0);
    }
    void Renderer::clear() const
{
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xaaaaaaff, 1.0f, 0);
    }
    void Renderer::pushRenderObject(const RenderObject * v)
    {
        m_RenderObjects.push_back(v);
    }
}
