      subroutine cmdstr
c ====================================================================
c     interprets comands arguments (blank or quote delimited character strings)
c ====================================================================
c     Copyright INRIA
      include '../stack.h'
      integer quote,bl(nsiz),iadr,sadr,last
      logical cresmat,strtyp
      integer blank,eol,semi,comma
      data blank/40/,semi/43/,eol/99/,comma/52/
      data quote/53/, bl/nsiz*673720360/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(err1.gt.0) return
      last=0
      rhs=0
      lhs=1


 01   continue
c      strtyp=abs(char1).eq.quote.and.lin(lpt(4)).eq.blank
      strtyp=abs(char1).eq.quote
C     maximum number of characters that we can store
      ilmax=iadr(lstk(bot)-1)
      n = 0
c
      if(comp(1).ne.0)then
c     compilation [3 number-of-char chars-vector]
c     we dont known yet how many chars will be stored
         lkp=comp(1)
         err=sadr(lkp+2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         istk(lkp)=3
         ln=lkp+2
         l0=lkp+1
         comp(1)=lkp
      else
         if(top+2.ge.bot) then
            call error(18)
            return
         endif
         top=top+1
         infstk(top)=0
         call putid(idstk(1,top),bl)
         if (.not.cresmat("getstr",top,1,1,1)) return
         ln=iadr(lstk(top))+6
      endif


c     Begin : reading the string 
      l=ln
      if(strtyp) then
c     quote delimited string
         call getch
 06      if (abs(char1) .eq. quote) go to 08
 07      ln = l+n
         if (char1 .eq. eol) then
            call error(31)
            return
         endif
         if(ln.ge.ilmax) then 
            err=sadr(ln)-lstk(bot)
            call error(17)
            return
         endif
         istk(ln) = char1
         n = n+1
         call getch
         go to 06
 08      call getch
         if (abs(char1) .eq. quote) go to 07

         if (n .le. 0) then
            call error(31)
            return
         endif
         if (char1.eq.eol.or.char1.eq.comma.or.char1.eq.semi) then
            last=1
            goto 19
         endif
      else
 16      if (abs(char1) .eq. blank) go to 18
         ln = l+n
         if (char1.eq.eol.or.char1.eq.comma.or.char1.eq.semi) goto 19
         if(ln.ge.ilmax) then 
            err=sadr(ln)-lstk(bot)
            call error(17)
            return
         endif
         istk(ln) = char1
         n = n+1
         call getch
         go to 16
      endif
 18   call getch
      if (abs(char1) .eq. blank) go to 18
 19   continue
c     end reading : the string is stored in istk(ln-> ln+(n-1))
c     Storing size info in data Base
      if (n.gt.0) then
         if(comp(1).ne.0) then 
            istk(l0)=n
            comp(1)=l+n
         else
            if (.not.cresmat("getstr",top,1,1,n)) return
         endif
         rhs=rhs+1
      endif
      if (char1.eq.eol.or.char1.eq.comma.or.char1.eq.semi) last=1
      sym=char1
      if(last.eq.0) goto 01
c     next line added for getf getfun
      lpt(3)=lpt(4)
      return
      end
