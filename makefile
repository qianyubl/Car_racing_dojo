UTLIB = googletest/build/googlemock/libgmock_main.a\
        googletest/build/googlemock/libgmock.a\
        googletest/build/googlemock/gtest/libgtest.a

UTINC = -I./googletest/googlemock/include\
        -I./googletest/googletest/include\
        -I./Include

UTFLAG = -g -pthread -DUNIT_TEST -Wno-write-strings

all:
	g++ -std=c++14 -pthread -I./Include Source/*.cpp -o CarRacing.exe -g

ut:
	g++ -std=c++14 ${UTFLAG} ${UTINC} ${UTLIB} Source/*.cpp Test_modules/*.cpp -o ut_CarRacing.exe

clean:
	rm -rf *.o
	rm -rf Test_modules/*.o
	rm -rf *.exe
