function ilib_gen_gateway(name,table)
//------------------------------------
// generate an interface gateway named name
// from table table

k=strindex(name,['/','\'])
if k~=[] then
  path=part(name,1:k($))
  name=part(name,k($)+1:length(name))
else
  path=''
end
name=strsubst(name,'.c','')

[mt,nt]=size(table);
if nt<>2 then error('second argument has wrong size ');end 

t=[ '#include <mex.h> ';
    'extern Gatefunc ' + table(:,2) + ';';
    'static GenericTable Tab[]={'
    '  {(Myinterfun)sci_gateway,'+table(:,2)+',""'+table(:,1)+'""},'
    '};'
    ' '
    'int C2F('+name+')()'
    '{'
    '  Rhs = Max(0, Rhs);'
    '  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);'
    '  return 0;'
    '}'];
mputl(t,path+name+'.c')    
