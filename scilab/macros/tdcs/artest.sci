function []=artest(f_l,odem,xdim,npts)
//[]=artest(f_l,[odem,xdim,npts])
// Integration of the lorentz system 
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs <=3, npts=[100,1],end;
if rhs <=2  then xdim=100*[-1,1,-1,1,-1,1];end;
if rhs <=1, odem='default';end;
if rhs <=0, f_l='arnol';end
x_message(["Integration of the arnold equation";
          "[20;2;2], is a good initial point"]);
portr3d(f_l,odem,xdim,npts);


endfunction
function [xdot]=arnold(t,x)
//[xdot]=arnold(t,x)
// an example of dynamical system with chaotic behaviour
// due to arnold.
//!
xdot(1)= cos(x(2)) + sin(x(3))
xdot(2)= cos(x(3)) + sin(x(1))
xdot(3)= cos(x(1)) + sin(x(2))


endfunction
function []=iarf(aa)
//[]=iarf([aa])
// Initialisation des parametres aa(6) pour l'equation d'arnold
//      ydot(1)=aa(1)*cos(y(2)) +aa(2)*sin(y(3))
//      ydot(2)=aa(3)*cos(y(3)) +aa(4)*sin(y(1))
//      ydot(3)=aa(5)*cos(y(1)) +aa(6)*sin(y(2))
// si aucun des arguments n'est fourni on utilise des valeurs
// par defaut
//!
[lhs,rhs]=argn(0)
if rhs==0, aa=ones(1,6);end
fort('arset',aa,1,'r','sort');
endfunction
