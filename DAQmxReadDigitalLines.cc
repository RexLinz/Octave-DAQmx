// [pattern, statusCode] = DAQmxReadDigitalLines(taskHandle, maxSamples)
//
// read digital data from task referenced by taskHandle
// return 0/1 pattern captured as matrix, channels in columns
//
// NOTES
//   if samplesPerChannel is not given, it will use settings from task
//   samplesPerChannel will be limited by task settings
//   channels ordered as configured in task

// mkoctfile -I. -L. -lNIDAQmx -s DAQmxReadDigitalLines.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

// #define VERBOSE

DEFUN_DLD (DAQmxReadDigitalLines, args, nargout,
"[pattern, statusCode] = DAQmxReadDigitalU32(taskHandle, maxSamples)\n\
read digital inputs using task referenced by taskHandle\n\
return 0/1 pattern captured as matrix, channels in columns\n\
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
  {
    statusCode = DAQmxGetSampQuantSampPerChan(taskHandle, &sampPerChan);
    if (sampPerChan==0)
    {
      sampPerChan = 1; // set to 1 if we could not read from task
      statusCode = 0; // reset
    }
  }
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
  uint8NDArray pattern(dim_vector(numChannels, sampPerChan)); // one column per scan
  uInt8 *dest = reinterpret_cast<uInt8*>(pattern.rwdata()); //uInt8
  bool32 fillMode = DAQmx_Val_GroupByScanNumber; // octave store data in columns

	int32 samplesRead = 0;
  int32 bytesPerSamp = 0;
  if (statusCode>=0)
  {
#ifdef VERBOSE
		printf("Acquiring %lld samples from %ld channels at %f samples per second\n", sampPerChan, numChannels, sampClkRate);
#endif
	  // read result
    statusCode = DAQmxReadDigitalLines(taskHandle, sampPerChan, timeout, fillMode, dest, numChannels*sampPerChan, &samplesRead, &bytesPerSamp, NULL);
  }
#ifdef VERBOSE
  printf("%d samples acquired, %d bytes per sample\n", samplesRead, bytesPerSamp);
#endif

  octave_value_list retval(2);
// TODO
// remove unused samples from result array in case we have read less data than expected
// -> find  function matching resize for uint8NDArray
//  if (samplesRead < sampPerChan)
//    pattern.resize(numChannels, samplesRead); // cut unused columns (samples)
  retval(0) = octave_value(pattern.transpose()); // one channel per column
  retval(1) = octave_value(statusCode);
  return retval;
}

