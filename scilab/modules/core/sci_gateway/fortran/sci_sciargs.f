      subroutine intsciargs(fname)
c     sciargs()
      character*(*) fname
      logical checkrhs,checklhs
      integer iadr,sadr,sciiargc
      include 'stack.h'
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs = max(0,rhs)
      lbuf = 1

      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return

      nargs = sciiargc()
      top=top+1
      il=iadr(lstk(top))
      l=il+5+nargs+1
      err=sadr(l)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il)=10
      istk(il+1)=nargs+1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1

      do 20 k=0,nargs
         call scigetarg(k,buf)
         l1=len(buf)+1
 10      l1=l1-1
         if(buf(l1:l1).eq.' ') goto 10
         err=sadr(l+l1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call cvstr(l1,istk(l),buf,0)
         istk(il+5+k)=istk(il+4+k)+l1
         l=l+l1
 20   continue
      lstk(top+1)=sadr(l)
      return
      end

