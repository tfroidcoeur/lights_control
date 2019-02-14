ifeq ($(OS),Windows_NT)
SHELL = cmd
RMDIR = rmdir /s /q
RM = del /q
mymkdir = if not exist "$1" mkdir "$1"
else
RMDIR = rm -fr
RM = rm -f
mymkdir = mkdir -p $1
endif

LIBRARIES=lib
ARDUINOSTL=$(LIBRARIES)/ArduinoSTL
CORELIB=$(LIBRARIES)/core
ETHERNETLIB=$(LIBRARIES)/Ethernet
CONTROLLINOLIB=$(LIBRARIES)/controllino
BUILD=build

OBJCOPY=avr-objcopy
GCC=avr-gcc
GPP=avr-g++
AR=avr-gcc-ar

#INCLUDES=-I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/cores/arduino" -I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/variants/Controllino_maxi" -I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/libraries/Ethernet/src" -I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/libraries/SoftwareSerial/src" -I"/home/fraco/.arduinocdt/libraries/CONTROLLINO_Library" -I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/libraries/SPI/src" -I"/home/fraco/.arduinocdt/libraries/ArduinoSTL/1.0.5/src"
#INCLUDES=-I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/cores/arduino" -I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/variants/Controllino_maxi" -I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/libraries/Ethernet/src" -I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/libraries/SoftwareSerial/src" -I"/home/fraco/.arduinocdt/libraries/CONTROLLINO_Library" -I"/home/fraco/.arduinocdt/packages/CONTROLLINO_Boards/hardware/avr/2.0.1/libraries/SPI/src" -I"/home/fraco/.arduinocdt/libraries/ArduinoSTL/1.0.5/src"
INCLUDES=-I"$(CORELIB)/cores/arduino" \
		 -I"$(BUILD)/avr/variants/Controllino_maxi/" \
		 -I"$(ETHERNETLIB)/src" \
		 -I"$(CORELIB)/libraries/SoftwareSerial/src" \
		 -I"$(CONTROLLINOLIB)" \
		 -I"$(CORELIB)/libraries/SPI/src" \
		 -I"$(ARDUINOSTL)/src"

TARGETS = \
	$(BUILD)/lights.hex

all: $(TARGETS)


PROJECT_OBJS = \
	$(BUILD)/src/Button.cpp.o \
	$(BUILD)/src/Controller.cpp.o \
	$(BUILD)/src/Dimmer.cpp.o \
	$(BUILD)/src/InPin.cpp.o \
	$(BUILD)/src/MotionSpot.cpp.o \
	$(BUILD)/src/OutPin.cpp.o \
	$(BUILD)/src/Runner.cpp.o \
	$(BUILD)/src/Sequencer.cpp.o \
	$(BUILD)/src/Teleruptor.cpp.o \
	$(BUILD)/src/lights.cpp.o

-include $(PROJECT_OBJS:.o=.d)

PLATFORM_CORE_OBJS = \
	$(BUILD)/core/HardwareSerial3.cpp.o \
	$(BUILD)/core/wiring_shift.c.o \
	$(BUILD)/core/PluggableUSB.cpp.o \
	$(BUILD)/core/Stream.cpp.o \
	$(BUILD)/core/HardwareSerial2.cpp.o \
	$(BUILD)/core/HardwareSerial.cpp.o \
	$(BUILD)/core/main.cpp.o \
	$(BUILD)/core/Tone.cpp.o \
	$(BUILD)/core/abi.cpp.o \
	$(BUILD)/core/Print.cpp.o \
	$(BUILD)/core/hooks.c.o \
	$(BUILD)/core/WMath.cpp.o \
	$(BUILD)/core/wiring_analog.c.o \
	$(BUILD)/core/wiring_digital.c.o \
	$(BUILD)/core/IPAddress.cpp.o \
	$(BUILD)/core/wiring.c.o \
	$(BUILD)/core/WInterrupts.c.o \
	$(BUILD)/core/new.cpp.o \
	$(BUILD)/core/USBCore.cpp.o \
	$(BUILD)/core/wiring_pulse.c.o \
	$(BUILD)/core/HID.cpp.o \
	$(BUILD)/core/HardwareSerial0.cpp.o \
	$(BUILD)/core/wiring_pulse.S.o \
	$(BUILD)/core/WString.cpp.o \
	$(BUILD)/core/CDC.cpp.o \
	$(BUILD)/core/HardwareSerial1.cpp.o

-include $(PLATFORM_CORE_OBJS:.o=.d)

