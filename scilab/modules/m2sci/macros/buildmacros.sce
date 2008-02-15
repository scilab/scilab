// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('m2scilib','SCI/modules/m2sci/macros');
//------------------------------------
mprintf("-- Creation of [m2sci] (Macros) --\n");
//------------------------------------
lines(0);
CurrentDirectorym2sci=pwd();
 Directoriesm2sci=["kernel",
 "percent",
 "sci_files"];

stacksize(5000000);

Dim=size(Directoriesm2sci);
for i=1:Dim(1) do 
  chdir(Directoriesm2sci(i));
  exec('buildmacros.sce');
  chdir(CurrentDirectorym2sci);
end

clear CurrentDirectorym2sci Directoriesm2sci Dim
//------------------------------------
