#ifndef DmxReceiver_h
#define DmxReceiver_h
#include <Arduino.h>

/**
 * Arduino library for receiving DMX signals.
 *
 * Based on dmx_serial_sender by Kevin Deus:
 * https://github.com/ktgow/dmx_serial_sender
*/
class DmxReceiver {
  public:
    /**
     * Constructor.
     *
     * @param pin pin to receive data from
     * @param channels number of channels to receive
     * @todo Add channelOffset parameter.
    */
    DmxReceiver(byte pin, unsigned int channels);

    /**
     * Destructor.
    */
    ~DmxReceiver();

    /**
     * Check for new data. Call as often as possible from your
     * main loop.
     *
     * @return true if new data was available, otherwise false
    */
    bool poll();

    /**
     * Returns the value of the given channel.
     *
     * @param channel index of the channel
     * @return value of the channel or zero, if an invalid index was given
    */
    byte getValue(unsigned int channel);
  private:
    /**
     * Waits until the value of the given pin changes or a timeout occurs.
     *
     * @param pinValue contains the value of the pin at the beginning
     * @param valueTime contains the time until the pin changed
     * @param timeoutUs timeout in microseconds
     * @return false if a timeout occured, otherwise true
    */
    bool readPinUntilChange(unsigned char* pinValue, unsigned long* valueTime,
        unsigned long timeoutUs);

    /**
     * Configuration.
    */
    byte _pin;
    unsigned int _channels;

    /**
     * Value of millis() when the last data was read successfully.
    */
    unsigned long _lastRead;

    /**
     * Values of the channels.
    */
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
