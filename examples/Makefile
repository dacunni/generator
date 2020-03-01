
GEN_DIR=..
HDR = $(GEN_DIR)/generator.h \
      $(GEN_DIR)/range_generator.hpp \
      $(GEN_DIR)/nested_generator.hpp

INC=-I$(GEN_DIR)

examples: examples.cpp $(HDR)
	c++ examples.cpp -o examples -std=c++11 $(INC)

