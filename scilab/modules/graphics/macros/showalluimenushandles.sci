// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// This function is to warn user that he's using a deprecated function

function showalluimenushandles(fig)

rhs = argn(2);

if rhs <> 1 then
    error(msprintf(_("%s: Wrong number of input argument: %d expected.\n"), "showalluimenushandles", 1));
end

if typeof(fig)<>"handle" | fig.type<>"Figure" then
    error(msprintf(_("%s: Wrong type for input argument: ''%s'' handle expected.\n"), "showalluimenushandles", "Figure"));
end

set(get(0), "ShowHiddenHandles", "on");

warnobsolete("set(get(0), ""ShowHiddenHandles"", ""on"");", "5.4.1");

endfunction
