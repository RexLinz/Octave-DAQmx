% DAQmx.m
%
% Overview over DAQmx wrapper functions implemented.
%
% Please use each functions help for more details
% Naming is following the function names from the API.
%
% [taskHandle, statusCode] = DAQmxLoadTask(taskName)
%   Load a task defined in NI MAX and return a task handle
% [sampPerChan, statusCode] = DAQmxGetSampQuantSampPerChan(taskHandle)
%   Get the number of samples per channel defined for that task
% [numChannels, statusCode] = DAQmxGetTaskNumChans(taskHandle)
%   Get the number of channels the task is using
% [numChannels, statusCode] = DAQmxGetTaskNumChans(taskHandle)
%   Get the sampling clock rate the task is using
% statusCode = DAQmxStartTask(taskHandle)
%   Start the task
% [y, statusCode] = DAQmxReadAnalogF64(taskHandle, maxSamples)** \
%   Read analog data from task, channels in columns.
%   If maxSamples is not specified it will return the
%   number of samples specified for the task.
% [samplesWritten, statusCode] = DAQmxWriteAnalogF64(taskHandle, data)
%   Write analog data to DAC
% statusCode = DAQmxStopTask(taskHandle)** \
%   Stop the task. It is not required to stop a task if you clear it next.
% statusCode = DAQmxClearTask(taskHandle)** \
%   Clear a task. A cleared task could not be restarted.
% errorString = DAQmxGetErrorString(statusCode)** \
%   Get string describing given statusCode (error)
% errorString = DAQmxGetExtendedErrorInfo()** \
%   Get string describing last (active) error
%
% NOTES
% - DAQmxLoadTask will fail if you did not clear your task.
%   If you loose your taskHandle you will have to
%   restart Octave to be able to start the same task again.
% - Negative statusCodes are errors, positive are warnings.
%
% EXAMPLES
% DAQmxTestAnalog.m, DAQmxTestDigital
%   Usage of functions step by step with some error checking.
% DAQmxAcquireAnalogF64.cc
%   Usage of NI's C/C++ API in oct files.

