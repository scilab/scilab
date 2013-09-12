c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$

      subroutine intzgebal(fname)

c     [V,D]=balanc(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character JOB
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )
c
      minrhs=1
      maxrhs=1
      minlhs=2
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.ne.N) then
         err=1
         call error(20)
         return
      endif
      if(N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lZ)) return
         lhsvar(1) = 1
         lhsvar(2) = 2
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif  

      if(.not.createvar(2,'d', N, N, lZ)) return
      if(.not.createvar(3,'d', 1, N, lSCALE)) return
      JOB='B'
      call ZGEBAL( JOB, N, zstk(lA), N, ILO, IHI,stk(lSCALE),INF0)


      call DLASET( 'F', N, N, ZERO, ONE, stk(lZ), N ) 
c     SUBROUTINE DGEBAK( JOB, SIDE, N, ILO, IHI, SCALE, M, V, LDV, INFO )
      call DGEBAK( JOB, 'R', N, ILO, IHI, stk(lSCALE), N,
     $     stk(lZ), N, INFO )

      lhsvar(1)=1
      lhsvar(2)=2
c     
      end


