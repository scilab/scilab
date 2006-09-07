function [J,H] = derivative(F, x, h, order, H_form, Q)
//
//  PURPOSE
//     First and second order numerical derivatives of a function  F: R^n --> R^m 
//     by finite differences.
//     J=J(x) is the m x n Jacobian (the gradient for m=1), and H=H(x) the Hessean 
//     of the m components of F at x. The default form of H is a mxn^2 matrix; 
//     in this form the Taylor series of F up to second order terms is given by:  
//    
//        F(x+dx) = F(x) + J(x)*dx + 1/2*H(x)*(dx.*.dx) +... 
//
//  NOTES
//     1/ See derivative.cat for details of the parameters 
//
//     2/ This function uses the 3 "internal" functions (following
//        this one in this file) :
//   
//          %DF_      => used to compute the Hessean by "differentiating
//                       the derivative"
//          %deriv1_  => contains the various finite difference formulae
//          %R_       => to deal with F as this arg may be a scilab
//                       function or a list embedding a function with
//                       its parameters
//  AUTHORS
//     Rainer von Seggern, Bruno Pincon
//
   [lhs,rhs]=argn();
   if rhs<2 | rhs>6 then, error('Wrong number of input arguments'), end
   if type(x) ~= 1 then, error('x has wrong type'), end
   [n,p] = size(x)
   if p ~= 1 then, error('x must be a column vector'), end
   
   if ~exists('order','local') then
      order = 2
   elseif (order ~= 1 & order ~= 2 & order ~= 4) then
      error('order must be 1, 2 or 4')
   end
    
   if ~exists('H_form','local'), H_form = 'default', end
   
   if ~exists('Q','local') then 
     Q = eye(n,n);
   else
     if norm(clean(Q*Q'-eye(n,n)))>0 then
     error('Q must be orthogonal');
     end
   end   
   if ~exists('h','local') then
      h_not_given = %t
      select order  // stepsizes for approximation of first derivatives
       case 1 , h = sqrt(%eps)
       case 2 , h = %eps^(1/3)
       case 4 , h = %eps^(1/4)
      end	 
   else
       h_not_given = %f	
   end
   
   J = %deriv1_(F, x, h, order, Q)
   m = size(J,1);
   
   if lhs == 1 then, return, end

   if h_not_given then
      select order  // stepsizes for approximation of second derivatives
       case 1 , h = %eps^(1/3)
       case 2 , h = %eps^(1/4)
       case 4 , h = %eps^(1/6)
      end	 
    end
   H = %deriv1_(%DF_, x, h, order, Q)        // H is a mxn^2 block matrix
   if     H_form == 'default' then
     H = matrix(H',n*n,m)'                   // H has the old scilab form
   end
   if H_form == 'hypermat' then
     if m>1, H=H'; H=hypermat([n n m],H(:)); // H is a hypermatrix if m>1
     end 
   end
   if (H_form ~= 'blockmat')&(H_form ~= 'default')&(H_form ~= 'hypermat') then 
     error('H_form must be ''default'',''blockmat'' or ''hypermat''')
   end
endfunction

function z=%DF_(x)
   z = %deriv1_(F, x, h, order, Q)';      // Transpose !
   z = z(:);
endfunction 

function g=%deriv1_(F_, x, h, order, Q)
   n=size(x,'*') 
   Dy=[];
   select order
     case 1
       D = h*Q;
       y=%R_(F_,x);
       for d=D, Dy=[Dy %R_(F_,x+d)-y], end             
       g=Dy*Q'/h                    
     case 2
       D = h*Q;
       for d=D, Dy=[Dy %R_(F_,x+d)-%R_(F_,x-d)], end       
       g=Dy*Q'/(2*h)
     case 4
       D = h*Q;
     for d=D
	dFh =  (%R_(F_,x+d)-%R_(F_,x-d))/(2*h)
	dF2h = (%R_(F_,x+2*d)-%R_(F_,x-2*d))/(4*h)
	Dy=[Dy (4*dFh - dF2h)/3]
     end
     g = Dy*Q'
    end
endfunction

function y=%R_(F_,x)  
if type(F_)==15 then  
    R=F_(1); y=R(x,F_(2:$)); // See extraction, list or tlist case: ...
                             // But if the extraction syntax is used within a function
			     // input calling sequence each returned list component is
			     // added to the function calling sequence.
elseif  type(F_)==13  then
  y=F_(x);
else
  error('The first input variable has wrong type.');
end
endfunction
