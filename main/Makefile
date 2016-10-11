
main:main.cpp sfm.h Makefile
	g++ -o main main.cpp -Lurg_cpp.a `pkg-config --cflags opencv` `pkg-config --libs opencv` 

sfm:sfm.cpp Makefile
	g++ -o sfm sfm.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`

calibration_opencv2:calibration_opencv2.cpp Makefile
	g++ -o calibration_opencv2 calibration_opencv2.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`


test:test.cpp Makefile
	g++ -o test test.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`


moter:moter.cpp Makefile
	g++ -o moter moter.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`



get_distance:moter.cpp Makefile
	g++ -o moter moter.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`


testmoter:testmoter.cpp Makefile
	g++ -o testmoter testmoter.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`

