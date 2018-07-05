CPPFLAGS=-g -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv7-a -mtune=arm1176jzf-s
LDLIBS=-L /usr/local/lib -lrf24-bcm
INC=-I /usr/local/include -I.

default: mi_remote

mi_remote: MiRemote.cpp NRF24MiLightRadio.o PL1167_nRF24.o MiLightRadioConfig.o
	g++ $(CPPFLAGS) $(INC) $(LDLIBS) -o mi_remote MiRemote.cpp NRF24MiLightRadio.o PL1167_nRF24.o MiLightRadioConfig.o

MiRemote.o: NRF24MiLightRadio.o MiLightRadioConfig.o
	g++ $(CPPFLAGS) -c MiRemote.cpp

NRF24MiLightRadio.o: NRF24MiLightRadio.cpp PL1167_nRF24.o
	g++ $(CPPFLAGS) -c NRF24MiLightRadio.cpp

PL1167_nRF24.o: PL1167_nRF24.cpp
	g++ $(CPPFLAGS) -c PL1167_nRF24.cpp

MiLightRadioConfig.o: MiLightRadioConfig.cpp
	g++ $(CPPFLAGS) -c MiLightRadioConfig.cpp

clean: 
	@rm -f mi_remote *.o
