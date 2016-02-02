
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
// Copyright (C) 2008      - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

lines(0);
CurrentDirScicos = get_absolute_file_path("buildmacros.sce");
macrosdirs       = ["scicos_auto","scicos_utils","scicos_scicos"];

for i=1:size(macrosdirs,"*") do
    exec(CurrentDirScicos+"/"+macrosdirs(i)+"/buildmacros.sce");
end

clear my_script_path CurrentDirScicos macrosdirs
