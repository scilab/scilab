function config()
// Scilab basic configuration
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
  
  vt=format();
  if (vt(1)==0) then v4="v"; else v4="e"; end;
  v5=string(vt(2));
  format('v',10);
  v1=stacksize();
  v1=string(v1(1));
  v2=gstacksize();
  v2=string(v2(1));
  v3=string(ieee());
  if (vt(1)==0) then v4="v"; else v4="e"; end;
  v5=string(vt(2));
  vt=lines();
  v6=string(vt(2));
  v7=string(vt(1));
  if (exists("%ODEOPTIONS")==1) then
    locode=%ODEOPTIONS;
  else
    locode=[1,0,0,%inf,0,2,500,12,5,0,-1,-1];
  end
  v8=""
  for j=1:length(locode),
    if ((j==4)&(locode(4)==%inf)) then
      v8=v8+'%inf '
    else
      v8=v8+string(locode(j))+' ';
    end
  end
  vt=v1+' '+v2+' '+v3+' '+v4+' '+v5+' '+v6+' '+v7+' '+v8;
  TCL_EvalStr('sciGUIConfig -1 {'+vt+'}');
  format(v4,evstr(v5));
endfunction
