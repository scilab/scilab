function [flag]=%choose(x)
// Utility function for use with schur
//     [U,dim]=schur(A,choose) returns orth. basis
//     for eigenspace associated to selected polynomials
//Needs two global variables : 
//     %sel = list of selected polynomials (user defined)
//     eps = threshold for polynomials selection (eps= 0.0001 as default value)
//            see below 
//     Example:
//     A=...
//     chis=poly(A,'s');  //Characteristic polynomial
//     w=factors(chis);    //Factors of chis in a list
//     %sel=list(w(2),w(3)); // two selected polynomials
//     eps=0.01;     //Threshold (see almosteq below)
//     [U,dim]=schur(A,%choose);     //Ordered Schur form
//     U1=U(:,1:dim);chi1=poly(U1'*A*U1,'s')  //Check
//     w1=factors(chi1)          // w1 = %sel ?
//
// Copyright INRIA
eps=0.0001;         //modify eps here !
// 
flag=0;s=poly(0,'s');
select x(1)
case 1 then
  // ASSUME x(3) NOT ZERO   (for gev pb. x(3)=0 => eval @ infty)
  vp=x(2)/x(3);pol=s-vp; //disp(pol);
  for p=%sel; if almosteq(pol,p,eps) then flag=1;end;end
case 2 then
  pol=s^2-x(2)*s+x(3);  //disp(pol);
  for p=%sel; if almosteq(pol,p,eps) then flag=1;end;end   
end

endfunction
function trfa=almosteq(pol,p,eps)
// returns %T if pol ~ p     %F if not
if degree(pol)<>degree(p) then trfa=%F;return;end
if norm((coeff(p)-coeff(pol)),1)<=eps then trfa=%T;return;end
trfa=%F;
endfunction
