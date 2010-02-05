// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function levels=contour2d(x,y,z,nz,style,strf,leg,rect,nax,void)

[lhs,rhs]=argn(0)
if rhs<4 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "contour2d", 4));
end

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


fig=gcf();
autoc=fig.auto_clear;
if autoc=="on" then, clf(),end
a=gca();
v=fig.immediate_drawing;
fig.immediate_drawing="off"
fig.auto_clear="off"
cnt=0


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
elseif (~rectSpecified) then
  // get rect any way for clipping
  rect = [min(x),min(y),max(x),max(y)];
end

k=1;n=yc(k); c=0; level = %inf;
while k < length(xc)
   n = yc(k)
   if xc(k) ~= level then 
     c = c+1; level = xc(k),levels=[level levels];

     if cnt>0 then glue(a.children(1:cnt)),cnt=0,end

   end
   err = execstr('plot2d(xc(k+(1:n)),yc(k+(1:n)),'+opts+')','errcatch','m');
   frameflag = 0 ;
   
   // add a test to see if plot2d call succeed
   // and, if not, restore good figure property values before exiting
   if err <> 0
     mprintf("Error %d : in plot2d called by contour2d",err);
     fig.immediate_drawing=v;
     fig.auto_clear=autoc;
     return;
   end
   

    unglue(a.children(1))
    cnt = cnt+1

   if stripblanks(fpf)<>'' then
      labelText = " " + msprintf(fpf,level);
	  labelPos = [xc(k+1+n/2),yc(k+1+n/2)];
	  labelBox = stringbox(labelText, labelPos(1), labelPos(2));
	  // check that the text is not outside the box
	  // better than clipping to avoid half cut strings
      if labelBox(1,1) > rect(1) & labelBox(2,1) > rect(2) & ...
         labelBox(1,3) < rect(3) & labelBox(2,3) < rect(4) then
        xstring(labelPos(1),labelPos(2),labelText)
		e = gce();e.clip_state = "off";
        cnt=cnt+1;
      end
   end
   k=k+n+1;
end


 if cnt>0 then glue(a.children(1:cnt)),cnt=0,end
 set('current_obj',a);
 fig.immediate_drawing=v;
 fig.auto_clear=autoc;
 draw(fig)

endfunction
