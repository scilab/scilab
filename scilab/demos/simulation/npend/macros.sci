function [n]=np()
// Return the size  of the Fortran pendulum 
// Copyright ENPC
  n=1;
  n=fort('np',n,1,'i','sort',1);
endfunction 

function [ydot]=npend ( t, th)
// Fortran version 
//      data r  / 1.0, 1.0, 1.0, 1.0 /
//      data m  / 1.0, 1.0, 1.0, 1.0 /
//      data j  / 0.3, 0.3, 0.3, 0.3 /
  ydot=ones(6,1)
  ydot=fort('npend',3,1,'i',t,2,'d',th,3,'d',ydot,4,'d','sort',4);
endfunction 

function [E]=ener( th)
  E=0.0;
  E=fort('ener',th,1,'d',E,2,'d','sort',2);
endfunction 

function [ydot]=npend3 ( t, th)
// Scilab version of the three link pendulum 
  n=3
  t1 = -th(3)
  t7 = 2*r(1)*m(3)*r(3)*cos(th(1)+t1)
  t16 = 2*cos(th(1)-th(2))*r(1)*r(2)*(m(2)+2*m(3))
  t17 = r(3)**2
  t25 = 2*r(2)*m(3)*r(3)*cos(th(2)+t1)
  t26 = r(2)**2
  t31 = r(1)**2
  ME3S(1,3) = t7
  ME3S(2,1) = t16
  ME3S(1,2) = t16
  ME3S(3,3) = m(3)*t17+J(3)
  ME3S(3,2) = t25
  ME3S(3,1) = t7
  ME3S(2,2) = 4*t26*m(3)+m(2)*t26+J(2)
  ME3S(1,1) = J(1)+m(1)*t31+4*t31*m(2)+4*t31*m(3)
  ME3S(2,3) = t25
  t1 = -th(3)
  t6 = r(1)*m(3)*r(3)*sin(th(1)+t1)
  t15 = r(1)*r(2)*sin(th(1)-th(2))*(m(2)+2*m(3))
  t22 = r(2)*m(3)*r(3)*sin(th(2)+t1)
  CC3S(1,3) = 2*t6
  CC3S(2,1) = -2*t15
  CC3S(1,2) = 2*t15
  CC3S(3,3) = 0
  CC3S(3,2) = -2*t22
  CC3S(3,1) = -2*t6
  CC3S(2,2) = 0
  CC3S(1,1) = 0
  CC3S(2,3) = 2*t22
  t1 = cos(th(2))
  t5 = m(3)*g
  t14 = cos(th(1))
  Const(2,1) = m(2)*g*r(2)*t1+2*r(2)*t1*t5
  Const(3,1) = m(3)*g*r(3)*cos(th(3))
  Const(1,1) = 2*r(1)*t14*m(2)*g+2*r(1)*t14*t5+m(1)*g*r(1)*t14

  ydot(1:n,1)=th((n+1):2*n)
  const= const+ cc3S*( th((n+1):2*n,1).* th((n+1):2*n,1));
  ydot((n+1):2*n,1)= -me3s\const
endfunction 

  
function [E]=ener3(yt)
// Scilab version for th three link pendulum 
  th=yt(1:n);
  thd=yt((n+1):2*n);
  t1 = r(1)**2
  t2 = sin(th(1))
  t3 = t2**2
  t4 = thd(1)**2
  t7 = cos(th(1))
  t8 = t7**2
  t16 = r(1)*t2
  t22 = sin(th(2))
  t27 = (-2*r(1)*t2*thd(1)-r(2)*t22*thd(2))**2
  t35 = (2*r(1)*t7*thd(1)+r(2)*cos(th(2))*thd(2))**2
  t39 = thd(2)**2
  E= m(1)*(t1*t3*t4+t1*t8*t4)/... 
     2+J(1)*t4/2+m(1)*g*t16+m(2)*(t27+t35)/2+...
     J(2)*t39/2+m(2)*g*(2*t16+r(2)*t22)
endfunction 


function []=npend_build_and_load() 
// since this demo can be run by someone 
// who has no write access in this directory 
// we use TMPDIR 

  if ~c_link('npend') then
    cd = getcwd(); 
    chdir(TMPDIR); 
    fcode=mgetl(SCI+'/demos/npend/Maple/dlslv.f');mputl(fcode,'dlslv.f')
    fcode=mgetl(SCI+'/demos/npend/Maple/ener.f');mputl(fcode,'ener.f')
    fcode=mgetl(SCI+'/demos/npend/Maple/np.f');mputl(fcode,'np.f')
    fcode=mgetl(SCI+'/demos/npend/Maple/npend.f');mputl(fcode,'npend.f')
    files = ['npend.o','np.o','ener.o','dlslv.o' ];
    ilib_for_link(['npend';'np';'ener'],files,[],"f");
    exec loader.sce 
    chdir(cd) 
  end
endfunction 

