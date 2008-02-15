c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

       subroutine mklist(n)
c     form the list built with last n variables stored in top-n+1:top
c      positions of the stack
c!
      call mklistt(n,15)
      return
      end
      subroutine mktlist(n)
c     form the list built with last n variables stored in top-n+1:top
c      positions of the stack
c!
      call mklistt(n,16)
      return
      end
      subroutine mkmlist(n)
c     form the list built with last n variables stored in top-n+1:top
c      positions of the stack
c!
      call mklistt(n,17)
      return
      end

      subroutine mklistt(n,typ)
c     form the list built with last n variables stored in top-n+1:top
c      positions of the stack
c!
      include 'stack.h'
      integer n,typ,srhs,slhs,sfin
c
      srhs=rhs
      slhs=lhs
      sfin=fin
c
      rhs=n
      if(typ.eq.15) then
         fin=1
      elseif(typ.eq.16) then
         fin=2
      elseif(typ.eq.17) then
         fin=7
      endif

      lhs=1
      call ref2val
      call gwdatastructures1
      rhs=srhs
      lhs=slhs
      fin=sfin
      return
      end
