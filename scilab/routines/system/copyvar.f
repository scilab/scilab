      subroutine copyvar(il,sz)
c =============================================================
c     copy a variable  at the top of the stack
c =============================================================
c
c     Copyright INRIA
      include '../stack.h'
c
c     il : pointer on the beginning of the variable
c     sz : memory size used by the variable (in stk words)
c     on return top is incremented
      integer il,sz
c
      integer iadr,sadr
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      top=top+1
      err=lstk(top)+sz-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(top.ge.bot) then
         call error(18)
         return
      endif
      call unsfdcopy(sz,stk(sadr(il)),1,stk(lstk(top)),1)
      lstk(top+1)=lstk(top)+sz
      return
      end
