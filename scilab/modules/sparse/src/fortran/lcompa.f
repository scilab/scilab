c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1995-2008 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      logical function lcompa(a,b,op)
c     compare two boolean coded integer numbers a and b
c! calling sequence
c     logical function dcompa(a,b,op)
c     a,b  : boolean coded integer
c     op is an integer indicator of the comparison to perform
c     op=50 : a==b
c     op=119: a~=b
c
      integer a,b
      integer op
      integer less,great,equal
      logical l
      data less/59/,great/60/,equal/50/
      if(op.eq.equal) then
         l=a.eq.b
      elseif(op.eq.less+great) then
         l=a.ne.b
      endif
      lcompa=l
      end
