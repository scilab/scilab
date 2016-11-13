//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) DIGITEO - Clément DAVID <clement.david@scilab.org>
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
// See the file ./license.txt
//

function [X,U,Y,XP] = steadycos(scs_m,X,U,Y,Indx,Indu,Indy,Indxp,param)
    // NAME
    // steadycos - Finds an equilibrium state of a general
    // dynamical system described by a scicos diagram

    // SYNTAX
    //
    // [X,U,Y,XP] = steadycos(scs_m,X,U,Y,Indx,Indu,Indy [,Indxp [,param ] ])
    //
    // scs_m: a Scicos data structure
    // X: column vector. Continuous state. Can be set to [] if zero.
    // U: column vector. Input. Can be set to [] if zero.
    // Y: column vector. Output. Can be set to [] if zero.
    // Indx: index of entries of X that are not fixed. If all can vary, set to 1:$
    // Indu: index of entries of U that are not fixed. If all can vary, set to 1:$
    // Indy: index of entries of Y that are not fixed. If all can vary, set to 1:$
    // Indxp: index of entries of XP (derivative of x) that need not be zero.
    //        If all can vary, set to 1:$. Default [].
    // param: list with two elements (default list(1.d-6,0))
    //   param(1): scalar. Perturbation level for linearization; the following variation is used
    //             del([x;u])_i = param(1)+param(1)*1d-4*abs([x;u])_i
    //   param(2): scalar. Time t.
    //

    //** This function can be (ab)used from the Scilab command line and
    //** inside a Scicos "context". In order to handle the different situations,
    //** the required libraries are loaded if not already present in the
    //** "semiglobal-local-environment".

    if ~exists("scicos_diagram") then
        loadXcosLibs();
    end

    if exists("scicos_scicoslib")==0 then
        load("SCI/modules/scicos/macros/scicos_scicos/lib") ;
    end

    if exists("scicos_autolib")==0 then
        load("SCI/modules/scicos/macros/scicos_auto/lib") ;
    end

    if exists("scicos_utilslib")==0 then
        load("SCI/modules/scicos/macros/scicos_utils/lib") ;
    end

    // Define Scicos data tables ===========================================
    if ( ~isdef("modelica_libs") | ..
        ~isdef("scicos_pal_libs") ) then
        [modelica_libs, scicos_pal_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables();
    end
    // =====================================================================

    [lhs,rhs] = argn(0) ;

    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "steadycos", 1));
    end
    if typeof(scs_m)<>"diagram" & typeof(scs_m)<>"cpr" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A diagram expected.\n"), "steadycos", 1));
    end

    // compile and post-process the diagram
    if typeof(scs_m)=="diagram" then

        // Propagate context through all blocks
        %state0     = list();
        needcompile = 4;
        %cpr        = struct();
        %cpr.state  = %state0;
        %scicos_context = struct();
        context = scs_m.props.context;

        [%scicos_context, ierr] = script2var(context, %scicos_context);
        [scs_m,%cpr,needcompile,ok] = do_eval(scs_m, %cpr, %scicos_context);
        if ~ok then
            error(msprintf(gettext("%s: Error during block parameters evaluation.\n"), "lincos"));
        end

        IN  = [];
        OUT = [];

        // check and do version
        current_version = get_scicos_version()
        scicos_ver = find_scicos_version(scs_m)
        if scicos_ver<>current_version then
            scs_m=do_version(scs_m,scicos_ver);
        end

        if rhs==7 then
            Indxp=[ ];param=list(1.d-6,0)
        elseif rhs==8 then
            param=list(1.d-6,0)
        elseif rhs==9 then
        else
            error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "steadycos", 7, 9));
        end

        for i=1:length(scs_m.objs)
            if typeof(scs_m.objs(i))=="Block" then
                if or(scs_m.objs(i).gui==["IN_f", "INPUTPORT"]) then
                    scs_m.objs(i).gui="INPUTPORT";
                    IN=[IN scs_m.objs(i).model.ipar]
                elseif or(scs_m.objs(i).gui==["OUT_f", "OUTPUTPORT"]) then
                    scs_m.objs(i).gui="OUTPUTPORT";
                    OUT=[OUT  scs_m.objs(i).model.ipar]
                end
            end
        end

        if IN == [] then
            error(msprintf(gettext("%s: Unable to find diagram inputs\n"), "steadycos"));
        end
        if OUT == [] then
            error(msprintf(gettext("%s: Unable to find diagram outputs\n"), "steadycos"));
        end

        IN=-gsort(-IN);
        if or(IN<>[1:size(IN,"*")]) then
            error(msprintf(gettext("%s: Input ports are not numbered properly.\n"), "steadycos"))
        end

        OUT=-gsort(-OUT);
        if or(OUT<>[1:size(OUT,"*")]) then
            error(msprintf(gettext("%s: Output ports are not numbered properly.\n"), "steadycos"))
        end

        // compile scs_m
        [bllst,connectmat,clkconnect,cor,corinv,ok] = c_pass1(scs_m);
        if ~ok then
            error(msprintf(gettext("%s: Diagram does not compile in pass %d.\n"),"steadycos",1));
        end
        %cpr = c_pass2(bllst,connectmat,clkconnect,cor,corinv);
        if %cpr==list() then
            ok=%f,
        end
        if ~ok then
            error(msprintf(gettext("%s: Diagram does not compile in pass %d.\n"),"steadycos",2));
        end

        // compile and post-process the diagram end
    end

    sim=%cpr.sim;state=%cpr.state;
    //
    inplnk=sim.inplnk;inpptr=sim.inpptr;
    outlnk=sim.outlnk;outptr=sim.outptr;ipptr=sim.ipptr;

    ki=[];ko=[];nyptr=1;
    for kfun=1:length(sim.funs)
        if sim.funs(kfun)=="output" then
            sim.funs(kfun)="bidon"
            ko=[ko;[kfun,sim.ipar(ipptr(kfun))]];

        elseif sim.funs(kfun)=="input" then
            sim.funs(kfun)="bidon"
            ki=[ki;[kfun,sim.ipar(ipptr(kfun))]];

        end
    end
    [junk,ind]=gsort(-ko(:,2));ko=ko(ind,1);
    [junk,ind]=gsort(-ki(:,2));ki=ki(ind,1);

    pointo=[];
    for k=ko'
        pointo=[pointo;inplnk(inpptr(k))]
    end
    pointi=[];
    for k=ki'
        pointi=[pointi;outlnk(outptr(k))]
    end
    nx=size(state.x,"*");
    nu=0; for k=pointi', nu=nu+size(state.outtb(k),"*"), end
    ny=0; for k=pointo', ny=ny+size(state.outtb(k),"*"), end

    if X==[] then X=zeros(nx,1);end
    if Y==[] then Y=zeros(ny,1);end
    if U==[] then U=zeros(nu,1);end
    if param(1)==0 then param(1)=1.d-6;end

    t = param(2)

    ux0 = [U(Indu);X(Indx)];
    sindu = size(U(Indu),"*");
    sindx = size(X(Indx),"*");
    [err,uxopt,gopt] = optim(cost,ux0)
    U(Indu) = uxopt(1:sindu);
    X(Indx) = uxopt(sindu+1:sindx+sindu);
    state.x = X;
    Uind=1

    for k=pointi'
        state.outtb(k) = matrix(U(Uind:Uind+size(state.outtb(k),"*")-1),size(state.outtb(k)));
        Uind = size(state.outtb(k),"*")+1;
    end

    [state,t] = scicosim(state,t,t,sim,"start",[.1,.1,.1,.1]);
    [state,t] = scicosim(state,t,t,sim,"linear",[.1,.1,.1,.1]);
    [state,t] = scicosim(state,t,t,sim,"finish",[.1,.1,.1,.1]);

    XP=state.x;
    Yind=1

    for k=pointo'
        Y(Yind:Yind+size(state.outtb(k),"*")-1) = state.outtb(k)(:);
        Yind = size(state.outtb(k),"*")+1
    end

