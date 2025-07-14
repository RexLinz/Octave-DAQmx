% DAQmxTestDigital

clear

switch 3
  case 1 % digital input
    disp("loading digital input task configured in MAX");
    [taskHandle, statusCode] = DAQmxLoadTask("SimDigIn"); % finite samples
    if statusCode<0;
      error("load task failed");
    end

    disp("starting task");
    statusCode = DAQmxStartTask(taskHandle);
    if statusCode<0
      warning("starting task failed");
    end

    disp("acquiring data");
    [pattern, statusCode] = DAQmxReadDigitalLines(taskHandle, 1); % 1 sample is default

    disp("clearing task");
    statusCode = DAQmxClearTask(taskHandle); clear taskHandle;
    if statusCode<0
      error("clear task failed");
    end

  case 2 % digital output
    disp("loading digital output task configured in MAX");
    [taskHandle, statusCode] = DAQmxLoadTask("SimDigOut"); % finite samples
    if statusCode<0;
      error("load task failed");
    end

    % optional as DAQmxWriteDigitalLines using autoStart=1
    %disp("starting task");
    %statusCode = DAQmxStartTask(taskHandle);
    %if statusCode<0
    %  warning("starting task failed");
    %end

    disp("writing data");
    [samplesWritten, statusCode] = DAQmxWriteDigitalLines(taskHandle,[1 0 0 1]);

    disp("clearing task");
    statusCode = DAQmxClearTask(taskHandle); clear taskHandle;
    if statusCode<0
      error("clear task failed");
    end

  case 3 % digital loopback
    % NOTE: running two tasks in parallel look like not working on simulated device
    disp("loading and starting digital input task configured in MAX");
    [inTaskHandle, statusCode] = DAQmxLoadTask("TestDigIn"); % finite samples
    if statusCode<0;
      error("load task failed");
    end
    statusCode = DAQmxStartTask(inTaskHandle);
    if statusCode<0
      warning("starting task failed");
    end

    disp("loading digital output task configured in MAX");
    [outTaskHandle, statusCode] = DAQmxLoadTask("TestDigOut"); % finite samples
    if statusCode<0;
      error("load output task failed");
    end

    disp("reading inputs, write to output");
    disp("... press any key to stop");
    while (length(kbhit(1))==0) && (statusCode==0)
      [pattern, statusCode] = DAQmxReadDigitalLines(inTaskHandle, 1);
      % input task is 4 bits, output task 8 bits -> add data for remaining lines
      [samplesWritten, statusCode] = DAQmxWriteDigitalLines(outTaskHandle, [pattern 0 0 1 1]);
    end
    if (statusCode<0)
      DAQmxGetExtendedErrorInfo
    end

    disp("clearing tasks");
    statusCode = DAQmxClearTask(inTaskHandle); clear inTaskHandle;
    if statusCode<0
      error("clear input task failed");
    end
    statusCode = DAQmxClearTask(outTaskHandle); clear outTaskHandle;
    if statusCode<0
      error("clear output task failed");
    end

  otherwise
    disp("select test case");
end

