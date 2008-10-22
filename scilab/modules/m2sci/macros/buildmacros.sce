// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
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
mprintf(gettext("-- Creation of [%s] (Macros) --\n"),"m2sci");
//------------------------------------

lines(0);
CurrentDirectorym2sci = get_absolute_file_path("buildmacros.sce");
macrosdirs            = ["kernel","percent","sci_files"];

for i=1:size(macrosdirs,"*") do
  exec(CurrentDirectorym2sci+"/"+macrosdirs(i)+"/buildmacros.sce");
end

clear my_script_path CurrentDirectorym2sci macrosdirs
