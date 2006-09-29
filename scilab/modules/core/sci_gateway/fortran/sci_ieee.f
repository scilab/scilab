      subroutine intsieee(fname)
c     --------------------------
      character*(*) fname
      logical checkrhs,checklhs
      include 'stack.h'
      logical cremat, getscalar

      integer iadr
c
      iadr(l)=l+l-1
C
      nbvars = 0
      rhs = max(0,rhs)
      if(.not.checkrhs(fname,0,1)) return
      if(.not.checklhs(fname,1,1)) return
      if(rhs.le.0) then
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr)=ieee
      else
         if(.not.getscalar(fname,top,top,lr)) return
         i=stk(lr)
         if(i.lt.0.or.i.gt.2) then
            err=1
            call error(116)
            return
         endif
         ieee=i
         il=iadr(lstk(top))
         istk(il)=0
      endif
      end
      