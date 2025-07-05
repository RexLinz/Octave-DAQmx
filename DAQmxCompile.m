% recompile all DAQmx oct files
%
% required include and lib files could be found in the
% external C compiler support folder provided by National Instruments
% C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\
%   include\NIDAQmx.h
%   lib64\msvc\NIDAQmx.lib
% copy these files to the local folder to be able to compile

% TODO functions to improve usabiltity in future
% * errorString = DAQmxGetErrorString(errorCode)
% * -> remove printing error from functions then
% * access digital ports (read and write)
% * write to DAC

% load a task
mkoctfile -I. -L. -lNIDAQmx -s DAQmxLoadTask.cc
% get  some task information
mkoctfile -I. -L. -lNIDAQmx -s DAQmxGetSampQuantSampPerChan.cc
mkoctfile -I. -L. -lNIDAQmx -s DAQmxGetTaskNumChans.cc
mkoctfile -I. -L. -lNIDAQmx -s DAQmxGetSampClkRate.cc
% start task and read data using task
mkoctfile -I. -L. -lNIDAQmx -s DAQmxStartTask.cc
mkoctfile -I. -L. -lNIDAQmx -s DAQmxReadAnalogF64.cc
% stop and clear task
mkoctfile -I. -L. -lNIDAQmx -s DAQmxStopTask.cc
mkoctfile -I. -L. -lNIDAQmx -s DAQmxClearTask.cc

% complete example of acquiring analog data as a single CC file
% might be more efficient to modify for some applications
% which read just a single block of data on demand
mkoctfile -I. -L. -lNIDAQmx -s DAQmxAcquireAnalogF64.cc

