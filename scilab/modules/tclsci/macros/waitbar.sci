function hout=waitbar(x,mes,h)
// Draw a wait bar
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez 
// mailto:jaime_urzua@yahoo.com
// rev. 0.1
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
  
  sciGUI_init()
  
  x1=-1;
  m1="";
  h1=-1;
  rhs=argn(2);
  doerror=%t;

  if (rhs==0) then
    m1="This is an example"
    doerror=%f;
  end

  if ((rhs==1)&(type(x)==1)) then
    if abs(x)>1 then error(49); end
    x1=abs(x);
    doerror=%f;
  end

  if ((rhs==1)&(type(x)==10)) then
    x1=0;
    m1=x;
    doerror=%f;
  end


  if (rhs==2) then
    if ((type(x)==1)&(type(mes)==10)) then
      if abs(x)>1 then error(49); end
      x1=abs(x);
      m1=mes;
      doerror=%f;
    end
    if ((type(x)==1)&(type(mes)==1)) then
      if abs(x)>1 then error(49); end
      x1=abs(x);
      h1=abs(mes);
      doerror=%f;
    end
    if ((type(x)==10)&(type(mes)==1)) then
      m1=x;
      h1=mes;
      doerror=%f;
    end
  end

  if (rhs==3) then
    if ((type(x)==1)&(type(mes)==10)&(type(h)==1)) then
      if abs(x)>1 then error(49); end
      x1=abs(x);
      m1=mes;
      h1=abs(h);
      doerror=%f;
    end
  end

  if doerror then	error(49); end
  if (m1=="") then
    TCL_EvalStr('set waitbartmp [sciGUIBarWait '+string(h1)+' """" '+string(x1)+']');
  else
    TCL_EvalStr('set waitbartmp [sciGUIBarWait '+string(h1)+' ""'+m1+'"" '+string(x1)+']');
  end
  hout=evstr(TCL_GetVar('waitbartmp'))
endfunction
