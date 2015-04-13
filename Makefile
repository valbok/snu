###
# @author VaL Doroshchuk
# @license GNU GPL v2
###

GTEST_DIR = lib/gtest
ENKI_DIR = lib/enki

# Flags passed to the preprocessor.
CPPFLAGS += -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -I$(ENKI_DIR)

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -std=c++0x -I.

# All tests produced by this Makefile. Remember to add new tests you
# created to the list.
TARGETS = snu/brain/tests/build/mempot \
	snu/brain/tests/build/memosc \
	snu/brain/tests/build/CElegansTest \
	snu/robots/tests/build/SnuPuckTest

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

snu/brain/build/%.o : snu/brain/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

snu/robots/build/%.o : snu/robots/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

snu/brain/tests/build/%.o : snu/brain/tests/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

snu/robots/tests/build/%.o : snu/robots/tests/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

snu/brain/tests/build/mempot : snu/brain/build/Neuron.o snu/brain/tests/build/mempot.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

snu/brain/tests/build/memosc : snu/brain/build/Neuron.o snu/brain/tests/build/memosc.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

snu/brain/tests/build/CElegansTest : snu/brain/build/Neuron.o \
	snu/brain/build/CElegans.o \
	snu/brain/tests/build/CElegansTest.o \
	$(GTEST_DIR)/build/gtest_main.a
	$(CXX) -lpthread $(CPPFLAGS) $(CXXFLAGS) -pthread $^ -o $@

snu/robots/tests/build/SnuPuckTest : snu/robots/build/SnuPuck.o \
	snu/robots/tests/build/SnuPuckTest.o \
	$(GTEST_DIR)/build/gtest_main.a
	$(CXX) -lpthread $(CPPFLAGS) $(CXXFLAGS) -pthread $^ -o $@
