// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function c=curvature(data)
  d1=diff(data,1,1);
  d2=diff(data,2,1);
  if size(data,2)==3 then
    c=sqrt(...
        (d2(:,3).*d1(2:$,2)-d2(:,2).*d1(2:$,3)).^2 +...
        (d2(:,1).*d1(2:$,3)-d2(:,3).*d1(2:$,1)).^2  +...
        (d2(:,2).*d1(2:$,1)-d2(:,1).*d1(2:$,2)).^2)/...
      ((d1(2:$,1)^2+d1(2:$,2)^2+d1(2:$,3)^2)^1.5)
  else
    c=(d1(2:$,1).*d2(:,2)-d1(2:$,2).*d2(:,1))./((d1(2:$,1)^2+d1(2:$,2)^2)^1.5)
  end
endfunction

