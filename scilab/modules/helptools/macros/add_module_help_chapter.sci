// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function add_module_help_chapter(modulename)

    if and(getscilabmode() <> ["NW";"STD"]) then
        return;
    end

    path_addchapter = SCI+"/modules/"+modulename+"/help/"+getlanguage()+"/addchapter.sce";

    if ~isfile(path_addchapter) then
        path_addchapter = SCI+"/modules/"+modulename+"/help/"+getdefaultlanguage()+"/addchapter.sce";
    end

    if isfile(path_addchapter) then
        exec(path_addchapter);
    end

endfunction
