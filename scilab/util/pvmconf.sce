mode(-1);
Env=unix_g('set');
k=grep(Env,["COMPUTERNAME","computername"]);
if k<>[] then
  k=k(1);
  w=Env(k);
  k=strindex(w,"=");
  name=stripblanks(part(w,k+1:length(w)));
  sci=strsubst(SCI,'/','\')+'\';
  conf=name+' dx=%PVM_ROOT%\lib\WIN32\pvmd3.exe ep='+sci+'bin;'+sci+'pvm3\bin\WIN32';
  mputl(conf,sci+'.pvmd.conf');
end
quit

