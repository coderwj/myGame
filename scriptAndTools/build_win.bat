rd ../build/win /q /s
mkdir ../build/win

cd ./tolua
call build.bat
cd ..

cd ../build/win
cmake ../../
cd ../../scriptAndTools
