#include "Material.h"

#include "config.h"
#include "HelperFunc.h"

namespace myEngine
{
	Material::Material()
	{
	}

	Material::~Material()
	{
		bgfx::destroy(m_program_handle);
	}

	void Material::setProgram(const bgfx::ProgramHandle& _program_handle)
	{
		m_program_handle = _program_handle;
	}

	void Material::setProgram(const char* vs_name, const char* fs_name)
	{
		string vs_path = Config::shader_bin_path + vs_name;
		int vs_bin_size = HelperFunc::getFileSize(vs_path.c_str());
		char * vs_bin = new char[vs_bin_size];
		HelperFunc::LoadFromFile(vs_path.c_str(), vs_bin, vs_bin_size);
		const bgfx::Memory* _vs_men = bgfx::makeRef(vs_bin, vs_bin_size);
		bgfx::ShaderHandle _vsh = bgfx::createShader(_vs_men);

		string fs_path = Config::shader_bin_path + fs_name;
		int fs_bin_size = HelperFunc::getFileSize(fs_path.c_str());
		char * fs_bin = new char[fs_bin_size];
		HelperFunc::LoadFromFile(fs_path.c_str(), fs_bin, fs_bin_size);
		const bgfx::Memory* _fs_men = bgfx::makeRef(fs_bin, fs_bin_size);
		bgfx::ShaderHandle _fsh = bgfx::createShader(_fs_men);

		m_program_handle = bgfx::createProgram(_vsh, _fsh, true);
	}

	const bgfx::ProgramHandle & Material::getProgram()
	{
		return m_program_handle;
	}

}
