      subroutine oldload
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      integer id(nsiz),mode(2),h(nsiz)
      integer top2,job,semi
      logical opened,eptover
      integer iadr,sadr
c
      data blank/40/,semi/43/

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)
      il=iadr(lstk(top))
      if(rhs.lt.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(1)=-101
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
      call savlod(lunit,id,-2,top)
      if(err.gt.0) goto 39
c     
      if(rhs.gt.1) goto 40
 36   job = lstk(bot) - lstk(top)
      id(1)=blank
      call savlod(lunit,id,job,top)
      il=iadr(lstk(top))
      if(istk(il).eq.0) goto 39
      sym = semi
      rhs = 0
      call stackp(id,1)
      top = top + 1
      go to 36
 39   if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      istk(il)=0
      go to 999
c     
 40   top=top2
      sym=semi
      m=rhs
      rhs=0
      do 44 k=2,m
         job = lstk(bot) - lstk(top)
         il=iadr(lstk(top))
         if(istk(il).ne.10) then
            err=k
            call error(55)
            return
         endif
         lc=il+5+istk(il+1)*istk(il+2)
         nc=min(nlgh,istk(il+5)-1)
         call namstr(h,istk(lc),nc,0)
         call savlod(lunit,h,job,top)
         if(istk(il).eq.0) goto 39
         call stackp(h,1)
         if(k.lt.m) rewind(lunit)
 44   continue
      il=iadr(lstk(top))
      goto 39
 999  return
      end

      subroutine oldsave
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      integer id(nsiz),mode(2)
      integer top2,job,semi
      logical opened,eptover
      integer iadr,sadr
c
      data blank/40/,semi/43/

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     

      lw=lstk(top+1)
      il=iadr(lstk(top))
      if(rhs.lt.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(1)=100
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c     
      call savlod(lunit,id,-1,top)
      if(err.gt.0) goto 33
      if(rhs.ge.2) then
         k=top2
      else
         k=bbot-1
         if(k.lt.bot) goto 999
      endif
 32   continue
      l=k
      ilk=iadr(lstk(k))
      if(istk(ilk).lt.0) l=istk(ilk+2)
      call savlod(lunit,idstk(1,k),0,l)
      k = k-1
      if(k.ge.bot.and.rhs.eq.1 .or. k.gt.top.and.rhs.gt.1) goto 32
 33   if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      istk(il)=0
 999  return
      end
