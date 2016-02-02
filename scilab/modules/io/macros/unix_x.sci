// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) XXXX-2008 - INRIA - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function unix_x(cmd)
    //unix_x - shell command execution, results redirected in a window
    //%Syntax
    // unix_x(cmd)
    //%Parameters
    // cmd - a character string
    //%Description
    // cmd instruction is passed to shell, the standard output is redirected
    // to  a  window
    //%Examples
    // unix_x("ls")
    //%See also
    // host unix_g unix_s
    //!

    [lhs,rhs] = argn(0);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"unix_x",1));
    end

    if type(cmd) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"unix_x",1));
    end

    if size(cmd,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"unix_x",1));
    end

    if getos() == "Windows" then
        [rep,stat]=dos(cmd);
        if (stat) then
            messagebox(rep);
        else
            for i=1:size(rep,"*") do write(%io(2),"   "+rep(i));end
            error(msprintf(gettext("%s: error during ""%s"" execution"),"unix_x",cmd));
        end
    else
        tmp=TMPDIR+"/unix.out";
        cmd1="("+cmd+")>"+ tmp +" 2>"+TMPDIR+"/unix.err;";
        stat=host(cmd1);
        select stat
        case 0 then
            rep=mgetl(tmp)
            if (size(rep,"*")==0) | (length(rep)==0) then
                rep=[]
            end
            messagebox(rep);
        case -1 then // host failed
            error(msprintf(gettext("%s: The system interpreter does not answer..."),"unix_x"));
        else //sh failed
            msg=read(TMPDIR+"/unix.err",-1,1,"(a)")
            error("unix_x: "+msg(1))
        end
    end

endfunction
