.DEFAULT_GOAL := memory

SRCDIR := .
OBJDIR := .obj

CXXFLAGS := 
LDFLAGS :=

$(shell mkdir -p $(OBJDIR))

SRCS := $(shell find $(SRCDIR) -name "*.cpp")
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all : memory

memory : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean : 
	$(RM) -rf $(OBJDIR) $(EXE)

$(OBJDIR)/Memory.o : Memory.cpp Memory.h
$(OBJDIR)/Driver.o : Driver.cpp

$(OBJDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
