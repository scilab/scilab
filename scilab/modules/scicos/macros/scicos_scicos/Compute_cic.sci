//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//
function ok=Compute_cic(method,Nunknowns)
    global icpr
    ok=%f

    if icpr==list() then
        return
    end

    tolerances=scs_m.props.tol
    solver=tolerances(6)
    if (and (solver <> [100 101 102])) then solver=100; end
    tolerances(6)=solver
    Atol=tolerances(1)
    Rtol=tolerances(2)
    %tcur=0;
    tf=tolerances(3)
    state=icpr.state;
    nx=size(state.x,"r");
    nx2=round(nx/2);
    nxModelica=evstr(Nunknowns);

    if nxModelica~=nx2 then
        msg = _("Your model contains states defined in standard Scicos blocks.\nCurrent initialization interface does not support mixed models.")
        messagebox(msprintf(msg),"error","modal")
        return
    end

    //------------------------------
    ierr=0;
    try
        ierr=execstr("[state,t]=scicosim(state,%tcur,tf,icpr.sim,''start'',tolerances)","errcatch")
        if ierr<>0 then
            messagebox(msprintf(_("Initialisation problem in %s "),"Scicosim-start"),"error","modal");
            return
        end
    catch
        messagebox(msprintf(_("Initialisation problem in %s "),"Scicosim-start"),"error","modal");
        return
    end
    //--------------------------------------------------------------
    Ida_ierr=0;
    if method=="Ida (init)" then
        try
            // scicos menu, not any more in xcos
            // setmenu(curwin,'stop')
            Ida_ierr=execstr("[state,t]=scicosim(state,%tcur,tf,icpr.sim,''run'',tolerances)","errcatch")
            // unsetmenu(curwin,'stop')
            if Ida_ierr<>0 then,
                messagebox(msprintf(_("Initialisation problem in %s "),"Sundials"),"error","modal");
            end
        catch
            messagebox(msprintf(_("Initialisation problem in %s "),"Sundials"),"error","modal");
        end
    end
    //--------------------------------------------------------------
    if method=="Kinsol" then
        try
            // scicos menu, not any more in xcos
            // setmenu(curwin,'stop')
            ierr=execstr("[state2,t]=scicosim(state,%tcur,tf,icpr.sim,''Kinsol'',tolerances)","errcatch")
            if ierr==0 & (or(isnan(state2.x)) | or(isinf(state2.x))) then
                ierr=-1;
            end
            if ierr==0 then
                state=state2;
            end
            // unsetmenu(curwin,'stop')
            if ierr<>0 then,
                messagebox(msprintf(_("Initialisation problem in %s "),"Kinsol"),"error","modal");
            end
        catch
            messagebox(msprintf(_("Initialisation problem in %s "),"Kinsol"),"error","modal");
        end
    end
    //--------------------------------------------------------------
    if method=="Fsolve" then
        try
            x0=state.x(1:nx2);
            [xres]=fsolve(x0,fsim);
            ierr=0
            if or(isnan(xres)) | or(isinf(xres)) then
                ierr=-1;
            end
            if ierr==0 then
                for i=1:nx2, state.x(i)=xres(i);end
                fsim(xres);// just to perform an idoit to update outputs in mixed_models
            end
        catch
            messagebox(msprintf(_("Initialisation problem in %s "),"Fsolve"), "error","modal");
        end
    end
    //--------------------------------------------------------------
    if method=="Optim" then
        try
            x0=state.x(1:nx2);
            [f,xres]=optim(fsumsquare,x0);
            ierr=0
            if or(isnan(xres)) | or(isinf(xres)) then
                ierr=-1;
            end
            if ierr==0 then
                for i=1:nx2, state.x(i)=xres(i);end
                fsim(xres);// just to perform an idoit to update outputs in mixed_models
            end
        catch
            messagebox(msprintf(_("Initialisation problem in the %s method"),"Optim"), "error","modal");
        end
    end
    //--------------------------------------------------------------
    if method=="Nelder_Mead" then
        try
            x0=state.x(1:nx2);
            [xmin,fmin,epsilo,ls,fs] = neldermead(rand(nx2,nx2+1),Atol,1,0.5,1.5)

            for i=1:nx2, state.x(i)=xmin(i);end
        catch
            messagebox(msprintf(_("Initialisation problem in the %s method")," Nelder_Mead"), "error","modal");
        end
    end
    //--------------------------------------------------------------
    if method=="Hompack77" then
        try
            ierr=execstr("[state,t]=scicosim(state,%tcur,tf,icpr.sim,''hompack77'',tolerances)","errcatch")
            if ierr<>0 then,
                messagebox(msprintf(_("Initialisation problem in the %s method"),"hompack77"), "error","modal");
            end
        catch
            messagebox(msprintf(_("Initialisation problem in the %s method"),"hompack77"), "error","modal");
        end

    end
    //--------------------------------------------------------------
    if method=="Fsolve_Stepping" then
        x0=state.x(1:nx2);
        Res0=fsim(x0);
        Lambda=0;
        Steps=1000;
        xres=x0;
        for i=1:Steps
            Lambda=i/Steps;
            [xres]=fsolve(xres,fsim_step);
            if modulo(i,10)==0 then,
                disp("Source-stepping: progress="+string(i/Steps*100)+"%, Error="+string(norm(fsim_step(xres))));
            end
        end
        for i=1:nx2, state.x(i)=xres(i);end
    end
    //--------------------------------------------------------------
    if method=="Sundials_Stepping" then
    end
    //--------------------------------------------------------------
    Err="?"
    if Ida_ierr==0 & ierr==0 then
        ss=fsim(state.x(1:nx2));if ss~=[] then Err=string(max(abs(ss)));else Err="0";end//using inf norm
    end

    try
        if  Ida_ierr==0 then //cossimdaskr is followed by a cosend in case of error
            ierr=execstr("[state,t]=scicosim(state,%tcur,tf,icpr.sim,''finish'',tolerances)","errcatch")
            if ierr<>0 then
                messagebox(_("Initialisation problem in the finish phase"), "error","modal");
                return
            end
        end
    catch
        messagebox(_("Initialisation problem in the finish phase"), "error","modal");
        return
    end
    ok=%t;
