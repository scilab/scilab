function [N,D]=lcmdiag(H,flag)
//returns N and diagonal D
//such that: 
//  flag='row' => H=D^(-1)*N  and D(k,k)=lcm of kth row of H('den')
//  flag='col' => H=N*D^(-1)  and D(k,k)=lcm of kth col of H('den')
// default flag = 'col'
// Copyright INRIA
Num=H('num');
Den=H('den');
[m,n]=size(H);
D=[];N=[];
[lhs,rhs]=argn(0);
if rhs==1 then flag='col';end
select flag
  case 'row'
for k=1:m
[pgcd,fact]=lcm(Den(k,:));  //H(k,:)=(Num(k,:).*fact)/pgcd
D=diag([diag(D);pgcd]);
N=[N;Num(k,:).*fact];
end
  case 'col'
for k=1:n
[pgcd,fact]=lcm(Den(:,k));  //
D=diag([diag(D);pgcd]);
N=[N,Num(:,k).*fact];  
end
end
endfunction
