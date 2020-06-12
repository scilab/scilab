//This file is part of the Cardiovascular Wawes Analysis toolbox
//Copyright (C) 2014 - INRIA - Serge Steer
//This file must be used under the terms of the CeCILL.
//This source file is licensed as described in the file COPYING, which
//you should have received as part of this distribution.  The terms
//are also available at
//http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [B,C] = sgolay(k,nf,w)
// Savitzky-Golay Filter Design.
//   k : a positive scalar with integer value: the polynomial order
//   nf : a positive scalar with integer value: the FIR filter length,
//        must be odd and greater the k+1 
//   w : a real vector of length nf with positive entries: the
//       weights. If omitted no weights are applied.

//   B : a real n by n array: the set of filter coefficients: the early rows
//       of B smooth based on future values and later rows smooth based on
//       past values, with the middle row using half future and half past.
//       In particular, you can use row i to estimate x(k) based on the i-1
//       preceding values and the n-i following values of x values as 
//       y(k) = B(i,:) * x(k-i+1:k+n-i).

//   C : a real n by k+1 array: the matrix of differentiation filters.  Each
//       column of C is a differentiation filter for derivatives of order
//       P-1 where P is the column index.  Given a signal X with length nf,
//       an estimate of the P-th order derivative of its middle value can be
//       found from:
//                      (P)
//                     X   ((nf+1)/2) = P!*C(:,P+1)'*X
//   References:
  
//   - Abraham Savitzky et Marcel J. E. Golay, « Smoothing and
//     Differentiation of Data by Simplified Least Squares Procedures »,
//     Analytical Chemistry, vol. 8, no 36,‎ 1964, p. 1627–1639 (DOI 10.1021/ac60214a047)

//   - http://en.wikipedia.org/wiki/Savitzky%E2%80%93Golay_filter

  if type(k)<>1|int(k)<>k|size(k,'*')<>1 then
    error(msprintf(_("%s: Wrong value for input argument #%d: An integer value expected.\n"),"sgolay",2))
  end
  
  if k > nf-1 then
    error('The degree must be less than the frame length.'),
  end
  if type(k)<>1|int(k)<>k|size(k,'*')<>1 then
    error(msprintf(_("%s: Wrong type for input argument #%d: a scalar with integer value expected.\n"),...
                   "sgolay",1))
  end
 
  if type(nf)<>1|int(nf)<>nf|size(nf,'*')<>1 then
    error(msprintf(_("%s: Wrong type for input argument #%d: a scalar with integer value expected.\n"),...
                   "sgolay",2))
  end
  
  if modulo(nf,2)<>1 then
    error(msprintf(_("%s: Wrong value for input argument #%d: Must be odd.\n"),"sgolay",2))
  end
  if nf<=k then
    error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Argument #%d expected to be less than argument #%d.\n"),"sgolay",1,2,2,1))
  end
  
  //compute the Vandermonde matrix
  J=(ones(k+1,1)*(-(nf-1)./2:(nf-1)./2)).^((0:k)'*ones(1,nf));
  if argn(2)==3 then
    if type(w)<>1|~isreal(w) then
      error(msprintf(_("%s: Wrong type for argument %d: Real vector expected.\n"),...
                     "sgolay",3))
    end
    if size(w,"*")<>nf then
      error(msprintf(_("%s: Incompatible input arguments #%d and #%d\n"),"sgolay",2,3))
    end
    // Check to see if all elements are positive
    if min(w) <= 0 then
      error(msprintf(_("%s: Wrong values for input argument #%d: Elements must be positive.\n"),...
                     "sgolay",3))

    end
    // Diagonalize the vector to form the weighting matrix
    J = J*diag(sqrt(w))
  end
  //Compute the matrix of differentiators C=J'/(J*W*J')
  C = pinv(J); 
  // Compute the projection matrix B
  B = C*J;

endfunction
