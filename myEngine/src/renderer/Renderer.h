#pragma once

#include <vector>


namespace myEngine
{
    class RenderObject;
    
    class Renderer
    {
    public:
        static Renderer* getInstance();

        static void destroyInstance();
    
        bool init();
    
        void render();

        void setViewPort(int width, int height);

        void clearDepth() const;
        void clearColor() const;
        void clear() const;

        void pushRenderObject(const RenderObject* v);

    private:

        Renderer();
        ~Renderer();

        static Renderer* r;
        std::vector<const RenderObject*> m_RenderObjects;

        int m_viewport_width;
        int m_viewport_height;
    };
}
