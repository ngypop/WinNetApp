# Compiler
CC := g++

# Compiler flags
CFLAGS := -Wall -Wextra -std=c++11
BUILDDIR := build
DISTDIR := dist

# Executable
EXECUTABLE := $(addprefix $(DISTDIR)/, winnetapp)

# Default target
build: $(EXECUTABLE)

# Source files
SRCS := src/main.cpp

# Compile source files into object files
$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Object files
OBJS := $(addprefix $(BUILDDIR)/,$(SRCS:.cpp=.o))

# Link object files into executable
$(EXECUTABLE): $(OBJS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

# Run the executable
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Clean up object files and executable
clean:
	rm -rf $(BUILDDIR) $(DISTDIR)