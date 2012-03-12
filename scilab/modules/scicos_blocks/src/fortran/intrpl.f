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
      subroutine intrpl(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u1,nu1,y1,ny1)
c     Copyright INRIA

c     Scicos block simulator
c     ipar(1) : the number of input
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*)
      double precision y1,u1
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*),i
      integer nipar
c
c    
      do 100 i=2,nrpar/2
         if (u1.le.rpar(i)) goto 200 
 100  continue
      i=nrpar/2
 200  continue
      y1=rpar(nrpar/2+i-1) + ((rpar(nrpar/2+i)-rpar(nrpar/2+i-1))/
     $     (rpar(i)-rpar(i-1)))*(u1-rpar(i-1))
      return
      end


