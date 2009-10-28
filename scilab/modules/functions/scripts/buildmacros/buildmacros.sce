
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// ------------------------------------
//warning('off'); // disable warnings
// ------------------------------------

if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
// ------------------------------------
lines(0);
Directories=[];
CurrentDirectory=pwd();

// set stacksize
// because scilab.start not called when we build macros
// Previously (wrong place), it was defined in buildmacros.sce of metanet module

stacksize(5000000);

modules=getmodules();
index=size(modules);

for i=1:index(1) do 
 Directories=[Directories;"SCI/modules/"+modules(i)+"/macros"];
end  
clear modules index i
 
Dim=size(Directories);


for i=1:Dim(1) do 
  chdir(Directories(i));
  if (fileinfo('buildmacros.sce')<>[]) then
    exec('buildmacros.sce');
  end
  chdir(CurrentDirectory);
end
clear CurrentDirectory Dim Directories
exit
// ------------------------------------
