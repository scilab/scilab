//  Scipad - programmer's editor and debugger for Scilab
//
//  Copyright (C) 2002 -      INRIA, Matthieu Philippe
//  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
//  Copyright (C) 2004-2008 - Francois Vogel
//
//  Localization files ( in tcl/msg_files/) are copyright of the 
//  individual authors, listed in the header of each file
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file scipad/license.txt
//
function scipad(varargin)
// start Scipad editor

// find out if we are in scilab4 or scilab5, and
//  make some workarounds for scilab4
    if listfiles(SCI+"/modules/scipad/")<>[] then
      scilab5=%t;
    else
      scilab5=%f;
    end
    if ~scilab5 then
      // fake gettext function - no localization of Scilab in Scilab 4.x
      function sss=gettext(sss)
      endfunction
    end

    global SCIPADISSTARTING
    // ensure that no concurrent launching occurs
    // this fixes the issue that shows up when quickly
    // clicking twice the menu or icon button in the
    // Scilab window (bug 2226) [relies on exists(..."nolocal"), only in scilab5]
    if exists("SCIPADISSTARTING","nolocal") & scilab5 then
        return
    end
    SCIPADISSTARTING = 1;  // guard variable

    // check if Scipad can be launched, given the Scilab configuration
    nwnimode=find(sciargs()=="-nwni");
    noguimode=find(sciargs()=="-nogui");
    if (nwnimode <>[] | noguimode <>[]) then
        clear nwnimode noguimode
        warning(gettext(' Scilab in no window no interactive mode : Scipad unavailable.'));
        abort;
    end;
    clear nwnimode noguimode

    if with_tk() then
        if ~TCL_ExistInterp("scipad") then
            TCL_EvalStr("interp create scipad")
            TCL_EvalStr("load {'+gettklib()+'} Tk scipad")
            TCL_EvalStr("wm withdraw .","scipad")
            TCL_EvalStr("scipad alias ScilabEval ScilabEval")
            TCL_EvalStr("set sciprompt 0", "scipad")   ; // <TODO>: FIX THIS! SCILAB TEAM WORK IN PROGRESS...
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
            TCL_EvalStr("set env(SCIINSTALLPATH) """+strsubst(SCI,"\","/")+"""","scipad")
        else
            TCL_EvalStr("set tmpdir """+pathconvert(TMPDIR,%f,%t)+"""","scipad")
            TCL_EvalStr("set env(SCIINSTALLPATH) """+pathconvert(SCI,%f,%t)+"""","scipad")
        end
        if scilab5 then
          TCL_EvalStr("source """+SCI+"/modules/scipad/tcl/scipad.tcl""","scipad")
        else
          TCL_EvalStr("source """+SCI+"/tcl/scipadsources/scipad.tcl""","scipad")
        end
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
//                          warning(gettext('Function ')+funname+gettext('is not contained in a loaded library, ') ..
//                                  +gettext('Scipad doesn't know where to find its source'))
//                      end
//                  end
                else
                    warning(gettext('Scipad cannot open a ')+typeof(f)+gettext(' object!'))
                end
                if validfile then
                    // Tcl handles the filenames correctly with any path separator but on
                    // Windows the separator is \ and in Tcl this is an escape. We cannot
                    // use the scilab macro pathconvert(,"u") alone to solve the problem, as
                    // by construction it creates cygwin paths. Therefore, we replace ad hoc
                    // slashes.
                    filetoopen=pathconvert(filetoopen,%f,%t);
                    if MSDOS then
                        filetoopen=strsubst(filetoopen,"\","/");
                    end
                    // The complication below is needed to comply with ScilabEval sync limitations: what
                    // is executed during a sync is not available in the interpreter after the sync has
                    // ended (because sync launches a new interpreter - same thing as running in a function),
                    // thus sync cannot be used for loading colorization arrays chset and words, and a seq
                    // is used. But then the file opening should be queued after the loading of keywords,
                    // which means the following nested complication is needed
                    TCL_EvalStr("ScilabEval {TCL_EvalStr(""openfile {"+ filetoopen +"}"",""scipad"")} ""seq"" ")
//                    TCL_EvalStr("openfile """+filetoopen+"""","scipad")
                end
            end  // end of "for i=1:nfiles"
        end  // end of "if nfiles>0"
    else
        // with_tk() is %f
        clearglobal SCIPADISSTARTING
        error(gettext('Scilab has not been built with Tcl/Tk: Scipad unavailable.'))
    end
    clearglobal SCIPADISSTARTING
endfunction
