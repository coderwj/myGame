:: shaderc -f common_vs.sc -i . -i ..\..\lib\bgfx.cmake\bgfx\src -o ..\shaderbin\common_vs.bin --varyingdef varying.def.sc --platform windows --type vertex -p 150 -O3
:: shaderc -f common_fs.sc -i . -i ..\..\lib\bgfx.cmake\bgfx\src -o ..\shaderbin\common_fs.bin --varyingdef varying.def.sc --platform windows --type fragment -p 150 -O3
set def_v=HAS_NORMALS;HAS_TANGENTS;HAS_UV
set def_f=HAS_BASECOLORMAP;HAS_TANGENTS;HAS_NORMALS;HAS_NORMALMAP;HAS_EMISSIVEMAP;HAS_METALROUGHNESSMAP

rem shaderc -f pbr_gltf_vs.sc -o ..\shaderbin\pbr_gltf_vs.bin --define %def_v% --varyingdef pbr_gltf_varying.def.sc --platform windows --type vertex  -p vs_4_0 -O 3
rem shaderc -f pbr_gltf_fs.sc -o ..\shaderbin\pbr_gltf_fs.bin --define %def_f% --varyingdef pbr_gltf_varying.def.sc --platform windows --type fragment  -p ps_4_0 -O 3

shaderc -f pbr_gltf_vs.sc -o ..\shaderbin\pbr_gltf_vs.bin --define %def_v% --varyingdef pbr_gltf_varying.def.sc --platform windows --type vertex  -p 150 -O 3
shaderc -f pbr_gltf_fs.sc -o ..\shaderbin\pbr_gltf_fs.bin --define %def_f% --varyingdef pbr_gltf_varying.def.sc --platform windows --type fragment  -p 150 -O 3



rem shaderc -f simple_vs.sc -o ..\shaderbin\simple_vs.bin --varyingdef simple_varying.def.sc --platform windows --type vertex  -p vs_4_0 -O 3
rem shaderc -f simple_fs.sc -o ..\shaderbin\simple_fs.bin --varyingdef simple_varying.def.sc --platform windows --type fragment  -p ps_4_0 -O 3

shaderc -f simple_vs.sc -o ..\shaderbin\simple_vs.bin --varyingdef simple_varying.def.sc --platform windows --type vertex  -p 120 -O 3
shaderc -f simple_fs.sc -o ..\shaderbin\simple_fs.bin --varyingdef simple_varying.def.sc --platform windows --type fragment  -p 120 -O 3