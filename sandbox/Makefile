#TODO: Refactor and cleanup.

DIR := $(subst /,\,${CURDIR})
SRCDIR = $(DIR)\src
EXTENSION = .exe
OBJ_DIR := obj-bin
BUILD_DIR = bin
CC = clang 
ASSEMBLY := sandbox

CFLAGS = -g -Wall

DEPENDENCIES = Dependencies
DEFINES := -DBLX_DEBUG -D_CRT_SECURE_NO_WARNINGS

INCLUDE_FLAGS := -IDependencies\BloxEngine\include -IDependencies\BloxEngine\include\BLX

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRC_FILES := $(call rwildcard, src/,*.c)
HEADER_FILES := $(call rwildcard, Dependencies/,*.h)


DIRECTORIES := \$(DIR) $(subst $(DIR),,$(shell dir $(DIR) /S/ AD/ B | findstr /i src | findstr /V /i obj-bin))
LINKER_FLAGS := -g -luser32 -lGdi32 -lkernel32 -llibcmt -lShell32 -lBloxEngine -LDependencies\BloxEngine
OBJ_FILES := $(SRC_FILES:%=$(OBJ_DIR)/%.o)

SRC_DIR := src


all: scaffold compile link

.PHONY: scaffold
scaffold:

	@echo $(DIRECTORIES)
	@echo Building $(ASSEMBLY)...
	@echo Scaffolding folder structure...
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(addprefix $(OBJ_DIR), $(DIRECTORIES)) 2>NUL || cd .
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(OBJ_DIR)\src 2>NUL || cd .
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(BUILD_DIR) 2>NUL || cd .
	@echo Done.

.PHONY: link
link: scaffold $(OBJ_FILES)
	@echo Linking $(ASSEMBLY)...
	$(CC) $(OBJ_FILES) -o $(BUILD_DIR)\$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)
	@copy "Dependencies\BloxEngine\BloxEngine.dll" "bin\"
	@xcopy .\res $(BUILD_DIR)\res /s /e /i /q /y /d
	@xcopy .\Dependencies\BloxEngine\builtin $(BUILD_DIR)\builtin /s /e /i /q /y /d
	@echo Sand Box Build Finished.

.PHONY: compile
compile:
	@echo Compiling...


.PHONY: clean
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	
$(OBJ_DIR)/%.c.o: %.c
	@echo $<...
	$(CC) $< $(CFLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)
	
