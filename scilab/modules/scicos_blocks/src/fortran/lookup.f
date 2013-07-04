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

      subroutine lookup(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     rpar(1:n)  =  u coordinate discretization must be strictly increasing
c     rpar(n+1:2*n)  =  y coordinate discretization
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      double precision du,dout
      integer i
c
      n=nrpar/2
      if(n.gt.2) then
         do 10 i=2,n-1
            if (u(1).le.rpar(i)) goto 20
 10      continue
      else
         if(n.eq.1) then
            y(1)=rpar(2)
            return
         endif
         i=2
      endif
 20   continue
      du=rpar(i)-rpar(i-1)
      dout=rpar(n+i)-rpar(n+i-1)
      y(1)=rpar(n+i)-(rpar(i)-u(1))*dout/du
      end
