function Gt=gtild(G,flag)
// input:
// G(s): a polynomial matrix or a rational matrix
// represented in state-space or in transfer form
//
// Gt=gtild(G) or  Gt=gtild(G,'c')
// returns Gt = G(-s)' (in transfer form or in state-space)
// for continuous time system G
//or 
// Gt=gtild(G) or  Gt=gtild(G,'d')
// returns Gt = z^n * G(1/z)' (n = maximum degree of G)
// for discrete-time matrix polynomials
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
  if typeof(G)=='rational'
    flag=G(4)
   elseif typeof(G)=='state-space'
    flag=G(7)
   elseif typeof(G)=='polynomial'
    flag=[]
  end
end

select typeof(G)
case 'polynomial'
if flag=='c'
  Gt=cp_tilde(G);return;
elseif flag=='d'  
  Gt=dp_tilde(G);
elseif flag==[]
  warning('gtild: flag not given! --> assumes c');
  Gt=cp_tilde(G);return;
end

case 'rational'
v=varn([G(2),G(3)]);s=poly(0,v);
if flag=='c'
    Gt=horner(G',-s);return;
elseif flag=='d' 
  Gt=horner(G',1/s);return;
elseif flag==[]
  warning('Flag not given : assumes c');
  Gt=cp_tilde(G);return;
end

case 'state-space'
if flag==[] then dom=G(7);else dom=flag;end
[A,B,C,D]=abcd(G);
if dom=='c' then
     if typeof(D)=='polynomial'
        Dp=horner(D,-poly(0,varn(D)));
        Gt=syslin(dom,-A',-C',B',Dp');return;
     elseif typeof(D)=='constant'
        Gt=syslin(dom,-A',-C',B',D');return
     end
elseif dom=='d'
     if typeof(G(5))=='polynomial'
        Dp=horner(D,1/poly(0,varn(D)));
	Dp=tf2ss(Dp);
	[A,B,C,D]=abcd(G');
	w=tlist(['des','A','B','C','D','E'],eye(A),B,C,0*C*B,A);
	z=poly(0,'z');zss=-tf2ss(z);zss(7)='d';
	Gt=zss*des2ss(w)+Dp';
     elseif typeof(G(5))=='constant'
        z=poly(0,'z');zss=-tf2ss(z);zss(7)='d';
        [A,B,C,D]=abcd(G');  //lazy way for transposition...
        w=tlist(['des','A','B','C','D','E'],eye(A),B,C,0*D,A);
        Gt=zss*des2ss(w)+D;   //-z*C*inv(I-z*A)*B + D
     end
end
//
case 'constant'
Gt=G';return;
end

endfunction
function mpt=dp_tilde(mp)
//mp(z) =  polynomial matrix 
// mpt(i,j)= z^n*conj(mp(j,i))(1/z)
[m,n]=size(mp),z=varn(mp)
//max degree
nmax=maxi(degree(mp));
for i=1:m
  for j=1:n
    mpt(j,i)=poly(coeff(conj(mp(i,j)),nmax:-1:0),z,'c')
  end
end

endfunction
function mpt=cp_tilde(mp)
//mp(s) =  polynomial matrix 
// mpt(i,j)= conj(mp(j,i))(s)
s=poly(0,varn(mp));
pr=real(mp);pi=imag(mp);
mpt=horner(pr',-s);
if pi==0*s then return;end
mpt=mpt+%i*horner(pi',-s);
endfunction
