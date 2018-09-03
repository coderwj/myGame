#pragma once

#include "bgfx/bgfx.h"

namespace myEngine
{
	class Material
	{
	public:
		Material();
		~Material();

		void setProgram(const bgfx::ProgramHandle& _program_handle);
		void setProgram(const char * vs_name, const char * fs_name);
		const bgfx::ProgramHandle& getProgram();

	private:
		bgfx::ProgramHandle m_program_handle;
	};
}

