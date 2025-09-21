DIR := $(subst /,\,${CURDIR})
EXTENSION = .dll
OBJ_DIR := obj-bin
BUILD_DIR = bin
CC = clang 
ASSEMBLY := BloxEngine
INCLUDES_DIR = bin\include

#recursive wildcard
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#BEGIN CPP
CXX = clang++
CXXFLAGS = -std=c++17 -fdeclspec -Wno-inconsistent-dllimport -Wall
SRC_FILES_CPP := $(call rwildcard, src/,*.cpp)
OBJ_FILES_CPP := $(SRC_FILES_CPP:%=$(OBJ_DIR)/%.o)
#END CPP


SRC_FILES := $(call rwildcard, src/,*.c)
OBJ_FILES := $(SRC_FILES:%=$(OBJ_DIR)/%.o)
DIRECTORIES := \$(DIR) $(subst $(DIR),,$(shell dir $(DIR) /S/ AD/ B | findstr /i src | findstr /V /i obj-bin))

CFLAGS := -Wall -fdeclspec -Wno-inconsistent-dllimport
# TODO: Remove -DCGLM_ALL_UNALIGNED once you remove cglm!
DEFINES := -DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS -DBLXEXPORT -DCGLM_ALL_UNALIGNED
INCLUDE_FLAGS := -IDependencies\CGLM\include -IDependencies\GLEW\include -IDependencies\GLFW\include -Isrc\vendor\stb_image -Isrc
LINKER_FLAGS := -shared -lDependencies\GLEW\lib\Release\x64\glew32s -lDependencies\GLFW\lib-vc2022\glfw3_mt -lopengl32 -luser32 -lGdi32 -lkernel32 -llibcmt -lShell32 -lComdlg32

all: configure scaffold compile link

.PHONY: configure
configure:
ifdef release
	@echo building in RELEASE mode
	$(eval CFLAGS += -O2)
else
	@echo building in DEBUG mode
	$(eval CFLAGS += -g)
	$(eval CXXFLAGS += -g)
	$(eval LINKER_FLAGS += -g)
	$(eval DEFINES += -DBLX_DEBUG)
endif

.PHONY: scaffold
scaffold:
	@echo Building $(ASSEMBLY)...
	@echo Scaffolding folder structure...
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(addprefix $(OBJ_DIR), $(DIRECTORIES)) 2>NUL || cd .
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(OBJ_DIR)\src 2>NUL || cd .
	@xcopy Dependencies\GLFW\include\GLFW $(INCLUDES_DIR)\GLFW /s /e /i /q /y /d
	@xcopy Dependencies\GLEW\include\GL $(INCLUDES_DIR)\GL /s /e /i /q /y /d
	@xcopy Dependencies\CGLM\include\cglm $(INCLUDES_DIR)\cglm /s /e /i /q /y /d
	@xcopy builtin $(BUILD_DIR)\builtin /s /e /i /q /y /d
	@xcopy "src\*.h" $(INCLUDES_DIR)\BLX /s /e /i /q /y /d
	@rmdir /s /q "$(INCLUDES_DIR)\BLX\internal
	@echo Done.

.PHONY: link
link: scaffold $(OBJ_FILES) $(OBJ_FILES_CPP)
	@echo Linking $(ASSEMBLY)...
	$(CXX) $(OBJ_FILES) $(OBJ_FILES_CPP) -o $(BUILD_DIR)\$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)
	@echo Build Finished.
	
.PHONY: compile
compile:
	@echo Compiling...

.PHONY: clean
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@echo Done Cleaning.
	
$(OBJ_DIR)/%.c.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)

$(OBJ_DIR)/%.cpp.o: %.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)
	
