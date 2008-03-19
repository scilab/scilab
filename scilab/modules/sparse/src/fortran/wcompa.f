c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1995-2008 - INRIA - Serger STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      logical function wcompa(ar,ai,br,bi,op)
c     compare two double precision complex numbers a and b
c! calling sequence
c     logical function dcompa(ar,ai,br,bi,op)
c     ar,ai : real and imaginary part of a
c     br,bi : real and imaginary part of b
c     op is an integer indicator of the comparison to perform
c     op=50 : a==b
c     op=119: a~=b
c
      double precision ar,ai,br,bi
      integer op
      integer less,great,equal
      logical l
      data less/59/,great/60/,equal/50/
      if(op.eq.equal) then
         l=ar.eq.br.and.ai.eq.bi
      elseif(op.eq.less+great) then
         l=ar.ne.br.or.ai.ne.bi
      endif
      wcompa=l
      end
