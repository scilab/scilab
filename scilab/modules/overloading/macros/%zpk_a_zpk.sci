// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%zpk_a_zpk(a,b)
    //r=a+b
    if a.dt<>b.dt then
        error(msprintf(_("%s: time domains are not compatible.\n"),"[a,b]"))
    end

    [ma,na]=size(a)
    [mb,nb]=size(b)
    if ma*na==1 then
        r=b;
        for i=1:mb
            for j=1:nb
                r(i,j)=%zpk_sum([a,b(i,j)])
            end
        end
    elseif mb*nb==1 then
        r=a
        for i=1:mb
            for j=1:nb
                r(i,j)=%zpk_sum([a(i,j),b])
            end
        end
    else
        if ma<>mb|na<>nb  then
            error(_("Inconsistent row/column dimensions.\n"))
        end
        r=a
        for i=1:mb
            for j=1:nb
                r(i,j)=%zpk_sum([a(i,j),b(i,j)])
            end
        end
    end
endfunction
