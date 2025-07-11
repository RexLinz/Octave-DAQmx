// errorString = DAQmxGetExtendedErrorInfo()
//
// return text for last error

// compile and link:
// mkoctfile -I. -L. -lNIDAQmx -s DAQmxGetExtendedErrorInfo.cc

#include <iostream>
#include <octave/oct.h>
#include <NIDAQmx.h>

DEFUN_DLD (DAQmxGetExtendedErrorInfo, args, nargout,
  "errorString = DAQmxGetExtendedErrorInfo()")
{
  if (args.length()!=0)
    error("this function has no arguments");

   // read buffer size required
  int32 responseLength = 0;
  responseLength = DAQmxGetExtendedErrorInfo(NULL, 0);
  if (responseLength>0)
  { // error message available
    char *errorString = new char[responseLength];
    DAQmxGetExtendedErrorInfo(errorString, responseLength);
    return octave_value(errorString);
  }
  else
  { // no error pending
    char noError = 0;
    return octave_value(noError);
  }
}

