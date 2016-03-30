// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

function generated_files = xmltopdf(dirs,titles,directory_language,default_language)

    lhs=argn(1);
    rhs=argn(2);

    select rhs

    case 0 then
        generated_files = xmltoformat("pdf");
    case 1 then
        generated_files = xmltoformat("pdf",dirs);
    case 2 then
        generated_files = xmltoformat("pdf",dirs,titles);
    case 3 then
        generated_files = xmltoformat("pdf",dirs,titles,directory_language);
    case 4 then
        generated_files = xmltoformat("pdf",dirs,titles,directory_language,default_language);
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): At most %d expected.\n"),"xmltopdf",4));

    end

endfunction
