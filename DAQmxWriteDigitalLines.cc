// [samplesWritten, statusCode] = DAQmxWriteDigitalLines(taskHandle, pattern)
//
// write digital pattern to DAC channels

// mkoctfile -I. -L. -lNIDAQmx -s DAQmxWriteDigitalLines.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

// #define VERBOSE

DEFUN_DLD (DAQmxWriteDigitalLines, args, nargout,
"[samplesWritten, statusCode] = DAQmxWriteDigitalLines(taskHandle, pattern)\n\
write 0/1 pattern to digital outputs using task referenced by taskHandle\n\
NOTES\n\
 - implementation using autoStart=1\n\
 - channels ordered as configured in task, channels in columns\n\
 - will give error or unexpected output if pattern width do not match number of channels in task\n\
 - function has been tested for output 'on demand' only as I have no device supporting timed output")
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
    error("expect binary data to write");
  uint8NDArray pattern = args(1).array_value();
  uInt8 *src = reinterpret_cast<uInt8*>(pattern.rwdata()); //uInt8
  bool32 dataLayout = DAQmx_Val_GroupByChannel; // octave store data in columns

  uInt32 numChannels = pattern.columns();
  int32 sampPerChan = pattern.rows();

  bool32 autoStart = 1;
  // sample rate (to calculate timeout and build time vector)
	float64 sampClkRate = 0.0f;
  float64 timeout = 1.0;
  // tasks not using timed output will fail to read sampClkRate
  // TODO might be replaced by some attribute read
  DAQmxGetSampClkRate(taskHandle, &sampClkRate);
  if (sampClkRate>0)
    timeout += sampPerChan/sampClkRate; // time for data + 1 second

#ifdef VERBOSE
  printf("Writing %lld samples for %ld channels at %f samples per second\n", sampPerChan, numChannels, sampClkRate);
#endif
	  // result
  int32 samplesWritten = 0;
  int32 statusCode = 0;
  statusCode = DAQmxWriteDigitalLines(taskHandle, sampPerChan, autoStart, timeout, dataLayout, src, &samplesWritten, NULL);

#ifdef VERBOSE
  printf("%d samples written\n", samplesWritten);
#endif

  octave_value_list retval(2);
  retval(0) = octave_value(samplesWritten);
  retval(1) = octave_value(statusCode);

  return retval;
}

