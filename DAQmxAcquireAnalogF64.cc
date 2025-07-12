// [y, t] = DAQmxAcquireAnalogF64(taskName, samplesPerChannel)
//
// Acquire analog data running a task preconfigured in MAX
// if samplesPerChannel is not given, it will use settings from task
//   note samplesPerChannel will be limited by task settings for tasks set to "finite samples"
// return data captured and time vector
// stop and clear task

// mkoctfile -I. -L. -lNIDAQmx -s DAQmxAcquireAnalogF64.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

//#define VERBOSE

char  errBuff[2048] = { '\0' };

DEFUN_DLD (DAQmxAcquireAnalogF64, args, nargout,
"[y, t] = DAQmxAcquireAnalogF64(taskName, samplesPerChannel)\n\
Acquire analog data running a task preconfigured in MAX\n\
if samplesPerChannel is not given, it will use settings from task\n\
  note samplesPerChannel will be limited by task settings for tasks set to 'finite samples'\n\
return data captured and time vector\n\
stop and clear task")
{
  if (args.length()<1)
    error("invalid number of input arguments");
  if (!args(0).is_string())
    error("expect task name string as 1st argument");
  const char *taskName = args(0).string_value().c_str();

	uInt64 sampPerChan = 0;
  if (args.length()>=2)
  {
    if (!args(1).is_scalar_type())
      error("expect number of samples per channel as 2nd argument");
    sampPerChan = args(1).int_value();
  }

  int32 statusCode = 0;
  // create task
  TaskHandle taskHandle = NULL;
  statusCode = DAQmxLoadTask(taskName, &taskHandle);
//  printf("taskHandle = %lld, status code = %d\n", taskHandle, statusCode);

// get some task settings
   // number of samples per channel if not given as parameter
  if ((statusCode>=0) && (sampPerChan==0))
	  statusCode = DAQmxGetSampQuantSampPerChan(taskHandle, &sampPerChan);
  // number of channels
  uInt32 numChannels = 0;
	if (statusCode>=0)
    statusCode = DAQmxGetTaskNumChans(taskHandle, &numChannels);
  // sample rate (to calculate timeout and build time vector)
	float64		sampClkRate = 0.0f;
  if (statusCode>=0)
    statusCode = DAQmxGetSampClkRate(taskHandle, &sampClkRate);

// now run the task
	int32 samplesRead = 0;
	float64 *data = NULL;
  if (statusCode>=0)
  {
#ifdef VERBOSE
		printf("Acquiring %lld samples from %ld channels at %f samples per second\n", sampPerChan, numChannels, sampClkRate);
#endif
    data = new float64[sampPerChan*numChannels]; // malloc(sampPerChan * numChannels * sizeof(float64));
  	// start task
  	statusCode = DAQmxStartTask(taskHandle);
	  // read result
	  if (statusCode>=0)
      statusCode = DAQmxReadAnalogF64(taskHandle, sampPerChan, 1.0+ sampPerChan / sampClkRate, DAQmx_Val_GroupByScanNumber, data, numChannels * sampPerChan, &samplesRead, NULL);
  }

	if (statusCode<0)
  {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
  }
	if (taskHandle != NULL)
		DAQmxClearTask(taskHandle);

#ifdef VERBOSE
  if (data!=NULL)
    printf("%d samples acquired\n", samplesRead);
#endif

  // prepare results
  octave_value_list retval(nargout);

  Matrix y(samplesRead,numChannels); // channels in columns
  float64 *src = data;
  for (int s=0; s<samplesRead; s++)
  {
    for (int c=0; c<numChannels; c++)
      y(s,c) = *(src++); // data must be sorted by sample (DAQmx_Val_GroupByScanNumber)
  }
  delete []data;  // Clean up the dynamically allocated memory
  retval(0) = octave_value(y);

  if (nargout>1)
  { // provide time vector
    Matrix t(samplesRead,1);  // time vector
    for (int s=0; s<samplesRead; s++)
      t(s) = s/sampClkRate;
    retval(1) = octave_value(t);
  }

  return retval;
}

