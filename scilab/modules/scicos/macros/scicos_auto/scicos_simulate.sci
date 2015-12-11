//  Scicos
//
//  Copyright (C) Scilab Enterprises - 2013 - Bruno JOFRET
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) 2011 - INRIA - Serge Steer

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

function Info = scicos_simulate(scs_m, Info, updated_vars, flag, Ignb)
    // Function for running scicos simulation in batch mode
    // Info = scicos_simulate(scs_m[,Info][,updated_vars][,flag][,Ignb])
    //
    // scs_m: scicos diagram (obtained by "load file.cos"). Note that
    // the version of file.cos must be the current version. If not, load
    // into scicos and save.
    //
    // updated_vars: a scilab struct containing values of
    // symbolic variables used in the context and Scicos blocks. This
    // is often used to change a parameter in the diagram context. In that
    // case, make sure that in the diagram context the variable is defined such
    // that it can be modified. Say a variable "a" is to be defined in the
    // context having value 1, and later in batch mode, we want to change
    // the value of "a". In that case, in the context of the diagram place:
    //  if ~exists('a') then a=1,end
    // If you want then to run the simulation in batch mode using the value
    // a=2, set:
    // updated_vars.a=2
    //
    // Info: a list. It must be list() at the first call, then use output
    // Info as input Info for the next calls. Info contains compilation and
    // simulation information and is used to avoid recompilation when not
    // needed.
    //
    // flag: string. If it equals 'nw' (no window), then blocks using
    // graphical windows are not executed. Note that the list of such
    // blocks must be updated as new blocks are added.
    //
    // Ignb : matrix of string : The name of blocks to ignore.
    // If flag is set and equal to 'nw' then Ignb contains
    // name of blocks that are added to the list
    // of blocks to ignore.
    //


    //** check/set rhs parameters
    //---------------------------
    if argn(2) == 1 then
        //scicos_simulate(scs_m)
        Info            = list()
        updated_vars = struct()
        flag            = ""
        Ignb            = ""
    elseif argn(2) == 2 then
        //scicos_simulate(scs_m, Info)
        //or scicos_simulate(scs_m,updated_vars)
        //or scicos_simulate(scs_m,flag)
        select typeof(Info)
        case "st" then //scicos_simulate(scs_m,updated_vars)
            updated_vars = Info
            Info=list()
            ku=2
            flag = ""
        case "string" then //scicos_simulate(scs_m,flag)
            flag=Info
            Info=list()
            kf=2
            updated_vars = struct()
        else  //scicos_simulate(scs_m,Info)
            flag = ""
            updated_vars = struct()
            ki=2
        end
        Ignb = ""
    elseif argn(2) == 3 then
        //scicos_simulate(scs_m, Info,updated_vars) or scicos_simulate(scs_m, Info,"nw")
        if type(updated_vars) == 10 then
            flag = updated_vars
            updated_vars = struct()
        else
            ku=3
            flag = ""
        end
        Ignb = ""
    elseif argn(2) >= 4 then
        //scicos_simulate(scs_m, Info,updated_vars,"nw") or
        //scicos_simulate(scs_m, Info,"nw",updated_vars)
        ku=3;kf=4
        if type(updated_vars) == 10 then
            [updated_vars,flag]=(flag,updated_vars)
            kf=3;ku=4
        end
        if argn(2) == 4 then Ignb = "",end
    else
        error(msprintf(_("%s: Wrong number of input arguments. Must be between %d and %d.\n"),...
        "scicos_simulate", 1, 5))
    end

    //Check variable types
    if typeof(scs_m)<>"diagram" then
        error(msprintf(_("%s: Wrong type for input argument #%d: %s data structure expected.\n"),...
        "scicos_simulate",1,"scs_m"))
    end
    if type(Info)<>15 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A list expected.\n"),..
        "scicos_simulate",2))
    end
    if typeof(updated_vars)<>"st" then
        error(msprintf(_("%s: Wrong type for input argument #%d: A list expected.\n"),...
        "scicos_simulate",ku))
    end
    if and(stripblanks(flag)<>["nw" ""]) then
        error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),...
        "scicos_simulate",kf,"nw"))
    end
    if type(Ignb) <> 10 then
        error(msprintf(_("%s: Wrong type for input argument #%d: String array expected.\n"),...
        "scicos_simulate",5))
    end

    if or(sciargs() == "-nogui")|or(sciargs() == "-nwni")  then
        flag = "nw"
    end
    //**blocks to ignore requested by user
    Ignb = matrix(Ignb,1,-1)//make it row vector

    //**blocks to ignore in any cases
    Ignore=["affich",...
    "affich2"]
    //**blocks to ignore with flag=="nw"
    Ignoreb = ["bouncexy", ...
    "cscope", ...
    "cmscope", ...
    "canimxy", ...
    "canimxy3d", ...
    "cevscpe", ...
    "cfscope", ...
    "cscopxy", ...
    "cscopxy3d", ...
    "cmatview", ...
    "cmat3d", ...
    "bplatform2"]

    //** load the scicos function libraries
    //------------------------------------

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
    [modelica_libs, scicos_pal_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables();
    // =====================================================================

    //** initialize a "scicos_debug_gr" variable
    %scicos_debug_gr = %f;


    //** redefine some functions
    prot = funcprot();funcprot(0);
    do_terminate = do_terminate1
    funcprot(prot)


    //check version
    current_version = get_scicos_version()
    scicos_ver = find_scicos_version(scs_m)

    //do version
    if scicos_ver <> current_version then
        ierr = execstr("scs_m = do_version(scs_m, scicos_ver)","errcatch")
        if ierr <> 0 then
            messagebox(_("Can''t convert old diagram (problem in version)"),"modal")
            return
        end
    end

    //** prepare from and to workspace stuff
    //-------------------------------------
    scicos_workspace_init()

    if flag == "nw" then
        Ignore = [Ignore,Ignoreb]
    end
    //add user ignored blocks if any
    Ignore = [Ignore, Ignb]

    //** retrieve Info list
    if Info <> list() then
        [%tcur, %cpr, alreadyran, needstart, needcompile, %state0] = Info(:)
    else
        %tcur = 0;
        %cpr = list();
        alreadyran = %f;
        needstart = %t;
        needcompile = 4;
        %state0 = list();
    end

    //** set solver
    tolerances     = scs_m.props.tol
    solver         = tolerances(6)
    %scicos_solver = solver
    //** set variables of context
    [%scicos_context, ierr] = script2var(scs_m.props.context,struct())
    //overload %scicos_context variables with those defined by updated_vars
    contextvars=fieldnames(%scicos_context)
    updatedvars=fieldnames(updated_vars)
    for k=1:size(updatedvars,"*")
        u=updatedvars(k)
        if or(u==contextvars) then
            %scicos_context(u)=updated_vars(u)
        else
            mprintf(_("Warning the variable %s does not match any context variable name\nignored"),u)
        end
    end
    if ierr == 0 then //++ no error
        [scs_m, %cpr, needcompile, ok] = do_eval(scs_m, %cpr,%scicos_context)
        if ~ok then
            error(msprintf(gettext("%s: Error during block parameters evaluation.\n"), "scicos_simulate"));
        end
        if needcompile <> 4 & size(%cpr) > 0 then
            %state0 = %cpr.state
        end
        alreadyran = %f
    else
        error(["Incorrect context definition, " + lasterror()])
    end

    // perform block suppression only on partial compilation and full compilation
    if needcompile > 1 then
        need_suppress  =%t
    else
        need_suppress = %f
    end

    [%cpr, %state0_n, needcompile, alreadyran, ok] = do_update(%cpr, ...
    %state0, needcompile)

    if ~ok then
        error(msprintf(gettext("%s: Error during block parameters update.\n"), "scicos_simulate"));
    end

    if or(%state0_n <> %state0) then //initial state has been changed
        %state0 = %state0_n
        [alreadyran, %cpr, Resume_line, TOWS_vals, Names] = do_terminate1(scs_m, %cpr)
        choix = []
    end
    if (%cpr.sim.xptr($) - 1) < size(%cpr.state.x,"*") & solver < 100 then
        warning(msprintf(_("Diagram has been compiled for implicit solver\nswitching to implicit solver.\n")))
        solver = 100
        tolerances(6) = solver
    elseif (%cpr.sim.xptr($) - 1) == size(%cpr.state.x,"*") & ...
        (or (solver == [100 101 102])) & size(%cpr.state.x,"*") <> 0 then
        warning(msprintf(_("Diagram has been compiled for explicit solver\nswitching to explicit solver.\n")))
        solver = 0
        tolerances(6) = solver
    end

    if need_suppress then //this is done only once
        for i = 1:length(%cpr.sim.funs)
            if type(%cpr.sim.funs(i)) <> 13 then
                if find(%cpr.sim.funs(i)(1) == Ignore) <> [] then
                    if (%cpr.sim.funs(i)(1) == "bplatform2") then
                        %cpr.sim.funtyp(i) = 4; // BPLATFORM block has function type 5, so need to set it to 4, like the trash block.
                    end
                    %cpr.sim.funs(i)(1) = "trash";
                end
            end
        end
    end

    if needstart then //scicos initialization
        if alreadyran then
            [alreadyran, %cpr, Resume_line, TOWS_vals, Names] = do_terminate1(scs_m, %cpr)
            alreadyran = %f
        end
        %tcur = 0
        %cpr.state = %state0
        tf = scs_m.props.tf;
        if tf*tolerances == [] then
            error(_("Simulation parameters not set:"));
        end
        ierr = execstr("[state, t] = scicosim(%cpr.state, %tcur, tf, %cpr.sim," + ..
        "''start'', tolerances)","errcatch")
        if ierr <> 0 then
            error(_("Initialization problem:"))
        end
        %cpr.state = state
    end

    ierr = execstr("[state, t] = scicosim(%cpr.state, %tcur, tf, %cpr.sim," + ..
    "''run'', tolerances)","errcatch")
    if ierr == 0 then
        %cpr.state = state
        alreadyran = %t
        if (tf - t) < tolerances(3) then
            needstart = %t
            [alreadyran, %cpr, Resume_line, TOWS_vals, Names] = do_terminate1(scs_m, %cpr)
            for i=1:size(Names, "c")
                ierr = execstr(Names(i)+" = TOWS_vals("+string(i)+");", "errcatch");
                if ierr <> 0 then
                    str_err = split_lasterror(lasterror());
                    message(["Simulation problem" ; "Unable to find To Workspace Variable {"+Names(i)+"}:" ; str_err]);
                    break;
                end
            end
        else
            %tcur = t
        end
    else
        error([_("Simulation problem: ");lasterror()])
    end

    Info = list(%tcur, %cpr, alreadyran, needstart, needcompile, %state0)

    // Executing the resume() function at the end, because it does not return control
    if ~isempty(Names) then
        execstr(Resume_line);
    end
endfunction

function [alreadyran, %cpr, Resume_line, TOWS_vals, Names] = do_terminate1(scs_m, %cpr)
    // Copyright INRIA

    // Default return values
    Resume_line = "";
    TOWS_vals = struct("values",[],"time",[]);
    Names = [];

    if prod(size(%cpr)) < 2 then
        alreadyran = %f
        return
    end

    par = scs_m.props;

    if alreadyran then
        alreadyran = %f
        // terminate current simulation
        ierr = execstr("[state, t] = scicosim(%cpr.state, par.tf, par.tf, %cpr.sim, ''finish'', par.tol)", "errcatch")

        %cpr.state = state
        if ierr <> 0 then
            error([_("End problem: ");lasterror()])
        end

        // Restore saved variables in Scilab environment ("To workspace" block)

        // First step: Search the %cpr tree for TOWS_c blocks, and extract the variable names.
        path = %cpr.sim;
        buff_sizes = [];
        increment = 1;
        for i=1:size(path.funs)
            increment2 = path.ipptr(i+1);
            if (increment2 - increment <> 0) then   // ipar has at least a few elements
                space = increment2 - increment;      // The number of elements that the current block pushed into ipar
                if (path.funs(i) == "tows_c") then  // Found a Tow_workspace block
                    varNameLen = space - 2;         // Space minus 'buffer_size' and 'var length' ipar elements
                    varNameCode = path.ipar(increment+2:increment+2+varNameLen-1);  // varName is stored in Scilab code
                    varName = ascii(varNameCode);
                    Names = [Names varName];        // Append varName in Names
                    buff_size  = path.ipar(increment);  // Retrieve buffer size
                    buff_sizes = [buff_sizes buff_size];
                end
                increment = increment2;
            end
        end
        // At the end, Names contains the string names of the variables and buff_sizes the respective buffer sizes

        // Second step: Link the variable names to their values vectors,
        //and call '[names(1), names(2), ...] = resume(names(1), names(2), ...)' to save the variable into Scilab
        if ~isempty(Names) then
            for i=1:size(Names, "c")
                execstr("NamesIval = "+Names(i)+"_val;");
                execstr("NamesIvalt = "+Names(i)+"_valt;");
                // If input is a matrix, use function matrix() to reshape the saved values
                // Check condition using time vector, if we have more values than time stamps, split it
                if (size(NamesIval, "r") > size(NamesIvalt, "r")) then  // In this case, size(Names(i), 'r') = buff_sizes(i) * nCols2
                    nRows  = size(NamesIvalt, "r");
                    nCols  = size(NamesIval, "c");
                    nCols2 = size(NamesIval, "r") / nRows;
                    NamesIval = matrix(NamesIval, nCols, nCols2, nRows);
                end
                if i == 1 then
                    // Replace default struct with value vector of first element of 'Names'
                    TOWS_vals.values = NamesIval;
                    TOWS_vals.time   = NamesIvalt;
                    Resume_line_args = Names(1);
                else
                    ierr = execstr("TOWS_vals = [TOWS_vals struct(''values'', NamesIval, ''time'', NamesIvalt)];", "errcatch");
                    if ierr <> 0 then
                        str_err = split_lasterror(lasterror());
                        message(["Simulation problem" ; "Unable to find To Workspace Variable {"+Names(i)+"}:" ; str_err]);
                        break;
                    end
                    Resume_line_args   = Resume_line_args + ", " + Names(i);  // Concatenate the variable names up to the last one
                end
            end
            Resume_line = "[" + Resume_line_args + "] = resume(" + Resume_line_args + ");";  // Build the message
            // Will execute Resume_line at the end of the main function, because it does not return control
        end
    end
endfunction
