rm -r ../build/ios
mkdir ../build/ios
cd ../build/ios
cmake ../../src/platform/ios/ -G Xcode
open .
cd ../../scriptAndTools