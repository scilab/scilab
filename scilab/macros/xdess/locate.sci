function [x,but]=locate(n,flag)
//[x]=locate(n,flag)
//fonction permettant d'obtenir les coordonnees  d'un ou plusieurs
//points designes a l'aide de la souris sur un trace.
//Les resultats ne sont significatifs que sur les axes lineaires.
//x=locate(n) . Si n>0 retourne dans x(2,n) les coordonnees des  n
//             points designes.
//           . Si n<=0 retourne dans x les coordonnees des points
//             designes jusqu'a l'indication de fin de saisie, ce
//             dernier point n'etant pas retenu.
//x=locate()  . equivault au cas n <= 0
//si flag=1, on trace une croix sur chaque point saisi
//
//Sur les systemes ayant une souris a boutons, on designe un point
//en cliquant sur un des boutons, la fin de saisie pour  n<=0  est
//indiquee en cliquant sur le bouton de gauche.
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
but=[]
xselect();
if rhs<=1,flag=0;end
if rhs==0;n=-1;end

deff('[]=clearmode(flag)',['modek=xget(''alufunction'')';
    'xset(''alufunction'',6)';
    'if flag==1,xpoly(x(1,:),x(2,:),''marks'',0);end';
    'xset(''alufunction'',modek);';
    'xset(''mark'',xxx(1),xxx(2))']);

x=[];
xxx=xget('mark');
xset('mark',2,xxx(2));
wc=xget('window')
if n >= 0 then 
  for i=1:n,
    while %t
      [i,x1,y1,w,m]=xclick();
      if w==wc then break,end
    end
    if flag==1,xpoly(x1,y1,'marks',0);end
    x=[x,[x1;y1]];
    but=[but,i]
  end
else while %t, 
    while %t
      [i,x1,y1,w,m]=xclick(), 
      if w==wc then break,end
    end
    if i==0 then
      clearmode(flag),return
    elseif flag==1 then
      xpoly(x1,y1,'marks',0)
    end
    x=[x,[x1;y1]];
    but=[but,i]
  end
end
clearmode(flag);
endfunction
