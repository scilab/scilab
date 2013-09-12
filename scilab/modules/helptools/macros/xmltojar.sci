// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function generated_files = xmltojar(dirs,titles,directory_language,default_language)

    lhs=argn(1);
    rhs=argn(2);

    select rhs

    case 0 then
        generated_files = xmltoformat("javaHelp");
    case 1 then
        generated_files = xmltoformat("javaHelp",dirs);
    case 2 then
        generated_files = xmltoformat("javaHelp",dirs,titles);
    case 3 then
        generated_files = xmltoformat("javaHelp",dirs,titles,directory_language);
    case 4 then
        generated_files = xmltoformat("javaHelp",dirs,titles,directory_language,default_language);
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): At most %d expected.\n"),"xmltojar",4));

    end

endfunction
