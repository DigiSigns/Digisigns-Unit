CC = clang++
CFLAGS = -Wall -pedantic -pipe -Ofast -g
OBJECTS = build/cpp/driver.o build/cpp/branch.o build/cpp/state.o build/cpp/video.o

.PHONY: clean all

all: debug get_videos

debug: $(OBJECTS)
	$(CC) $(CFLAGS) -o debug $(OBJECTS) -lvlc -lpthread

get_videos: src/go/get_video.go
	./src/go/build_go.sh

build/cpp/driver.o: src/cpp/driver.cpp
	$(CC) $(CFLAGS) -c -o build/cpp/driver.o src/cpp/driver.cpp

build/cpp/branch.o: src/cpp/branch.cpp
	$(CC) $(CFLAGS) -c -o build/cpp/branch.o src/cpp/branch.cpp 

build/cpp/state.o: src/cpp/state.cpp 
	$(CC) $(CFLAGS) -c -o build/cpp/state.o src/cpp/state.cpp

build/cpp/video.o: src/cpp/video.cpp
	$(CC) $(CFLAGS) -c -o build/cpp/video.o src/cpp/video.cpp

clean:
	@rm -rf debug get_videos ./build/cpp/*
