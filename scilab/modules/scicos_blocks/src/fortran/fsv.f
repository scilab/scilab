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

      subroutine fsv(flag,nevprt,t,xd,x,nx,z,nz,tvec,
     $        ntvec,rpar,nrpar,ipar,nipar
     $        ,u1,nu1,y1,ny1)
c
c
      double precision t,xd(*),x(*),z(*),tvec(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      double precision rpar(*),u1(*),y1(*)
c
c
      doubleprecision a,a0,b0,g
      a=u1(1)
      y1(1)=0.d0
      if(a.gt.1.0d0) then
         return
      endif
      g=1.4d0
      a0=2.0d0/g
      b0=(g+1)/g
      if(a.lt.0.528d0) then
         a=(2/(g+1.0d0))**(g/(g-1.0d0))  
      endif
      y1(1)=sqrt(2.0d0*g*(a**a0-a**b0)/(g-1.0d0))
   
 
c
c
      end
