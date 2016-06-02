// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%zpk_m_zpk(a,b)
    //r=a.*b for zpk systems
    [ma,na]=size(a)
    [mb,nb]=size(b)
    if a.dt<>b.dt then
        error(msprintf(_("%s: time domains are not compatible.\n"),"[a,b]"))
    end
    if ma*na==1 then
        Z=cell(mb,nb);P=cell(mb,nb);
        for k=1:nb
            for l=1:mb
                Z{l,k}=[b.Z{l,k};a.Z{1}]
                P{l,k}=[b.P{l,k};a.P{1}]
            end
        end
        K=a.K.*b.K
        r=zpk(Z,P,K,a.dt)
    elseif mb*nb==1 then
        Z=cell(ma,na);P=cell(ma,na);K=zeros(ma,na);
        for k=1:na
            for l=1:ma
                Z{l,k}=[b.Z{1};a.Z{l,k}]
                P{l,k}=[b.P{1};a.P{l,k}]
            end
        end
        K=a.K.*b.K
        r=zpk(Z,P,K,a.dt)
    else
        if na<>mb then
            error(_("Inconsistent multiplication.\n"))
        end
        Zb=cell(mb,nb);cPb=cell(1,nb);
        for j=1:nb
            //reduce b(:,j) to a common denominator
            [z,p]=reduceToCommonDenominator(b(:,j).Z,b(:,j).P,b(:,j).K)
            Zb(:,j)=z;
            cPb(j)={p};
        end
        Z=cell(ma,nb)
        P=cell(ma,nb)
        for i=1:ma
            [za,cpa]=reduceToCommonDenominator(a(i,:).Z,a(i,:).P,a(i,:).K);
            for j=1:nb
                p=[cpa;cPb{j}];

                //create products a(i,k)*b(k,j) for each k
                for k=1:na
                    Tij(k)=zpk([za{k};Zb{k,j}],p,a(i,k).K*b(k,j).K,a.dt)
                end
                r(i,j)=%zpk_sum(Tij)
            end
        end
    end
    r=simplify_zp(r);
endfunction
