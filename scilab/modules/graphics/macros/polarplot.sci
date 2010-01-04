// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function polarplot(theta,rho,style,strf,leg,rect)
  [lhs,rhs]=argn(0)
  if rhs<=0 then
    theta=0:.01:2*%pi;rho=sin(2*theta).*cos(2*theta)
    clf();polarplot(theta,rho)
    return
  end
  if size(theta,1)==1 then theta=theta(:),end
  if size(rho,1)==1 then rho=rho(:),end
  rm=max(rho)
  x=rho.*cos(theta)
  y=rho.*sin(theta)

  opts=[]
  isstrf=%f ,isframeflag=%f,isrect=%f
  if exists('style','local')==1 then opts=[opts,'style=style'],end
  if exists('strf','local')==1 then 
    opts=[opts,'strf=strf'],
    isstrf=%t
  end
  if exists('leg','local')==1 then opts=[opts,'leg=leg'],end
  if exists('rect','local')==1 then
    opts=[opts,'rect=rect'],
    isrect=%t
  end
  if exists('frameflag','local')==1 then 
    opts=[opts,'frameflag=frameflag'],
    isframeflag=%t
  end

  if size(opts,2)<rhs-2 then  error('invalid named arguments'),end
  if ~isrect then
    rect=[-rm -rm rm rm]*1.1
    opts=[opts,'rect=rect']
  end
  if isstrf& isframeflag then
    error('frameflag  cannot be used with strf')
  end
  if ~(isstrf) then
    axesflag=0
    opts=[opts,'axesflag=axesflag'],
  end
  if ~(isstrf|isframeflag) then
    frameflag=4
    opts=[opts,'frameflag=frameflag'],
  end
  drawlater()
  execstr('plot2d(x,y,'+strcat(opts,',')+')')

  step=rm/5
  r=step;dr=0.02*r;
  for k=1:4
    xarc(-r,r,2*r,2*r,0,360*64)
    e = gce();e.clip_state = "off",e.line_style=3
    xset("dashes",1)
    xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))
    //clipping off 
    e = gce();e.clip_state = "off";e.line_style=1
    r=r+step
  end
  xarc(-r,r,2*r,2*r,0,360*64)
  e = gce();e.clip_state = "off";e.line_style=1
  xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))
  //clipping off 
  e = gce();e.clip_state = "off"

  rect=xstringl(0,0,'360');w=rect(3);h=rect(4);d=sqrt(w^2+h^2)/1.8
  r=rm+d
  for k=0:11
    xsegs([0;rm*cos(k*(%pi/6))],[0;rm*sin(k*(%pi/6))])
    e = gce();e.clip_state = "off",e.line_style=3
    xset("dashes",1)
    //clipping off 
    xstring(r*cos(k*(%pi/6))-w/2,r*sin(k*(%pi/6))-h/2,string(k*30))
    e = gce();e.clip_state = "off"
  end
  drawnow()
endfunction
