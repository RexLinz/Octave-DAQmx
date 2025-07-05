// read analog data from task referenced by taskHandle
//
// return data captured as matrix, channels in columns
//
// NOTES
//   if samplesPerChannel is not given, it will use settings from task
//   samplesPerChannel will be limited by task settings
//   channels ordered as configured in task

// mkoctfile -I. -L. -lNIDAQmx DAQmxReadAnalogF64.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

//#define VERBOSE

char  errBuff[2048] = { '\0' };

DEFUN_DLD (DAQmxReadAnalogF64, args, nargout,
  "[y, statusCode] = DAQmxReadAnalogF64(taskHandle, maxSamples)")
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
  if (statusCode>=0)
    statusCode = DAQmxGetSampClkRate(taskHandle, &sampClkRate);

  // now read data
	int32 samplesRead = 0;
	float64 *data = NULL;
  if (statusCode>=0)
  {
#ifdef VERBOSE
		printf("Acquiring %lld samples from %ld channels at %f samples per second\n", sampPerChan, numChannels, sampClkRate);
#endif
    data = new float64[sampPerChan*numChannels]; // malloc(sampPerChan * numChannels * sizeof(float64));
	  // read result
    statusCode = DAQmxReadAnalogF64(taskHandle, sampPerChan, 1.0+ sampPerChan / sampClkRate, DAQmx_Val_GroupByScanNumber, data, numChannels * sampPerChan, &samplesRead, NULL);
  }

	if (statusCode<0)
  {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
  }
#ifdef VERBOSE
  if (data!=NULL)
    printf("%d samples acquired\n", samplesRead);
#endif

  Matrix y(samplesRead,numChannels); // channels in columns
  float64 *src = data;
  for (int s = 0; s < samplesRead; s++)
  {
    for (int c = 0; c < numChannels; c++)
      y(s,c) = *(src++);
  }
  delete []data;  // Clean up the dynamically allocated memory

  octave_value_list retval(2);
  retval(0) = octave_value(y);
  retval(1) = octave_value(statusCode);
  return retval;
}

