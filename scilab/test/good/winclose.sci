function sta=winclose(h)
// Close a window
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua  Grez
// mailto:jaime_urzua@yahoo.com
// rev. 0.1  
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

  if execstr('TCL_GetVar(''sciGUITable(win,id)'')','errcatch')<>0 then
    sta=1
    return
  end
  sta=0;
  for j=1:length(h),
    TCL_EvalStr('sciGUIDestroy '+string(h(j)));
  end
endfunction
