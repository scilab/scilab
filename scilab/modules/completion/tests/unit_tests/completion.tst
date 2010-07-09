// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

//===============================
// unit tests completion
//===============================

// parameters tests
r = completion('%');
s = size(r,'*');
if s < 748 then pause,end
//===============================
functions1 = completion('%','functions');
s = size(functions1,'*');
if s < 17 then pause,end
//===============================
commands1 = completion('%','commands');
s = size(commands1,'*');
if s <> 0 then pause,end
//===============================
variables1 = completion('%','variables');
s = size(variables1,'*');
if s < 22 then pause,end
//===============================
macros1 = completion('%','macros');
s = size(macros1,'*');
if s < 705 then pause,end
//===============================
graphic_properties1 = completion('%','graphic_properties');
s = size(graphic_properties1,'*');
if s <> 0 then pause,end
//===============================
files1 = completion('%','files');
s = size(files1,'*');
if s <> 0 then pause,end
//===============================
[functions2,commands2,variables2,macros2,graphic_properties2,files2] = completion('%');
if functions2 <> functions1 then pause,end
if commands2 <> commands1 then pause,end
if variables2 <> variables1 then pause,end
if macros2 <> macros1 then pause,end
if graphic_properties2 <> graphic_properties1 then pause,end
if files2 <> files1 then pause,end
//===============================
[functions3,commands3,variables3,macros3,graphic_properties3] = completion('%');
if functions3 <> functions1 then pause,end
if commands3 <> commands1 then pause,end
if variables3 <> variables1 then pause,end
if macros3 <> macros1 then pause,end
if graphic_properties3 <> graphic_properties1 then pause,end
//===============================
[functions4,commands4,variables4,macros4] = completion('%');
if functions4 <> functions1 then pause,end
if commands4 <> commands1 then pause,end
if variables4 <> variables1 then pause,end
if macros4 <> macros1 then pause,end
//===============================
[functions5,commands5,variables5] = completion('%');
if functions5 <> functions1 then pause,end
if commands5 <> commands1 then pause,end
if variables5 <> variables1 then pause,end
//===============================
[functions6,commands6] = completion('%');
if functions6 <> functions1 then pause,end
if commands6 <> commands1 then pause,end
//===============================
// check memory leak
//for i=1:1000000,a=completion('w');end;
//for i=1:1000000,a=completion('w',"functions");end;
//for i=1:1000000,a=completion('w',"commands");end;
//for i=1:1000000,a=completion('w',"variables");end;
//for i=1:1000000,a=completion('w',"macros");end;
//for i=1:1000000,a=completion('w',"graphic_properties");end;
//for i=1:1000000,a=completion('w',"files");end;
//===============================
