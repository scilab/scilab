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
//author: F. D.  date: August 1988
// Copyright INRIA

cels=[];
for col=x,
      nf=[col(1:2);1];
      nd=[col(3:4);1];
      cels=[cels,syslin([],poly(nf,'z','c'),poly(nd,'z','c'))];
end,
 
endfunction
