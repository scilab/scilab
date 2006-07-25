
      subroutine intiserror
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,getscalar,cremat
c
      rhs=max(0,rhs)

      if(.not.checklhs('iserror',1,1)) return
      if(.not.checkrhs('iserror',0,1)) return

      if(rhs.eq.1) then
         if(.not.getscalar('iserror',top,top,l)) return
         num=nint(stk(l))
      else
         num=0
         top=top+1
      endif
      if(.not.cremat('iserror',top,0,1,1,l,lc)) return
      if(num.ge.1) then
         if(err2.eq.num) then
            stk(l)=1.0d+0
         else
            stk(l)=0.0d+0
         endif
      else
         if(err2.ne.0) then
            stk(l)=1.0d+0
         else
            stk(l)=0.0d+0
         endif
      endif
      return
      end
