// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [nt,dt,rk]=trzeros(Sl)
    //Transmission zeros of Sl = nt./dt
    // Syntax : [nt,dt]=trzeros(Sl)
    //!

    [LHS,RHS]=argn(0);
    sltyp=typeof(Sl)

    if sltyp == "polynomial" then
        D=Sl;
        [m,n]=size(D);
        if m<>n then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"trzeros",1));
            return;
        end
        chis=det(D);nt=roots(chis);dt=ones(nt);
        if LHS==1 then
            nt=nt./dt;dt=[];rk=[];
        end
        return;
    end

    if sltyp == "rational" then
        if size(Sl)==1 then
            nt=roots(Sl("num"));dt=[];rk=1;
            return;
        end
        Sl=tf2ss(Sl);
    end

    if typeof(Sl)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A linear dynamical system or a polynomial expected.\n"),"trzeros",1))
    end

    //Sl=minss(Sl);
    [A,B,C,D]=Sl(2:5);
    if type(D)==2 then
        [m,n]=size(D);
        if m<>n then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Square system expected.\n"),"trzeros",1));
        end
        chis=det(systmat(Sl));nt=roots(chis);dt=ones(nt);
        if LHS==1 then nt=nt./dt;dt=[];rk=[];end
        return;
    end

    if size(A,"*")==0 then
        if type(D)==1 then nt=[];dt=[];return;end;
        if type(D)==2 then
            [m,n]=size(D);
            if m<>n then
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Square system expected.\n"),"trzeros",1));
            end
            chis=det(D);nt=roots(chis);dt=ones(nt);
            if LHS==1 then nt=nt./dt;dt=[];rk=[];end
            return;
        end;
    end;
    [ld,kd]=size(D);
    if norm(D,1)<sqrt(%eps)|ld==kd then
        [nt,dt,rk]=tr_zer(A,B,C,D);
        if norm(dt,1) > 1.d-10 then
            if LHS==1 then nt=nt./dt;dt=[];rk=[];end
            return;
        end
    end
    if ld < kd & norm(D*pinv(D)-eye(),1)< 1.d-10
        //nt=spec(A-B*pinv(D)*C);dt=ones(nt);
        [nt,dt]=tr_zer(A,B,C,D);
        rk=ld;
        if LHS==1 then nt=nt./dt;end;
        return;
    end
    if ld > kd & norm(pinv(D)*D-eye(),1)< 1.d-10
        //nt=spec(A-B*pinv(D)*C);dt=ones(nt);
        [nt,dt]=tr_zer(A,B,C,D);
        rk=kd;
        if norm(dt,1) > 1.d-10 then
            if LHS==1 then nt=nt./dt;dt=[];rk=[];end;return;
        end
    end
    //warning('Trzeros:non-square system with D non zero and not full')
    //By kronecker form
    s=poly(0,"s");
    syst_matrix=systmat(Sl); //form system matrix
    [Q,Z,Qd,Zd,numbeps,numbeta]=kroneck(syst_matrix);
    ix=Qd(1)+Qd(2)+1:Qd(1)+Qd(2)+Qd(3);
    iy=Zd(1)+Zd(2)+1:Zd(1)+Zd(2)+Zd(3);
    finitepencil=Q(ix,:)*syst_matrix*Z(:,iy);
    [E,A]=pen2ea(finitepencil);
    [nt,dt]=spec(A,E);rk=[];
    if LHS==1 then nt=nt./dt;dt=[];rk=[];end;
endfunction
