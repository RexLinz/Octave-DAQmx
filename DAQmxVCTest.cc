// test calling DAQmx functions
// note this function is not really using the input arguments
// but creating its own task instead

// compile and link:
// mkoctfile -I. -L. -lNIDAQmx DAQmxTest.cc

/* TODO
For continous straming create the following set of functions

[taskHandle, statusCode] = DAQmxLoadTask(taskName);
statusCode = DAQmxClearTask(taskHandle);

statusCode = DAQmxStartTask(handle);
statusCode = DAQmxStopTask(taskHandle);

[sampleClockRate, statusCode] = DAQmxGetSampClkRate(taskHandle);
[y, statusCode] = DAQmxReadAnalogF64(taskHandle, maxSamples);

*/

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

char  errBuff[2048] = { '\0' };

DEFUN_DLD (DAQmxTest, args, nargout,
  "[y, taskHandle] = DAQmxTest(taskName, testUI64)")
{
	uInt64		sampPerChan = 2000;
	uInt32		numChannels = 4;
	float64		sampClkRate = 10000.0f;
	int32     samplesRead = 0;
	float64		*data = NULL;

  printf("number of args passed %d\n", args.length());
  if (args.length()!=2)
    error("invalid number of input arguments");
  if (nargout!=2)
    error("invalid number of output arguments");
  if (!args(0).is_string())
    error("expect task name string");
  if (!args(1).is_scalar_type())
    error("expect int as 2nd argument");

  const char *taskName = args(0).string_value().c_str();
//  uInt64 intTest = args(1).int_value();
//  printf("running task %s, handle = %lld\n", taskName, intTest);
  uint64NDArray arr = args(1).uint64_array_value();
  uint64_t intTest = arr(0);
  TaskHandle testHandle = reinterpret_cast<TaskHandle>(intTest);
  printf("running task %s, handle = %lld\n", taskName, intTest);

  int32 statusCode = 0;
  // create task
  TaskHandle taskHandle = 0;
  statusCode = DAQmxCreateTask("", &taskHandle);
  printf("taskHandle = %lld, status code = %d\n", taskHandle, statusCode);
  // configure channels
  if (statusCode>=0)
    statusCode = DAQmxCreateAIVoltageChan(taskHandle, "USB6009-Sim/ai0", "CurrentA", DAQmx_Val_Diff, -2.5, +2.5, DAQmx_Val_Volts, NULL);
	if (statusCode>=0)
    statusCode = DAQmxCreateAIVoltageChan(taskHandle, "USB6009-Sim/ai2", "CurrentB", DAQmx_Val_Diff, -2.5, +2.5, DAQmx_Val_Volts, NULL);
  if (statusCode>=0)
  	statusCode = DAQmxCreateAIVoltageChan(taskHandle, "USB6009-Sim/ai1", "VoltageA", DAQmx_Val_Diff, -2.4, +2.4, DAQmx_Val_Volts, NULL);
  if (statusCode>=0)
	  statusCode = DAQmxCreateAIVoltageChan(taskHandle, "USB6009-Sim/ai3", "VoltageB", DAQmx_Val_Diff, -2.4, +2.4, DAQmx_Val_Volts, NULL);
	// set sample timing and trigger
  if (statusCode>=0)
	  statusCode = DAQmxCfgSampClkTiming(taskHandle, "", sampClkRate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, sampPerChan);
  if (statusCode>=0)
  {
		printf("Acquiring %lld samples from %ld channels at %f samples per second\n", sampPerChan, numChannels, sampClkRate);
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
	if (taskHandle != 0)
  {
//		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}

  if (data!=NULL)
    printf("%d samples acquired\n", samplesRead);

  NDArray result(dim_vector(samplesRead,numChannels));  // channels in columns
  float64 *src = data;
  for (int s = 0; s < samplesRead; s++)
    for (int c = 0; c < numChannels; c++)
      result(s,c) = *(src++);

  delete []data;  // Clean up the dynamically allocated memory

  octave_value_list retval(2);
  retval(0) = octave_value(result);

// returning a void* pointer as uint64
  uintptr_t ptr_as_int = reinterpret_cast<uintptr_t>(taskHandle);
//  return octave_value(static_cast<uInt64>(ptr_as_int));
  retval(1) =octave_value(uint64NDArray(dim_vector(1,1), ptr_as_int));

  return retval;
}
