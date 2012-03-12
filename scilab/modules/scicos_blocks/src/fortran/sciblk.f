c     Scicos
c     
c     Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
c     
c     This program is free software; you can redistribute it and/or modify
c     it under the terms of the GNU General Public License as published by
c     the Free Software Foundation; either version 2 of the License, or
c     (at your option) any later version.
c     
c     This program is distributed in the hope that it will be useful,
c     but WITHOUT ANY WARRANTY; without even the implied warranty of
c     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
c     GNU General Public License for more details.
c     
c     You should have received a copy of the GNU General Public License
c     along with this program; if not, write to the Free Software
c     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
c     
c     See the file ./license.txt
c     

      subroutine sciblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,
     &     ntvec,rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

C     routine used to evaluate a block defined by a scilab function
C     scilab function syntax must be
C     [y,x,z,tvec,xd]=func(flag,nevprt,t,x,z,rpar,ipar,u)
C     with 
C        t      scalar current time
C        x      column vector continuous state
C        z      column vector discrete state
C        u      column vector block input
C        nevprt integer
C        flag   integer
C        y      column vector block output
C        xd     column vector block state derivative
C!
      include "stack.h"
C

      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

      integer ptr
      integer mlhs,mrhs
      logical allowptr
C
      common /ierode/ iero
      common /scsptr/ ptr
C
      if (ddt .eq. 4) then
c     to build with f2c      
c       write (buf(1:12),"(3i4)") top, ptr
c       call basout(io,wte," sciblk  top:"//buf(1:4)//" ptr :"//
c    &       buf(5:8)      )
      endif
C
      mlhs = 5
      mrhs = 8
      iero = 0
      call itosci(flag,1,1)
      if (err .gt. 0) goto 9999
      call itosci(nevprt,1,1)
      if (err .gt. 0) goto 9999
      call dtosci(t,1,1)
      if (err .gt. 0) goto 9999
      call dtosci(x,nx,1)
      if (err .gt. 0) goto 9999
      call dtosci(z,nz,1)
      if (err .gt. 0) goto 9999
      call dtosci(rpar,nrpar,1)
      if (err .gt. 0) goto 9999
      call itosci(ipar,nipar,1)
      if (err .gt. 0) goto 9999
      call dtosci(u,nu,1)
      if (err .gt. 0) goto 9999
C     
C     macro execution 
C     
      pt = pt + 1
      if (pt .gt. psiz) then
        call error(26)
        goto 9999
      endif
      ids(1,pt) = lhs
      ids(2,pt) = rhs
      ids(3,pt) = flag
      rstk(pt) = 1001
      lhs = mlhs
      rhs = mrhs
      niv = niv + 1
      fun = 0
      fin = ptr
C     
      icall = 5
      krec = -1
      include "callinter.h"
 200  lhs = ids(1,pt)
      rhs = ids(2,pt)
      flag= ids(3,pt)
      pt = pt - 1
C+    
C     transfer output variables to fortran

      if(flag.eq.1) then
c y or z computation
         top=top-2
         call scitod(z,nz,1)
         call scitod(x,nx,1)
         call scitod(y,ny,1)
      elseif(flag.eq.0) then
         call scitod(xd,nx,1)
         top=top-4
      elseif(flag.eq.2) then
c  x'  computation
         top=top-2
         call scitod(z,nz,1)
         call scitod(x,nx,1)
         top=top-1
      elseif(flag.eq.3) then
         top=top-1
         call scitod(tvec,ntvec,1)
         top=top-3
      elseif(flag.eq.4.or.flag.eq.5) then
         top=top-2
         call scitod(z,nz,1)
         call scitod(x,nx,1)
         top=top-1
      elseif(flag.eq.6) then
         top=top-2
         call scitod(z,nz,1)
         call scitod(x,nx,1)
         call scitod(y,ny,1)
      endif
      if (err .gt. 0) goto 9999
C+    
      niv = niv - 1
      return
C     
 9999 continue
      iero = -1
      flag=iero
      niv = niv - 1
      return
      end

