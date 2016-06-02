// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function [n,m]=%zpk_size(T,o)
    nm=size(T.K)
    if  argn(2)==2 then
        if argn(1)<>1 then
            error(msprintf(_("%s: Wrong number of output argument: %d expected.\n"),"size",1))
        end
        if o=="*" then
            n=prod(nm)
        elseif o==1|o=="r" then
            n=nm(1)
        elseif o==2|o=="c" then
            n=nm(2)
        end
    else
        if argn(1)==1 then
            n=nm
        else
            m=nm(2)
            n=nm(1)
        end
    end
endfunction
