#ifndef IR_PROTOCOL
#define IR_PROTOCOL

#include <Arduino.h>

#define PULSE_LENGTH 1
#define PULSE_DISTANCE 2
#define PULSE_BOTH 3

#define readIR()  

class IRBase
{
public:
  IRBase(int pin, int prot, int stLimOn, int stLimOff, int limOn, int limOff, int numFields, int *lengths) :
    protocol(prot), startThreshOn(stLimOn), startThreshOff(stLimOff),
    threshOn(limOn), threshOff(limOff), IRPin(pin), nFields(numFields), fieldLengths(lengths)
  {
    nBits = 0;
    for (int i = 0; i < nFields; i++)
      nBits += fieldLengths[i];
  }
  void begin() { pinMode(IRPin, INPUT); }
  bool getData(int *data, byte *bytes, long timeout);
  int getNFields() { return nFields; }
private:
  int protocol, IRPin;
  int startThreshOn, startThreshOff;
  int threshOn, threshOff;
  int nFields, *fieldLengths, nBits;
};


static int IRHeliXTrust_fields[] = {6, 1, 5, 1, 5, 1, 5, 4, 2, 1, 1, 8};
class IRHeliXTrust : public IRBase
{
public:
  IRHeliXTrust(int pin) : 
    IRBase(pin, PULSE_BOTH, 1500, 0, 600, 600, 12, IRHeliXTrust_fields) { }
  bool poll(unsigned long timeout)
  {
    if(getData(data, bytes, timeout) && checksum(bytes))
    {
      throttle = data[0];
      yaw = data[2] * ((data[1])?-1:1);
      trim = data[4] * ((data[3])?-1:1);
      pitch = data[6] * ((data[5])?-1:1);
      channel = data[8];
      mode = data[9];
      toggle = data[10];
      return true;
    }
    return false;
  }
  int getThrottle() { return throttle; }
  int getPitch() { return pitch; }
  int getYaw() { return yaw; }
  int getTrim() { return trim; }
  int getChannel() { return channel; }
  int getMode() { return mode; }
  int getToggle() { return toggle; }
private:
  bool checksum(byte *bytes)
  {
    byte total = 0;
    for (int i = 0; i < 5; i++) total += bytes[i];
    return total%256 == 0;
  }
  int data[15];
  byte bytes[10];
  int throttle, pitch, yaw, trim, channel, mode, toggle;
};

static int IRHeliGyro_fields[] = {8, 8, 1, 7, 8, 8};
class IRHeliGyro : public IRBase
{
public:
  IRHeliGyro(int pin) : 
    IRBase(pin, PULSE_DISTANCE, 1500, 1500, 0, 400, 5, IRHeliGyro_fields) { }
  bool poll(unsigned long timeout)
  {
    if(getData(data, bytes, timeout))
    {
      yaw = data[0];
      pitch = data[1];
      channel = data[2];
      throttle = data[3];
      trim = data[4];
      return true;
    }
    return false;
  }
  int getThrottle() { return throttle; }
  int getPitch() { return pitch; }
  int getYaw() { return yaw; }
  int getTrim() { return trim; }
  int getChannel() { return channel; }
private:
  int data[15];
  byte bytes[10];
  int throttle, pitch, yaw, trim, channel;
};

static int IRRemoteStandard_fields[] = {8, 8, 8, 8};
class IRRemoteStandard : public IRBase
{
public:
  IRRemoteStandard(int pin) : 
    IRBase(pin, PULSE_DISTANCE, 8000, 4000, 0, 1000, 4, IRRemoteStandard_fields) { }
  bool poll(unsigned long timeout)
  {
    if(getData(data, bytes, timeout) && checksum(bytes))
    {
      model = data[0];
      button = data[2];
      return true;
    }
    return false;
  }
  int getModel() { return model; }
  int getButton() { return button; }
private:
  bool checksum(byte *bytes)
  {
    return (bytes[0] == bytes[1] || (bytes[0] | bytes[1]) == 255) &&
    		((bytes[2] | bytes[3]) == 255);
  }
  int data[15];
  byte bytes[10];
  int model, button;
};
//IRBase ir(16, PULSE_DISTANCE, 8000, 4000, 0, 1000, 4, fields);

#endif //IR_PROTOCOL