CXX = g++

CPP_FLAG = -std=c++11 -Wall

BASE_DIR = $(shell pwd)

OBJ = $(BASE_DIR)/obj

BUILD = $(BASE_DIR)/build

BUILD_DIR = $(wildcard ./framework/*/*.cpp ./*.cpp)

OBJ_DIR = $(patsubst %.cpp,%.o,$(BUILD_DIR))

LIB = -lstdc++

run:clean app
	$(BASE_DIR)/app

app:$(OBJ_DIR)
	$(CXX) $(CPP_FLAG) -o $(BASE_DIR)/$@ $(foreach dir, $(notdir $^),$(OBJ)/$(dir)) $(LIB)

%.o:%.cpp
	$(CXX) $(CPP_FLAG) -o $(OBJ)/$(notdir $*.o) -c $< $(LIB)

clean:
	rm -r -f $(OBJ)
	rm -r -f $(BUILD)
	mkdir $(OBJ)
	mkdir $(BUILD)
