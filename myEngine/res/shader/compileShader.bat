shaderc -f common_vs.sc -i . -i ..\..\lib\bgfx.cmake\bgfx\src -o ..\shaderbin\common_vs.bin --varyingdef varying.def.sc --platform windows --type vertex -p 150 -O3
shaderc -f common_fs.sc -i . -i ..\..\lib\bgfx.cmake\bgfx\src -o ..\shaderbin\common_fs.bin --varyingdef varying.def.sc --platform windows --type fragment -p 150 -O3


shaderc -f pbr_gltf_vs.sc -o ..\shaderbin\pbr_gltf_vs.bin --varyingdef pbr_gltf_varying.def.sc --platform windows --type vertex -p 150 -O3
shaderc -f pbr_gltf_fs.sc -o ..\shaderbin\pbr_gltf_fs.bin --varyingdef pbr_gltf_varying.def.sc --platform windows --type fragment -p 150 -O3