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
% [pattern, statusCode] = DAQmxReadDigitalLines(taskHandle, maxSamples)
%   read digital inputs
% [samplesWritten, statusCode] = DAQmxWriteDigitalLines(taskHandle, pattern)
%   write digital outputs
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

% POSSIBLE FUTURE EXTENSIONS

% read multiple channels as one byte
% int32 DAQmxReadDigitalU8             (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, uInt8 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);
% int32 DAQmxWriteDigitalU8            (TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, const uInt8 writeArray[], int32 *sampsPerChanWritten, bool32 *reserved);

% create task
% int32 DAQmxCreateTask                (const char taskName[], TaskHandle *taskHandle);
% int32 DAQmxCreateAIVoltageChan       (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], int32 terminalConfig, float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);
% int32 DAQmxCreateAICurrentChan       (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], int32 terminalConfig, float64 minVal, float64 maxVal, int32 units, int32 shuntResistorLoc, float64 extShuntResistorVal, const char customScaleName[]);
% int32 DAQmxCreateAOVoltageChan       (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);
% int32 DAQmxCreateAOCurrentChan       (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);
% int32 DAQmxCreateDIChan              (TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);
% int32 DAQmxCreateDOChan              (TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);
% int32 DAQmxSaveTask                  (TaskHandle taskHandle, const char saveAs[], const char author[], uInt32 options);
% int32 DAQmxSaveGlobalChan            (TaskHandle taskHandle, const char channelName[], const char saveAs[], const char author[], uInt32 options);

% timing and trigger
% int32 DAQmxCfgSampClkTiming          (TaskHandle taskHandle, const char source[], float64 rate, int32 activeEdge, int32 sampleMode, uInt64 sampsPerChan);
% int32 DAQmxDisableStartTrig          (TaskHandle taskHandle);

% custom scale
% int32 DAQmxCreateLinScale            (const char name[], float64 slope, float64 yIntercept, int32 preScaledUnits, const char scaledUnits[]);
% int32 DAQmxCreateMapScale            (const char name[], float64 prescaledMin, float64 prescaledMax, float64 scaledMin, float64 scaledMax, int32 preScaledUnits, const char scaledUnits[]);
% int32 DAQmxCreatePolynomialScale     (const char name[], const float64 forwardCoeffs[], uInt32 numForwardCoeffsIn, const float64 reverseCoeffs[], uInt32 numReverseCoeffsIn, int32 preScaledUnits, const char scaledUnits[]);
% int32 DAQmxCreateTableScale          (const char name[], const float64 prescaledVals[], uInt32 numPrescaledValsIn, const float64 scaledVals[], uInt32 numScaledValsIn, int32 preScaledUnits, const char scaledUnits[]);
% int32 DAQmxCalculateReversePolyCoeff (const float64 forwardCoeffs[], uInt32 numForwardCoeffsIn, float64 minValX, float64 maxValX, int32 numPointsToCompute, int32 reversePolyOrder, float64 reverseCoeffs[]);
% int32 DAQmxSaveScale                 (const char scaleName[], const char saveAs[], const char author[], uInt32 options);
