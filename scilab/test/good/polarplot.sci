function polarplot(theta,rho,style,strf,leg,rect)
[lhs,rhs]=argn(0)
if rhs<=0 then
  theta=0:.01:2*%pi;rho=sin(2*theta).*cos(2*theta)
  xbasc();polarplot(theta,rho)
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
execstr('plot2d(x,y,'+strcat(opts,',')+')')

step=rm/5
r=step;dr=0.02*r;
for k=1:4
  xset("dashes",3)
  xarc(-r,r,2*r,2*r,0,360*64)
  xset("dashes",1)
  xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))
  r=r+step
end
xset("dashes",1)
xarc(-r,r,2*r,2*r,0,360*64)
xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))

rect=xstringl(0,0,'360');w=rect(3);h=rect(4);d=sqrt(w^2+h^2)/1.8
r=rm+d
for k=0:11
  xset("dashes",3)
  xsegs([0;rm*cos(k*(%pi/6))],[0;rm*sin(k*(%pi/6))])
  xset("dashes",1)
  xstring(r*cos(k*(%pi/6))-w/2,r*sin(k*(%pi/6))-h/2,string(k*30))
end
xset("dashes",1)
endfunction
