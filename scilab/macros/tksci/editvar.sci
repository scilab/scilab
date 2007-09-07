function editvar(%_name)
// Simple Variable Editor
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez 
// mailto:jaime_urzua@yahoo.com
// rev. 0.2 2004/06/24
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

  sciGUI_init()
  
  if (type(%_name)~=10) then error(44), end
  if execstr("%_var="+%_name,'errcatch')~=0 then error(44), end
  %_type=typeof(%_var)
  if and(type(%_var)~=[1 4 8 10 16])&%_type~="xlssheet" then error(44), end
  namesheet='';
  [%_ni,%_nj]=size(%_var);
  if (%_type=="xlssheet") then
    namesheet=%_var.name;
    %_var=string(%_var);
  else
    %_var=string(%_var);
  end
  
  TCL_EvalStr("set EdVarLoc [sciGUIEditVar -1]");
  %_winId=TCL_GetVar("EdVarLoc");

  TCL_SetVar("sciGUITable(win,"+%_winId+",data,name)",%_name);
  TCL_SetVar("sciGUITable(win,"+%_winId+",data,type)",%_type);
  TCL_SetVar("sciGUITable(win,"+%_winId+",data,ni)",string(%_ni));
  TCL_SetVar("sciGUITable(win,"+%_winId+",data,nj)",string(%_nj));
  if %_type=="xlssheet" then
     
      TCL_SetVar("sciGUITable(win,"+%_winId+",data,sheetname)",namesheet);
      clear namesheet;
  end

  editvar_set_values( %_var, %_winId ) ;

  TCL_EvalStr("sciGUIEditVarDrawGrid "+%_winId);

endfunction