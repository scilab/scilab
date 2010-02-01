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
c     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
c     
c     See the file ./license.txt
c     

      subroutine affich2(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Displays the value of the input in a graphic window
c     
c     ipar(1) = font
c     ipar(2) = fontsize
c     ipar(3) = color
c     ipar(4) = win
c     ipar(5) = nt : total number of output digits
c     ipar(6) = nd number of rationnal part digits
c     ipar(7) = rows/columns 

c     
c     z(1)=window
c     z(2)=x
c     z(3)=y
c     z(4)=width
c     z(5)=height
c     z(6)=text_index
c     z(7:7+nu*nu2)=value

      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny
      integer wid,nu2

      double precision sciround,ur
      
      nu2 = int(nu/ipar(7))

c     ----------- State Update -------------------------------
      if(flag.eq.2) then
c     state evolution
         ok = 1

         do 1 i=1,nu
            ur = 10.0d0**ipar(6)
            ur = sciround(u(i)*ur)/ur
            if (ur.ne.z(6+i)) then
               goto 2
            endif
 1       continue
         
         if (ok.eq.1) then
            return
         endif
         
 2       wid = z(1)
         if(wid.lt.0) return

         do 3 i=1,nu
            
            ur = 10.0d0**ipar(6)
            ur = sciround(u(i)*ur)/ur
            z(6+i) = ur
            
 3       continue
         
         call affdraw2(ipar(5),z(7),z(2),ipar(7),nu2,z(1),z(6))


c     ----------- Initializiation ------------------------------
      elseif(flag.eq.4) then
c     init
c     .  reset initial value         
         do 4 i=1,nu
            z(6+i)=0.0d0
 4       continue
c     .  get graphic window where the block is
         call getgeom(z(1))

         if(z(1).lt.0.0d0) return
      endif
      end

      subroutine affdraw2(form,val,r,nu,nu2,winnum,textind)
      integer form(2)
      double precision val(*)
      character*40 fmt,value
      integer nu,nu2
      double precision textind
      double precision C
      write(fmt,'(''(f'',i3,''.'',i3,'')'')') form(1),form(2)
      do 20 j=1,nu
         value=''
         ln2=0
         do 15 i=1,nu2
            C = val((i-1)*nu+j)
            write(value,fmt) C
            ln=lnblnk(value)
            value(ln+1:ln+1)= char(0)
c     update element of the string
            call settxtel(j, i, winnum, textind, value)         
 15      continue
 20   continue

c     display the updated text
      call affup2(winnum, textind)

      return
      end
      
c     ---------------------------------------------


