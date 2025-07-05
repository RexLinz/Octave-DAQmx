// [taskHandle, statusCode] = DAQmxLoadTask(taskName)
//
// load a DAQmx task configured in NI MAX
// return task handle

// compile and link:
// mkoctfile -I. -L. -lNIDAQmx DAQmxLoadTask.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

char  errBuff[2048] = { '\0' };

DEFUN_DLD (DAQmxLoadTask, args, nargout,
  "[taskHandle, statusCode] = DAQmxLoadTask(taskName)")
{
  if (args.length()!=1)
    error("invalid number of input arguments");
  if (nargout<1)
    error("invalid number of output arguments");
  if (!args(0).is_string())
    error("expect task name string");
  const char *taskName = args(0).string_value().c_str();
//  printf("loading task %s\n", taskName);

  TaskHandle taskHandle = 0;
  int32 statusCode = 0;
  statusCode = DAQmxLoadTask(taskName, &taskHandle);

	if (statusCode<0)
  {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
  }

  octave_value_list retval(2);
// returning a void* pointer as uint64
  uintptr_t ptr_as_int = reinterpret_cast<uintptr_t>(taskHandle);
//  return octave_value(static_cast<uInt64>(ptr_as_int));
  retval(0) = octave_value(uint64NDArray(dim_vector(1,1), ptr_as_int));
  retval(1) = octave_value(statusCode);

  return retval;
}
