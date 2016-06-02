// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%zpk_l_zpk(a,b)
    //r=a/b for zpk systems
    [ma,na]=size(a)
    [mb,nb]=size(b)
    if a.dt<>b.dt then
        error(msprintf(_("%s: time domains are not compatible.\n"),"[a,b]"))
    end
    if ma*na==1 then
        Z=cell(mb,nb);P=cell(mb,nb);K=zeros(mb,nb);
        for k=1:nb
            for l=1:mb
                [Z{l,k},P{l,k}]=simplify_zp([a.P{1};b.Z{l,k}],[a.Z{1};b.P{l,k}])
            end
        end
        K=a.K\b.K
        r=zpk(Z,P,K,a.dt)
    else
        r=zpk2ss(a)\zpk2ss(b)
    end
endfunction

