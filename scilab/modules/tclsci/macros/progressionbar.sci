// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - Jaime Urzua
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function hout=progressionbar(h,mes)
// Draw a Progression Bar
// This file is part of sciGUI toolbox
// rev. 0.1
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

  sciGUI_init()
  rhs=argn(2);
  h1=-1;
  m1="";

  allok=%f;
  if (rhs==0) then
    // Create a new Progression bar with default values
    allok=%t;
    m1=gettext('Progression bar');
  end
  
  if ((rhs==1)&(type(h)==1)) then 
    // Create a new or update
    allok=%t;
    h1=round(abs(h));
  end

  if ((rhs==1)&(type(h)==10)) then
    // Create a new
    allok=%t;
    m1=h;
  end

  if ((rhs==2)&(type(h)==1)&(type(mes)==10)) then
    // Create or update
    allok=%t;
    h1=round(abs(h));
    m1=mes;
  end

  if (~allok) then
    error(42);
  end


  if (m1=="") then
    TCL_EvalStr('set progressionbartmp [sciGUIBarProgression '+string(h1)+' """"]');
  else
    TCL_EvalStr('set progressionbartmp [sciGUIBarProgression '+string(h1)+' ""'+m1+'""]');
  end
  hout=evstr(TCL_GetVar('progressionbartmp'));

endfunction

