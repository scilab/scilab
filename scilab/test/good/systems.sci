function []=systems()

// Copyright INRIA

endfunction
function [xdot]=bioreact(t,x)
//[xdot]=bioreact(t,x)
// modele de bioreacteur
//   x(1): concentration de biomasse
//   x(2): ''            de sucre
//!
xdot(1)=mu_td(x(2))*x(1)- debit*x(1);
xdot(2)=-k*mu_td(x(2))*x(1)-debit*x(2)+debit*x2in;


endfunction
function [y]=mu_td(x)
//[y]=mu_td(x)
// mu : taux specifique de croissance
//!
y=x/(1+x);



endfunction
function [xdot]=compet(t,x,u)
//[xdot]=compet(t,x,[u]) u est optionnel
// Un modele de competition
// x(1),x(2) deux populations vivant sur une meme ressource
// 1/u est le niveau de la ressource (vaut 1 par defaut)
// ex : deux souches de levure
// ex : des crustaces sur une algue
//!
[lhs,rhs]=argn(0);
if rhs==2,u=1,end
xdot=0*ones(2,1);
xdot(1) = ppr*x(1)*(1-x(1)/ppk) - u*ppa*x(1)*x(2) ,
xdot(2) = pps*x(2)*(1-x(2)/ppl) - u*ppb*x(1)*x(2) ,


endfunction
function [f,g,h,linsy]=lincomp(ue)
//[f,g,h,linsy]=lincomp(ue)
// fournit les matrices f,g et h du modele compet avec sortie
// linearise  autour du point d'equilibre donne par equilpp(ue).
// la sortie y est la premiere composante x(1) du systeme.
// linsy : est une macro qui donne la dynamique du syt\`eme
// lin\'eaire tangent
//!
[f,g,linsy]=tangent('compet',equilcom(ue),ue);
h=[1,0];



endfunction
function [xe]=equilcom(ue)
//[xe]=equilcom([ue])
// calcule un point d'equilibre du systeme compet
// pour un niveau de ressource ue  (vaut 1 par defaut)
//!
[lhs,rhs]=argn(0);
if rhs==0,ue=1,end
mat=[  ppr/ppk , ue*ppa; ue*ppb , pps/ppl ]
cte=[ppr;pps ]
xe= inv(mat)*cte;


endfunction
function [xdot]=cycllim(t,x)
//[xdot]=cycllim(t,x)
//-----------------------------------
// xdot=a*x+qeps(1-||x||**2)x
//------------------------------------
//!
xdot= [ 0 ,-1 ; 1 , 0]*x+ qeps*(1-(x'*x))*x;



endfunction
function [xdot]=linear(t,x)
//[xdot]=linear(t,x)
// linear : a linear system 
// alin must be set as a global variable before calling ode.
//!
xdot=alin*x,



endfunction
function [xdot]=linper(t,x)
//[xdot]=linper(t,x)
//-----------------------------------
// systeme lineaire avec perturbation
// quadratique
//------------------------------------
//!
xdot= alin*x+(1/2)*qeps*[(x')*q1linper*x;(x')*q2linper*x]+rlinper



endfunction
function [xdot]=pop(t,x)
//[xdot]=pop(t,x)
// dynamique de la population de poissons
//!
xdot= 10*x*(1-x/K)- peche(t)*x



endfunction
function [xdot]=p_p(t,x,u)
//[xdot]=p_p(t,x,[u])
// Un modele proie-predateur avec insecticide
// x(1) : la proie (insecte nuisible)
// x(2) : le predateur (un gentil insecte)
// u    : taux de mortalite du a l'insecticide
//        qui agit a la fois sur la proie et le predateur
//        par defaut u=0 (pas d'insecticide)
//!
[lhs,rhs]=argn(0);
if rhs==2,u=0,end
xdot(1) = p_ppr*x(1)*(1-x(1)/p_ppk) - p_ppa*x(1)*x(2) - u*x(1);
xdot(2) = -p_ppm*x(2)             + p_ppb*x(1)*x(2) - u*x(2);


endfunction
function [xe]=equilpp(ue)
//[xe]=equilpp([ue])
// calcule un point d'equilibre xe du systeme p_p
// pour la commande constante ue a choisir lors de l'appel de la macro
//  ue=0 par defaut
//!
[lhs,rhs]=argn(0);if rhs==0,ue=0,end
xe(1) =  (p_ppm+ue)/p_ppb;
xe(2) =  (p_ppr*(1-xe(1)/p_ppk)-ue)/p_ppa;


endfunction
function [xdot]=lincom(t,x,k)
// systeme lineaire commande par
// feedback lineaire d'etat (u=-k*x)
//!
xdot= lic_a*x +lic_b*(-k*x);
endfunction
