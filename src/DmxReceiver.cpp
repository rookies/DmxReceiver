#include <DmxReceiver.h>
#include <Arduino.h>

DmxReceiver::DmxReceiver(byte pin, unsigned int channels) : _lastRead(0) {
  /* Save pin number: */
  _pin = pin;
  /* Save channel number: */
  _channels = channels;
  /* Set pin as input: */
  pinMode(pin, INPUT);
  /* Init data array: */
  _data = new byte[channels];
}

/* TODO: delete[] _data; in Deconstructor */

bool DmxReceiver::poll() {
  /* If less than 16ms passed, return false: */
  if (millis() - _lastRead <= 16) {
    return false;
  };
  /* If pin is not HIGH, return false: */
  if (digitalRead(_pin) != HIGH) {
    return false;  
  };
  /* Init variables: */
  int bytesRead = 0;
  memset(_data, 0, _channels); /* FIXME: Maybe doesn't work this way */
  unsigned char* dataPtr = _data;
  const unsigned char* dataEnd = dataPtr + _channels;
  const uint8_t bit = digitalPinToBitMask(_pin);
  const uint8_t port = digitalPinToPort(_pin);
  unsigned char pinValue;
  unsigned long valueTime;
  unsigned long resetStart;
  /*
   * Read pin until change and return false if it doesn't change in 2 seconds,
   * wasn't HIGH on the beginning or changed its value in less than 89 ms:
  */
  if (_readPinUntilChange(&pinValue, &valueTime, 2000) && (pinValue == HIGH) && (valueTime > 88)) {
    resetStart = micros();
    while (((*portInputRegister(port) & bit) == 0) && (micros() - resetStart < 88)) {}
    if (micros() - resetStart > 88) {
      noInterrupts();
      /* Read to the end of the reset: */
      while ((*portInputRegister(port) & bit) == 0) {}
      /* Read the first mark: */
      while ((*portInputRegister(port) & bit) != 0) {}
      /* First byte is always 0.  Wait for its stop bits: */
      while ((*portInputRegister(port) & bit) == 0) {}
      /* Finally, read data: */
      while(dataPtr < dataEnd) {
        /*
         * Remove the stop bits, which are variable length.
         * Note that idle is also caught by this. To ensure
         * that this doesn't sit through an entire idle signal,
         * DATA_BYTES should be smaller than the number of
         * transmitted bytes. What a hack.
        */
        while ((*portInputRegister(port) & bit) != 0) {}
        *dataPtr = 0;
        //DMXRECEIVER_DELAY_2_US;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 0;  // Start bit is always 0.
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 0;  // LSB
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 1;
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 2;
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 3;
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 4;
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 5;
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 6;
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        *dataPtr |= ((*portInputRegister(port) & bit) ? 1 : 0) << 7;  // MSB
        DMXRECEIVER_DELAY_4_US_AFTER_READ;
        ++dataPtr;
      }
      interrupts();
      bytesRead = dataPtr - _data;
    };
  };
  /* Return true if we read bytes: */
  return (bytesRead > 0);
}

byte DmxReceiver::getValue(unsigned int channel) {
  if (channel < _channels) {
    return _data[channel];
  } else {
    return 0;
  };
}

bool DmxReceiver::_readPinUntilChange(unsigned char* pinValue, unsigned long* valueTime, unsigned long timeoutUs) {
  /* Variable declaration: */
  unsigned long valueStart;
  /* Get start value: */
  *pinValue = digitalRead(pinDmx);
  /* Get start time: */
  valueStart = micros();
  /* Loop until pin change or timeout: */
  while (digitalRead(pinDmx) == *pinValue) {
    if ((micros() - valueStart) > timeoutUs) {
      /* Timeout, return failure: */
      return false;
    };
  }
  /* Calculate duration: */
  *valueTime = micros() - valueStart;
  /* Return success: */
  return true;
}
