rm -r ../build/ios
mkdir ../build/ios
cd ../build/ios
cmake ../../ -G Xcode -DIOS=true
cd ../../scriptAndTools