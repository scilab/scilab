c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1995-2007 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      logical function dcompa(a,b,op)
c     compare two double precision numbers a and b
c! calling sequence
c     logical function dcompa(a,b,op)
c     a,b : double precision numbers
c     op is an integer indicator of the comparison to perform
c     op=50 : a==b
c     op=59 : a<b
c     op=60 : a>b
c     op=119: a~=b
c     op=109: a<=b
c     op=110: a>=b
c
      double precision a,b
      integer op
      integer less,great,equal
      logical l
      data less/59/,great/60/,equal/50/
      if(op.eq.less) then
         l=a.lt.b
      elseif(op.eq.great) then
         l=a.gt.b
      elseif(op.eq.equal) then
         l=a.eq.b
      elseif(op.eq.less+great) then
         l=a.ne.b
      elseif(op.eq.less+equal) then
         l=a.le.b
      elseif(op.eq.great+equal) then
         l=a.ge.b
      endif
      dcompa=l
      end
