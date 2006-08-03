
      subroutine intmode
c     Copyright INRIA
      include 'stack.h'
      integer cmode,topk
      logical checkrhs,checklhs,getscalar,cremat
      integer iadr,sadr
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
      if(.not.checklhs('mode',1,1)) return
      if(.not.checkrhs('mode',0,1)) return

      if (rhs.eq.1) then
c     .  set  line mode value
         if(.not.getscalar('mode',top,top,l)) return
         lct(4)=int(stk(l))
         if(lct(4).eq.7.or.lct(4).eq.4) call msgs(26,0)
         call objvide('mode',top)
      else
c     .  get line  mode value
         top=top+1
         if(.not.cremat('mode',top,0,1,1,lr,lc)) return
         stk(lr)=lct(4)
      endif
      return
      end