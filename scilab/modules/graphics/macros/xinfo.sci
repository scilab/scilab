// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xinfo( newMessage )
// xinfo function
// Set the text of the message bar of the current figure

if argn(2)<1 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "xinfo", 1));
end

curFig = gcf() ;
curFig.info_message = newMessage ;

endfunction
