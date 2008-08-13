// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function st=statgain(sl)

  select typeof(sl)
  case 'rational' then
    dom=sl('dt')
    if dom=='c' then
      st=freq(sl('num'),sl('den'),0)
    else
      st=freq(sl('num'),sl('den'),1)
    end;
  case 'state-space' then
    dom=sl.dt;
    [m,p]=size(sl(2));
    if dom=='c' then
      if rank(sl(2)) <> m then 
	error(msprintf(gettext("%s: Wrong values for input argument #%d: State matrix is singular.\n"),"statgain",1)),
      end
      st=sl(5)-sl(4)*inv(sl(2))*sl(3);
    else
      if rank(eye(m,m)-sl(2))<>m then 
	error(msprintf(gettext("%s: Wrong values for input argument #%d: State matrix - eye is singular.\n"),"statgain",1))
      end
      st=sl(5)+sl(4)*inv(eye(m,m)-sl(2))*sl(3);
    end;
  else
    error(97,1)
  end
 
endfunction
