# Makefile for C++ project

# Compiler
CXX := g++

# Source files for server
SRCS_SERVER := Server.cpp RunServer.cpp DbSqlite.cpp
OBJS_SERVER := $(SRCS_SERVER:.cpp=.o)
TARGET_SERVER := server

# Source files for client
SRCS_CLIENT := Client.cpp Main.cpp
OBJS_CLIENT := $(SRCS_CLIENT:.cpp=.o)
TARGET_CLIENT := client

# Compiler flags
CXXFLAGS :=  -std=c++11

# Linker flags
LDFLAGS := -lsqlite3

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(OBJS_SERVER)
	$(CXX) $(OBJS_SERVER) -o $(TARGET_SERVER) $(LDFLAGS)

$(TARGET_CLIENT): $(OBJS_CLIENT)
	$(CXX) $(OBJS_CLIENT) -o $(TARGET_CLIENT)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

rserver: $(TARGET_SERVER)
	./$(TARGET_SERVER)

rclient: $(TARGET_CLIENT)
	./$(TARGET_CLIENT)

clean:
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT) $(TARGET_SERVER) $(TARGET_CLIENT)
