function cels=casc(x,z)
//cels=casc(x,z)
//Creates cascade realization of filter
//from a matrix of coefficients
//  x    :(4xN)-Matrix where each column is a cascade
//       :element, the first two column entries being
//       :the numerator coefficients and the second two
//       :column entries being the denominator coefficients
//  z    :Character string representing the cascade variable
//  cels :Resulting cascade representation
//
//EXAMPLE:
//  x=[ 1.     2.     3.  ;
//      4.     5.     6.  ;
//      7.     8.     9.  ;
//      10.    11.    12. ]
//
//  cels=casc(x,'z')
//  cels      =
//
//  !             2               2               2  !
//  !   1 + 4z + z      2 + 5z + z      3 + 6z + z   !
//  !  ------------    ------------    ------------  !
//  !              2               2               2 !
//  !   7 + 10z + z     8 + 11z + z     9 + 12z + z  !
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - F.D
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

cels=[];
for col=x,
      nf=[col(1:2);1];
      nd=[col(3:4);1];
      cels=[cels,syslin([],poly(nf,'z','c'),poly(nd,'z','c'))];
end,
 
endfunction
