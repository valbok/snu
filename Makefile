###
# @author VaL Doroshchuk
# @license GNU GPL v2
###

GTEST_DIR = lib/gtest

# Flags passed to the preprocessor.
CPPFLAGS += -isystem $(GTEST_DIR)/include -I$(GTEST_DIR)

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -std=c++0x -I.

# All tests produced by this Makefile. Remember to add new tests you
# created to the list.
TARGETS = snu/tests/build/mempot snu/tests/build/memosc snu/tests/build/CElegansTest

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

all : $(TARGETS)

clean :
	find ./ -name "*.o" -exec rm -f {} \;

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(GTEST_DIR)/build/gtest-all.o : $(GTEST_SRCS_)
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(GTEST_DIR)/build/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(GTEST_DIR)/build/gtest.a : $(GTEST_DIR)/build/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^ -o $@

$(GTEST_DIR)/build/gtest_main.a : $(GTEST_DIR)/build/gtest-all.o $(GTEST_DIR)/build/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^ -o $@

snu/build/%.o : snu/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

snu/tests/build/%.o : snu/tests/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

snu/tests/build/mempot : snu/build/Neuron.o snu/tests/build/mempot.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

snu/tests/build/memosc : snu/build/Neuron.o snu/tests/build/memosc.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

snu/tests/build/CElegansTest : snu/build/Neuron.o \
	snu/build/CElegans.o \
	snu/tests/build/CElegansTest.o \
	$(GTEST_DIR)/build/gtest_main.a
	$(CXX) -lpthread $(CPPFLAGS) $(CXXFLAGS) -pthread $^ -o $@
