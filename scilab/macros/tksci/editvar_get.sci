function outvar=editvar_get(winId)
// Simple variable editor - import data
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez
// mailto:jaime_urzua@yahoo.com
// rev. 0.1
  //
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

 // mprintf("Please wait...");
  base="sciGUITable(win,"+string(winId)+",data";
  varType=TCL_GetVar(base+',type)');
  varni=evstr(TCL_GetVar(base+',ni)'));
  varnj=evstr(TCL_GetVar(base+',nj)'));
  
  outvar=[];
  count=0;  winId=progressionbar('Updating, please wait');

  select varType
  case "constant" then
    outvar=zeros(varni,varnj)
    for j=1:varnj,
      for i=1:varni,
	count=modulo(count+1,100)
	if count==0 then  progressionbar(winId);end
	outvar(i,j)=evstr(TCL_GetVar(base+','+string(i)+','+string(j)+')'))
      end
    end
  case "boolean" then
    outvar(varni,varnj)=%f;
    for j=1:varnj,
      for i=1:varni,
	count=modulo(count+1,100)
	if count==0 then  progressionbar(winId);end
	outvar(i,j)=convstr(TCL_GetVar(base+','+string(i)+','+string(j)+')')=='t')
      end
    end
  case "string" then
    for j=1:varnj,
      for i=1:varni,
	count=modulo(count+1,100)
	if count==0 then  progressionbar(winId);end
	outvar(i,j)=TCL_GetVar(base+','+string(i)+','+string(j)+')')
      end
    end
  case "xlssheet" then
    T=emptystr(varni,varnj)
    V=%nan*ones(varni,varnj)
    for j=1:varnj,
      for i=1:varni,
	count=modulo(count+1,100)
	if count==0 then  progressionbar(winId);end
	q=TCL_GetVar(base+','+string(i)+','+string(j)+')');
	if execstr('v='+q,'errcatch')==0 then
	  V(i,j)=v;
	else
	  T(i,j)=q;
	end
      end
    end
    sheetname=TCL_GetVar(base+',sheetname)')
    outvar=mlist(["xlssheet","name","text","value"],sheetname,T,V)
  else
    if or(varType==["int8","int16","int32","uint8","uint16","uint32"]) then
      execstr('outvar='+varType+'(zeros(varni,varnj))')
      for j=1:varnj,
	for i=1:varni,
	  count=modulo(count+1,100)
	  if count==0 then  progressionbar(winId);end
	  q=evstr(TCL_GetVar(base+','+string(i)+','+string(j)+')'))
	  execstr('outvar(i,j)='+varType+'(q)')
	end
      end
    end
  
  end
  winclose(winId);

endfunction

