% recompile all DAQmx oct files
%
% required include and lib files could be found in the
% external C compiler support folder provided by National Instruments
% C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\
%   include\NIDAQmx.h
%   lib64\msvc\NIDAQmx.lib
% copy these files to the local folder to be able to compile

% TODO functions to improve usabiltity in future
% * add help to functions and/or create DAQmx.m (function overview or just help)
% * pass statusCode as int32
% * access digital ports (read and write)
% * write to DAC

% In some cases mkoctfile could not write output files because
% they have not been proper closed (may happen on error conditions)
% "clear all" usually helps in that case
clear all

% define a function to compile to make changing options more easy
function compile(sourcefile)
  disp(["compiling " sourcefile]);
  % -s to strip debugging in formation
  % -Wno-deprecated to skip deprecated warnings
%  [output, status] = mkoctfile("-I.", "-L.", "-lNIDAQmx", sourcefile);
  [output, status] = mkoctfile("-I.", "-L.", "-lNIDAQmx", "-s", sourcefile);
%  [output, status] = mkoctfile("-I.", "-L.", "-lNIDAQmx", "-s", "-Wno-deprecated", sourcefile);
  if length(output)>0
    disp(output);
  end
  if status~=0
    error(["compile failed on file " sourcefile]);
  end
endfunction

% load a task
compile("DAQmxLoadTask.cc");
% get  some task information
compile("DAQmxGetSampQuantSampPerChan.cc");
compile("DAQmxGetTaskNumChans.cc");
compile("DAQmxGetSampClkRate.cc");
compile("DAQmxStartTask.cc");
% read and write analog data using task
compile("DAQmxReadAnalogF64.cc");
compile("DAQmxWriteAnalogF64.cc");
% read and write digital data using task
compile("DAQmxReadDigitalLines.cc");
compile("DAQmxWriteDigitalLines.cc");
% stop and clear task
compile("DAQmxStopTask.cc");
compile("DAQmxClearTask.cc");
% get error information from statusCode
compile("DAQmxGetErrorString.cc"); % error with specific code
compile("DAQmxGetExtendedErrorInfo.cc"); % last pending error

% complete example of acquiring analog data as a single CC file
% might be more efficient to modify for some applications
% which read just a single block of data on demand
compile("DAQmxAcquireAnalogF64.cc");

