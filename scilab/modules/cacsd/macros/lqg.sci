// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000-2016 INRIA -
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function K=lqg(P,varargin)
    // returns the (strictly proper) lqg (H2) controller
    // for the augmented plant P
    if and(typeof(P)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"lqg",1))
    end

    if typeof(P)=="rational" then
        P=tf2ss(P),flag=%f
    else
        flag=%t
    end

    if P.dt==[] then
        warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"lqg",1));
        P.dt="c";
    end

    if size(varargin)==1 then //K=lqg(P_aug,r)
        r=varargin(1)
        if typeof(r)<>"constant"|~isreal(r) then
            error(msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"),"lqg",2))
        end

        if size(r,"*")<>2 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: %d expected.\n"),"lqg",2,2))
        end

        r=int(r);
        if or(r<=0) then
            error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be positive.\n"),"lqg",2))
        end

        //split outputs index
        o1=1:$-r(1) ;
        o2=$-r(1)+1:$;
        //split inputs index
        i1=1:$-r(2)
        i2=$-r(2)+1:$

        if norm(P(o1,i1).D,1) <> 0 then
            warning(msprintf(gettext("%s: %s is not zero! (set to zero)"),"lqg","D11"));
        end

        Kc=lqr(P(o1,i2));
        Kf=lqe(P(o2,i1));
        K=obscont(P(o2,i2),Kc,Kf);
    elseif or(size(varargin)==(2:4)) then //K=lqg(P,Qxu,Qwv [,Qi [,#dof]])
        [ny,nu,nx]=size(P);
        Qxu=varargin(1);
        Qwv=varargin(2);
        if or(size(Qxu)<>nx+nu) then
            error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"lqg",2,nx+nu,nx+nu))
        end

        if norm(Qxu.'-Qxu,1)>100*%eps*norm(Qxu,1) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqg",2))
        end

        if or(size(Qwv)<>nx+ny) then
            error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"lqg",3,nx+ny,nx+ny))
        end

        if norm(Qwv.'-Qwv,1)>100*%eps*norm(Qwv,1) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqg",3))
        end

        //compute observer gain (convert system to lqe form adding G and H matrices)
        Kf=lqe(P,Qwv(1:nx,1:nx),Qwv(nx+1:$,nx+1:$),Qwv(1:nx,nx+1:$));
        if size(varargin)==2 then
            Kc=lqr(P,Qxu(1:nx,1:nx),Qxu(nx+1:$,nx+1:$),Qxu(nx+1:$,1:nx));
            K=obscont(P,Kc,Kf);
        else //K=lqg(P,Qxu,Qwv,Qi)
            Qi=varargin(3);
            if size(varargin)==3 then
                #dof=2;
            else
                #dof=varargin(4);
                if type(#dof)<>1|and(#dof<>[1 2]) then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"lqg",5,"1,2"))
                end
            end

            if or(size(Qi)<>ny) then
                error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"lqg",4,ny,ny))
            end

            if norm(Qi.'-Qi,1)>100*%eps*norm(Qi,1) then
                error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqg",4))
            end
            Kc=lqi(P,sysdiag(Qxu(1:nx,1:nx),Qi),Qxu(nx+1:$,nx+1:$),...
            [Qxu(1:nx,nx+1:nx+nu);zeros(ny,nu)]);
            // Form the controller
            //O=observer(P,Kf)
            //I=syslin("c",eye(ny,ny)/%s);
            //if P.dt<>"c" then I=dscr(I,P.dt);end

            //K=[([eye(nu,nu);eye(nu,nu)]*Kc)*[zeros(nx,ny) O(:,[nu+1:nu+ny 1:nu]);I, -I zeros(ny,nu)]]

            if P.dt<>"c" then
                dt=P.dt
                if dt=="d" then
                    dt=1;
                end

                A=[P.A-P.B*Kc(:,1:nx)-Kf*(P.D*Kc(:,1:nx)-P.C),(-P.B-Kf*P.D)*Kc(:,nx+1:$);
                zeros(ny,nx) eye(ny,ny)];

            else
                dt=1
                A=[P.A-P.B*Kc(:,1:nx)-Kf*(P.D*Kc(:,1:nx)-P.C),(-P.B-Kf*P.D)*Kc(:,nx+1:$);
                zeros(ny,nx) zeros(ny,ny)];
            end

            if #dof==2 then //2 degrees if freedom
                K=syslin(P.dt,A,[zeros(nx,ny) -Kf;[eye(ny,ny) -eye(ny,ny)]*dt],-Kc);
            else
                //1 degree of freedom
                K=syslin(P.dt,A,[Kf;eye(ny,ny)*dt],-Kc);
            end
        end
    else
        error(msprintf(_("%s: Wrong number of input argument(s): %d or %d expected."),"lqg",2,3))
    end

    if ~flag then
        K=ss2tf(K);
    end
endfunction
