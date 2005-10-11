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
  st=get('figure_style')=='new'
  if rhs<=1,flag=0;end
  if rhs==0;n=-1;end

  if ~st then //old graphic mode
    xxx=xget('mark');
    xset('mark',2,xxx(2));

    deff('[]=clearmode(flag)',[
	'modek=xget(''alufunction'')';
	'xset(''alufunction'',6)';
	'if flag==1,xpoly(x(1,:),x(2,:),''marks'',0);end';
	'xset(''alufunction'',modek);';
	'xset(''mark'',xxx(1),xxx(2))']);
  else
    ax=gca()
    mark_style=ax.mark_style;mark_size=ax.mark_size;mark_size_unit=ax.mark_size_unit;
    ax.mark_style=2;ax.mark_size=0;ax.mark_size_unit = "tabulated"
    deff('[]=clearmode(flag)',[
	'npt=size(x,2);'
	'if npt>0&flag==1 then'
	'  delete(ax.children(1:npt))'
	'end'
	'ax.mark_size_unit=mark_size_unit;'
	'ax.mark_style=mark_style;'
	'ax.mark_size=mark_size;']);
  end
  x=[];
  if n >= 0 then 
    for i=1:n,
      while %t
	[ib,x1,y1]=xclick();
	if or(ib==[0:5 10 11 12]) then break,end
	if ib==-100 then return,end //the window has been closed
      end
      if flag==1,xpoly(x1,y1,'marks');end
      x=[x,[x1;y1]];
      but=[but,ib]
    end
  else 
    while %t, 
      while %t
	[ib,x1,y1]=xclick(), 
	if or(ib==[0:5 10 11 12]) then break,end
	if ib==-100 then return,end //the window has been closed
      end
      if or(ib==[0 3 10]) then 
	break //terminate the acquisition loop
      elseif flag==1 then xpoly(x1,y1,'marks'),end
	x=[x,[x1;y1]];
	but=[but,ib]
    end
  end
  clearmode(flag);
endfunction
