function t__=sci2map(a,nom)
//converts a scilab object to a maple instruction
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error('sci2map: 2 input parameters!');end;

t__=[],
[ft]=format();format(20) // on fixe un format etendu
  if type(a) <= 10 then
    [ma,na]=size(a),
    if ma*na  > 1 then
      t__(1)=nom+' := array(1.'+'.'+string(ma)+',1.'+'.'+string(na)+');',
    end,
  else
  if type(a)==15|type(a)==16 then ma=size(a), end,
end,
//
//type
//
select type(a)
case 1 then //matrice de scalaires
  t__=[t__ ; str2map(nom,a,ma,na)]
  //
case 2  then //matrice a coefficients polynomiaux
  t__=[t__ ; poly2map(nom,a,ma,na)]
  //
case 10 then // matrice de chaines de caracteres
  for i=1:ma ; 
    for j=1:na ;
      a(i,j)= '`' + a(i,j) + '`'
    end
  end
  t__=[t__ ; str2map(nom,a,ma,na)]
  //
case 11 then // fct non compilee
  error('Not yet implemented')
  //comp(a)
  //tramac(nom,a,b_2_m)
  //
case 13 then // fct compilee
  //tramac(nom,a,b_2_m)
  
  error('Not yet implemented')
case 15 then //listes
  a1=a(1);
  t__=list2map(nom,a,[1:size(a)]),
case 16 then //listes
  a1=a(1)
  select a1(1)
  case 'r' then //matrice de fractions rationnelles
    num_=a('num'),den_=a('den'),
    t__=[t__ ; frac2map(nom,num_,den_)],
  case 'lss' then // systeme d'etat : passage des matrices
    t_ind__=[2:size(a)-1],
    t__=list2map(nom,a,t_ind__),
  else //autre cas
    t__=list2map(nom,a,[1:size(a)]),
  end
  //
end //select
format(ft(2),ft(1)),


endfunction
function [t__]=str2map(nom,a,ma,na)
//string to maple
//!
t__=[]
if ma*na==1 then
  t__= nom+' := '+new_str(a)+ ';',
else
  nomi= nom+'[';
      for i=1:ma,
	si= nomi+string(i)+',',
	for j=1:na,
	  terme =  si+string(j)+'] := '+new_str(a(i,j)) + ';' ,
      t__= [t__ ;terme]
    end,
  end,
end


endfunction
function [n_str]=new_str(expr)
//utility
//
select type(expr)
case 10 then
  // matrix of strings
  n_str=string(expr)
  //
case 1 then// matrix R or C
  st_real=string(real(expr)), st_imag=string(imag(expr)),
  lt_real=length(st_real), lt_imag=length(st_imag),
  n_str=st_real
  if lt_real>4 then
    if part(st_real,lt_real-3:lt_real-3) =='D' then
      n_str=part(st_real,1:lt_real-4)+'*10^(',
      n_str=n_str+part(st_real,lt_real-2:lt_real)+')',
    end
  end
  if st_imag <> '0' then
    c_str=' + I*(',x_str=st_imag,
    if lt_imag>4 then
      if part(st_imag,lt_imag-3:lt_imag-3) =='D' then
	x_str=part(st_imag,1:lt_imag-4)+'*10^(',
	x_str=x_str+part(st_imag,lt_imag-2:lt_imag)+')',
      end,
    end,
    c_str=c_str+x_str+')',n_str=n_str+c_str,
  end, //ifffffff
else
  error('numeric expression  or character strings'),
end, //sel

endfunction
function [t__,k]=poly_fm(nom,poly_,f_par,f_fin,l_deb)
//utility
//!
bl='                                                ',
l_nom=length(nom),bl_nom=part(bl,1:l_nom),
//
t__=[],k=1,
//
deg_=degree(poly_),coef_=coeff(poly_),
[lhs,rhs]=argn(0),
if rhs==2 then f_par=0 ,f_fin=1, l_deb=0, end,
//
if nom <> bl_nom then
  nom=nom+ ' := ' ,
  l_nom=l_nom+4
  p = nom
else
  p=part(bl,1:l_deb),
end
if f_par == 1 then p=p+'(' ,end,

