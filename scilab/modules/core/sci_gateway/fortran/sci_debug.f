
      subroutine intdebug()
c     Copyright INRIA
      include 'stack.h'
      logical checkrhs,checklhs,getscalar,cremat
c
      rhs=max(0,rhs)
      if(.not.checklhs('debug',1,1)) return
      if(.not.checkrhs('debug',0,1)) return
      if (rhs.eq.1) then
c     .  set debug mode
         if(.not.getscalar('debug',top,top,l)) return
         ddt = int(stk(l))
         write(buf(1:4),'(i4)') ddt
         call basout(io,wte,' debug '//buf(1:4))
         call objvide('debug',top)
      else
c     .  get debug mode
         top=top+1
         if(.not.cremat('debug',top,0,1,1,lr,lc)) return
         stk(lr)=ddt
      endif 
      return
      end
