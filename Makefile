# makefile

CC = gcc
CXX = g++
AR = ar rcs
RM = rm -f
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra $(EXT_FLAGS)

BUILD_DIR = ./build
STATIC_OBJS_DIR = $(BUILD_DIR)/static_obj
DYNAMIC_OBJS_DIR = $(BUILD_DIR)/dynamic_obj
LIBS_DIR = ./lib
INCLUDE_DIR = ./include
SRC_DIR = ./src
STATIC_DEPENDS_DIR = $(BUILD_DIR)/static_depends
DYNAMIC_DEPENDS_DIR = $(BUILD_DIR)/dynamic_depends
DIRS = $(BUILD_DIR) $(STATIC_OBJS_DIR) $(DYNAMIC_OBJS_DIR) $(LIBS_DIR) \
	   $(STATIC_DEPENDS_DIR) $(DYNAMIC_DEPENDS_DIR)

SOURCES = ${wildcard $(SRC_DIR)/*.cpp}
SRCS = ${notdir $(SOURCES)}
OBJS = ${patsubst %.cpp, %.o, $(SRCS)}
STATIC_OBJS = ${addprefix $(STATIC_OBJS_DIR)/, $(OBJS)}
DYNAMIC_OBJS = ${addprefix $(DYNAMIC_OBJS_DIR)/, $(OBJS)}
STATIC_DEPENDS = ${addprefix $(STATIC_DEPENDS_DIR)/, $(patsubst %.cpp, %.d, $(SRCS))}
DYNAMIC_DEPENDS = ${addprefix $(DYNAMIC_DEPENDS_DIR)/, $(patsubst %.cpp, %.d, $(SRCS))}

all: libs

libs: dirs static_lib dynamic_lib

dirs: $(DIRS)

$(DIRS):
	if [ ! -d $@ ]; then mkdir -p $@; fi

static_lib: dirs $(STATIC_DEPENDS) $(LIBS_DIR)/libklibcpp.a

$(LIBS_DIR)/libklibcpp.a: $(STATIC_OBJS)
	$(AR) $(LIBS_DIR)/libklibcpp.a $(STATIC_OBJS)

dynamic_lib: dirs $(DYNAMIC_DEPENDS) $(LIBS_DIR)/libklibcpp.so

$(LIBS_DIR)/libklibcpp.so: $(DYNAMIC_OBJS)
	$(CXX) $(CXXFLAGS) -fPIC -shared $(DYNAMIC_OBJS) -o $@

$(STATIC_OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) $< -c -o $@

$(DYNAMIC_OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -I $(INCLUDE_DIR) $< -c -o $@

-include $(STATIC_DEPENDS)
-include $(DYNAMIC_DEPENDS)

$(STATIC_DEPENDS_DIR)/%.d: $(SRC_DIR)/%.cpp
	@if [ ! -d $(STATIC_DEPENDS_DIR) ]; then mkdir -p $(STATIC_DEPENDS_DIR); fi
	@set -e; $(RM) $@; \
	$(CXX) -MM $(CXXFLAGS) -I $(INCLUDE_DIR) $< > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,$(STATIC_OBJS_DIR)/\1.o $@:,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

$(DYNAMIC_DEPENDS_DIR)/%.d: $(SRC_DIR)/%.cpp
	@if [ ! -d $(DYNAMIC_DEPENDS_DIR) ]; then mkdir -p $(DYNAMIC_DEPENDS_DIR); fi
	@set -e; $(RM) $@; \
	$(CXX) -MM $(CXXFLAGS) -I $(INCLUDE_DIR) $< > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,$(DYNAMIC_OBJS_DIR)/\1.o $@:,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

clean:
	$(RM) $(STATIC_OBJS_DIR)/*.o
	$(RM) $(DYNAMIC_OBJS_DIR)/*.o
	$(RM) $(STATIC_DEPENDS_DIR)/*.d
	$(RM) $(DYNAMIC_DEPENDS_DIR)/*.d

cleanall: clean
	$(RM) $(LIBS_DIR)/*.a
	$(RM) $(LIBS_DIR)/*.so

