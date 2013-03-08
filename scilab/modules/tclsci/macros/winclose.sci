// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - Jaime Urzua
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function sta=winclose(h)
// Close a window
// This file is part of sciGUI toolbox
// rev. 0.1  
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

warnobsolete("close", "5.5.0");

// Java waitbar or progressbar
if type(h) == 9 then
  delete(h);
  sta=1
  return
end

  if execstr('TCL_GetVar(''sciGUITable(win,id)'')','errcatch')<>0 then
    sta=1
    return
  end
  sta=0;
  for j=1:length(h),
    TCL_EvalStr('sciGUIDestroy '+string(h(j)));
  end
endfunction
