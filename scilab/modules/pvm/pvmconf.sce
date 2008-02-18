// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


mode(-1)
Env=unix_g('set');
k=grep(Env,["COMPUTERNAME","computername"])
if k<>[]
  k=k(1);
  w=Env(k);
  k=strindex(w,"=");
  name=stripblanks(part(w,k+1:length(w)));
  sci=strsubst(SCI,'/','\')+'\';
  conf=name+' dx=%PVM_ROOT%\lib\WIN32\pvmd3.exe ep='+sci+'bin;'+sci+'module\pvm\src\pvm3\bin\WIN32';
  mputl(conf,sci+'.pvmd.conf');
end
exit

