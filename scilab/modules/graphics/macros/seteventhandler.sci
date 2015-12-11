// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007- INRIA - Jean-Baptiste Silvy
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function seteventhandler(name)

    [lhs,rhs] = argn(0) ;

    if ( rhs <> 1 ) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "seteventhandler", 1));
        return ;
    end

    fig = get("current_figure") ;
    fig.event_handler_enable = "off" ;
    fig.event_handler = name ;
    if ( name <> "" ) then
        fig.event_handler_enable = "on" ;
    end
endfunction
