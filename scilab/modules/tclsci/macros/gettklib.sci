// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function tklib=gettklib()
    tklib = [];
    tcltkver = TCL_GetVersion("numbers");
    // don't use string() but msprintf because of format() - see bug 3602
    major = msprintf("%d",tcltkver(1));
    minor = msprintf("%d",tcltkver(2));
    if getos() == "Windows" then
        tklib = "tk" + major + minor + getdynlibext();
    else
        cur_verbose = ilib_verbose();
        ilib_verbose(0);

        // In the binary version libtk8.X.so has been copied in
        // the SCI/bin directory and scilab script add SCI/bin
        // to the LD_LIBRARY_PATH (or SHLIB_PATH).
        // So, If libtk8.X.so (or .sl) exists in SCI/bin ... it's ok
        libname = "libtk" + major + "." + minor;
        if isfile("SCI/bin/"+libname+getdynlibext()) then
            tklib = libname + getdynlibext();
            ilib_verbose(cur_verbose);
            return;
        end
        cmd ="link(''"+libname+getdynlibext()+"'')";
        ierr = execstr(cmd, "errcatch");
        if (ierr == 0) then
            tklib = libname + getdynlibext();
        else
            cmd ="link(''"+libname+getdynlibext()+".0'')";
            ierr = execstr(cmd, "errcatch");
            if (ierr == 0) then
                tklib = libname + getdynlibext() + ".0";
            else
                warning(msprintf(gettext("Error %s while loading library %s"),lasterror(),libname+getdynlibext()));
            end
        end

        ilib_verbose(cur_verbose);

        clear libname;
        clear cmd;
    end
    clear tcltkver;
    clear major;
    clear minor;
endfunction
