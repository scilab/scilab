c ====================================================================
      subroutine intgstacksize
c     Copyright INRIA
      include 'stack.h'
      integer offset
      logical checkrhs,checklhs,cremat,getscalar
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('gstacksize',1,1)) return
      if(.not.checkrhs('gstacksize',0,1)) return

      if (rhs.eq.0) then
         top=top+1
         if(.not.cremat('gstacksize',top,0,1,2,l,lc)) return
         stk(l)=lstk(gbot)-lstk(isiz+2)+1
         if(gtop.ge.isiz+2) then
            stk(l+1)=lstk(gtop+1)-lstk(isiz+2)+1
         else 
            stk(l+1)=0.0d0
         endif
         return
      endif

      if(.not.getscalar('gstacksize',top,top,l)) return
      mem=stk(l)
      memold=lstk(gbot)-lstk(isiz+2)
      if (mem.eq.memold) goto 10
      l=lstk(gtop+1)-lstk(isiz+2)
      if (mem.lt.l) then
         buf='Required memory too small for defined data'
         call error(1503)
         return
      endif
      
      call scigmem(mem+1,offset)
      
      if(offset.eq.0) then
         call error(112)
         return
      endif
      
      call adjustgstacksize(mem,offset)
 10   call objvide('gstacksize',top)
      return
      end
c ====================================================================      
