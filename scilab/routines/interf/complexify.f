      logical function complexify(n)
c     transforms the real matrix at position n into complex
      include '../stack.h'
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      complexify=.false.
      nn=top-rhs+n
      ln=lstk(nn)
      ln1=lstk(nn+1)
      isize=ln1-ln
      iln=iadr(ln)
      if(istk(iln+3).eq.1) then
         complexify=.true.
         return
      endif
      istk(iln+3)=1
      m1=istk(iln+1)
      n1=istk(iln+2)
      mn=m1*n1
      do 1 k=top-rhs+n,top
         lstk(k+1)=lstk(k+1)+mn
 1    continue
      lfree=lstk(top+1)
      lw=lfree-ln
      err=lfree+lw+mn-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      call scidcopy(isize,stk(ln),1,stk(lfree),1)
      call dcopy(mn,0.0d0,0,stk(lfree+isize),1)
      call scidcopy(lw-isize-mn,stk(ln1),1,stk(lfree+isize+mn),1)
      call scidcopy(lw,stk(lfree),1,stk(ln),1)
      complexify=.true.
      return
      end

