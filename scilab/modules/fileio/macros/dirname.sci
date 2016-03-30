// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [files]= dirname(files,flag,flagexpand)
    // return the dirnames of files
    // if flag == %t files are first converted
    // according to getos() == 'Windows'. if flagexpand is also true
    // HOME SCI or ~ are also expanded.
    // the returned directory names are not terminated by sep
    [lhs,rhs]=argn(0)
    if rhs <= 1 then flag = %t ; end
    if rhs <= 2 then flagexpand = %t ; end

    if flag == %t then
        files = pathconvert(files,%f,flagexpand);
    end

    sep = filesep();

    for i=1:size(files,"*")
        fname = files(i)
        pref_pos = strindex(fname,sep);
        if pref_pos <> [] then
            fname = part(fname,1:pref_pos($)-1);
        else
            fname = "."; //emptystr();
        end
        files(i)= fname;
    end
endfunction

