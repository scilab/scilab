// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function datatipToggle(fig)
//Toggles activation of the datatip edition mode for the given or current
//figure
  if type(fig)<>9|or(fig.type<>"Figure") then
    error(msprintf(_("%s: Wrong size for input argument #%d: A ''%s'' handle expected.\n"),"datatipCreate",1,"Figure"))
  end
  datatipManagerMode(fig)
endfunction
