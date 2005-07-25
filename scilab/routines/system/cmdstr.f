      subroutine cmdstr
c ====================================================================
c     interprets comands arguments (blank or quote delimited character strings)
c ====================================================================
c     Copyright INRIA
      include '../stack.h'
      integer quote,bl(nsiz),iadr,sadr
      logical cresmat,strtyp,last
      integer blank,eol,semi,comma,slash
      data blank/40/,semi/43/,eol/99/,comma/52/,slash/48/
      data quote/53/, bl/nsiz*673720360/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(err1.gt.0) return

      rhs=0
      lhs=1
      last=.false.


 01   continue
c     get next argument
      strtyp=abs(char1).eq.quote
c
c     preserve current position in the line: the beginning of the arg
     

c     Compute the length of the current argument
      n = 0
      if(strtyp) then
c     .  quote delimited string
         l4=lpt(4)
 05      call fortrangetch
         if (abs(char1) .eq. quote) then
c     .     one quote found, check for double quote
            call fortrangetch
            if (abs(char1) .ne. quote) then
c     .        a simple quote: end of argument
               goto 10
            endif
         endif
         if (char1 .eq. eol) then
            call error(31)
            return
         endif
         n = n+1
         go to 05
      else
c     .  argument terminated by a blank, a comma, a semicolumn, a
c     .  comment or a eol
         l4=lpt(4)-1
 06      if (abs(char1) .eq. blank) goto 10
         if (char1.eq.eol .or. char1.eq.comma .or. char1.eq.semi) then
            last   = .true.
            goto 10
         endif
         if (char1.eq.slash.and.lin(lpt(4)).eq.slash) then
            lpt(4)=lpt(4)-1
            last   = .true.
            goto 10
         endif
         n = n+1
         call fortrangetch
         go to 06
      endif


 10   continue
c     one argument found
c      if (n .le. 0) then
c         call error(31)
c         return
c      endif
      if(n.gt.0) then
c     store the argument
         if(comp(1).ne.0)then
c     compilation [3 number-of-char chars-vector]
            lkp=comp(1)
            err=sadr(lkp+2+n)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            istk(lkp)=3
            istk(lkp+1)=n
            call icopy(n,lin(l4),1,istk(lkp+2),1)
            comp(1)=lkp+2+n
         else
            if(top+2.ge.bot) then
               call error(18)
               return
            endif
            top=top+1
            infstk(top)=0
            call putid(idstk(1,top),bl)
            if (.not.cresmat("getstr",top,1,1,n)) return
            ln=iadr(lstk(top))+6
            call icopy(n,lin(l4),1,istk(ln),1)
         endif
         rhs = rhs+1
      endif

 15   if (abs(char1) .eq. blank) then
         call fortrangetch
         go to 15
      endif

c     Check for end of argument list
      last=last.or.char1.eq.eol.or.char1.eq.comma.or.char1.eq.semi
      last=last.or.(char1.eq.slash.and.lin(lpt(4)).eq.slash)
      if(.not.last) goto 01

c     next lines added for getf getfun
      sym=char1
      lpt(3)=lpt(4)
      return
      end
