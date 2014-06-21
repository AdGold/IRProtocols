#include "IRProtocols.h"

bool IRBase::getData(int *data, byte *bytes, long timeout)
{
	unsigned long start, on = 0, off = 0, funcStart = millis();
	while (digitalRead(IRPin) == HIGH) ;
	while (on < startThreshOn || off < startThreshOff)
	{
	  start = micros();
	  while (digitalRead(IRPin) == LOW && (timeout == -1 || funcStart + timeout < millis())) ;
	  on = micros() - start;
	  start = micros();
	  while (digitalRead(IRPin) == HIGH && (timeout == -1 || funcStart + timeout < millis())) ;
	  off = micros() - start;
	  if (timeout != -1 && funcStart + timeout > millis())
	    return false;
	}
	bool bits[nBits+1];
	for (int i = 0; i <= nBits/8; i++) bytes[i] = 0;
	for (int i = 0; i < nBits; i++)
	{
	  start = micros();
	  while (digitalRead(IRPin) == LOW && (timeout == -1 || funcStart + timeout < millis())) ;
	  on = micros() - start;
	  start = micros();
	  while (digitalRead(IRPin) == HIGH && (timeout == -1 || funcStart + timeout < millis())) ;
	  off = micros() - start;
	  if (timeout != -1 && funcStart + timeout > millis())
	    return false;
	  switch (protocol)
	  {
	    case PULSE_LENGTH:
	      bits[i] = (on > threshOn);
	      bytes[i/8] |= bits[i]<<(7-i%8);
	    break;
	    case PULSE_DISTANCE:
	      bits[i] = (off > threshOff);
	      bytes[i/8] |= bits[i]<<(7-i%8);
	    break;
	    case PULSE_BOTH:
	      bits[i] = (on > threshOn);
	      bytes[i/8] |= bits[i]<<(7-i%8);
	      i++;
	      bits[i] = (off > threshOff);
	      bytes[i/8] |= bits[i]<<(7-i%8);
	    break;
	  }
	}
	int place = 0;
	for (int field = 0; field < nFields; field++)
	{
	  data[field] = 0;
	  for (int bi = fieldLengths[field]-1; bi >= 0; bi--)
	  {
	    data[field] |= (bits[place++] << bi);
	  }
	}
	return true;
}