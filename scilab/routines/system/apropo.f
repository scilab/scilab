      subroutine apropo
c     ====================================================================
c     Call unix:  scihelp -k symbol
c     ====================================================================
c     Copyright INRIA
      include '../stack.h'
      character*80 h
      integer name,eol,quote
      data name/1/,eol/99/,quote/53/,dot/51/
      if (char1 .eq. eol) then 
         h='apropos'
         nstr=7
      else
         call getsym 
         if (sym .eq. name) then
            nstr=lpt(4)-lpt(3)+1
            call cvstr(nstr,lin(lpt(3)-1),h,1)
         else
C     argument is a string 
            if (sym.eq.0) sym = dot
            call cvstr(1,sym,h,1)
            nstr=1
            if (sym.eq.quote) then 
               call getstr
               if(err.gt.0) return
               call getsym
               ilog= getsmat("helpmg",top,top,ms,ns,1,1,lr,nstr)
               top=top-1
               call cvstr(min(nstr,80),istk(lr),h,1)
            endif
         endif
      endif
      h(nstr+1:nstr+1)= char(0)
      call isciap(buf,h(1:nstr+1),ierr)
      if(ierr.ne.0) then
         call error(85)
         return
      endif
      return
      end
