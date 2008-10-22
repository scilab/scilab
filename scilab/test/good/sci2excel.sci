function sci2excel(var,fname,sep,dec)
// var : a matrix of numbers or a matrix of strings
// fname : a character string, the path of the file to create
// sep   : the column separator mark, a tab by default
// dec   : the decimal mark . or ,
// Example
// a=rand(3,5);
// sci2excel(a,'/tmp/foo.txt',sep=',')
if exists('sep','local')==0 then sep=code2str(-40),end
if exists('dec','local')==0 then dec=',',end
if dec<>['.',','] then error('decimal mark must be ''.'' or '','''),end
if type(var)==1 then
  var=string(var)
  if dec<>'.' then var =strsubst(var,'.',','),end
end
t=var(:,1)
for k=2:size(var,2)
  t=t+sep+var(:,k)
end
mputl(t,fname)
endfunction
