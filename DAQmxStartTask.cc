// statusCode = DAQmxStartTask(taskHandle)
//
// start the task referenced by taskHandle

// compile and link:
// mkoctfile -I. -L. -lNIDAQmx DAQmxStartTask.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

char  errBuff[2048] = { '\0' };

DEFUN_DLD (DAQmxStartTask, args, nargout,
  "statusCode = DAQmxStartTask(taskHandle)")
{
  if (args.length()!=1)
    error("expect task handle as argument");
  if (!args(0).is_scalar_type())
    error("expect task handle as argument");

  uint64NDArray arr = args(0).uint64_array_value();
  uint64_t intHandle = arr(0);
  TaskHandle taskHandle = reinterpret_cast<TaskHandle>(intHandle);
//  printf("clearing handle = %lld\n", intHandle);

  int32 statusCode = 0;
  statusCode = DAQmxStartTask(taskHandle);

	if (statusCode<0)
  {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
  }

//  return octave_value(static_cast<uInt64>(ptr_as_int));
  return octave_value(statusCode);

}
