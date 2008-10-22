c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine contr(nstate,nin,a,b,icontr,tol)
c     computes the order of controllablility of a  state-space representation
      double precision a(nstate,*),b(nstate,*)
      double precision tol
      integer nstate,nin,icontr
c
      double precision w(200)
      integer iw(20)
c
      lz=1
      lwrka=lz+nstate*nstate
      lwrk1=lwrka+nstate*nin
      lwrk2=lwrk1+nin
      lw=lwrk2+nin
      if(lw.gt.200) then
         call erro('Too big system')
         return
      endif

      lnblk=1
      liwrk=lnblk+nstate
      lw=liwrk+nin
      if(lw.gt.20) then
         call erro('Too big system')
         return
      endif

      call ssxmc(nstate,nin,a,nstate,b,icontr,indcon,iw(lnblk),
     1     w(lz),w(lwrka), w(lwrk1), w(lwrk2),iw(liwrk),
     2     tol,0)
      return
      end
