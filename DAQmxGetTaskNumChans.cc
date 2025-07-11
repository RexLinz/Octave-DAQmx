// [numChannels, statusCode] = DAQmxGetTaskNumChans(taskHandle)
//
// get number of channels the task referenced by taskHandle is using

// mkoctfile -I. -L. -lNIDAQmx -s DAQmxGetTaskNumChans.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

DEFUN_DLD (DAQmxGetTaskNumChans, args, nargout,
  "[numChannels, statusCode] = DAQmxGetTaskNumChans(taskHandle)")
{
  if (args.length()<1)
    error("expect task handle as argument");

  if (!args(0).is_scalar_type())
    error("expect task handle as argument");
  uint64NDArray arr = args(0).uint64_array_value();
  uint64_t intHandle = arr(0);
  TaskHandle taskHandle = reinterpret_cast<TaskHandle>(intHandle);

  int32 statusCode = 0;
	uInt32 numChannels = 0;
  statusCode = DAQmxGetTaskNumChans(taskHandle, &numChannels);

  octave_value_list retval(2);
  retval(0) = octave_value(numChannels);
  retval(1) = octave_value(statusCode);
  return retval;
}

