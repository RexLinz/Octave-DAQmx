// statusCode = DAQmxClearTask(taskHandle)
//
// clear the task referenced by taskHandle
// NOTE: a cleared task could NOT be restarted again

// compile and link:
// mkoctfile -I. -L. -lNIDAQmx -s DAQmxClearTask.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

DEFUN_DLD (DAQmxClearTask, args, nargout,
"statusCode = DAQmxClearTask(taskHandle)\n\
clear the task referenced by taskHandle\n\
NOTE: a cleared task could NOT be restarted again")
{
  if (args.length()!=1)
    error("invalid number of input arguments");
  if (!args(0).is_scalar_type())
    error("expect task handle as argument");

  uint64NDArray arr = args(0).uint64_array_value();
  uint64_t intHandle = arr(0);
  TaskHandle taskHandle = reinterpret_cast<TaskHandle>(intHandle);
//  printf("clearing handle = %lld\n", intHandle);

  int32 statusCode = 0;
  statusCode = DAQmxClearTask(taskHandle);

//  return octave_value(static_cast<uInt64>(ptr_as_int));
  return octave_value(statusCode);

}
