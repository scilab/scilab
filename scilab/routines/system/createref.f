      subroutine createref1(topk)
      include '../stack.h'
      integer topk,sz
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      call createref(iadr(lstk(topk)),topk,lstk(topk+1)-lstk(topk))
      end
      subroutine createref(il,topk,sz)
c =============================================================
c     create a reference to a variable at the top of the stack
c =============================================================
c
c     Copyright INRIA
      include '../stack.h'
c
c     il   : pointer on the beginning of the variable
c     topk : index of the variable in lstk if any
c     sz   : mem size of the variable
c
      integer topk,sz
      integer iadr,sadr
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      top=top+1
      if(top.ge.bot) then
         call error(18)
         return
      endif
c
      ilv=iadr(lstk(top))
      err=sadr(ilv+4)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(istk(il).lt.0) then
         call icopy(4,istk(il),1,istk(ilv),1)
      else
         istk(ilv)=-istk(il)
         istk(ilv+1)=sadr(il)
         istk(ilv+2)=topk
         istk(ilv+3)=sz
      endif
      lstk(top+1)=sadr(ilv+4)
      return
      end

      subroutine setref(topk)
c =============================================================
c     creates a variable on the top of stack to inform stackp that 
c     corresponding variable as already been stored
c =============================================================
c
c     Copyright INRIA
      include '../stack.h'
c
c     topk : index of the stored variable in lstk 
c
      integer topk
      integer iadr,sadr
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      top=top+1
      if(top.ge.bot) then
         call error(18)
         return
      endif
c
      ilv=iadr(lstk(top))
      err=sadr(ilv+4)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      istk(ilv)=-1
      istk(ilv+1)=-1
      istk(ilv+2)=topk
      if(topk.gt.0) then
         istk(ilv+3)=lstk(topk+1)-lstk(topk)
      else
         istk(ilv+3)=0
      endif
      lstk(top+1)=sadr(ilv+4)
      return
      end