if coef_(1) <>0 then
  p=p+new_str(coef_(1))
  if deg_ <>0 then p=p+'+',end
end
//l0=length(p)
rp=' '
for l=2:deg_
  c1=coef_(l)
  if c1==0 then
    p=p+z+'*(',
  else
    p=p+z+'*('+new_str(c1)+'+'
  end
  rp=')'+rp
  //   ln=length(p)
  //   if ln >70 then
  //      t__(k)=part(p,1:l0)+'\'
  //      k=k+1
  //      p=part(bl,1:l_nom)+part(p,l0+1:ln)
  //      l0=length(p)
  //     else
  //      l0=ln
  //   end
end
if deg_ <> 0 then
  c1=coef_(deg_+1)
  if c1<>1 then
    p=p+z+'*('+new_str(coef_(deg_+1))+')'+rp
  else
    p=p+z+rp
  end
end
//ln=length(p)
//  if ln >70 then
//   t__(k)=part(p,1:l0)+'\'
//   k=k+1
//   p=part(bl,1:l_nom)+part(p,l0+1:ln)
//   l0=length(p)
//  end
if f_par == 1 then p=p+')' ,end,
if f_fin == 1 then p=p+';' ,end,
 
t__(k)=p

endfunction
function [t__]=list2map(nom,a,t_ind__)
//[t__]=list2map(nom,a,t_ind__) : list to maple
// nom : name
// a   :  Scilab list
// t_ind__ : index of list
//!
t__=nom+' := [];'
ind__=1,
for i=t_ind__,
  list__ = nom + ' := [ ' + 'op('+nom+'),'
      n_elt_li = nom+'['+string(ind__)+']',
      l_elt_li=length(n_elt_li)+ 4,
      t__resu =  gen_map(n_elt_li,a(i)),
      t__resu(1)=list__+part(t__resu(1),l_elt_li+1: ..
	  (length(t__resu(1)) -1)) + '];'
  t__=[t__ ; t__resu],
  ind__=ind__+1,
end //forrr


endfunction
function [t__]=poly2map(nom,a,ma,na)
// polynomial to maple
//!
z=varn(a);nz=length(z)
while part(z,nz)==' ' then nz=nz-1,end
z=part(z,1:nz);
t__=[]
if na*ma ==1 then
   t__=poly_fm(nom,a),
 else
   nomd=nom+'[',
   for i=1:ma,
       nomi=nomd+string(i)+',',
       for j=1:na,
           nomij=nomi+string(j)+']',
           t__=[t__ ; poly_fm(nomij,a(i,j))]
       end,
   end,
end,
endfunction
function [t__,k]=fra2map(nomi,num_,den_)
//utility
//!
t__ = []
bl = '                                                ',
l_nom = length(nomi) , bl_nom = part(bl,1:l_nom+4),
t_num_ = poly_fm(nomi,num_,1,0,0)
l_t_num_ = prod(size(t_num_)) ,t_num_(l_t_num_)=t_num_(l_t_num_) +' / '
t_den_ = poly_fm(bl_nom,den_,1,1,length(t_num_(l_t_num_)) )
// concatenation numerateur denominateur
tden_=part(t_den_(1),length(t_num_(l_t_num_)):length(t_den_(1))),
t_num_(l_t_num_) = t_num_(l_t_num_) + tden_
t__ = [ t_num_ ; t_den_(2:prod(size(t_den_))) ],

endfunction
function [t__]=frac2map(nom,num_,den_)
//utility
//!
[ma,na]=size(num_),
z=varn(num_);nz=length(z)
while part(z,nz)==' ' then nz=nz-1,end
z=part(z,1:nz),
t__=[],
if ma*na ==1 then
   t__=fra2map(nom,num_,den_)
  else
   t__(1)=nom+' := array(1.'+'.'+string(ma)+',1.'+'.' ...
                  +string(na)+');',
   nomd=nom+'[',
   for i=1:ma,
       nomi=nomd+string(i)+',',
       for j=1:na,
           nomij=nomi+string(j)+']',
           t__=[t__ ; fra2map(nomij,num_(i,j),den_(i,j)) ]
       end,
   end,
end,
endfunction
