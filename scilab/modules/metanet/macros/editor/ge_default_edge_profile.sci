
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xyp=ge_default_edge_profile()
  xp=[0 1 3 4]'/4;
  yp=[0 1 1 0]';
  xyp=[xp yp]
  use_splin=%t
  if use_splin then
    xn=(0:0.1:1)';
    yn = interp(xn, xyp(:,1), xyp(:,2), splin(xyp(:,1), xyp(:,2)) );
    xyp=[xn yn];
  end
endfunction
