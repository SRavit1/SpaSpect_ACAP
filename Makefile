CC=g++
CCFLAGS = 
LIBS = -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lopencv_imgproc

all: executable

debug: CCFLAGS += -DDEBUG -g
debug: executable

executable:
	$(CC) $(CCFLAGS) main.cpp $(LIBS)

test: CCFLAGS += -DDEBUG -g
test:
	$(CC) $(CCFLAGS) test.cpp $(LIBS)

clean:
	rm a.out
