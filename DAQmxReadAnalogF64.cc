// [y, statusCode] = DAQmxReadAnalogF64(taskHandle, maxSamples)
//
// read analog data from task referenced by taskHandle
// return data captured as matrix, channels in columns
//
// NOTES
//   if samplesPerChannel is not given, it will use settings from task
//   samplesPerChannel will be limited by task settings
//   channels ordered as configured in task

// mkoctfile -I. -L. -lNIDAQmx -s DAQmxReadAnalogF64.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

//#define VERBOSE

DEFUN_DLD (DAQmxReadAnalogF64, args, nargout,
"[y, statusCode] = DAQmxReadAnalogF64(taskHandle, maxSamples)\n\
read analog data from task referenced by taskHandle\n\
return data captured as matrix, channels in columns\n\
NOTES\n\
   if samplesPerChannel is not given, it will use settings from task\n\
   samplesPerChannel will be limited by task settings\n\
   channels ordered as configured in task")
{
  if (args.length()<1)
    error("expect task handle as argument");

  if (!args(0).is_scalar_type())
    error("expect task handle as argument");
  uint64NDArray arr = args(0).uint64_array_value();
  uint64_t intHandle = arr(0);
  TaskHandle taskHandle = reinterpret_cast<TaskHandle>(intHandle);

  int32 statusCode = 0;

	uInt64 sampPerChan = 0;
  if (args.length()==1)
    statusCode = DAQmxGetSampQuantSampPerChan(taskHandle, &sampPerChan);
  else
  {
    if (!args(1).is_scalar_type())
      error("expect number of samples per channel as 2nd argument");
    sampPerChan = args(1).int_value();
  }

  // number of channels
  uInt32 numChannels = 0;
	if (statusCode>=0)
    statusCode = DAQmxGetTaskNumChans(taskHandle, &numChannels);
  // sample rate (to calculate timeout and build time vector)
	float64 sampClkRate = 0.0f;
  float64 timeout = 1.0;
  // tasks not using timed output will fail
  // TODO might be replaced by some attribute read
  DAQmxGetSampClkRate(taskHandle, &sampClkRate);
  if (sampClkRate>0)
    timeout += sampPerChan/sampClkRate; // time for data + 1 second

  // we will fill data by scan, as we could get less than requested
  Matrix y(numChannels, sampPerChan); // one column per scan
  float64 *dest = y.rwdata();
  bool32 dataLayout = DAQmx_Val_GroupByScanNumber; // octave store data in columns

	int32 samplesRead = 0;
  if (statusCode>=0)
  {
#ifdef VERBOSE
		printf("Acquiring %lld samples from %ld channels at %f samples per second\n", sampPerChan, numChannels, sampClkRate);
#endif
	  // read result
    statusCode = DAQmxReadAnalogF64(taskHandle, sampPerChan, timeout, dataLayout, dest, numChannels*sampPerChan, &samplesRead, NULL);
  }
#ifdef VERBOSE
  printf("%d samples acquired\n", samplesRead);
#endif

  octave_value_list retval(2);
  if (samplesRead < sampPerChan)
    y.resize(numChannels, samplesRead); // cut unused columns (samples)
  retval(0) = octave_value(y.transpose()); // one channel per column
  retval(1) = octave_value(statusCode);
  return retval;
}

