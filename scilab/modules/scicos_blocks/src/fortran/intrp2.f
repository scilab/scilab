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

      subroutine intrp2(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u1,nu1,u2,nu2,y1,ny1)
c     Copyright INRIA

c     Scicos block simulator
c     ipar(1) : the number of input
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*)
      double precision y1,u1,u2
      double precision vx1,vx2,vy1,vy2,vz1,vz2,vz3,vz4
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*),i
      integer nipar
c
c    
      do 100 i=2,ipar(1)
         if (u1.le.rpar(i)) goto 200 
 100  continue
      i=ipar(1)
 200  do 300 j=2,ipar(2)
         if (u2.le.rpar(j+ipar(1))) goto 400 
 300  continue
      j=ipar(2)
 400  continue
      vy1=rpar(ipar(1)+j-1)
      vy2=rpar(ipar(1)+j)
      vz1=rpar(ipar(1)+ipar(2)+(i-2)*ipar(2)+j-1)
      vz4=rpar(ipar(1)+ipar(2)+(i-2)*ipar(2)+j)
      vz2=rpar(ipar(1)+ipar(2)+(i-1)*ipar(2)+j-1)
      vz3=rpar(ipar(1)+ipar(2)+(i-1)*ipar(2)+j)
      vx1=rpar(i-1)
      vx2=rpar(i)     
      y1=(1.0d0-(u2-vy1)/(vy2-vy1))*(vz1+(vz2-vz1)*(u1-vx1)/(vx2-vx1))
     $     +((u2-vy1)/(vy2-vy1))*(vz4+(vz3-vz4)*(u1-vx1)/(vx2-vx1))
      return
      end


