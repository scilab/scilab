function [f,d]=invr(h,flag)
//if h is a scalar, polynomial or rational fonction matrix, invr
//computes h^(-1).
//!

// Copyright INRIA
if type(h)==1 then f=inv(h);return;end
[lhs,rhs]=argn(0);
if rhs==1 then flag='C';end
if type(h) == 2 then
  //     POLYNOMIAL MATRIX               
  [m,n]=size(h);   
  if m<>n then error(20),end
  ndeg=maxi(degree(h));
  if ndeg==1 then
    //           MATRIX PENCIL
    E=coeff(h,1);A=-coeff(h,0);
    if norm(E-eye(E),1) < 100*%eps then
      // sI -A 
      [num,den]=coff(A,varn(h));f=num/den;
      return
    end
    [Bfs,Bis,chis]=glever(E,A,varn(h));
    f=Bfs/chis - Bis;
    if lhs==2 then
      d=lcm(f('den'));
      f=f*d;f=f('num');
    end
    return;
  end;
  //   GENERAL POLYNOMIAL MATRIX 
  select flag
  case 'L'
    f=eye(n,n);
    for k=1:n-1,
      b=h*f,
      d=-sum(diag(b))/k
      f=b+eye(n,n)*d,
    end;
    d=sum(diag(h*f))/n,
    if degree(d)==0 then d=coeff(d),end,
    if lhs==1 then f=f/d;end
    return;
  case 'C'
    [f,d]=coffg(h);
    if degree(d)==0 then d=coeff(d),end
    if lhs==1 then f=f/d;end
    return;
  end;
end

//-compat type(h)==15 retained for list/tlist compatibility
if type(h)==15|type(h)==16 then
  h1=h(1);
  if h1(1)<> 'r' then error(44),end
  [m,n]=size(h(2));
  if m<>n then error(20),end
  select flag
  case 'L'
    //    Leverrier 
    f=eye(n,n);
    for k=1:n-1,
      b=h*f,
      d=0;for l=1:n,d=d+b(l,l),end,d=-d/k;
      f=b+eye(n,n)*d,
    end;
    b=h*f;d=0;for l=1:n,d=d+b(l,l),end;d=d/n,
    if lhs==1 then f=f/d;end
    return;
  case 'A'
    //   lcm of all denominator entries
    denh=lcm(h('den'));
    Num=h*denh;Num=Num('num');
    [N,d]=coffg(Num);
    f=N*denh; 
    if lhs==1 then f=f/d;end
    return;
  case 'C'
    // default method by polynomial inverse
    [Nh,Dh]=lcmdiag(h); //h=Nh*inv(Dh); Dh diagonal;
    [N,d]=coffg(Nh);
    f=Dh*N;
    if lhs==1 then f=f/d;end
    return;
  case 'Cof'
    // cofactors method
    [f,d]=coffg(h);
    if lhs==1 then f=f/d;end
  end;
end;
error('invalid input to invr');
endfunction
