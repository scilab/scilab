function contour2d(x,y,z,nz,style,strf,leg,rect,nax,void)
opts=[]
if ~exists('style','local')==1 then 
  if size(nz,'*')==1 then
    style=1:nz,
  else
    style=1:size(nz,'*')
  end
end
if exists('strf','local')==1 then 
  yflag=part(strf,2)
  if or(yflag==['2' '4' '6' '8']) then
     rect=[min(x),min(y),max(x),max(y)]
    yflag=string(evstr(yflag)-1)
    strf=part(strf,1)+yflag+part(strf,3)
  end
  opts=[opts,'strf=strf'],
end
if exists('leg','local')==1 then opts=[opts,'leg=leg'],end
if exists('rect','local')==1 then opts=[opts,'rect=rect'],end
if exists('nax','local')==1 then opts=[opts,'nax=nax'],end
if exists('logflag','local')==1 then opts=[opts,'logflag=logflag'],end
if exists('frameflag','local')==1 then opts=[opts,'frameflag=frameflag'],end
if exists('axesflag','local')==1 then opts=[opts,'axesflag=axesflag'],end
opts=strcat([opts,"style=style(c)"],',')
[xc,yc]=contour2di(x,y,z,nz);
fpf=xget("fpf");if fpf=='' then fpf='%.3f',end

k=1;n=yc(k);c=1;level=xc(k)
while k+yc(k)<size(xc,'*')
  n=yc(k);
  if xc(k)<>level then 
    c=c+1;level=xc(k),
  end
  execstr('plot2d(xc(k+(1:n)),yc(k+(1:n)),'+opts+')')
  if stripblanks(fpf)<>'' then
    xstring(xc(k+1+n/2),yc(k+1+n/2),sprintf(fpf,level))
  end
  k=k+n+1;
end
endfunction
