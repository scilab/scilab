
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [gm]=fspecg(g)
// gm=fspecg(g) : stable factorization.
// g and gm are continuous-time linear systems in state-space form.
//       gtild(g)*g = gtild(gm)*gm
//with gm stable.
// Imaginary-axis poles are forbidden.
//
//!
  if typeof(g)<>'state-space' then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear system expected.\n"),"fspecg",1))
  end
  if g.dt<>"c" then
    error(msprintf(gettext("%s: Wrong type for argument %d: In continuous time expected.\n"),"fspecg",1))
  end

[a,b,c,d]=abcd(g),
g=[];
a=-a;
b=-b;
h=[-a',c'*c;
   0*eye(a),a];
x=ric_desc(h);h=[]
gm=syslin('c',-a'+c'*c*x,-c',b'-d'*c*x,d')';
endfunction
