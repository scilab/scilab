function []=plotdb(f,gain,thetaphi,modxy)
//plotdb(f), plotdb(f,gain), plotdb(f,modxy), plotdb(f,gain,tetaphi)
//plotdb(f,gain,modxy), plotdb (f,gain,tetaphi,modxy)
//% description
//plots a projection of the function f(x,y). The point of view of  the
//projection is from above the x-y plane of the matrix grid.The matrix
//is rotated by theta in the x-y plane about  the geometric  center of
//the matrix and then tilted in the y-z plane by -phi.
//The resulting plot is the  projection of  the rotated  function into
//the x-y plane.
//The independent grid variables are implicitly  taken to be the indi-
//ces of the matrix f.
//% description of input variables
// f        :matrix to be plotted
// gain     :plot is actually of gain*f (default  1)
//           When gain='agc' the gain is  calculated to obtain  a plot
//           whose heigth to width ratio is 1/4.
//           The agc gain is printed on the screen.
//
// thetaphi : [teta phi]  (defaut  [%pi/4,%pi/3])
//           theta :rotation angle (around z-axis) in the x-y plane
//           phi   :elevation viewing angle (around x-axis) above the
//                  x-y plane
//  modxy   :flag with possible values:
//            'x' : only horizontal lines are drawn
//            'y' : only vertical  lines are drawn
//            'xy' : both h and v lines are drawn
//!
//author: C. Bunks  date: Jan 22, 1990
//defaults for gain, theta, and phi
// Copyright INRIA
[lhs,rhs]=argn(0),
select rhs
   case 1 then,
     gain=1;
     thetaphi=[%pi/4,%pi/3];
     modxy='xy'
   case 2 then,
     if type(gain)==10 then
       select gain
         case 'agc' then modxy='xy'
         case 'x'   then modxy='x';gain=1
         case 'y'   then modxy='y';gain=1
         case 'xy'  then modxy='xy';gain=1
         else error('appel incorrect')
       end,
       thetaphi=[%pi/4,%pi/3];
     else
       if and(size(gain)==[1,2]) then,
          thetaphi=gain;
          gain=1;
          modxy='xy';
       else,
          thetaphi=[%pi/4,%pi/3];
          modxy='xy';
       end,
     end
   case 3 then,
     if type(thetaphi)==10 then
       modxy=thetaphi;
       if type(gain)==10 then,
         thetaphi=[%pi/4,%pi/3];
       else,
         if and(size(gain)==[1,2]) then,
            thetaphi=gain;
            gain=1;
         else,
            thetaphi=[%pi/4,%pi/3];
         end,            
       end,
     else
       modxy='xy';
     end
end,
theta=thetaphi(1),phi=thetaphi(2);
st=sin(theta);ct=cos(theta);sp=sin(phi);cp=cos(phi);
//grid variables
[nr,nc]=size(f);
x=(-(nc-1)/2:(nc-1)/2);
y=((nr-1)/2:-1:-(nr-1)/2)';
xe=[-(nc-1)/2,(nc-1)/2],
ye=[(nr-1)/2,-(nr-1)/2];
//gain
   range=(maxi(f)-mini(f));
   if gain=='agc' then,
      if range<>0 then,
         gain=.25*sp*(nr*abs(st)+nc*abs(ct))/range;
         print(6,gain),
      else,
         gain=1;
         print(6,'NOTE: function is constant'),
      end,
   end,
   f=f*gain;
//rotation and projection
//(xr,yr)=PROJ(ROT(theta,phi)*(x,y,f))
//where ROT is the rotation matrix:
//   [ct,-st,0;cp*st,cp*ct,-sp;sp*st,sp*ct,cp]
//and PROJ((xr,yr,fr))=(xr,yr)
//frame
xmax=maxi(ct*xe-st*ye);xmin=mini(ct*xe-st*ye);
ymax=[]
ymin=[]
for k=1:nr
   yk=cp*(st*x+ct*y(k)*ones(x))+sp*f(k,:)
   ymax=maxi([ymax,yk])
   ymin=mini([ymin,yk])
end
dx=0.04*(xmax-xmin)
dy=0.04*(ymax-ymin)

plot2d(0,0,[1],"012",' ',[xmin-dx,ymin-dy,xmax+dx,ymax+dy])
//plot matrix in both directions
if part(modxy,1)=='x' then
   for k=1:nr,
      plot2d([ct*x-st*y(k)*ones(x)]',...
             [cp*(st*x+ct*y(k)*ones(x))+sp*f(k,:)]',[1],"000");
   end,
end
if part(modxy,length(modxy))=='y' then
   for k=1:nc,
     plot2d([ct*x(k)*ones(y)-st*y]',...
            [cp*(st*x(k)*ones(y)+ct*y)+sp*f(:,k)]',[1],"000");
   end,
end


