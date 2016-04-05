//
// Scilab ( httzp://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2013 - Bruno JOFRET
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

function [%cpr, ok] = xcos_simulate(scs_m, needcompile)

    // Load the block libs if not defined
    prot = funcprot();
    funcprot(0);
    if ~exists("Sourceslib") then
        loadXcosLibs();
    end
    funcprot(prot);
    [modelica_libs, scicos_pal_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables();

    // Hook according to SEP066
    function [ok]=invoke_pre_simulate(fun, scs_m, needcompile)
        ok=%f;
        ierr=execstr("[continueSimulation]="+fun+"(scs_m, needcompile);", "errcatch");
        if ierr<>0 then
            disp(_("Error occurred in pre_xcos_simulate: Cancelling simulation."));
            [str,n,line,func]=lasterror();
            mprintf(" at line %d of function %s\n", line, func);
            return
        end
        if ~continueSimulation then
            return
        end
        ok=%t;

        // Force update on the parent in case of scoped modification
        scs_m=resume(scs_m);
    endfunction

    if isdef("pre_xcos_simulate") then
        if type(pre_xcos_simulate) == 15 then
            // If has a multiple implementation (on a list)
            for f=pre_xcos_simulate;
                ok=invoke_pre_simulate(f, scs_m, needcompile);
                if ~ok then
                    %cpr=[];
                    return;
                end
            end
        else
            // If has a unique implementation
            ok=invoke_pre_simulate("pre_xcos_simulate", scs_m, needcompile);
            if ~ok then
                %cpr=[];
                return;
            end
        end
    end

    //**---- prepare from and to workspace stuff ( "From workspace" block )
    scicos_workspace_init()


    //** extract tolerances from scs_m.props.tol
    tolerances = scs_m.props.tol ;
    //** extract solver type from tolerances
    solver = tolerances(6) ;
    //** initialize a "scicos_debug_gr" variable
    %scicos_debug_gr = %f;

    ////////////////////////////////////////////////////////////////
    // Add global environment variable so that scicos is not lost //
    ////////////////////////////////////////////////////////////////
    if needcompile == 4 then
        %state0     = list();
        needcompile = 4;
        curwin      = 1000;
        %cpr        = struct();
        %tcur       = 0;
        %cpr.state  = %state0;
        alreadyran = %f;
    else
        %state0 = %cpr.state;
        alreadyran = %f;
    end

    tf          = scs_m.props.tf;
    %zoom       = 1.4;
    Select      = [];

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
        msg = msprintf(gettext("%s: Error during block parameters evaluation.\n"), "Xcos");
        if getscilabmode() <> "NWNI" then
            messagebox(msg, "Xcos", "error");
        end
        error(msprintf(gettext("%s: Error during block parameters evaluation.\n"), "xcos_simulate"));
    end

    //** update parameters or compilation results
    [%cpr,%state0_n,needcompile,alreadyran,ok] = do_update(%cpr,%state0,needcompile)
    if ~ok then
        error(msprintf(gettext("%s: Error during block parameters update.\n"), "xcos_simulate"));
    end

    //** if alreadyran then set the var choice
    if alreadyran then
        choix = ["Continue";"Restart";"End"]
    else
        choix = [] ;
    end

    issequal = %t;
    //** initial state has been changed
    if ~isequal(%state0_n,%state0) then
        issequal = %f
    else
        //** test typeof outtb element
        for i=1:lstsize(%state0_n.outtb)
            if typeof(%state0_n.outtb(i))<>typeof(%state0.outtb(i))
                issequal = %f
                break
            end
        end
        //** test typeof oz element
        for i=1:lstsize(%state0_n.oz)
            if typeof(%state0_n.oz(i))<>typeof(%state0.oz(i))
                issequal = %f
                break
            end
        end
    end

    //** if state have changed
    //** finish the simulation via do_terminate()
    if ~issequal then
        %state0 = %state0_n
        [alreadyran,%cpr] = do_terminate()
        choix = []
    end

    //** switch to appropriate solver
    if %cpr.sim.xptr($)-1<size(%cpr.state.x,"*") & solver<100 then
        warning(["Diagram has been compiled for implicit solver"
        "switching to implicit Solver"])
        solver = 100 ; //** Magic number
        tolerances(6) = solver ; //** save Magic number solver type
    elseif (%cpr.sim.xptr($)-1==size(%cpr.state.x,"*")) & ((or (solver == [100 101 102])) & size(%cpr.state.x,"*")<>0) then
        message(["Diagram has been compiled for explicit solver"
        "switching to explicit Solver"])
        solver = 0 ; //** Magic number
        tolerances(6) = solver ; //** save Magic number solver type
    end

    //** ask user what to do
    if choix<>[] then
        //** open dialog box
        to_do = choose(choix,"What do you want to do")

        //** if cancel then exit
        if to_do==0 then
            ok = %f
            return
        end

        select choix(to_do)

        case "Continue" then
            needstart = %f ;
            state     = %cpr.state ;

        case "Restart" then
            needstart = %t ;
            state     = %state0 ;

        case "End" then
            state     = %cpr.state ;
            needstart = %t ;
            tf        = scs_m.props.tf;

            //Alan: Cannot call do_terminate() here ?
            //Answer: No, because do_terminate() doesn't
            //          return control to the user

            //** run scicosim via 'finish' flag
            ierr = execstr("[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,"+..
            "''finish'',tolerances)","errcatch")

            %cpr.state = state
            alreadyran = %f

            //** error case
            if ierr<>0 then
                str_err = split_lasterror(lasterror());
                kfun    = curblock()
                corinv  = %cpr.corinv

                if kfun<>0 & length(corinv) > kfun then //** block error
                    path = corinv(kfun)
                    //** get error cmd for the block
                    get_errorcmd(path,"End problem.",str_err);

                else //** simulator error
                    message(["End problem:";str_err])
                    //scf(curwin);
                end
                ok = %f
            end

            return
        end

    else //** Normal first start simulation

        needstart = %t
        state     = %state0

    end

    //gh_win = gcf();

    //** scicos initialization
    if needstart then
        //** if the simulation has already run
        //** and is not finished then call do_terminate
        if alreadyran then
            [alreadyran,%cpr] = do_terminate()
            alreadyran = %f ;
        end
        //** set initial values for a new simulation
        %tcur      = 0
        %cpr.state = %state0
        tf         = scs_m.props.tf;
        if tf*tolerances==[] then
            message(["Simulation parameters not set";"use setup button"]);
            return;
        end

        //** Run the normal first start simulation here

        //** run scicosim via 'start' flag
        ierr = execstr("[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,"+..
        "''start'',tolerances)","errcatch")

        %cpr.state = state ; //** save the state
        //** error case
        if ierr<>0 then
            str_err=split_lasterror(lasterror());
            kfun=curblock()
            corinv=%cpr.corinv

            if kfun<>0 & length(corinv) > kfun then  //** block error
                path=corinv(kfun)
                //** get error cmd for the block
                disp(str_err);
                get_errorcmd(path,gettext("Initialisation problem"),str_err);


            else //** simulator error
                message(["Initialisation problem:";str_err])
                //scf(curwin);
            end

            ok = %f;
            //xset('window',curwin)
            return
        end
        //scf(gh_win);
        //xset('window',win);
    end

    //** scicos simulation
    tf = scs_m.props.tf

    // Inform Xcos the simulator is going to run
    if getscilabmode() <> "NWNI"
        xcosSimulationStarted();
    end

    //** run scicosim via 'start' flag
    ierr = execstr("[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,"+..
    "''run'',tolerances)","errcatch")

    %cpr.state = state

    //** no error
    if ierr==0 then
        alreadyran = %t;
        //** we are at the end of the simulation
        //** finish the simulation via do_terminate()
        if tf-t<tolerances(3) then
            //disp('fin');
            //Alan : removing do_terminate() here because it
            //       doesn't return control
            //[alreadyran,%cpr]=do_terminate()
            needstart  = %t;
            alreadyran = %f;
            //** run scicosim via 'finish' flag
            ierr = execstr("[state,t]=scicosim(%cpr.state,tf,tf,%cpr.sim,"+..
            "''finish'',tolerances)","errcatch")

            %cpr.state = state;

            //** error case
            if ierr<>0 then
                str_err = split_lasterror(lasterror());
                kfun   = curblock()
                corinv = %cpr.corinv

                if kfun<>0 & length(corinv) > kfun then //** block error
                    path = corinv(kfun)
                    //** get error cmd for the block
                    get_errorcmd(path,gettext("End problem"),str_err);
                else //** simulator error
                    message(["End problem:";str_err])
                    //scf(curwin);
                end
            end
        else
            %tcur = t;
        end
        //** error case
    else
        str_err = split_lasterror(lasterror());

        alreadyran = %f;
        kfun       = curblock();
        corinv     = %cpr.corinv;
        if kfun<>0 & length(corinv) > kfun then //** block error
            path = corinv(kfun);
            //** get error cmd for the block
            execstr(get_errorcmd(path,gettext("Simulation problem"),str_err));
        else //** simulator error
            message(["Simulation problem:";str_err])
            //scf(curwin);
        end
        ok = %f;
    end

    // Restore saved variables in Scilab environment ("To workspace" block)

    // First step: Search the %cpr tree for TOWS_c blocks, and extract the variable names.
    path = %cpr.sim;
    Names = [];
    buff_sizes = [];
    increment = 1;
    for i=1:size(path.funs)
        increment2 = path.ipptr(i+1);
        if (increment2 - increment <> 0) then   // ipar has at least a few elements
            space = increment2 - increment;     // The number of elements that the current block pushed into ipar
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
            execstr("NamesIval  = "+Names(i)+"_val;");
            execstr("NamesIvalt = "+Names(i)+"_valt;");
            // If input is a matrix, use function matrix() to reshape the saved values
            // Check condition using time vector, if we have more values than time stamps, split it
            if (size(NamesIval, "r") > size(NamesIvalt, "r")) then
                nRows  = size(NamesIvalt, "r");
                nCols  = size(NamesIval, "c");
                nCols2 = size(NamesIval, "r") / nRows;
                NamesIval = matrix(NamesIval, nCols, nCols2, nRows);
            end
            ierr = execstr(Names(i)+" = struct(''values'', NamesIval, ''time'', NamesIvalt)", "errcatch");
            if ierr <> 0 then
                str_err = split_lasterror(lasterror());
                message(["Simulation problem" ; "Unable to resume To Workspace Variable {"+Names(i)+"}:" ; str_err]);
                break;
            end
            if i == 1 then
                Resume_line_args = Names(1);
            else
                Resume_line_args = Resume_line_args + ", " + Names(i);  // Concatenate the variable names up to the last one
            end
        end
        Resume_line = "[" + Resume_line_args + "] = resume(" + Resume_line_args + ");";  // Build the message
        // Will execute Resume_line at the end of the function, to run the following Hook instructions
    end

    // Hook according to SEP066
    function ok=invoke_post_simulate(fun, %cpr, scs_m, needcompile)
        ok=%f;
        ierr=execstr(fun+"(%cpr, scs_m, needcompile);", "errcatch");
        if ierr<>0 then
            disp(_("Error in post_xcos_simulate: ending simulation."))
            return
        end
        ok=%t
        // Force update on the parent in case of scoped modification
        scs_m=resume(scs_m);
    endfunction

    if isdef("post_xcos_simulate") then
        if type(post_xcos_simulate) == 15 then
            // If has a multiple implementation (on a list)
            for f=post_xcos_simulate;
                ok=invoke_post_simulate(f, %cpr, scs_m, needcompile);
                if ~ok then
                    %cpr=[];
                    return;
                end
            end
        else
            // If has a unique implementation
            ok=invoke_post_simulate("post_xcos_simulate", %cpr, scs_m, needcompile);
            if ~ok then
                %cpr=[];
                return;
            end
        end
    end

    // Executing the resume() function at the end, because it does not return control
    if ~isempty(Names) then
        execstr(Resume_line);
    end

endfunction
