cd bin
protoc -I=..\my_protoc --cpp_out=..\..\..\src\gamesrc\protocgen ..\my_protoc\message.protoc
cd ..