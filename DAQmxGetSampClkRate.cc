// [sampleClockRate, statusCode] = DAQmxGetSampClkRate(taskHandle)
//
// get the sample clock the task referenced by taskHandle is using

// mkoctfile -I. -L. -lNIDAQmx -s DAQmxGetSampClkRate.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

DEFUN_DLD (DAQmxGetSampClkRate, args, nargout,
"[sampleClockRate, statusCode] = DAQmxGetSampClkRate(taskHandle)\n\
get the sample clock the task referenced by taskHandle is using")
{
  if (args.length()<1)
    error("expect task handle as argument");

  if (!args(0).is_scalar_type())
    error("expect task handle as argument");
  uint64NDArray arr = args(0).uint64_array_value();
  uint64_t intHandle = arr(0);
  TaskHandle taskHandle = reinterpret_cast<TaskHandle>(intHandle);

  int32 statusCode = 0;
	float64 sampClkRate = 0.0f;
  statusCode = DAQmxGetSampClkRate(taskHandle, &sampClkRate);

  octave_value_list retval(2);
  retval(0) = octave_value(sampClkRate);
  retval(1) = octave_value(statusCode);
  return retval;
}

