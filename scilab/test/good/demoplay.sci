function demoplay(filename)
// Scilab Demo Play GUI
// This routine is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez
// mailto:jaime_urzua@yahoo.com
// rev. 0.2 2004/06/24
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
  
  sciGUI_init()

  goon=%f;
  if (argn(2)==0) then
    filename=SCI+'/demos/sciGUI/test_demoplay.dpf';
    goon=%t;
  end

  if (argn(2)==1) then
    if (type(filename)==10) then goon=%t; end
  end

  if (goon) then
    ll=lines();lines(0);
    TCL_EvalStr('set editvartmp [sciGUIGenFileName ]');
    slideFile=strsubst(TMPDIR,'\','/')+'/'+TCL_GetVar('editvartmp');
    fileFile=strsubst(filename,'\','/');
    TCL_EvalStr('set demoplaytmp [sciGUIDemoPlay -1 ""'+fileFile+'"" ""'+slideFile+'"" ]');
    demoplaytmp=evstr(TCL_GetVar('demoplaytmp'));
    while %t
      realtimeinit(0.1);
      realtime(0);realtime(0.1);
      demoplayfind=find(winlist()==demoplaytmp);	
      if (demoplayfind==[]) then break; end
    end
    lines(ll(2),ll(1))
  else
    error(49);
  end
endfunction