LIBRARIES_OBJS = \
	$(BUILD)/lib/Ethernet/src/EthernetUdp.cpp.o \
	$(BUILD)/lib/Ethernet/src/Dhcp.cpp.o \
	$(BUILD)/lib/Ethernet/src/EthernetServer.cpp.o \
	$(BUILD)/lib/Ethernet/src/Dns.cpp.o \
	$(BUILD)/lib/Ethernet/src/EthernetClient.cpp.o \
	$(BUILD)/lib/Ethernet/src/socket.cpp.o \
	$(BUILD)/lib/Ethernet/src/utility/w5100.cpp.o \
	$(BUILD)/lib/Ethernet/src/Ethernet.cpp.o \
	$(BUILD)/lib/SoftwareSerial/src/SoftwareSerial.cpp.o \
	$(BUILD)/lib/CONTROLLINO_Library/Controllino.cpp.o \
	$(BUILD)/lib/SPI/src/SPI.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/func_exception.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/valarray.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/complex.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/char_traits.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/vector.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/list.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/eh_alloc.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/ios.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/support.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/del_opv.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/new_handler.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/del_ops.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/ArduinoSTL.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/ostream.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/iomanip.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/ostream_helpers.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/del_opvnt.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/exception.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/queue.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/new_opvnt.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/iterator.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/bitset.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/new_opv.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/locale.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/sstream.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/algorithm.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/typeinfo.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/istream.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/associative_base.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/set.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/utility.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/limits.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/new_opnt.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/iostream.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/map.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/streambuf.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/del_op.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/del_opnt.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/deque.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/new_op.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/eh_globals.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/numeric.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/stack.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/stdexcept.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/abi/abi.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/del_opvs.cpp.o \
	$(BUILD)/lib/ArduinoSTL/src/string.cpp.o

-include $(LIBRARIES_OBJS:.o=.d)

CONTROLLINO_BOARDS=$(BUILD)/avr/variants/Controllino_mega/pins_arduino.h

$(CONTROLLINO_BOARDS):
	unzip -u -d $(BUILD) $(CONTROLLINOLIB)/Boards/ControllinoHW.zip

$(BUILD)/lights.hex: $(BUILD)/lights.elf
	$(OBJCOPY) -O ihex -R .eeprom  "$(BUILD)/lights.elf" "$(BUILD)/lights.hex"

$(BUILD)/lights.elf: $(PROJECT_OBJS) $(LIBRARIES_OBJS) $(BUILD)/core.a
	$(GCC) -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560  -o "$(BUILD)/lights.elf" $(PROJECT_OBJS) $(LIBRARIES_OBJS) "$(BUILD)/core.a" "-L." -lm

$(BUILD)/core.a:	$(PLATFORM_CORE_OBJS) $(PLATFORM_VARIANT_OBJS)

clean:
	-$(RMDIR) $(BUILD)

size:
	$(SIZE) -A "$(BUILD)/lights.elf"

$(BUILD)/src/%.cpp.o: src/%.cpp $(CONTROLLINO_BOARDS)
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"

$(BUILD)/core/%.cpp.o: $(CORELIB)/cores/arduino/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"
	$(AR) rcs  "$(BUILD)/core.a" "$@"

$(BUILD)/core/%.cpp.o: $(CORELIB)/libraries/HID/src/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"
	$(AR) rcs  "$(BUILD)/core.a" "$@"

$(BUILD)/core/%.c.o: $(CORELIB)/cores/arduino/%.c
	@$(call mymkdir,$(dir $@))
	$(GCC) -c -g -Os -w -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR $(INCLUDES) "$<" -o "$@"
	$(AR) rcs  "$(BUILD)/core.a" "$@"

$(BUILD)/lib/Ethernet/src/%.cpp.o:  $(ETHERNETLIB)/src/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"

$(BUILD)/lib/Ethernet/src/utility/%.cpp.o:  $(ETHERNETLIB)/src/utility/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"

$(BUILD)/lib/SoftwareSerial/src/%.cpp.o: $(CORELIB)/libraries/SoftwareSerial/src/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"

$(BUILD)/lib/SoftwareSerial/src/%.cpp.o: $(CORELIB)/libraries/SoftwareSerial/src/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"

$(BUILD)/lib/CONTROLLINO_Library/%.cpp.o: $(CONTROLLINOLIB)/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"

$(BUILD)/lib/SPI/src/%.cpp.o:  $(CORELIB)/libraries/SPI/src/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"

$(BUILD)/lib/ArduinoSTL/src/%.cpp.o: $(ARDUINOSTL)/src/%.cpp
	@$(call mymkdir,$(dir $@))
	$(GPP) -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     $(INCLUDES) "$<" -o "$@"

$(BUILD)/core/%.S.o: $(CORELIB)/cores/arduino/%.S
	@$(call mymkdir,$(dir $@))
	$(GCC) -c -g -x assembler-with-cpp -flto -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR $(INCLUDES) "$<" -o "$@"
	$(AR) rcs  "$(BUILD)/core.a" "$@"

