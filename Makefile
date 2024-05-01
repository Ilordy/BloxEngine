DIR := $(subst /,\,${CURDIR})
EXTENSION = .dll
OBJ_DIR := obj-bin
BUILD_DIR = bin
CC = clang 
ASSEMBLY := BloxEngine
INCLUDES_DIR = bin\include

#recursive wildcard
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
SRC_FILES := $(call rwildcard, src/,*.c)
OBJ_FILES := $(SRC_FILES:%=$(OBJ_DIR)/%.o)
DIRECTORIES := \$(DIR) $(subst $(DIR),,$(shell dir $(DIR) /S/ AD/ B | findstr /i src | findstr /V /i obj-bin))

CFLAGS := -Wall -fdeclspec -Wno-inconsistent-dllimport
DEFINES := -DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS -DBLXEXPORT 
INCLUDE_FLAGS := -IDependencies\CGLM\include -IDependencies\GLEW\include -IDependencies\GLFW\include -Isrc\vendor\stb_image
LINKER_FLAGS := -shared -lDependencies\GLEW\lib\Release\x64\glew32s -lDependencies\GLFW\lib-vc2022\glfw3_mt -lopengl32 -luser32 -lGdi32 -lkernel32 -llibcmt -lShell32

all: configure scaffold compile link

.PHONY: configure
configure:
ifdef release
	@echo building in RELEASE mode
	$(eval CFLAGS += -O2)
else
	@echo building in DEBUG mode
	$(eval CFLAGS += -g)
	$(eval LINKER_FLAGS += -g)
	$(eval DEFINES += -D_DEBUG)
endif

.PHONY: scaffold
scaffold:
	@echo Building $(ASSEMBLY)...
	@echo Scaffolding folder structure...
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(addprefix $(OBJ_DIR), $(DIRECTORIES)) 2>NUL || cd .
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(OBJ_DIR)\src 2>NUL || cd .
	@xcopy Dependencies\GLFW\include\GLFW $(INCLUDES_DIR)\GLFW /s /e /i /q /y
	@xcopy Dependencies\GLEW\include\GL $(INCLUDES_DIR)\GL /s /e /i /q /y
	@xcopy Dependencies\CGLM\include\cglm $(INCLUDES_DIR)\cglm /s /e /i /q /y
	@xcopy "src\*.h" $(INCLUDES_DIR)\BLX /s /e /i /q /y
	@echo Done.

.PHONY: link
link: scaffold $(OBJ_FILES)
	@echo Linking $(ASSEMBLY)...
	$(CC) $(OBJ_FILES) -o $(BUILD_DIR)\$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)
	
.PHONY: compile
compile:
	@echo Compiling...

.PHONY: clean
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@echo Done Cleaing.
	
$(OBJ_DIR)/%.c.o: %.c
	@echo	$<...
	$(CC) $< $(CFLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)
	
