function []=lotest(f_l,odem,xdim,npts,pinit)
//[]=lotest(f_l,[odem,xdim,npts,pinit])
// Integration of the lorenz system 
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs <=3, npts=[1000,0.01],end;
if rhs <=2 then xdim=[-20,20,-30,30,0,50];end;
if rhs <=1, odem='default';end;
if rhs <=0, f_l='loren';end
x_message(["Integration of the lorenz equation";
          "1.e-8[1;1;1], is a good initial point"]);
p3d_ylast=[1;1;1]*1.e-8;
if rhs <=4 then  portr3d(f_l,odem,xdim,npts);
else  portr3d(f_l,odem,xdim,npts,pinit);end


endfunction
function [y]=lorenz(t,x)
//[y]=lorenz(t,x)
// The lorenz system 
//!
y(1)=sig*(x(2)-x(1));
y(2)=ro*x(1) -x(2)-x(1)*x(3);
y(3)=-Beta*x(3)+x(1)*x(2);



endfunction
function []=ilo(sig,ro,Beta)
//[]=ilo([sig,ro,Beta])
// Initialisation des parametres sig ro et Beta
// si aucun des arguments n'est fourni on utilise des valeurs
// par defaut
//!
[lhs,rhs]=argn(0)
if rhs==0,sig=10,ro=28,Beta=8/3;end
[sig,ro,Beta]=resume(sig,ro,Beta)



endfunction
function []=ilof(sig,ro,Beta)
//[]=ilof([sig,ro,Beta])
// Initialisation des parametres sig ro et Beta
// si aucun des arguments n'est fourni on utilise des valeurs
// par defaut
//!
[lhs,rhs]=argn(0)
if rhs==0,sig=10,ro=28,Beta=8/3;end;
fort('loset',sig,1,'r',ro,2,'r',Beta,3,'r','sort');
endfunction
