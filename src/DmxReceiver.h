#ifndef DmxReceiver_h
#define DmxReceiver_h

class DmxReceiver {
  public:
    DmxReceiver(byte pin, unsigned int channels); /* TODO: channelOffset */
    bool poll();
    byte getValue(unsigned int channel);
  private:
    bool _readPinUntilChange(unsigned char* pinValue, unsigned long* valueTime, unsigned long timeoutUs);
    /* Variables: */
    byte _pin;
    unsigned int _channels;
    unsigned long _lastRead;
    byte* _data;
};

#define DMXRECEIVER_DELAY_1_US \
        __asm__ __volatile__ ( \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
        )

#define DMXRECEIVER_DELAY_2_US \
  DMXRECEIVER_DELAY_1_US; \
  DMXRECEIVER_DELAY_1_US

#define DMXRECEIVER_DELAY_AFTER_READ \
        __asm__ __volatile__ ( \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
                "nop" "\n\t"   \
        )

#define DMXRECEIVER_DELAY_4_US_AFTER_READ \
  DMXRECEIVER_DELAY_AFTER_READ; \
  DMXRECEIVER_DELAY_1_US; \
  DMXRECEIVER_DELAY_1_US; \
  DMXRECEIVER_DELAY_1_US

#endif
