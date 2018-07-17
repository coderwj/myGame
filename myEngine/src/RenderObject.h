#pragma once

namespace myEngine
{
	class Material;

	class RenderObject
	{
	private:
		Material*	m_material;
		void*		m_vertexBuffer;
	public:
		RenderObject();
		~RenderObject();
	
		void draw();
	};
}