endfunction
//**-------------------------------------------------------------------------------------------

function [f,g,ind]=cost(ux,ind)
    state;
    X;
    U;
    X(Indx)=ux(sindu+1:sindx+sindu);
    U(Indu)=ux(1:sindu);
    state.x=X;
    Uind=1
    for k=pointi'
        state.outtb(k) = matrix(U(Uind:Uind+size(state.outtb(k),"*")-1),size(state.outtb(k)));
        Uind = size(state.outtb(k),"*")+1;
    end
    // state.outtb(pointi)=U;

    [state,t] = scicosim(state,t,t,sim,"start",[.1,.1,.1,.1]);
    [state,t] = scicosim(state,t,t,sim,"linear",[.1,.1,.1,.1]);
    [state,t] = scicosim(state,t,t,sim,"finish",[.1,.1,.1,.1]);

    zer=ones(X);zer(Indxp)=0;xp=zer.*state.x;

    Yind=1
    for k=pointo'
        y(Yind:Yind+size(state.outtb(k),"*")-1)=state.outtb(k)(:);
        Yind = size(state.outtb(k),"*")+1
    end
    // y=state.outtb(pointo);
    zer = ones(y);
    zer(Indy) = 0;
    err       = zer.*(Y-y);
    f=.5*(norm(xp,2)+norm(err,2));

    sys = lincos(%cpr,X,U,param); //** lincos is used here

    g  = xp'*[sys.B(:,Indu) sys.A(:,Indx)] - err'*[sys.D(:,Indu) sys.C(:,Indx)];

endfunction

