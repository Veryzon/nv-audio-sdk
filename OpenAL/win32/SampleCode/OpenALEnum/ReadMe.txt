========================================================================
       CONSOLE APPLICATION : OpenALEnum
========================================================================

This program demonstrates NVIDIA extensions to the OpenAL interface.  In
normal usage, applications must know ahead of time which OpenAL
interface to request support for.  As future native implementations are
created, older applications will have no way of benefiting from these
native implementations.  Using the new function "alcEnumerateDevices" it
is possible to determine what OpenAL interfaces are present on the
system.  This implemenation also allows multiple native version of the
OpenAL API to coexist peacfully on the same system.  Under Win32, the
new extension will search for for other DLLs with the filename matching
*OpenAL*.dll and will enumerate them as well.


OpenALEnum.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

OpenALEnum.cpp
    This is the main application source file.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named OpenALEnum.pch and a precompiled types file named StdAfx.obj.
