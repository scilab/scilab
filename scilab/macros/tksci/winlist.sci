function hout=winlist()
// Return a list
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez
// mailto:jaime_urzua@yahoo.com
// rev. 0.1
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
  
  rhs=argn(2);
  if execstr('rep=TCL_GetVar(''sciGUITable(win,id)'')','errcatch')<>0 then
    hout=[];
    return
  end
  h1=evstr(rep);
  if (length(h1)==1) then
    h1=[];
  else
    h1=h1(2:$)';
  end
  hout=h1;
endfunction

