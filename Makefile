CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
TARGET   := bin/casa_nostra

SRCDIR   := src
BINDIR   := bin

SOURCES := \
    $(SRCDIR)/main.cpp \
    $(SRCDIR)/family_tree/family_tree.cpp \
    $(SRCDIR)/orphan_queue/orphan_queue.cpp \
    $(SRCDIR)/orphan_node/orphan_node.cpp \
    $(SRCDIR)/presentation/main_menu.cpp \
    $(SRCDIR)/parser/csv_parser.cpp

OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