endfunction

//------------------------------------------------------------
function  res=fsim(xin)
    nx=size(xin,"r");
    if nx==0 then res=[];return ;end
    state1=state
    for i=1:nx, state1.x(i)=xin(i);end
    ierr=execstr("[state2,t]=scicosim(state1,%tcur,tf,icpr.sim,''linear'',tolerances)","errcatch")
    res=state2.x(1:nx);
endfunction

function  res=fsim_step(xin)
    res=fsim(xin)-(1-Lambda)*Res0;
endfunction


function  [sumsq,grad,ind]=fsumsquare(xin,ind)
    nx=size(xin,"r");
    tolerances=scs_m.props.tol;
    atol=tolerances(1);
    rtol=tolerances(2);

    grad=[]
    if ind==2 | ind==4 | ind==3 then
        res=fsim(xin);
        sumsq=0;  for i=1:nx,sumsq=sumsq+res(i)*res(i);end
    end

    if ind==3 | ind==4 then
        for j=1:nx
            xin_p=xin;
            ewt_j=1/(abs(xin_p(j)*rtol+atol+%eps));
            delta_j=max(abs(xin_p(j))*%eps,1/ewt_j);
            xin_p(j)=xin_p(j)+delta_j;
            res_p=fsim(xin_p);
            sumsq_p=0;  for i=1:nx,sumsq_p=sumsq_p+(res_p(i)^2-res(i)^2)/delta_j;end
            grad(j)=sumsq_p;
        end
    end

endfunction


function [xmin,fmin,epsilo,ls,fs] = neldermead(s,epsil,alpha,beta,gama)
    // saved in the last versions
endfunction


function y=fsim2(x)
    x0=x(1);x1=x(2);x2=x(3);x3=x(4);
    x4=x(5);x5=x(6);x6=x(7);x7=x(8);

    v0 = -x3;
    v1 = -x2;

    y(1) = x4+v0;
    y(2) = x6+v1;
    y(3) = x7-x6;
    y(4) = x5-x4;
    y(5) = 1e-14-x3*x2;
    y(6) = v0+x2;
    y(7) = v0+abs(x1);
    y(8) = v1+abs(x0);

    disp(y');
endfunction

