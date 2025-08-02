TARGET = Server

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	$(CXX) -c $< -o $@

clean :
	rm $(TARGET) $(PREF_OBJ)*.o

TEST_SRC = ./test/test_encode.cpp
TEST_BIN = test_encode

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(PREF_SRC)encodeMorse.cpp
	$(CXX) $(TEST_SRC) $(PREF_SRC)encodeMorse.cpp -o $(TEST_BIN)

clean-test:
	rm -f $(TEST_BIN)