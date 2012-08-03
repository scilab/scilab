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

      subroutine lusat(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny,g,ng)
c     Copyright INRIA

c     Scicos block simulator
c     Lower-Upper saturation
c     Continuous block, MIMO
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      double precision g(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny,ng

c
      if (flag.eq.9) then
         do 10 i=1,nu
            g(i)=u(i)-rpar(1)
            g(i+nu)=u(i)-rpar(2)
 10      continue
      endif

      if (flag.eq.1) then
         do 15 i=1,nu
            if (u(i).le.rpar(1))then
               y(i)=rpar(1)*rpar(3)
            else if(u(i).ge.rpar(2))then
               y(i)=rpar(2)*rpar(3)
            else
               y(i)=rpar(3)*u(i)
            endif
 15      continue
      endif
      end
