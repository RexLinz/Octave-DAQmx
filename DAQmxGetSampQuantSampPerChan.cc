// [sampPerChan, statusCode] = DAQmxGetSampQuantSampPerChan(taskHandle)
//
// get number of samples per channel the task referenced by taskHandle is using

// mkoctfile -I. -L. -lNIDAQmx -s DAQmxGetSampQuantSampPerChan.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

DEFUN_DLD (DAQmxGetSampQuantSampPerChan, args, nargout,
  "[sampPerChan, statusCode] = DAQmxGetSampQuantSampPerChan(taskHandle)")
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
  statusCode = DAQmxGetSampQuantSampPerChan(taskHandle, &sampPerChan);

  octave_value_list retval(2);
  retval(0) = octave_value(sampPerChan);
  retval(1) = octave_value(statusCode);
  return retval;
}

