function scipad(varargin)
    // start Scipad editor

    global SCIPADISSTARTING
    // ensure that no concurrent launching occurs
    // this fixes the issue that shows up when quickly
    // clicking twice the menu or icon button in the
    // Scilab window (bug 2226)
    if exists("SCIPADISSTARTING","nolocal") then
        return
    end
    SCIPADISSTARTING = 1;  // guard variable

    // check if Scipad can be launched, given the Scilab configuration
    nwnimode=find(sciargs()=="-nwni");
    noguimode=find(sciargs()=="-nogui");
    if (nwnimode <>[] | noguimode <>[]) then
        clear nwnimode noguimode
        warning(" Scilab in no window no interactive mode : Scipad unavailable");
        abort;
    end;
    clear nwnimode noguimode

    global LANGUAGE

    if with_tk() then
        if ~TCL_ExistInterp("scipad") then
            TCL_EvalStr("interp create scipad")
            TCL_EvalStr("load {"+gettklib()+"} Tk scipad")
            TCL_EvalStr("wm withdraw .","scipad")
            TCL_EvalStr("scipad alias ScilabEval ScilabEval")
        end
        if exists("SCIHOME") then
            if MSDOS then
                TCL_EvalStr("set env(SCIHOME) """+strsubst(SCIHOME,"\","/")+"""","scipad")
            else
                TCL_EvalStr("set env(SCIHOME) """+pathconvert(SCIHOME,%f,%t)+"""","scipad")
            end
        end
        // set Scipad's tmpdir to Scilab's TMPDIR
        // note that global TMPDIR is not needed since TMPDIR is not a global
        // but an environment variable, in fact a local variable of the main
        // level that is inherited/visible in this macro (same as SCI)
        if MSDOS then
            TCL_EvalStr("set tmpdir """+strsubst(TMPDIR,"\","/")+"""","scipad")
        else
            TCL_EvalStr("set tmpdir """+pathconvert(TMPDIR,%f,%t)+"""","scipad")
        end
        // Although the following line might seem to be a bit too much it is
        // designed to take advantage of the ScilabEval sequential mode in
        // order to prevent flushing of events by Scilab to Tcl during the
        // launch of Scipad
        //        TCL_EvalStr("ScilabEval {TCL_EvalStr(""scipad eval {source """""+SCI+ ..
        //                "/modules/scipad/tcl/scipad.tcl""""}"")} ""seq"" ")
        TCL_EvalStr("source """+SCI+"/modules/scipad/tcl/scipad.tcl""","scipad")
        nfiles=argn(2)
        if nfiles>0 then
            for i=1:nfiles
                validfile=%f;
                f=varargin(i)
                select type(f)
                case 1 then filetoopen=string(f); validfile=%t;
                case 8 then filetoopen=string(f); validfile=%t;
                case 10 then filetoopen=f; validfile=%t;
                    ////nice try, but can't be done. The assignement to the argument
                    //// overrides the original function name, and this is correct.
                    //              case 13 then
                    //                  b=macr2tree(f); funname=b(2) //DOESN'T WORK. How else?
                    //                  libmacro=whereis(funname)
                    //                  disp(libmacro,funname)
                    //                  if libmacro<>[] & funname<>[] then
                    //                      a=string(eval(libmacro));libpath=a(1)
                    //                      if libpath<>[] then
                    //                          validfile=%t;
                    //                          //the correspondance between function and file name it is tacitly assumed
                    //                          filetoopen=pathconvert(libpath+funname+".sci",%f)
                    //                      else
                    //                          warning("Function "+funname+"is not contained in a loaded library, "..
                    //                                  +"Scipad doesn''t know where to find its source")
                    //                      end
                    //                  end
                else
                    warning("Scipad cannot open a "+typeof(f)+" object!")
                end
                if validfile then
                    filetoopen=pathconvert(filetoopen,%f,%t);
                    if MSDOS then
                        filetoopen=strsubst(filetoopen,"\","/");
                    end
                    //  Given that scipad is open via a ScilabEval as is done above, the initial
                    //  opening of files has to be done in the same way, so that the command is sequenced
                    //  after scipad is really open
                    //                  TCL_EvalStr("ScilabEval {TCL_EvalStr(""scipad eval {openfile {"+..
                    //                              filetoopen +"}}"")} ""seq"" ")
                    TCL_EvalStr("openfile """+filetoopen+"""","scipad")
                end
            end  // end of "for i=1:nfiles"
        end  // end of "if nfiles>0"
    else
        // with_tk() is %f
        clearglobal SCIPADISSTARTING
        error(" Scilab has not been built with Tcl/Tk: Scipad unavailable")
    end
    clearglobal SCIPADISSTARTING
endfunction
