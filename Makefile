CXX = g++
CXXFLAGS = -Wall -g 
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = main.cpp assets.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = asteroids

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC) $(LDLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)
