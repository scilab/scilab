// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function []=mtlb_type(filename)
    // Emulation function for type() Matlab function
    [x,ierr]=fileinfo(filename)
    if ierr==0 then // File exists
        disp(mgetl(mopen(filename,"r")))
    else
        if type(evstr(filename))==13 then // Compiled function
            disp(fun2string(evstr(filename)))
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
