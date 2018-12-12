Main: aaMain.cpp GPSInterp.o ModGPS.o Gps.o GGA.o RMC.o MergeTime.o LatLongToUtm.o
	g++ -std=c++11 -o lidar2PCD aaMain.cpp GPSInterp.o ModGPS.o MergeTime.o Gps.o GGA.o RMC.o LatLongToUtm.o



GPSInterp.o: GPSInterp.cpp 
	g++ -std=c++11 -c GPSInterp.cpp

ModGPS.o: ModGPS.cpp
	g++ -std=c++11 -c ModGPS.cpp

RMC.o: RMC.cpp
	g++ -std=c++11 -c RMC.cpp

GGA.o: GGA.cpp
	g++ -std=c++11 -c GGA.cpp

Gps.o: Gps.cpp
	g++ -std=c++11 -c Gps.cpp

MergeTime.o: MergeTime.cpp
	g++ -std=c++11 -c MergeTime.cpp

LatLongToUtm.o: LatLongToUtm.cpp
	g++ -std=c++11 -c LatLongToUtm.cpp

clean:
	rm lidar2PCD GPSInterp.o ModGPS.o RMC.o GGA.o Gps.o MergeTime.o
