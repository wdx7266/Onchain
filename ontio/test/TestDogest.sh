cd $workspaceRoot/test &&
	g++ TestDigest.cpp $(pkg-config --cflags --libs openssl libcurl) -lgtest_main -lgtest -lpthread -std=c++11 -o ../bin/test.out &&
	../bin/test.out &&
	rm ../bin/test.out &&
	cd $workspaceRoot
