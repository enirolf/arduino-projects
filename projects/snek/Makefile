include board.mk

MONITOR_PORT = /dev/ttyACM0

LEDCONTROL_ARCHIVE = build-$(BOARD_TAG)/libledcontrol.a
SEVSEG_ARCHIVE = build-$(BOARD_TAG)/libsevseg.a

CXXFLAGS += -ILedControlMS -ISevSeg
OTHER_OBJS = LedControlMS/$(LEDCONTROL_ARCHIVE) SevSeg/$(SEVSEG_ARCHIVE)

include $(ARDMK_DIR)/Arduino.mk

LedControlMS/$(LEDCONTROL_ARCHIVE):
	$(MAKE) -C LedControlMS $(LEDCONTROL_ARCHIVE)

SevSeg/$(SEVSEG_ARCHIVE):
	$(MAKE) -C SevSeg $(SEVSEG_ARCHIVE)

clean::
	$(MAKE) -C LedControlMS clean
	$(MAKE) -C SevSeg clean
