# Set of oct files to add access to DAQmx devices to GNU Octave on Windows

## Preface
DAQmx is a National Instruments standard library providing an API for data acquisition devices to several programming
languages from National Instruments like LabVIEW and LabWindows/CVI, but also providing a C/C++ API.

This files in this collection are provides without any warranty in the hope they migth be useful. You can use, modify and redistribute without restrictions.

## Functions provided
At the moment all API functions to acquire analog data with a task configured in NI-MAX are packed
from the C/C++ API to oct files. Naming is following the function names from the API.

- **[taskHandle, statusCode] = DAQmxLoadTask(taskName);** \
  Load a task defined in NI MAX and return a task handle

- **[sampPerChan, statusCode] = DAQmxGetSampQuantSampPerChan(taskHandle);** \
  Get the number of samples per channel defined for that task

- **[numChannels, statusCode] = DAQmxGetTaskNumChans(taskHandle)** \
  Get the number of channels the task is using

- **[numChannels, statusCode] = DAQmxGetTaskNumChans(taskHandle)** \
  Get the sampling clock rate the task is using

- **statusCode = DAQmxStartTask(taskHandle)** \
  Start the task

- **[y, statusCode] = DAQmxReadAnalogF64(taskHandle, maxSamples)** \
  Read analog data from task, channels in columns.
  If maxSamples is not specified it will return the number of samples specified for the task.

- **statusCode = DAQmxStopTask(taskHandle)** \
  Stop the task. It is not required to stop a task if you clear it next.

- **statusCode = DAQmxClearTask(taskHandle)** \
  Clear a task. A cleared task could not be restarted.

- **errorString = DAQmxGetErrorString(statusCode)** \
  Get string describing given statusCode (error)

- **errorString = DAQmxGetExtendedErrorInfo()** \
  Get string describing last (active) error

**NOTES**

- DAQmxLoadTask will fail if you did not clear your task.
  If you loose your taskHandle you will have to restart Octave to be able to start the same task again.
- Negative statusCodes are errors, positive are warnings.

## Examples

The examples below require you have set up an analog acquisition task in NI Max.
I have configured a task **SimTask** to acquire 2000 samples from 4 channels.
You might include custom scaling of channels defining your task.

File **DAQmxTest.m** showing the usage step by step with some error checking.

An example of acquiring analog data with a single call is provided in **DAQmxAcquireAnalogF64.cc**.
This might be a useful start if you like to have just a single call from your
Octave application without the need of supporting continous acquisition. \
**[y, t] = DAQmxAcquireAnalogF64(taskName, samplesPerChannel)**

## Compiling
Licencing of the National Instruments files does not allow to distribute the required header **NIDAQmx.h**
and library file **NIDAQmx.lib**. Please follow the instructions in **NIDAQmxCompile.m** to copy the files from your
National Instruments "External Compiler Support" folders. Then you can run **NIDAQmxCompile.m** to get the compiled oct files.

To use the functions you won't have to copy the CC files to your application folder.
Your application just need access to the compiled oct files.
So either copy the ones you are actually using to your application folder or add the folder to your path instead.

**NOTE** At the moment the binutils coming with Octave up to 10.2 cause mkoctfile to fail linking with the libraries
created by MSVC compiler. This will be fixed in Octave 10.3.
For now you can use the nightly Octave 10.2.1 build from the **mxe-default section** at [Octave nightly builds](https://nightly.octave.org/#/download).
