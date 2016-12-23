// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function [z,p,k]=ss2zp(S)
    if typeof(S)<>"state-space" then
        error(msprintf(_("%s: Wrong type for input argument #%d: Linear state space system expected.\n"),"ss2zp",1))
    end
    [ny,nu]=size(S)
    if nu<>1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: a single input system expected.\n"),"ss2zp",1))
    end
    p=spec(S.A);
    z=[];
    k=[];
    for i=1:ny
        //F=systmat(minss(S(i,1))); //Should be an alternative
        F=systmat(S(i,1));
        if type(S.D)==2&degree(S.D(i,1))>0 then
            //non proper system
            num=det(F)
            zi=roots(num);
            k=[k coeff(num,degree(num))];
        else
            [Q,Z,Qd,Zd]=kroneck(F);
            sgn=sign(det(Z))
            k_inf=1:Qd(2);
            F_inf=Q(k_inf,:)*F*Z(:,k_inf);
            if Qd(3)==0 then //no transmission zeros
                k=[k;prod(coeff(diag(F_inf),0))];
                zi=[];
            else
                k_finite=Qd(2)+(1:Qd(3));
                F_finite=Q(k_finite,:)*F*Z(:,k_finite);
                k=[k;sgn*prod(coeff(diag(F_inf),0))*prod(coeff(diag(F_finite),1))];
                zi=spec(-coeff(F_finite,0),coeff(F_finite,1));
                //force complex values to be exactly conjugate (spec may return
                //nearly complex conjugate pairs
                ii=find(imag(zi)>0)
                if ii<>[] then zi(ii+1)=conj(zi(ii));end
            end
        end

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
            z=[z zi];
        end
    end
    if and(imag(z)==0) then z=real(z);end

endfunction
