// statusCode = DAQmxStopTask(taskHandle)
//
// stop the task referenced by taskHandle
// NOTE: a stopped task could be restarted

// compile and link:
// mkoctfile -I. -L. -lNIDAQmx -s DAQmxStopTask.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

DEFUN_DLD (DAQmxStopTask, args, nargout,
  "statusCode = DAQmxStopTask(taskHandle)\n\
stop the task referenced by taskHandle\n\
NOTE: a stopped task could be restarted")
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
  statusCode = DAQmxStopTask(taskHandle);

  return octave_value(int32NDArray(dim_vector(1,1), statusCode));

}
