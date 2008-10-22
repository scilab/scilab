function [arc,resid]=armax1(r,s,q,y,u,b0f)
//[arc,resid>=armax1(r,s,q,y,u,[b0f])
//
// Calcule les coefficients d'un ARMAX monodimensionnel
//   A(z^-1)y= B(z^-1)u + D(z^-1)sig*e(t)
//           e(t) est un bruit blanc de variance 1
//           A(z)= 1+a1*z+...+a_r*z^r; ( r=0 => A(z)=1)
//           B(z)= b0+b1*z+...+b_s z^s ( s=-1 => B(z)=0)
//           D(z)= 1+d1*z+...+d_q*z^q  ( q=0 => D(z)=1)
// Entree
//     y : serie de sortie
//     u : serie d'entree
//     r,s et q : les ordres d'autoregression r >=0 et s >=-1 et
//       moyenne mobile q
//     b0f : est un parametre optionnel. Par defaut il vaut
//         0, et signifie qu'il faut identifier b0. Si on lui
//         donne la valeur 1, alors b0 est suppose valoir zero et
//         n'est pas identifie.
//	   
// Sortie
//     a est le vecteur <1,a1,...,a_r>
//     b est le vecteur <b0,......,b_s>
//     d est le vecteur <1,d1,....,d_q>
//     sig et resid=< sig*echap(1),....,>;
// Methode :
//     Cfre : Eykhoff (trends and progress in system identification) page 91
//     En introduisant
//        z(t)=[y(t-1),..,y(t-r),u(t),...,u(t-s),e(t-1),...,e(t-q)] et
//     coef= [-a1,..,-ar,b0,...,b_s,d1,...,d_q]'
//     y(t)= coef'* z(t) + sig*e(t).
//     on utilise alors la version sequentielle de l'estimation AR
//     ou l'on remplace e(t-i) par son estimee
//     Methode dite RLLS
//     si q=0, C'est une version sequentielle de l'algorithme de
//     moindre carre donne dans armax
// Exemple :
//
// Auteur: J-Ph. Chancelier ENPC Cergrene
//!
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<=5,b0f=0;end
  if s==-1,b0f=0;end // Seems not natural, but makes things work 
  u=matrix(u,1,-1);y=matrix(y,1,-1); //make u and y row vectors
  [n1,n2]=size(y)
  if size(y,'*')<>size(u,'*') then
    error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same numbers of elements expected.\n"),..
		   "armax1",4,5));
  end 
  //
  t0=maxi(maxi(r,s+1),1)+1;
  if r<>0;XTM1=y((t0-1):-1:(t0-r));else XTM1=[];end
  if s<>-1;UTM1=u(t0-b0f:-1:(t0-s));else UTM1=[];end
  if q<>0;ETM1=0*ones(1,q);else ETM1=[];end
  npar=r+s+1-b0f+q
  CTM1=0*ones(npar,1);
  ZTM1=[XTM1,UTM1,ETM1]';
  PTM1=10.0*eye(npar,npar);

  for t=t0+1:n2,
    if r<>0;XT=[ y(t-1), XTM1(1:(r-1))];else XT=[];end
    if s<>-1;UT=[ u(t-b0f), UTM1(1:(s-b0f))];else UT=[];end
    eeTM1=y(t-1)- CTM1'*ZTM1;
    if q<>0;ET=[ eeTM1, ETM1(1:(q-1))];else ET=[];end
    ZT=[XT,UT,ET]';
    //
    KT=PTM1*ZT*(1/(1+ ZT'*PTM1*ZT))
    CT=CTM1+KT*(y(t)-ZT'*CTM1)
    PT=PTM1-KT*ZT'*PTM1
    XTM1=XT;UTM1=UT;CTM1=CT;ETM1=ET;ZTM1=ZT;PTM1=PT;
  end
  // On extrait les coefficients a,b,d
  //
  if r<>0;a=[1;-CT(1:r)]';else a=1;end
  if s<>-1;
    if b0f==1,b=[0;CT(r+1:(r+s+1-b0f))]';else
      b=[CT(r+1:(r+s+1-b0f))]';end
      if q<>0;d=[1;CT(r+s+2-b0f:(r+s+q+1-b0f))]';else d=[1];end
  else
    b=0;
    if q<>0;d=[1;CT(r+s+1-b0f:(r+s+q-b0f))]';else d=[1];end
  end
  //On simule pour avoir l'erreur de prediction
  //
  [sig,resid]=epred(r,s,q,CTM1,y,u,b0f);
  arc=armac(a,b,d,1,1,sig);



endfunction

function [sig,resid]=epred(r,s,q,coef,y,u,b0f)
//=============================================
//[sig,resid]=epred(r,s,q,coef,y,u,b0f)
//  Utilisee par armax1 pour calculer l'erreur de prediction
//     coef= [-a1,..,-ar,b0,...,b_s,d1,...,d_q]'
// ou
//     coef= [-a1,..,-ar,b1,...,b_s,d1,...,d_q]' si b0f=1
//!
  [n1,n2]=size(y);
  t0=maxi(maxi(r,s+1),1)+1;
  if r<>0;XTM1=y((t0-1):-1:(t0-r));else XTM1=[];end
  if s<>-1;UTM1=u(t0-b0f:-1:(t0-s));else UTM1=[];end
  if q<>0;ETM1=0*ones(1,q);else ETM1=[];end
  npar=r+s+1-b0f+q
  ZTM1=[XTM1,UTM1,ETM1]';
  resid=0*ones(1,n2);
  for t=t0+1:n2,
    if r<>0;XT=[ y(t-1), XTM1(1:(r-1))];else XT=[];end
    if s<>-1;UT=[ u(t-b0f), UTM1(1:(s-b0f))];else UT=[];end
    resid(t)=y(t-1)- coef'*ZTM1;
    if q<>0;ET=[ resid(t), ETM1(1:(q-1))];else ET=[];end
    ZT=[XT,UT,ET]';
    XTM1=XT;UTM1=UT;ETM1=ET;ZTM1=ZT;
  end
  sig=sqrt(sum(resid.*resid)/size(resid,'*'))
endfunction
