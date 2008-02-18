// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [xy]=rotate(xy,teta,orig)
// effectue une rotation
//
// xy   : matrice a deux lignes
// teta : angle en radian , 0 si omis
// orig : centre de la rotation, <0;0> si omis
//!
[lhs,rhs]=argn(0)
select rhs
 case 2 then orig=[0;0];
 case 3 then orig=matrix(orig,2,1);
 else error(39)
end;
//
[m,n]=size(xy)
if m<>2 then  error('xy doit etre un vecteur a 2 lignes [x;y]'),end
//
xy=xy-orig*ones(1,n)
c=cos(teta),s=sin(teta)
xy=[c,-s ; s,c]*xy+orig*ones(1,n)
endfunction
