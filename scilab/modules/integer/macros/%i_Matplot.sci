function %i_Matplot(z,strf,rect,nax,frameflag,axesflag)

[N,M]=size(z)
opts=[];
isframeflag=%f
isaxesflag=%f
narg=0
if exists('strf','local')==1 then 
  opts=[opts,'frameflag='+part(strf,2),'axesflag='+part(strf,3)],
  isframeflag=%t
  isaxesflag=%t
  narg=narg+1
end
if exists('rect','local')==1 then 
  opts=[opts,'rect=rect'],
end
if exists('nax' ,'local')==1 then 
  opts=[opts,'nax=nax'] 
end
if exists('frameflag' ,'local')==1 then 
  if isframeflag then error('frameflag already defined by the strf argument'),end
  opts=[opts,'frameflag=frameflag']  ,
end
if exists('axesflag' ,'local')==1 then 
  if isaxesflag then error('axesflag already defined by the strf argument'),end
  opts=[opts,'axesflag=axesflag']  ,
end
if size(opts,2)+1-narg <argn(2) then  
  error('invalid named arguments'),
end



Max=stacksize();Max=int((Max(1)-Max(2))/3)
if Max<M*N+100 then stacksize(Max+M*N*1.5),end
execstr('Matplot(double(z),'+strcat(opts,',')+')')

endfunction
