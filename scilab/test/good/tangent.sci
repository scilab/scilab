function [f,g,newm]=tangent(nl_sys,xe,ue)
//[f,g,newm]=tangent(ff,xe,[ue])
// lin\'earise autour du point d'\'equilibre (xe,ue)
// le champ du syst\`eme dynamique d\'efinit par xdot=ff(t,x,[u])
// (suppos\'e en fait autonome)
// Arguments :
//    ff : chaine de caract\`ere donnant le nom du syt\`eme a lin\'eriser
//    x0 : vecteur colonne
//    u0 : constante. \'eventuellement absente s'il n'y a pas de commande
//
// Valeurs de retour :
//   f, g : deux matrices qui caract\'erisent
//     le syst\`eme dynamique lin\'earis\'e -> dxdot=f.dx + g.du
//     s'il n'y a pas de commande g sera nulle
//   newm : une macro de type [y]=newm(t,x,u) qui d\'ecrit la dynamique
//     du syt\'eme lin\'eaire obtenu. (newm(t,xe,ue)=0)
//
//!
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if type(nl_sys)==10 then //the name of the function
    execstr('nl_sys='+nl_sys)
  end
  if rhs==3 then
    deff('[y,xdot]=fff(x,u)',['xdot=nl_sys(0,x,u),y=x']);
  else 
    ue=0;deff('[y,xdot]=fff(x,u)',['xdot=nl_sys(0,x),y=x']);
  end
  newm=0;
  [yy,xx]=fff(xe,ue);
  if norm(xx) >= 1.e-4,
    error(' xe is not an equilibrium point!');
    return
  end
  [f,g,h,void]=lin(fff,xe,ue);
  fstr=string(f);gstr=string(g);
  xestr=string(xe);uestr=string(ue);
  deff('[xdot]=newm(t,x,u)',['[lhs,rhs]=argn(0);if rhs <=2,u=0;end;',...
		    'xdot(1)='+fstr(1,1)+'*(x(1)-('+xestr(1)+'))+('+fstr(1,2)+...
		    ')*(x(2)-('+xestr(2)+'))+('+gstr(1)+')*(u-('+uestr+'))',...
		    'xdot(2)='+fstr(2,1)+'*(x(1)-('+xestr(1)+'))+('+fstr(2,2)+...
		    ')*(x(2)-('+xestr(2)+'))+('+gstr(2)+')*(u-('+uestr+'))']);
endfunction
