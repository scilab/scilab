
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function conflicts=ge_check_names(g)

  names=g.node_name;
  re=find(names=='');ne=size(re,'*');
  [names,k1]=gsort(names,'g','i');
  
  conflicts=list(re);
  if size(names,'*') >1 then
    k3=find(names(2:$)==names(1:$-1));
    if k3<>[] then
      k3=unique([k3 k3+1])
      conflicts(2)=k1(k3)
    end
  end
endfunction
