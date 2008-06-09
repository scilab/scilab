
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
// Copyright (C) 2008      - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

lines(0);
CurrentDirScicos = get_absolute_file_path("buildmacros.sce");
macrosdirs       = ["scicos_auto","scicos_utils","scicos_scicos","scicos_menus"];

for i=1:size(macrosdirs,"*") do
  exec(CurrentDirScicos+"/"+macrosdirs(i)+"/buildmacros.sce");
end

clear my_script_path CurrentDirScicos macrosdirs
