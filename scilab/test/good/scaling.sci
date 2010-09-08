// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [xy]=scaling(xy,scalingFactor,orig)
//  xy=xy*scalingFactor+orig
//
// xy     : matrix with 2 rows
// scalingFactor : scaling factor
// orig   : origin, [0;0] if omitted
//!
[lhs,rhs]=argn(0)
select rhs
 case 2 then orig=[0;0];
 case 3 then orig=matrix(orig,2,1);
 else error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "scaling", 2, 3));
end;
//
[m,n]=size(xy)
if m<>2 then  error('xy must be a vector with 2 rows [x;y]'),end
//
xy=xy-orig*ones(1,n)
xy=scalingFactor*xy+orig*ones(1,n)
endfunction
