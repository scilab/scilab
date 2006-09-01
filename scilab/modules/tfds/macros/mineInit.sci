function []=mineInit()
// Macro qui initialise les donnees du
// td3
//!
// Copyright INRIA
n1=30
n2=40;
te=n2/2;
xe=n1/2;
k0=1;
kc=0.01;
[n1,n2,te,xe,k0,kc]=resume(...
        n1,n2,te,xe,k0,kc);

endfunction
function [cout,feed]=mine(n1,n2,uvect)
//[cout,feed]=mine(n1,n2,uvect)
// extraction optimale d'un minerai d'une mine a ciel ouvert
// en avancant progressivement (k=1,2,...,n2) et en
// prelevant a l'abcisse k+1 la tranche de profondeur x(k+1)
// par une commande u a partir de la profondeur x(k)
// a l'abcisse k.
//
// la resolution du probleme se fait par programmation dynamique
// en calculant la commande optimale maximisant le critere :
//
//   -- n2-1
//   \
//   /        f(x(k),k) + V_F(x,n2)
//   -- k=1
//  avec : x(k+1)=x(k) + u
//  x(k) est la profondeur de la mine a l'abcisse k (x=1
//      est le niveau du sol)
// la fonction gain instantane f(i,k) represente le benefice
//      si on creuse la profondeur i a l'abcisse k
//  V_F(i,n2) est un cout final destine a forcer l'etat final
//       a valoir 1 (pour sortir de la mine ...)
//       V_F(i,n2) vaut -10000 en tout les points i\ne1 et 0
//       pour i=1
//
// le programme mine necessite de connaitre
//    n1    : l'etat est discretise en n1 points
//	      1 c'est l'air 
//	      2:n1+1 du sol au fond 
//	      n1+2 zone interdite
//    n2    : nombre d'etapes 
//    uvect : vecteur ligne des valeurs discretes que peut
//          prendre u (3 valeurs, on monte, on descend ou on avance)
// et le programme mine retourne alors deux matrices
//    cout(n1,n2) : valeurs de la fonction de Bellman
//    feed(n1,n2) : valeurs de la commande a appliquer
//          lorsque l'etat varie de 1 a n1 et
//          l'etape de 1 a n2.
//    n1    : l'etat est discretise en n1 points
//	      on rajoute dans le calcul deux couches fictive 
//	      en 1 l'air en n1+2 le fond de la mine 
//	      1 c'est l'air 
//	      2:n1+1 du sol au fond 
//	      n1+2 zone interdite
//!
usize=prod(size(uvect))
xgr=1:(n1+2)
// tableau ou l'on stocke la fonction de Bellman
cout=0*ones(n1+2,n2);
// tableau ou l'on stocke le feedback u(x,t)
feed=0*ones(n1,n2);
// calul de la fonction de Bellman au  temps final
penal=10000;
// Le cout final est le cout au point de sortie de la zone d'extraction 
// on veut evidement que ce point de sortie soit le sol 
// le cout est donc 0 au niveau du sol et -inf ailleurs 
cout(:,n2)=-penal*ones(n1+2,1);
cout(2,n2)=0;
// calcul retrograde de la fonction de Bellman et
// du controle optimal au temps temp par l'equation de Bellman
for temp=n2:-1:2,
  loc=list();
  for i=1:usize,
    newx=mini(maxi(xgr+uvect(i)*ones(xgr),1*ones(xgr)),(n1+2)*ones(xgr)),
    loc(i)=cout(newx,temp)+ff_o(xgr,temp-1),
  end;
  [mm,kk]=maxi(loc),
  cout(xgr,temp-1)=mm;
  cout(1,temp-1)=-penal;
  cout(n1+2,temp-1)=-penal;
  feed(xgr,temp-1)=uvect(kk)';
end

endfunction
function [y]=ff_o(x,t)
//[y]=ff_o(x,t)
// gain instantane apparaissant dans le critere du
// programme mine.
//
// pour des raisons techniques, l'argument x doit
// etre un vecteur colonne et donc egalement la sortie y.
// en sortie y=[ ff_0(x(1),t),...ff_o(x(n),t)];
//!
xxloc=ones(x);
y=k0*(1-(t-te)**2/(n2**2))*xxloc - (x-xe*xxloc)**2/(n1**2) -kc*(x-1*xxloc)
y=y';
y(1:2)=[0;0]

endfunction
function []=showcost(n1,n2)
//[]=showcost(n1,n2)
// Montre en 3d la fonction de gain instantanee (ff)
// x: profondeur (n1)
// y: abscisse  (n2)
// en z : valeur de ff_o(x,t)
//!
[lhs,rhs]=argn(0)
m=[];
for i=1:n2,m=[m,ff_o(1:n1,i)],end
contour(1:n2,1:n1,m',10,0,0,' ',[2,2,0])

endfunction
function []=trajopt(feed)
//[]=trajopt(feed)
// feed est la matrice de feedback calculee par mine
// trajopt calcule et dessine la trajectoire et le controle
// optimaux pour un point de depart (1,1)
//!
[n1,n2]=size(feed)
xopt=0*ones(1,n2)
uopt=0*ones(1,n2+1)
xopt(1)=2;
for i=2:(n2+1),xopt(i)=feed(xopt(i-1),i-1)+xopt(i-1),
             uopt(i-1)=feed(xopt(i-1),i-1),end
plot2d2("gnn",[1:(n2+1)]',[-xopt]',[2],"111",...
       "trajectoire optimale",...
       [1,-n1,n2+1,2]);
plot2d2("gnn",[1:(n2)]',uopt(1:n2)',[1,-4],"111",...
       "commande optimale",...
       [1,-n1,n2+1,2]);
endfunction
