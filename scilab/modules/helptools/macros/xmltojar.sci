// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function generated_files = xmltojar(dirs,titles,directory_language,default_language)

    warnobsolete("tbx_build_help", "2027.1.0");

    rhs=argn(2);
    if rhs > 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At most %d expected.\n"),"xmltojar",4));
    end

    // No jar is built anymore: the help browser renders the XML pages.
    // Only the images of the given help directories are pre-generated.
    generated_files = [];
    if rhs < 1 then
        return
    end
    if rhs < 2 | size(titles, "*") <> size(dirs, "*") then
        titles = basename(dirs);
    end
    for k = 1:size(dirs, "*")
        tbx_build_help(titles(k), dirs(k));
    end

endfunction
