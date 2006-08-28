c     SCILAB function : dlgamma, fin = 8
      subroutine intsdlgamma(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr, sadr
      integer topk,rhsk,topl
      logical checkrhs,checklhs,getmat,getscalar,cremat
      double precision psi
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable x (number 1)
      if(.not.getmat(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
      if(m1*n1.eq.0) then
         top=top-rhs+1
         return
      endif
      if(it1.ne.0) then
         err=1
         call error(52)
         return
      endif
      do 10 i=0,n1*m1-1
         stk(lr1+i)=psi(stk(lr1+i))
 10   continue
c     
      return
      end

