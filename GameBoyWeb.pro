# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = GameBoyWeb

QT = core gui widgets

HEADERS = \
   $$PWD/src/backend/Cartridge.hpp \
   $$PWD/src/backend/CPU.hpp \
   $$PWD/src/backend/Disassembler.hpp \
   $$PWD/src/backend/GameBoy.hpp \
   $$PWD/src/backend/Instruction.hpp \
   $$PWD/src/backend/Memory.hpp \
   $$PWD/src/backend/RegisterPair.hpp \
   $$PWD/src/backend/utils.hpp \
   $$PWD/src/frontend/MainWindow.hpp

SOURCES = \
   $$PWD/res/opcodes.json \
   $$PWD/src/backend/Cartridge.cpp \
   $$PWD/src/backend/CMakeLists.txt \
   $$PWD/src/backend/CPU.cpp \
   $$PWD/src/backend/Disassembler.cpp \
   $$PWD/src/backend/GameBoy.cpp \
   $$PWD/src/backend/Instruction.cpp \
   $$PWD/src/backend/Memory.cpp \
   $$PWD/src/backend/ProceduresPrefixed.cpp \
   $$PWD/src/backend/ProceduresUnprefixed.cpp \
   $$PWD/src/backend/utils.cpp \
   $$PWD/src/frontend/CMakeLists.txt \
   $$PWD/src/frontend/MainWindow.cpp \
   $$PWD/src/frontend/MainWindow.ui \
   $$PWD/src/CMakeLists.txt \
   $$PWD/src/main.cpp \
   # $$PWD/tests/src/backend/CartridgeTests.cpp \
   # $$PWD/tests/src/backend/CMakeLists.txt \
   # $$PWD/tests/src/backend/CPUTests.cpp \
   # $$PWD/tests/src/backend/MemoryTests.cpp \
   # $$PWD/tests/src/backend/ProceduresPrefixedTests.cpp \
   # $$PWD/tests/src/backend/ProceduresUnprefixedTests.cpp \
   # $$PWD/tests/src/backend/RegisterPairTests.cpp \
   # $$PWD/tests/src/backend/UtilsTests.cpp \
   $$PWD/CMakeLists.txt \
   # $$PWD/LICENSE.md \
   # $$PWD/README.md

INCLUDEPATH = \
   $$PWD/src/backend/ \
   $$PWD/src/frontend/ \


#DEFINES = 

