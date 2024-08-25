CC          := clang
PROJECT_DIR := $(shell pwd)
CFLAGS      := -I$(PROJECT_DIR)
CFLAGS      +=  -Wall -Wno-unused-command-line-argument -Wno-void-pointer-to-int-cast -Wno-backslash-newline-escape -Werror=implicit
#CFLAGS 		+= -Werror
#CFLAGS 		+= -O3

LDLIBS      := -lncursesw -lm

SourceDirectory := src
HeaderDirectory := chip8


SourceFiles := $(shell find $(SourceDirectory)/ -name "*.c" | tr '\n' ' ')
HeaderDirectoryFiles := $(shell find $(HeaderDirectory)/ -name "*.h" | tr '\n' ' ')

ObjectFiles := $(patsubst $(SourceDirectory)/%.c, $(SourceDirectory)/%.o, $(SourceFiles))
MainFileObject := main.o


BinaryOutput := ./a.out


all := $(BinaryOutput)

$(BinaryOutput): $(ObjectFiles) $(SourceDirectory) $(MainFileObject)
	$(CC) -o $@ $(CFLAGS) $(ObjectFiles) $(MainFileObject) $(LDLIBS)

$(SourceDirectory)/%.o: $(SourceDirectory)/%.c
	$(CC) -c $< $(CFLAGS) $(LDLIBS) -o $@

$(MainFileObject)/%.o: $(MainFileObject)/%.c
	$(CC) -c $< $(CFLAGS) $(LDLIBS) -o $@


clean: 
	rm $(ObjectFiles)