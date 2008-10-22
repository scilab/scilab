function [xy]=scaling(xy,factor,orig)
//  xy=xy*factor+orig
//
// xy     : matrix with 2 rows
// factor : factor
// orig   : origin, [0;0] if omitted
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
select rhs
 case 2 then orig=[0;0];
 case 3 then orig=matrix(orig,2,1);
 else error(39)
end;
//
[m,n]=size(xy)
if m<>2 then  error('xy must be a vector with 2 rows [x;y]'),end
//
xy=xy-orig*ones(1,n)
xy=factor*xy+orig*ones(1,n)
endfunction
