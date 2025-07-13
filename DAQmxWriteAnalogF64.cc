// statusCode = DAQmxWriteAnalogF64(taskHandle, data)
//
// write analog data to DAC channels

// mkoctfile -I. -L. -lNIDAQmx -s DAQmxWriteAnalogF64.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

#define VERBOSE

DEFUN_DLD (DAQmxWriteAnalogF64, args, nargout,
"statusCode = DAQmxWriteAnalogF64(taskHandle, data)\n\
write analog data to DAC output task referenced by taskHandle\n\
NOTES\n\
 - implementation using autoStart=1\n\
 - channels ordered as configured in task, channels in columns\n\
 - function has been tested for output 'on demand' only as I have no device supporting other")
{
  if (args.length()<1)
    error("expect task handle as argument");
  if (!args(0).is_scalar_type())
    error("expect task handle as argument");
  uint64NDArray arr = args(0).uint64_array_value();
  uint64_t intHandle = arr(0);
  TaskHandle taskHandle = reinterpret_cast<TaskHandle>(intHandle);

  if (args.length()<2)
    error("no data given");
  if (!args(1).isnumeric())
    error("expect numeric data to write");
  Matrix data = args(1).array_value();

  uInt32 numChannels = data.columns();
  int32 sampPerChan = data.rows();

  // might use Method: float64 *src = data.rwdata(); to access data directly
  // in best case we might be able to use the array without conversion
  // octave store data in columns, so use DAQmx_Val_GroupBySampleNumber

  OCTAVE_LOCAL_BUFFER (float64, writeArray, numChannels*sampPerChan);
//  float64 *src = data.rwdata();
  float64 *dest = writeArray;
  for (int s = 0; s < sampPerChan; s++)
  {
    for (int c = 0; c < numChannels; c++)
    {
      *(dest++) = data(s,c);
//      printf("%f\n", *src++);
    }
  }

  // now write data
  int32 statusCode = 0;

  bool32 autoStart = 1;
  // sample rate (to calculate timeout and build time vector)
	float64 sampClkRate = 0.0f;
  float64 timeout = 1.0;
  // devices not supporting timed output will fail
  DAQmxGetSampClkRate(taskHandle, &sampClkRate);
  if (sampClkRate>0)
    timeout += sampPerChan/sampClkRate; // time for data + 1 second
  bool32 dataLayout = DAQmx_Val_GroupByScanNumber;

#ifdef VERBOSE
  printf("Writing %lld samples for %ld channels at %f samples per second\n", sampPerChan, numChannels, sampClkRate);
#endif
	  // result
  int32 samplesWritten = 0;
  statusCode = DAQmxWriteAnalogF64(taskHandle, sampPerChan, autoStart, timeout, dataLayout, writeArray, &samplesWritten, NULL);

#ifdef VERBOSE
  printf("%d samples written\n", samplesWritten);
#endif

  return octave_value(statusCode);
}

