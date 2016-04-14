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

function sys = lincos(scs_m,x0,u0,param)
    // NAME
    // lincos - Constructs by linearization a linear state-space
    // model from a general dynamical system described by a
    // scicos diagram

    // SYNTAX
    //
    // sys = lincos(scs_m [,x0,u0 [,param] ])
    //
    //
    // PARAMETERS
    //
    // scs_m: a Scicos data structure
    // x0: column vector. Continuous state around which linearization to be done (default 0)
    // u0: column vector. Input around which linearization to be done (default 0)
    // param: list with two elements (default list(1.d-6,0))
    //   param(1): scalar. Perturbation level for linearization; the following variation is used
    //             del([x;u])_i = param(1)+param(1)*1d-4*abs([x;u])_i
    //   param(2): scalar. Time t.
    //
    // sys: state-space system
    //
    // DESCRIPTION
    // Constructs by linearization a linear state-space
    // model from a general dynamical system described by a
    // scicos diagram scs_m. Input and output ports, normally
    // used inside superblocks, should be used to specify
    // inputs and outputs in the scicos diagram. Suppose the
    // scicos diagram to be linearized is called mysystem and
    // it is saved in mysystem.cos in the current directory. The scicos
    // diagram scs_m can be obtained either by
    //             scs_m = scicos('mysystem.cos');
    // followed by a quit in the scicos menu, or by
    //             load('mysystem.cos')
    // which creates by default a variable called scs_m.


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
    [lhs, rhs] = argn(0);

    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "lincos", 1));
    end
    if typeof(scs_m)<>"diagram" & typeof(scs_m)<>"cpr" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A diagram expected.\n"), "lincos", 1));
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

        // check version
        current_version = get_scicos_version()
        scicos_ver      = find_scicos_version(scs_m)
        if scicos_ver<>current_version then
            scs_m=do_version(scs_m, scicos_ver);
        end

        // looking for I/O
        for i=1:size(scs_m.objs)
            if typeof(scs_m.objs(i))=="Block" then
                if or(scs_m.objs(i).gui==["IN_f", "INPUTPORT"]) then
                    scs_m.objs(i).gui="INPUTPORT";
                    IN=[IN scs_m.objs(i).model.ipar]
                elseif or(scs_m.objs(i).gui==["OUT_f", "OUTPUTPORT"]) then
                    scs_m.objs(i).gui="OUTPUTPORT";
                    OUT=[OUT  scs_m.objs(i).model.ipar]
                elseif or(scs_m.objs(i).gui==["CLKIN_f","CLKINV_f", "INPUTPORTEVTS"]) then
                    scs_m.objs(i).gui="INPUTPORTEVTS";
                    scs_m.objs(i).model.sim(1)="bidon"
                elseif  or(scs_m.objs(i).gui==["CLKOUT_f","CLKOUTV_f", "OUTPUTPORTEVTS"]) then
                    scs_m.objs(i).gui="OUTPUTPORTEVTS";
                    scs_m.objs(i).model.sim(1)="bidon"
                end
            end
        end

        if IN == [] then
            error(msprintf(gettext("%s: Unable to find diagram inputs\n"), "lincos"));
        end
        if OUT == [] then
            error(msprintf(gettext("%s: Unable to find diagram outputs\n"), "lincos"));
        end

        IN=-gsort(-IN);
        if or(IN<>[1:size(IN,"*")]) then
            error(msprintf(gettext("%s: Input ports are not numbered properly.\n"),"lincos"))
        end

        OUT=-gsort(-OUT);
        if or(OUT<>[1:size(OUT,"*")]) then
            error(msprintf(gettext("%s: Output ports are not numbered properly.\n"),"lincos"))
        end

        //compile scs_m
        [bllst, connectmat, clkconnect,cor,corinv,ok] = c_pass1(scs_m);
        if ~ok then
            error(msprintf(gettext("%s: Diagram does not compile in pass %d.\n"),"lincos",1));
        end

        %cpr = c_pass2(bllst,connectmat,clkconnect,cor,corinv);

        if %cpr==list() then
            ok = %f ;
        end

        if ~ok then
            error(msprintf(gettext("%s: Diagram does not compile in pass %d.\n"),"lincos",2))
        end

        // compile and post-process the diagram end
    end

    sim   = %cpr.sim;
    state = %cpr.state;
    //
    inplnk = sim.inplnk;
    inpptr = sim.inpptr;
    outlnk = sim.outlnk;
    outptr = sim.outptr;
    ipptr  = sim.ipptr;

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

    if rhs<3 then
        x0=zeros(nx,1);u0=zeros(nu,1);
    else
        if size(x0,"*")<>nx then
            if nx == 0 then
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Empty matrix expected.\n"), "lincos", 2));
            else
                error(msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"lincos", 2, nx, 1))
            end
        elseif size(u0,"*")<>nu then
            if nx == 0 then
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Empty matrix expected.\n"), "lincos", 3));
            else
                error(msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"lincos", 3, nx, 1))
            end
        end
    end

    if rhs==4 then
        del = param(1)+param(1)*1d-4*abs([x0;u0])
        t   = param(2)
    else
        del = 1.d-6*(1+1d-4*abs([x0;u0]) )
        t   = 0;
    end

    x0 = x0(:);
    u0 = u0(:);

    state.x=x0;

    Uind = 1
    for k=pointi'
        state.outtb(k) = matrix(u0(Uind:Uind+size(state.outtb(k),"*")-1),size(state.outtb(k)));
        Uind = size(state.outtb(k),"*")+1;
    end

    [state,t]=scicosim(state,t,t,sim,"start",[.1,.1,.1,.1]);
    [state,t]=scicosim(state,t,t,sim,"linear",[.1,.1,.1,.1]);
    Yind=1
    for k=pointo'
        y0(Yind:Yind+size(state.outtb(k),"*")-1)=state.outtb(k)(:);
        Yind=size(state.outtb(k),"*")+1
    end
    xp0=state.x;
    zo0=[xp0;y0];

    F=zeros(nx+ny,nx+nu);
    z0=[x0;u0];
    zer=zeros(nx+nu,1);

    for i=1:nx+nu
        dz=zer;dz(i)=del(i);
        z=z0+dz;
        state.x=z(1:nx);
        Uind=nx+1
        for k=pointi'
            state.outtb(k)=matrix(z(Uind:Uind+size(state.outtb(k),"*")-1),size(state.outtb(k)));
            Uind=size(state.outtb(k),"*")+1;
        end
        [state,t]=scicosim(state,t,t,sim,"linear",[.1,.1,.1,.1]);
        zo=[]
        Yind=1
        for k=pointo'
            zo(Yind:Yind+size(state.outtb(k),"*")-1)=state.outtb(k)(:);
            Yind=size(state.outtb(k),"*")+1
        end
        zo=[state.x;zo];
        F(:,i)=(zo-zo0)/del(i);
    end
    [state,t]=scicosim(state,t,t,sim,"finish",[.1,.1,.1,.1]);

    sys = syslin("c",F(1:nx,1:nx),F(1:nx,nx+1:nx+nu),F(nx+1:nx+ny,1:nx),F(nx+1:nx+ny,nx+1:nx+nu));

endfunction
