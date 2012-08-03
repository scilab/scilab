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

      subroutine dsslti(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     discrete state space linear system simulator
c     rpar(1:nx*nx)=A
c     rpar(nx*nx+1:nx*nx+nx*nu)=B
c     rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny
      double precision w(100)
c
c
      la=1
      lb=nz*nz+la
      lc=lb+nz*nu
      if(flag.eq.4) then
         if(nz.gt.100) then
            flag=-1
            return
         endif
      elseif(flag.eq.2) then
c     x+=a*x+b*u
         call dcopy(nz,z,1,w,1)
         call dmmul(rpar(la),nz,w,nz,z,nz,nz,nz,1)
         call dmmul1(rpar(lb),nz,u,nu,z,nz,nz,nu,1)
      elseif(flag.eq.1.or.flag.eq.6) then
c     y=c*x+d*u
         ld=lc+nz*ny
         call dmmul(rpar(lc),ny,z,nz,y,ny,ny,nz,1)
         call dmmul1(rpar(ld),ny,u,nu,y,ny,ny,nu,1)
      endif
      end

