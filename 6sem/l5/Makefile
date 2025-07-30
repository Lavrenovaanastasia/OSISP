TARGET = CompIPC

PREF_SRC = ./src/
PREF_OBJ = ./obj/
CXX = g++
CXXFLAGS = -pthread -O2

SRC = $(wildcard $(PREF_SRC)*.c)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))

$(TARGET): $(OBJ)
	@mkdir -p $(dir $(TARGET))
	$(CXX) $(OBJ) $(CXXFLAGS) -o $(TARGET)

$(PREF_OBJ)%.o: $(PREF_SRC)%.cpp
	@mkdir -p $(PREF_OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(PREF_OBJ)*.o results.txt