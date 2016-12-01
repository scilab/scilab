// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%zpk_v_zpk(a,b)
    //r=a./b for zpk systems
    [ma,na]=size(a)
    [mb,nb]=size(b)
    if a.dt<>b.dt then
        error(msprintf(_("%s: time domains are not compatible.\n"),"[a,b]"))
    end
    if ma*na==1 then
        Z=cell(mb,nb);P=cell(mb,nb);
        for k=1:nb
            for l=1:mb
                Z{l,k}=[b.P{l,k};a.Z{1}]
                P{l,k}=[b.Z{l,k};a.P{1}]
            end
        end
        K=a.K.*b.K
    elseif mb*nb==1 then
        Z=cell(ma,na);P=cell(ma,na);K=zeros(ma,na);
        for k=1:na
            for l=1:ma
                Z{l,k}=[b.P{1};a.Z{l,k}]
                P{l,k}=[b.Z{1};a.P{l,k}]
            end
        end
        K=a.K./b.K
    else
        if ma<>mb|na<>nb then
            error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same dimensions expected.\n"),"a./b",1,2))
        end
        Z=cell(ma,na);P=cell(ma,na);K=zeros(ma,na);
        for k=1:na
            for l=1:ma
                Z{l,k}=[b.P{l,k};a.Z{l,k}]
                P{l,k}=[b.Z{l,k};a.P{l,k}]
            end
        end
        K=a.K./b.K
    end
    r=simplify_zp(zpk(Z,P,K,a.dt));
endfunction
