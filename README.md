# Set of oct files to add access to DAQmx devices to GNU Octave on Windows

## Preface

DAQmx is a National Instruments standard library providing an API for data acquisition devices to several programming
languages from National Instruments like LabVIEW and LabWindows/CVI, but also providing a C/C++ API.

This collection is wrapping functions from the C/C++ API into oct files to be used in GNU Octave.

## Functions provided

Please look at file **DAQmx.m** or type help DAQmx from within Octave for a full list of functions provided.

## Examples

- **DAQmxTestAnalog.m** showing analog input step by step with some error checking.

- **DAQmxTestDigital.m** showing digital IO step by step with some error checking.

- **DAQmxAcquireAnalogF64.cc** is an example of how to use NI's C/C++ API in oct files.

## Further reading

- **NIDAQmx.pdf**
describing general workflow using DAQmx functions.

- **daqmxcfunc.chm**
compiled help file for the DAQmx C API.

These files are provided by National Instruments with your installation.

## Installation / Compiling

Licencing of the National Instruments files does not allow to distribute the required header **NIDAQmx.h**
and library file **NIDAQmx.lib**. Please follow the instructions in **NIDAQmxCompile.m** to copy the files from your
National Instruments "External Compiler Support" folders.
Then you can run **NIDAQmxCompile.m** to get the compiled oct files.

To use the functions you won't have to copy the CC files to your application folder.
Your application just need access to the compiled oct files.
So either copy the ones you are actually using to your application folder or add the folder to your path instead.

**NOTE** Binutils coming with Octave up to 10.2 cause mkoctfile to fail linking with the libraries
created by MSVC compiler. This will be fixed in Octave 10.3.
For now you can use the nightly Octave 10.2.1 build from the **mxe-default** or **w64** section
at [Octave nightly builds](https://nightly.octave.org/#/download).

## Copyright

The files are provides without any warranty in the hope they migth be useful.
You can use, modify and redistribute without restrictions.

