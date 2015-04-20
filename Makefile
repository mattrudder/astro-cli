UNAME := $(shell uname)
ifeq ($(UNAME),$(filter $(UNAME),Linux Darwin))
	ifeq ($(UNAME),$(filter $(UNAME),Darwin))
		# TODO: Check for iOS build flags.
		OS=osx
	else
		OS=linux
	endif
	CXXFLAGS += -std=c++11
else
	OS=windows
endif

libgit2_CFLAGS := $(shell pkg-config --cflags --static libgit2)
libgit2_LDFLAGS := $(shell pkg-config --libs libgit2)

CXXFLAGS += $(libgit2_CFLAGS)
LDFLAGS += $(libgit2_LDFLAGS) -all_load

program_NAME := astro
program_SRCS := $(shell find src -type f -name '*.cpp')
program_OBJS := ${program_SRCS:.cpp=.o}
program_DEPS := ${program_OBJS:.o=.dep}
program_INCLUDE_DIRS := include deps
program_LIBRARY_DIRS :=
program_LIBRARIES :=

CXXFLAGS += -g -O0

ifeq ($(OS),$(filter $(OS),osx ios))
	CXXFLAGS += -stdlib=libc++
	LDFLAGS +=
endif

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach libdir,$(program_LIBRARY_DIRS),-L$(libdir))
LDFLAGS += $(foreach lib,$(program_LIBRARIES),-l$(lib))

.PHONY: all clean distclean

all: $(program_NAME)

-include $(program_DEPS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x c++ -MM -MT $@ -MF $(patsubst %.o,%.dep,$@) $<
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x c++ -c -o $@ $<

%.o: %.mm
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x objective-c++ -MM -MT $@ -MF $(patsubst %.o,%.dep,$@) $<
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x objective-c++ -c -o $@ $<

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(shell find src -type f -name '*.o')
	@- $(RM) $(shell find src -type f -name '*.dep')

distclean: clean
