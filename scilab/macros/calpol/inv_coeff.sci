function [P]=inv_coeff(c,d,name)
// inverse function of coeff 
[lhs,rhs]=argn(0);
[n,m]=size(c);
if rhs <= 2 ; name = 'x';end 
if rhs <= 1 ; d = (m/n-1) ; end 
if modulo(m,d+1) <> 0 then 
	write(%io(2),'wrong second arguments according to fisrt argument size')
	return;
end
p=poly(0,name);
P=p.^[0:d];
P=P.*.eye(m/(d+1),m/(d+1));
P= c*P'
endfunction
