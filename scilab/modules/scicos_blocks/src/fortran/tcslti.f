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

      subroutine tcslti(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u1,nu1,u2,nu2,y,ny)
c     Scicos block simulator
c     continuous state space linear system simulator
c     rpar(1:nx*nx)=A
c     rpar(nx*nx+1:nx*nx+nx*nu)=B
c     rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C
c     rpar(nx*nx+nx*nu+nx*ny+1:nx*nx+nx*nu+nx*ny+ny*nu)=D
c!
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*)
      double precision u1(*),u2(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu1,nu2,ny

c
      la=1
      lb=nx*nx+la
      lc=lb+nx*nu1

      if(flag.eq.1.or.flag.eq.6) then
c     y=c*x+d*u1     
         ld=lc+nx*ny
         call dmmul(rpar(lc),ny,x,nx,y,ny,ny,nx,1)
         call dmmul1(rpar(ld),ny,u1,nu1,y,ny,ny,nu1,1)
      elseif(flag.eq.2.and.nevprt.eq.1) then
c     x+=u2
         call dcopy(nx,u2,1,x,1)
      elseif(flag.eq.0.and.nevprt.eq.0) then
c     xd=a*x+b*u1
         call dmmul(rpar(la),nx,x,nx,xd,nx,nx,nx,1)
         call dmmul1(rpar(lb),nx,u1,nu1,xd,nx,nx,nu1,1)
      endif
      return
      end
