// errorString = DAQmxGetErrorString(statusCode)
//
// return error text for given status code

// compile and link:
// mkoctfile -I. -L. -lNIDAQmx -s DAQmxGetErrorString.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

DEFUN_DLD (DAQmxGetErrorString, args, nargout,
"errorString = DAQmxGetErrorString(statusCode)\n\
return error text for given status code")
{
  if (args.length()!=1)
    error("expect error number as argument");
  if (!args(0).is_scalar_type())
    error("expect error code as argument");
  int32 statusCode = args(0).int_value();

   // read buffer size required
  int32 responseLength = 0;
  responseLength = DAQmxGetErrorString(statusCode, NULL, 0);
  if (responseLength>0)
  { // error message available
    char *errorString = new char[responseLength];
    DAQmxGetErrorString(statusCode, errorString, responseLength);
    return octave_value(errorString);
  }
  else
  { // no error pending
    char noError = 0; // line termination
    return octave_value(noError);
  }
}

