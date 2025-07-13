% test DAQmx functions
%
% acquire analog data from task configured in MAX
% This example is following the workflow in LabVIEW
%
% The example below require you have set up an analog acquisition task in NI Max.
% I have configured a task **SimTask** to acquire 2000 samples from 4 channels.
% You might include custom scaling of channels defining your task.

disp("loading task configured in MAX");
[taskHandle, statusCode] = DAQmxLoadTask("SimTask"); % finite samples
%[taskHandle, statusCode] = DAQmxLoadTask("StepperMotor"); % continous
if statusCode<0;
  error("load task failed");
end

sampPerChannel = DAQmxGetSampQuantSampPerChan(taskHandle);
numChannels = DAQmxGetTaskNumChans(taskHandle);
sampleRate = DAQmxGetSampClkRate(taskHandle);
printf("  task is configured to read %d samples from %d channels at %f Hz\n",
  sampPerChannel, numChannels, sampleRate);

disp("starting task");
statusCode = DAQmxStartTask(taskHandle);
if statusCode<0
  warning("starting task failed");
end

disp("acquiring data");
tic
for n=1:1 % change to test continous acquisition
  % read the number of samples defined in task (or any number you like)
  % will get less values if we try to read too many from task with finite samples
  [y, statusCode] = DAQmxReadAnalogF64(taskHandle, sampPerChannel);
  if statusCode<0
    disp("  acquire failed");
    break;
  end
end
disp(["  took " num2str(toc) " seconds"]);
  t = (1:rows(y))' / sampleRate; % we might get less data than expected
  plot(1000*t, y); grid on;
  drawnow;
xlabel("t [ms]");
legend({"channel 1", "channel 2", "channel 3", "channel 4"});
%legend({"current A", "current B", "voltage A", "voltage B"});

% optional if task is cleared afterwards
% required if you like to restart the task again
disp("stopping task");
statusCode = DAQmxStopTask(taskHandle);
if statusCode<0
  warning("stopping task failed");
end

disp("clearing task");
statusCode = DAQmxClearTask(taskHandle); clear taskHandle;
if statusCode<0
  error("clear task failed");
end

