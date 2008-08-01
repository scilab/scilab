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

      integer cur,v
      double precision dv,sciround,ur
      character*40 drv
      data cur/0/


c     
c     DEBUG
c     do 111 i=1,2*nu
c       do 111 i=1,nu
c      write(6,'(''u='',F5.3)') u(i)
c 111  continue

      nu2 = int(nu/ipar(7))
c ----------- state evolution --------------------------------
      if(flag.eq.2) then
c     state evolution
c       write(6,'(''nu1='',i3,'' nu2='',i3)') ipar(7), nu2

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
  2      wid = z(1)
         if(wid.lt.0) return

        do 3 i=1,nu
 	   ur=10.0d0**ipar(6)
         ur=sciround(u(i)*ur)/ur
         z(6+i)=ur
 3       continue
 
         call affdraw2(ipar(1),ipar(5),z(7),z(2),ipar(7),nu2,z(1),z(6))


c ----------- INIT --------------------------------
      elseif(flag.eq.4) then
c     init
c     .  initial value         
        do 4 i=1,nu
          z(6+i)=0.0d0
 4      continue
 
c     .  get geometry of the block
         call getgeom(z(1))

         if(z(1).lt.0.0d0) return

        call affin2(ipar(1), ipar(2), ipar(3),
     &  ipar(7), nu2, z(1), z(2),z(3),z(4),z(5),z(6) )
           

c         call affdraw2(ipar(1),ipar(5),z(6),z(2),ipar(7),nu2)

      endif
      end


!      subroutine recterase2(r)
!      double precision r(4)
!      integer vs
!      double precision dv,dx,dy,x,y,w,h
!      data dx/0.06/,dy/0.06/
!
!      x=r(1)+dx*r(3)
!      y=r(2)+r(4)
!      w=r(3)*(1.0d0-dx)
!      h=r(4)*(1.0d0-dy)
!c TODO recreate xclea      
!c      call  dr1('xclea'//char(0),'v'//char(0),v,v,v,v,v,v,x,y,w,h)
!      return
!      end

      subroutine affdraw2(fontd,form,val,r,nu,nu2,winnum,textind)
      integer fontd(2),form(2)
      double precision val(*),x,y,angle,rect(4),r(4),dx,dy
      character*40 fmt,value
      character*80 value2
      integer nu,nu2
      double precision textind
      double precision C,D
c      COMPLEX*16 C
      integer font(5),nf,pix
      integer v,verb
      double precision dv
      data angle/0.0d0/,verb/0/
c      write(*,*) 'nu=',nu,'nu2=',nu2
c      do 1132 i=1,2*nu*nu2
c      write(*,*) 'val',i,'=',val(i)
c1132  continue
      write(fmt,'(''(f'',i3,''.'',i3,'')'')') form(1),form(2)

      do 20 j=1,nu
        value=''
c        value2=' '
        ln2=0
        do 15 i=1,nu2
          C = val((i-1)*nu+j)
          write(value,fmt) C
          ln=lnblnk(value)
          value(ln+1:ln+1)= char(0)
          call settxtel(j, i, winnum, textind, value)         
c          ln=lnblnk(value)
c          value(ln+1:ln+1)=' '
c          value2(ln2+1:ln2+ln+2)=value(1:ln+1)
c          ln2=lnblnk(value2)
 15     continue
c        ln2=lnblnk(value2)
c        value2(ln2+1:ln2+1)=char(0)
c        x=r(1)
c        y=r(2)
        
c       call dr1('xstringl'//char(0),value2,v,v,v,v,v,v,x,y,rect,dv)
        
c        x=r(1)+max(0.0d0,(r(3)-rect(3))/2.0d0)
c        y=max(0.0d0,(r(4)-rect(4)*(1+(nu-1)*1.2d0))/2.0d0)
c        y=r(2)+y+(nu-j)*(rect(4)*1.2d0)
c      call dr1('xstring'//char(0),value2,v,v,v,0,v,v,x,y,angle,dv)
 20   continue

c     display the updated text
      call affup2(winnum, textind)

      return
      end
      
c ---------------------------------------------

      subroutine getgeom2(g)
      include "stack.h"
      double precision g(*)
      integer scicstring,ret
      integer sadr,iadr
      iadr(l) = l + l - 1
      sadr(l)=(l/2)+1
      ret=scistring(rhs+1, 'getgeom',1,0)
      il=iadr(lstk(top+1))
      l=sadr(il+4)
      call dcopy(6,stk(l),1,g,1)
      return
      end

