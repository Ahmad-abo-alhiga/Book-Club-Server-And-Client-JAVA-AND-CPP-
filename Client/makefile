CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_thread -lboost_system -lpthread

all: Client
	g++  -o bin/Client bin/RFkeybord.o bin/RFserver.o bin/UserData.o bin/connectionHandler.o bin/echoClient.o  $(LDFLAGS)

Client: bin/RFkeybord.o bin/RFserver.o bin/UserData.o bin/connectionHandler.o bin/echoClient.o

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp
	
bin/RFkeybord.o: src/RFkeybord.cpp
	g++ $(CFLAGS) -o bin/RFkeybord.o src/RFkeybord.cpp

bin/RFserver.o: src/RFserver.cpp
	g++ $(CFLAGS) -o bin/RFserver.o src/RFserver.cpp

bin/UserData.o: src/UserData.cpp
	g++ $(CFLAGS) -o bin/UserData.o src/UserData.cpp

.PHONY: clean
clean:
	rm -f bin/*
