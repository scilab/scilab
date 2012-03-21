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

      subroutine powblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     rpar(1) is power
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c     
      if (nrpar.eq.1) then
         do 15 i=1,nu
            if(u(i).lt.0.d0) then
               if (flag.ge.4) return
               flag=-2
               return
            elseif(u(i).eq.0.d0.and.rpar(1).le.0) then
               if (flag.ge.4) return
               flag=-2
               return
            endif
            y(i)=u(i)**rpar(1)
 15      continue
      else
         do 25 i=1,nu
            if(ipar(1).le.0.and.u(i).eq.0.0d0) then
               if (flag.ge.4) return  
               flag=-2
               return
            endif
            y(i)=u(i)**ipar(1)
 25      continue
      endif
      
      end
