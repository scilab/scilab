function [A,B,D]=arma2p(ar) 
// Build three polynomial matrices 
// from an ar representation 
if ar(1)<>'ar' then write(%io(2)," This is not an ARMAX");return;end;
A=inv_coeff(ar(2));
if ar(3)<>[] then 
	[mb,nb]=size(ar(3));
	B=inv_coeff(ar(3),(nb/ar('nu'))-1);
else 
	B=[];
end
D=inv_coeff(ar(4));
endfunction
