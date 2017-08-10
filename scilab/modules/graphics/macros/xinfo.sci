// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function xinfo( newMessage )
    // xinfo function
    // Set the text of the message bar of the current figure

    if argn(2)<1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "xinfo", 1));
    end

    curFig = gcf() ;
    curFig.info_message = newMessage ;

endfunction
