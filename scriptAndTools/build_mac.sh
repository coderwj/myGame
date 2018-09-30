rm -r ../build/mac
mkdir ../build/mac

cd ./tolua
#sh build.sh
cd ..

cd ../build/mac
cmake ../../ -G Xcode
cd ../../scriptAndTools
