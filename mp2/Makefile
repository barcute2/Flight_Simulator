CXX = g++
LD = g++
CXXFLAGS = -Wall  -g -ggdb
EXEC = mp2
LDFLAGS = -o $(EXEC) -L/usr/lib -lGL -lGLU -lglut -lGLEW
SOURCES = mountain-retained.cpp
OBJS = $(SOURCES:.cpp=.o)


$(EXEC) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS)  

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm *.o

