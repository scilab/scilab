function editvar(varargin)
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
  
  [%_nams]=who('get');
  %_loc_type=type(varargin(1))
  if (%_loc_type~=10) then error(42), end
  %_in_list=find(%_nams==varargin(1));
  if (%_in_list==[]) then	error(42), end
  execstr("%_loc_var="+varargin(1));
  %_loc_type=type(%_loc_var);
  %_allo=find([1;4;10]==%_loc_type);
  if (%_allo==[]) then error(42), end

  disp("Please wait...");
  %_loc_nam=varargin(1);
  %_loc_ni=size(%_loc_var,1);
  %_loc_nj=size(%_loc_var,2);
  %_loc_type=string(%_loc_type);

  TK_EvalStr("set EdVarLoc [sciGUIEditVar -1]");

  %_winId=TK_GetVar("EdVarLoc");

  TK_SetVar("sciGUITable(win,"+%_winId+",data,name)",%_loc_nam);
  TK_SetVar("sciGUITable(win,"+%_winId+",data,type)",string(%_loc_type));
  TK_SetVar("sciGUITable(win,"+%_winId+",data,ni)",string(%_loc_ni));
  TK_SetVar("sciGUITable(win,"+%_winId+",data,nj)",string(%_loc_nj));

  //
  // This is a slow manner to move the data ...
  // Anyone have a faster method than TK_SetVar
  // I would like set some tcl variable like varname(pos_i,pos_j)
  //

  for %_j=1:%_loc_nj,
    for %_i=1:%_loc_ni,
      %_value=string(%_loc_var(%_i,%_j));
      %_varname="sciGUITable(win,"+%_winId+",data,"+string(%_i)+","+string(%_j)+")";
      TK_SetVar(%_varname,%_value);
    end
  end

  TK_EvalStr("sciGUIEditVarDrawGrid "+%_winId);

endfunction
