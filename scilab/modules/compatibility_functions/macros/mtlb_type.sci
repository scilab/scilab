// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=mtlb_type(filename)
    // Emulation function for type() Matlab function
    [x,ierr]=fileinfo(filename)
    if ierr==0 then // File exists
        disp(mgetl(mopen(filename,"r")))
    else
        if type(evstr(filename))==13 then // Compiled function
            tree=macr2tree(evstr(filename));
            disp(tree2code(tree))
        elseif strindex(filename,".")==[] then // File without extension
            Mfilename=filename+".m"
            [x,ierr]=fileinfo(Mfilename)
            if ierr==0 then // filename.m exists
                disp(mgetl(mopen(Mfilename,"r")))
            else
                SCIfilename=filename+".sci"
                [x,ierr]=fileinfo(SCIfilename)
                if ierr==0 then // filename.sci exists
                    disp(mgetl(mopen(SCIfilename,"r")))
                else
                    disp(msprintf(gettext("%s: Could not open neither ''%s'' nor ''%s'' nor ''%s''.\n"),"mtlb_type",filename,Mfilename,SCIfilename));
                end
            end
        end
    end
endfunction
