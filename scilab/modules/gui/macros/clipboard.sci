// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - INRIA - Allan CORNET
// Copyright (C) 20048- INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// clipboard Copy and paste strings to and from system clipboard.

function s=clipboard(varargin)

    if getscilabmode()<>"NWNI" then

        rhs=argn(2)

        if ( (rhs == 1) & (varargin(1) == "paste") ) then
            s=ClipBoard(varargin(1));
        else
            if (rhs == 2) then
                if (varargin(1) == "copy") then
                    param=string(varargin(2));
                    ClipBoard(varargin(1),param);
                    s=[];
                    break;
                else
                    if (varargin(1) == "do") then
                        if ( (varargin(2) == "copy") | (varargin(2) == "paste") | (varargin(2) == "empty") ) then
                            ClipBoard(varargin(1),varargin(2));
                            s=[];
                            break;
                        else
                            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'' or ''%s'' expected.\n"),"clipboard", 2,"copy","paste","empty"));
                        end
                    else
                        if ( type(varargin(1))==1 ) & ( (varargin(2) == "EMF") | (varargin(2) == "DIB") ) then
                            ClipBoard(varargin(1),varargin(2));
                            s=[];
                            break;
                        else
                            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "clipboard", 2, "EMF", "DIB"));
                        end
                    end
                end
            else
                error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "clipboard", 1, 2));
            end
        end
    else
        error(msprintf(gettext("%s: function not available in NWNI mode.\n"), "clipboard"));
    end

endfunction
