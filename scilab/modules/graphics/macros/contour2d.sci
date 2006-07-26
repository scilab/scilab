function levels=contour2d(x,y,z,nz,style,strf,leg,rect,nax,void)
opts=[]
levels=[]
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
rectSpecified = %f ;
if exists('rect','local')==1 then 
  opts=[opts,'rect=rect'];
  rectSpecified = %t ;
end
if exists('nax','local')==1 then opts=[opts,'nax=nax'],end
if exists('logflag','local')==1 then opts=[opts,'logflag=logflag'],end
//if exists('frameflag','local')==1 then opts=[opts,'frameflag=frameflag'],end
if exists('frameflag','local')==1 then 
  opts=[opts,'frameflag=frameflag'],
else 
  frameflag=[]
end
if exists('axesflag','local')==1 then opts=[opts,'axesflag=axesflag'],end
opts=strcat([opts,"style=style(c)"],',')

if or(type(z)==[11 13]) then 
  fun=z;clear z //to avoid redfinition warning
  if type(fun)==11 then comp(fun),end
  z=feval(x,y,fun)
end

[xc,yc]=contour2di(x,y,z,nz);
fpf=xget("fpf");if fpf=='' then fpf='%.3g',end

newstyle = get('figure_style')=='new'
if newstyle then
  fig=gcf();
  autoc=fig.auto_clear;
  if autoc=="on" then, xbasc(),end
  a=gca();
  v=fig.immediate_drawing;
  fig.immediate_drawing="off"
  fig.auto_clear="off"
  cnt=0
end

// we draw the contour with call to plot2d for each level line
// however the data_bounds will be always reset after each plot
// so we must compute before the bounding rectangle (xmin,ymin,xmax,ymax)
// and give it to each plot2d. This is useful for frameflag = (2,4,6,8)
// since the data_bounds are given by the extrema of x and y
if ( frameflag == 2 | frameflag == 4 | frameflag == 6 | frameflag == 8 )
  // get the bounding rectangle
  rect = [min(x),min(y),max(x),max(y)];
  // set the option accordingly
  if ~rectSpecified then
    opts=strcat([opts,'rect=rect'],',');
  end
  // the rect will be taken into account
  frameflag = frameflag - 1 ;
end

k=1;n=yc(k); c=0; level = %inf;
while k < length(xc)
   n = yc(k)
   if xc(k) ~= level then 
     c = c+1; level = xc(k),levels=[level levels];
     if newstyle then 
       if cnt>0 then glue(a.children(1:cnt)),cnt=0,end
     end
   end
   err = execstr('plot2d(xc(k+(1:n)),yc(k+(1:n)),'+opts+')','errcatch','m');
   frameflag = 0 ;
   
   // add a test to see if plot2d call succeed
   // and, if not, restore good figure property values before exiting
   if err <> 0
     mprintf("Error %d : in plot2d called by contour2d",err);
     if newstyle then
       fig.immediate_drawing=v;
       fig.auto_clear=autoc;
     end
     return;
   end
   
   if newstyle then 
      unglue(a.children(1))
      cnt = cnt+1
   end
   if stripblanks(fpf)<>'' then
      xstring(xc(k+1+n/2),yc(k+1+n/2)," "+msprintf(fpf,level))
      if newstyle then cnt=cnt+1,end
   end
   k=k+n+1;
end

if newstyle then 
   if cnt>0 then glue(a.children(1:cnt)),cnt=0,end
   set('current_obj',a);
   fig.immediate_drawing=v;
   fig.auto_clear=autoc;
end

endfunction
