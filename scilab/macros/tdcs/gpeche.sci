function [xk,ukp1]=gpeche(uk,pasg)
// pour une loi de commande uk
// calcule la trajectoire associee xk
// imprime le valeur du cout
// calcule une nouvelle valeur de commande
//!
// Copyright INRIA
[xk,pk]=equad(uk);
tk=(1/(npts-1))*(0:(npts-1));
xset("window",1)
if xget("window")==0 , xinit('unix:0.0'),xset("window",1),end
plot2d(tk',uk',[1,-1],"121","commande");
x0=30;
gcout = sum( uk.*xk-c*uk);
ppenco= gcout-ppen*(xk(npts)-x0)**2;
write(%io(2),gcout,'('' gain '',f7.2)')
write(%io(2),ppenco,'('' gain-penalise '',f7.2)')
grad =   xk-c*ones(xk) - pk.*xk
//gradient projete su [0,umax]
umax=10;
ukp1=maxi(mini(uk- pasg*grad,umax*ones(1,npts)),0*ones(1,npts));

endfunction
function [ut]=peche(t)
//[ut]=peche(t)
// la loi de commande u(t) constante par morceaux
// construite sur la loi de comande discrete uk
//!
[n1,n2]=size(uk);
ut=uk(mini(maxi(ent(t*npts),1),n2));

endfunction
function [pdot]=pechep(t,p)
//[pdot]=pechep(t,p)
//equation adjointe
//!
pdot=-p*(10*( 1 -2*traj(t)/K) - peche(t)) - peche(t)
endfunction
