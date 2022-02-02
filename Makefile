#https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

TARGET ?= a.out
SRC_DIRS ?= ./src
SRC_DIRS2 ?= ./libs

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

#Searches every folder and adds them to compiler. Will automatically find everything, if needed
INC_DIRS := $(shell find $(SRC_DIRS2) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

#First tells libs folder and then every single folder in it.
CPPFLAGS ?=  -L./libs $(INC_FLAGS) -MMD -MP -fopenmp -O3

#LDLIBS2 += -L/libs 


$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LDLIBS2) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS) -fopenmp -O3

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
