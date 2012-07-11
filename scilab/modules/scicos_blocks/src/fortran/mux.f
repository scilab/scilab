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

      subroutine mux(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,uy1,nuy1,uy2,nuy2,uy3,nuy3,uy4,
c     Copyright INRIA

     &     nuy4,uy5,nuy5,uy6,nuy6,uy7,nuy7,uy8,nuy8,uy9,nuy9)
c     Scicos block simulator
c     ipar(1) : the number of input
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*)
      double precision uy1(*),uy2(*),uy3(*),uy4(*),uy5(*),uy6(*)
      double precision uy7(*),uy8(*),uy9(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar
      integer k
c
c    
      k=0
      goto(20,30,40,50,60,70,80),ipar(1)-1
c
 20   do 25 i=1,nuy1
         k=k+1
         uy3(k)=uy1(i)
 25   continue
      do 27 i=1,nuy2
         k=k+1
         uy3(k)=uy2(i)
 27   continue
      return
c
 30   do 35 i=1,nuy1
         k=k+1
         uy4(k)=uy1(i)
 35   continue
      do 37 i=1,nuy2
         k=k+1
         uy4(k)=uy2(i)
 37   continue
      do 38 i=1,nuy3
         k=k+1
         uy4(k)=uy3(i)
 38   continue
      return
c
 40   do 41 i=1,nuy1
         k=k+1
         uy5(k)=uy1(i)
 41   continue
      do 42 i=1,nuy2
         k=k+1
         uy5(k)=uy2(i)
 42   continue
      do 43 i=1,nuy3
         k=k+1
         uy5(k)=uy3(i)
 43   continue
      do 44 i=1,nuy4
         k=k+1
         uy5(k)=uy4(i)
 44   continue
      return
c
 50   do 51 i=1,nuy1
         k=k+1
         uy6(k)=uy1(i)
 51   continue
      do 52 i=1,nuy2
         k=k+1
         uy6(k)=uy2(i)
 52   continue
      do 53 i=1,nuy3
         k=k+1
         uy6(k)=uy3(i)
 53   continue
      do 54 i=1,nuy4
         k=k+1
         uy6(k)=uy4(i)
 54   continue
      do 55 i=1,nuy5
         k=k+1
         uy6(k)=uy5(i)
 55   continue
      return
c
 60   do 61 i=1,nuy1
         k=k+1
         uy7(k)=uy1(i)
 61   continue
      do 62 i=1,nuy2
         k=k+1
         uy7(k)=uy2(i)
 62   continue
      do 63 i=1,nuy3
         k=k+1
         uy7(k)=uy3(i)
 63   continue
      do 64 i=1,nuy4
         k=k+1
         uy7(k)=uy4(i)
 64   continue
      do 65 i=1,nuy5
         k=k+1
         uy7(k)=uy5(i)
 65   continue
      do 66 i=1,nuy6
         k=k+1
         uy7(k)=uy6(i)
 66   continue
      return
c
 70   do 71 i=1,nuy1
         k=k+1
         uy8(k)=uy1(i)
 71   continue
      do 72 i=1,nuy2
         k=k+1
         uy8(k)=uy2(i)
 72   continue
      do 73 i=1,nuy3
         k=k+1
         uy8(k)=uy3(i)
 73   continue
      do 74 i=1,nuy4
         k=k+1
         uy8(k)=uy4(i)
 74   continue
      do 75 i=1,nuy5
         k=k+1
         uy8(k)=uy5(i)
 75   continue
      do 76 i=1,nuy6
         k=k+1
         uy8(k)=uy6(i)
 76   continue
      do 77 i=1,nuy7
         k=k+1
         uy8(k)=uy7(i)
 77   continue
      return
c
 80   do 81 i=1,nuy1
         k=k+1
         uy9(k)=uy1(i)
 81   continue
      do 82 i=1,nuy2
         k=k+1
         uy9(k)=uy2(i)
 82   continue
      do 83 i=1,nuy3
         k=k+1
         uy9(k)=uy3(i)
 83   continue
      do 84 i=1,nuy4
         k=k+1
         uy9(k)=uy4(i)
 84   continue
      do 85 i=1,nuy5
         k=k+1
         uy9(k)=uy5(i)
 85   continue
      do 86 i=1,nuy6
         k=k+1
         uy9(k)=uy6(i)
 86   continue
      do 87 i=1,nuy7
         k=k+1
         uy9(k)=uy7(i)
 87   continue
      do 88 i=1,nuy8
         k=k+1
         uy9(k)=uy8(i)
 88   continue
      return
      end
