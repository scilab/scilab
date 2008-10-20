function [macr]=obscont1(sysn)
//[macr]=obscont1(sysn)
//
//cette macro renvoit le systeme observe-commande
//construit a partir du systeme sysn linearise autour de (xe,ue)
//
// sysn : chaine de caractere donnant le nom du systeme a commander
// gaincom,gainobs : vecteurs colonnes des gains demandes
//
// Retour : une nouvelle macro donnant le syst\`eme observe-commande
// [x1dot]=macr(t,x1,abruit,pas,n)
//    x1=[x;xchap],
// pour l'appel il faudra creer un vecteur de bruit de nom br
// d'autre part la macro cr\'ee va chercher dans l'environnement
// global les valeurs de (xe,ue) le point d'equilibre
//   f,g,h les matrices du systeme linearise
//   l et k  les deux matrices de gain
// exemple :
//    pas=10;n=200;
//    br=rand(1,n)
//    ode([x0;xchap0],0,pas*(0:n),list(macr,1.0,pas,n));
//!
// Copyright INRIA
deff('[zdot]=macr(t,z,abr,pas,n)',...
    ['u=ue-k(1)*(z(3)-xe(1))-k(2)*(z(4)-xe(2))';
     'ff_brui=abr*br(int(mini(t/pas+1,n)))';
     'y=h(1)*z(1)+h(2)*z(2)+ff_brui';
     'xdot='+sysn+'(t,z(1:2),u)';
     'xdot1=f*(z(3:4)-xe) +g*(u-ue)-l*(h*z(3:4)-y)';
     'zdot=[xdot;xdot1]';]);
endfunction
