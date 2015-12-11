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

function  [%cpr,ok] = xcos_compile(scs_m)

    ////////////////////////////////////////////////////////////////
    // Add global environment variable so that scicos is not lost //
    ////////////////////////////////////////////////////////////////
    %state0     = list();
    needcompile = 4;
    curwin      = 1000;
    %cpr        = struct();
    %tcur       = 0;
    %cpr.state  = %state0;
    tf          = scs_m.props.tf;
    alreadyran  = %f;

    show_trace = %f; //** tracing and profiling (probably by Alan L. )

    if show_trace then
        disp("c_pass0:"+string(timer()));
    end

    if ~exists("%scicos_solver") then
        %scicos_solver = 0 ;
    end

    if ~exists("%scicos_debug_gr") then
        %scicos_debug_gr = %f; //** debug mode : default is "%f"
    end

    // modelica_libs needed to compile Modelica blocks
    if ~exists("modelica_libs") then
        modelica_libs = getModelicaPath();
    end

    par = scs_m.props;

    if alreadyran then
        // terminate current simulation
        do_terminate();
    end

    //** extract tolerances from scs_m.props.tol
    tolerances = scs_m.props.tol ;
    //** extract solver type from tolerances
    solver = tolerances(6) ;

    // Propagate context through all blocks
    %scicos_context = struct();
    context = scs_m.props.context;
    //** context eval here
    [%scicos_context, ierr] = script2var(context, %scicos_context);

    // For backward compatibility for scifunc
    if ierr==0 then
        %mm = getfield(1,%scicos_context)
        for %mi=%mm(3:$)
            ierr = execstr(%mi+"=%scicos_context(%mi)","errcatch")
            if ierr<>0 then
                break; //** in case of error exit
            end
        end
    end
    // End of for backward compatibility for scifuncpagate context values

    [scs_m,%cpr,needcompile,ok] = do_eval(scs_m, %cpr, %scicos_context);
    if ~ok then
        %cpr=list();
        return
    end

    //** update parameters or compilation results
    [%cpr,%state0_n,needcompile,alreadyran,ok] = do_update(%cpr,%state0,needcompile)

endfunction
