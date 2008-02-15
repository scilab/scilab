// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent Couvert
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=%hm_k_hm(M1,M2)
hm1size = double(M1.dims);
hm2size = double(M2.dims);
hm1size = [hm1size ones(1,length(hm2size)-length(hm1size))];
hm2size = [hm2size ones(1,length(hm1size)-length(hm2size))];
subs=list()
for k=length(hm2size):-1:1
  ind = (1:hm2size(k))';
  subs(k) = ind(:,ones(1,hm1size(k)));
end
M = M2(subs(:));
endfunction
