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

c
c     w(1)=window
c     z(2)=x
c     z(3)=y
c     z(4)=width
c     z(5)=height
c     z(6:6+nu*nu2)=value

      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny
      integer wid,nu2

      integer cur,v
      double precision dv,sciround,ur
      character*40 drv
      data cur/0/


c     
c     
c     do 111 i=1,2*nu
c       do 111 i=1,nu
c      write(6,'(''u='',F5.3)') u(i)
c 111  continue
      nu2=int(nu/ipar(7))
      if(flag.eq.2) then
c     state evolution
c       write(6,'(''nu1='',i3,'' nu2='',i3)') ipar(7), nu2
         ok=1
c         do 1 i=1,2*nu
         do 1 i=1,nu
          ur=10.0d0**ipar(6)
c          ur=10.0d0**3
          ur=sciround(u(i)*ur)/ur
          if (ur.ne.z(5+i)) then
            goto 2
          endif
 1       continue
c         if (ur.eq.z(6)) return
         if (ok.eq.1) then
           return
         endif
  2      wid=z(1)
         if(wid.lt.0) return

         call setblockwin2(int(z(1)),cur)
         call  dr1('xgetdr'//char(0),drv,v,v,v,v,v,v,dv,dv,dv,dv)
         call  dr1('xsetdr'//char(0),'X11'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)

         call recterase2(z(2))
c         do 3 i=1,2*nu
         do 3 i=1,nu
	  ur=10.0d0**ipar(6)
c          ur=10.0d0**3
          ur=sciround(u(i)*ur)/ur
         z(5+i)=ur
c       write(6,'(''z='',F5.3)') z(5+i)
 3       continue
c         z(6)=ur
         call affdraw2(ipar(1),ipar(5),z(6),z(2),ipar(7),nu2)
         call  dr1('xsetdr'//char(0),drv,v,v,v,v,v,v,dv,dv,dv,dv)
      elseif(flag.eq.4) then
c     init
c     .  initial value         
c        do 4 i=1,2*nu
        do 4 i=1,nu
          z(5+i)=0.0d0
 4      continue
c     .  get geometry of the block
         call getgeom2(z(1))

         if(z(1).lt.0.0d0) return
         call setblockwin2(int(z(1)),cur)
         call  dr1('xgetdr'//char(0),drv,v,v,v,v,v,v,dv,dv,dv,dv)
         call  dr1('xsetdr'//char(0),'X11'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)

         call recterase(z(2))
         call affdraw2(ipar(1),ipar(5),z(6),z(2),ipar(7),nu2)
         call  dr1('xsetdr'//char(0),drv,v,v,v,v,v,v,dv,dv,dv,dv)

      endif
      end

      subroutine setblockwin2(win,cur)
      integer win,cur
      integer v,verb
      double precision dv
      data verb/0/

      call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $     dv,dv,dv,dv)
      if(cur.ne.win) then
         call dr1('xset'//char(0),'window'//char(0),win,v,v,v,v,v,
     $        dv,dv,dv,dv)
      endif
      return
      end

      subroutine recterase2(r)
      double precision r(4)
      integer v
      double precision dv,dx,dy,x,y,w,h
      data dx/0.06/,dy/0.06/

      x=r(1)+dx*r(3)
      y=r(2)+r(4)
      w=r(3)*(1.0d0-dx)
      h=r(4)*(1.0d0-dy)
      call  dr1('xclea'//char(0),'v'//char(0),v,v,v,v,v,v,x,y,w,h)
      return
      end

      subroutine affdraw2(fontd,form,val,r,nu,nu2)
      integer fontd(2),form(2)
      double precision val(*),x,y,angle,rect(4),r(4),dx,dy
      character*40 fmt,value
      character*80 value2
      integer nu,nu2
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
      call dr1('xget'//char(0),'font'//char(0),verb,font,nf,v,v,
     $     v,dv,dv,dv,dv)
      call dr1('xset'//char(0),'font'//char(0),fontd(1),fontd(2),v,v,v,
     $     v,dv,dv,dv,dv)
      do 20 j=1,nu
        value=' '
        value2=' '
        ln2=0
        do 15 i=1,nu2
c          C=CMPLX (val((i-1)*nu+j),val((i-1)*nu+j+nu*nu2))
          C=val((i-1)*nu+j)
c          D=val((i-1)*nu+j+nu*nu2)
c              write(*,*)'c=',C,'d=',D
c              WRITE(value,112) C,D
c 112          format(' ',f7.3,'+',f7.3,'i')
          write(value,fmt) C
          ln=lnblnk(value)
c	  write(*,*) 'value=',value,'ln=',ln
c        value(ln+1:ln+1)=char(0)
          value(ln+1:ln+1)=' '
c        do 14 j=1,ln+1
          value2(ln2+1:ln2+ln+2)=value(1:ln+1)
c 14     continue
          ln2=lnblnk(value2)
c	   write(*,*) 'value2=',value2,'ln2=',ln2
 15     continue
        ln2=lnblnk(value2)
        value2(ln2+1:ln2+1)=char(0)

        x=r(1)
        y=r(2)
        call dr1('xstringl'//char(0),value2,v,v,v,v,v,v,x,y,rect,dv)
        x=r(1)+max(0.0d0,(r(3)-rect(3))/2.0d0)
        y=max(0.0d0,(r(4)-rect(4)*(1+(nu-1)*1.2d0))/2.0d0)
        y=r(2)+y+(nu-j)*(rect(4)*1.2d0)
        call dr1('xstring'//char(0),value2,v,v,v,0,v,v,x,y,angle,dv)
 20   continue
      call dr1('xset'//char(0),'font'//char(0),font(1),font(2),v,v,v,
     $     v,dv,dv,dv,dv)
      call dr1('xget'//char(0),'pixmap'//char(0),verb,pix,na,v,v,v,
     $     dv,dv,dv,dv)
      if(pix.eq.1) then
         call dr1('xset'//char(0),'wshow'//char(0),v,v,v,v
     $        ,v,v,dv,dv,dv,dv)
      endif
      return
      end


      subroutine getgeom2(g)
      include "../stack.h"
      double precision g(*)
      integer scicstring,ret
      integer sadr,iadr
      iadr(l) = l + l - 1
      sadr(l)=(l/2)+1
      ret=scistring(rhs+1, 'getgeom',1,0)
      il=iadr(lstk(top+1))
      l=sadr(il+4)
      call dcopy(5,stk(l),1,g,1)
c      top=top-1
      return
      end

