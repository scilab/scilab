//This file is part of the Cardiovascular Wawes Analysis toolbox
//Copyright (C) 2014 - INRIA - Serge Steer
//This file must be used under the terms of the CeCILL.
//This source file is licensed as described in the file COPYING, which
//you should have received as part of this distribution.  The terms
//are also available at
//http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function y=sgolayfilt(X,varargin)
//filter signal using  Savitzky-Golay Filter.
// y=sgolayfilt(X,B)
// y=sgolayfilt(X,k,nf [,w]) 
//
//   X :
//   B : a real n by n array: the set of filter coefficients: the early rows
//       of B smooth based on future values and later rows smooth based on
//       past values, with the middle row using half future and half past.
//       In particular, you can use row i to estimate x(k) based on the i-1
//       preceding values and the n-i following values of x values as 
//       y(k) = B(i,:) * x(k-i+1:k+n-i).

//   k : a positive scalar with integer value: the polynomial order, must
//        be odd
//   nf : a positive scalar with integer value: the FIR filter length,
//        must be odd and greater the k+1 
//   w : a real vector of length nf with positive entries: the weights.
//         If omitted no weights are applied.   
//   
//   References:
  
//   - Abraham Savitzky et Marcel J. E. Golay, « Smoothing and
//     Differentiation of Data by Simplified Least Squares Procedures »,
//     Analytical Chemistry, vol. 8, no 36,‎ 1964, p. 1627–1639 (DOI 10.1021/ac60214a047)

//   - http://en.wikipedia.org/wiki/Savitzky%E2%80%93Golay_filter
  if type(X)<>1|~isreal(X) then
    error(msprintf(_("%s: Wrong type for argument %d: Real matrix expected.\n"),...
                                      "sgolayfilt",1))
  end
  if size(X,1) == 1 then X = X(:);end

  if or(size(varargin)==[2 3]) then
    B = sgolay(varargin(:))
  else
    B=varargin(1)
    if type(B)<>1|~isreal(B) then
      error(msprintf(_("%s: Wrong type for argument %d: Real matrix expected.\n"),...
                                      "sgolayfilt",2))
    end
  end
  n=size(B,1);
  if size(X,1) < n, 
    error(msprintf(_("%s: Wrong size for argument %d: At least %d expected.\n"),"sgolayfilt",1,size(B,1)))
  end

  // The first k rows of B are used to filter the first k points
  // of the data set based on the first n points of the data set.
  // The last k rows of B are used to filter the last k points
  // of the data set based on the last n points of the dataset.
  // The remaining data is filtered using the central row of B.
  k = floor(n/2);
  z=[]
  for kc=1:size(X,2)
    z(:,kc) = filter(B(k+1,:), 1, X(:,kc));
  end
  y = [B(1:k,:)*X(1:n,:) ; z(n:$,:) ; B(k+2:n,:)*X($-n+1:$,:) ];
endfunction
