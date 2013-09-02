c     ==================================================

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) -2011 - INRIA - Serge Steer
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

c     ==================================================
      subroutine intfind
      include 'stack.h'
c
      external gettype
      integer gettype,vt,top0
c
      top0=top
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(39)
         return
      endif

      if(rhs.eq.2) then
c     max number of index to find
         call getrmat('find', top, top, m2, n2, l2)
         if(err.gt.0.or.err1.gt.0)  return
         nmax=stk(l2)
         if(nmax.le.0.and.nmax.ne.-1) then
            err=2
            call error(116)
            return
         endif
         top=top-1
      else
         nmax=-1
      endif


      vt=gettype(top)
      if(vt.eq.1.or.vt.eq.4) then
         call intsfind(nmax)
      elseif(vt.eq.5.or.vt.eq.6) then
         call intspfind(nmax)
      else
c     .  overloaded find
         call putfunnam('find',top)
         top=top0
         fun=-1
         return
      endif
      return
      end
c     ==================================================
