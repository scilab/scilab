// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function [z,p,k]=tf2zp(S)
    if typeof(S)<>"rational" then
        error(msprintf(_("%s: Wrong type for input argument #%d: transfer function expected.\n"),"tf2zp",1))
    end
    [ny,nu]=size(S)
    if nu<>1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: a single input system expected.\n"),"tf2zp",1))
    end
    if ny==1 then //siso case
        p=roots(S.den);
        z=roots(S.num);
        k=coeff(S.num,max(0,degree(S.num)))./coeff(S.den,max(0,degree(S.den)));
    else
        //find common denominator
        [den,fact]=lcm(S.den);
        num=S.num.*fact;
        p=roots(den,"e");

        k=[];
        for i=1:ny
            k=[k;coeff(num(i),max(0,degree(num(i))))];
            zi=roots(num(i),"e")
            if i==1 then
                z=zi
            else
                nz=size(z,1);
                nzi=size(zi,1);
                if nzi>nz then
                    z(nz+1:nzi)=%inf;
                elseif nz>nzi then
                    zi($+1:nz)=%inf
                end
                z=[z,zi];
            end
        end
        k=k/coeff(den,max(0,degree(den)));
    end
endfunction
