# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = GameBoyWeb

QT = core gui widgets

HEADERS = \
   src/Cartridge.hpp \
   src/DebugTypes.hpp \
   src/GameBoy.hpp \
   src/GraphicTypes.hpp \
   src/instruction_info.hpp \
   src/InterruptController.hpp \
   src/Joypad.hpp \
   src/Memory.hpp \
   src/PPU.hpp \
   src/Processor.hpp \
   src/QtRenderer.hpp \
   src/Ram.hpp \
   src/RegisterPair.hpp \
   src/Renderer.hpp \
   src/Rom.hpp \
   src/SerialDataTransfer.hpp \
   src/Timer.hpp \
   src/utils.hpp \
    src/MainWindow.h

SOURCES = \
   src/Cartridge.cpp \
   src/GameBoy.cpp \
   src/GraphicTypes.cpp \
   src/instructions.cpp \
   src/InterruptController.cpp \
   src/Joypad.cpp \
   src/main.cpp \
   src/Memory.cpp \
   src/PPU.cpp \
   src/Processor.cpp \
   src/QtRenderer.cpp \
   src/Ram.cpp \
   src/Rom.cpp \
   src/SerialDataTransfer.cpp \
   src/Timer.cpp \
   src/utils.cpp \
    src/MainWindow.cpp

INCLUDEPATH =

#DEFINES = 

FORMS += \
    src/MainWindow.ui

