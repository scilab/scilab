// This file is part of the zpk module
// Copyright (C) 2016 - Serge Steer - INRIA
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r = %zpk_n_zpk(s1, s2)
    //r=s1<>s2
    //work around for bug 14800  (should be removed later)
    if size(s1,"*")==1 then
        [m,n]=size(s2);
        r=s1.K<>s2.K|s1.dt<>s2.dt
        for i=1:m
            for j=1:n
                r(i,j)=r(i,j)|s1.Z<>s2(i,j).Z|s1.P<>s2(i,j).P
            end
        end
    elseif size(s2,"*")==1 then
        [m,n]=size(s1);
        r=s1.K<>s2.K|s1.dt<>s2.dt
        for i=1:m
            for j=1:n
                r(i,j)=r(i,j)|s2.Z<>s1(i,j).Z|s2.P<>s1(i,j).P
            end
        end
    else
        //keep only this instruction after bug 14800 fix
        r=s1.Z<>s2.Z|s1.P<>s2.P|s1.K<>s2.K|s1.dt<>s2.dt
    end
endfunction
