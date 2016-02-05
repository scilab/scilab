// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

if ~ isdef("genlib") then
    exec(SCI+"/modules/functions/scripts/buildmacros/loadgenlib.sce");
end

genlib("atomslib"         ,"SCI/modules/atoms/macros"                ,%f,%t);
genlib("atomsguilib"      ,"SCI/modules/atoms/macros/atoms_gui"      ,%f,%t);
genlib("atomsinternalslib","SCI/modules/atoms/macros/atoms_internals",%f,%t);
