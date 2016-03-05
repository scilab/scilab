// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// rmdir remove a directory
function [status,msg]=rmdir(varargin)
    lhs=argn(1);
    rhs=argn(2);

    DirName = "";
    status = 0;
    msg = "";
    SubDirMode = %F;

    select rhs
    case 0
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"rmdir"));
        break
    case 1
        DirName = varargin(1);
        break
    case 2
        DirName = varargin(1) ;
        SubDir = convstr(varargin(2),"u");
        if (SubDir == "S") then
            SubDirMode = %T;
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be ''%s''.\n"),"rmdir",2,"s"));
        end
        break
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"rmdir"));
    end

    if ~SubDirMode then
        if findfiles(DirName)<>[] then
            status = 0
            msg = gettext("Error: The directory is not empty.")
        else
            [status,msg] = hidden_rmdir(DirName);
        end
    else
        [status,msg] = hidden_rmdir(DirName);
    end
endfunction
//------------------------------------------------------------------------
function [status,msg]=hidden_rmdir(DirName)
    status = 0;
    msg = "";

    if isdir(DirName) then
        bOK = removedir(DirName);
        if bOK then
            msg = "";
            status = 1;
        else
            msg = msprintf(gettext("%s: An error occurred: %s\n"),"rmdir", gettext("Undefined"));
            status = 0;
        end
    else
        msg = msprintf(gettext("%s: An error occurred: %s\n"),"rmdir", gettext("The system cannot find the file specified."));
        status = 0;
    end

endfunction
//------------------------------------------------------------------------
