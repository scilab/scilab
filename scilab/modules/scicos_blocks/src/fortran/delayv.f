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

      subroutine delayv(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u1,nu1,u2,nu2,y,ny)
c     Copyright INRIA

c     rpar(1)=dt
c     delayv=u(nin)
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),y(*)
      double precision u1(*),u2(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu1,nu2,ny

      double precision a,dtat,u2r
      integer i,in,j,k 

      j=int((nz-1)/nu1)    
      if(flag.eq.3) then
         tvec(1)=t+rpar(1)     
         k=int(u2(1)/rpar(1))
         if(k.gt.(j-3)) tvec(2)=t
         if(k.lt.1) tvec(2)=t
      endif
c
c     .   shift buffer
      if(flag.eq.2) then
         do 10 i=1,j
            z(i)=z(i+1)
 10      continue            
         do 30 in=1,nu1-1
            do 35 ii=(in*j)+1,(in+1)*j
               z(ii)=z(ii+1)
 35         continue
 30      continue
         z(nz)=t
         
         do 20 in=1,nu1
            z(j*in)=u1(in)
 20      continue
      endif
      
      if(flag.eq.1.or.flag.eq.6) then
         dtat=t-z(nz)
         do 08 in=1,nu1    
c     extrapolate to find values at delta.t   
            if(u2(1).le.dtat) then 
c     initialisation start
               if(dtat.lt.rpar(1)/100.0d0) then
                  a=u2(1)/(rpar(1)+dtat)
c     delete negative delay
                  if(a.le.(0.0d0)) a=0.0d0
                  y(in)=(1-a)*z(j*in)+a*z((j*in)-1)
               else
                  a=u2(1)/dtat
c     delete negative delay
                  if(a.le.(0.0d0)) a=0.0d0
                  y(in)=(1-a)*u1(in)+a*z(j*in)
               endif  
            else 
               u2r=(u2(1)-dtat)
               k=int(u2r/rpar(1))
c     limitation of size buffer
               if(k.gt.(j-3)) then 
                  k=j-3
                  a=1.0d0
               else 
                  a=(u2r-(k*rpar(1)))/rpar(1)
               endif
c     interpolate to find values at t-delay 
               y(in)=(1-a)*z((j*in)-k)+a*z((j*in)-k-1)
            endif
 08      continue

      endif
      return
      end
      
      
      
  
