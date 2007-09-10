      subroutine getfunction 
c     Copyright INRIA
      include '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2,nz3=nsiz-3)
      integer cblock,rblock,fcount
      parameter (rblock=30,cblock=rblock*50)
      integer quote,eol,equal,lparen,rparen,comma,semi,left,right,dot
      integer blank,slash
      integer psym,qcount,strcnt,pchar
      integer func(nsiz),endfunc(nsiz),iadr,sadr
      logical cresmat,eqid,last,eof
      integer name,num,cmt
      data quote/53/,eol/99/,name/1/,num/0/,cmt/2/
      data equal/50/,lparen/41/,rparen/42/,slash/48/
      data comma/52/,semi/43/,left/54/,right/55/,dot/51/,blank/40/


      data func/202841615,387453469,nz2*673720360/
      data endfunc/252516110,487331614,672602130,nz3*673720360/

c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c

c
      last=.false.

      fcount=1
      if(top+3.ge.bot) then
         call error(18)
         return
      endif
c     syntax line
      l4=lpt(4)-1
c     analysing syntax to get the end of syntax definition
      call getsym
      if(sym.eq.name) then
c     .  a=func(..) or func(..) syntaxes
         if(char1.eq.equal) then
c     .     a=func(..) 
            call getsym
            call getsym
         endif
      elseif(sym.eq.left) then
c    .   [..]=func()
 41      call getsym
         if(sym.ne.name) goto  42
         call getsym
         if(sym.eq.comma) goto  41
 42      if(sym.ne.right) goto 50
c     
         call getsym
         if(sym.ne.equal) goto 50
         call getsym
      else
         goto 50
      endif
c     lhs analyzed
      if(sym.ne.name) goto 50
c     
      call getsym
      if(sym.eq.eol.or.sym.eq.cmt.or.sym.eq.semi.or.sym.eq.comma) then
         lpt(4)=lpt(3)
         goto 46
      endif
      if(sym.ne.lparen) goto 50
 44   call getsym
      if(sym.ne.name) goto  45
      call getsym
      if(sym.eq.comma) goto  44
 45   if(sym.ne.rparen) goto 50
c     ok, check is next sym is valid
      call getsym
      if(sym.ne.eol.and.sym.ne.semi.and.
     $     sym.ne.comma.and.sym.ne.cmt) goto 50
      lpt(4)=lpt(3)
 46   continue
c     rhs analyzed
      lf=lpt(4)
      if(sym.ne.eol) lf=lf-1
      if(sym.eq.cmt) then
         sym=semi
         char1=slash
      endif
      n=lf-l4
      goto 60
 50   continue
c     invalid syntax
      err=lct(8)
      call error(37)
      lct(8)=1
      return

 60   if(comp(1).ne.0) then 
c     .  str instruction added to store the syntax line
         lkp=comp(1)
         err=sadr(lkp+2+n)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         istk(lkp)=3
         l=lkp+2
         l0=lkp+1
         istk(lkp+1)=n
         call icopy(n,lin(l4),1,istk(l),1)
         comp(1)=l+n
         ilc=(l+n)
      else
         top=top+1
         if (.not.cresmat("getfunction",top,1,1,n)) return
         l=iadr(lstk(top))+6
         call icopy(n,lin(l4),1,istk(l),1)
         ilc=iadr(lstk(top+1))
      endif
c
c     statements of the function
      nc=0
      nr=0

      ilp=ilc+cblock
      ilw=ilp+rblock

      err=sadr(ilw)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(ilp)=1
      strcnt=0
      
      if(sym.eq.eol) then
         if (lpt(4).ge.lpt(6)) then
            if(comp(1).ne.0) then 
               call getlin(2,0)
               eof=.false.
            else
               call getlin(2,0)
               eof=fin.eq.-2
            endif
         else
            eof=.false.
            lpt(4)=lpt(4)+1
            call getsym
         endif
      else
         eof=.false.
      endif
      l4=lpt(4)-1
      psym=sym
     

 71   continue
      psym=sym
      call getsym1(strcnt)
      if(sym.eq.quote) then
         if(strcnt.ne.0) then
            qcount=0
 311        qcount=qcount+1
            if(abs(char1).ne.quote) goto 312
            call getsym1(strcnt)
            goto 311
 312        continue
            if(2*int(qcount/2).ne.qcount)  strcnt=0
         else
            pchar=lin(lpt(3)-2)
            if(abs(pchar).eq.blank) then
               strcnt=1
            elseif(psym.ne.num.and.psym.ne.name.and.psym.ne.rparen.and.
     $              psym.ne.right.and.psym.ne.dot.and.psym.ne.quote)
     $              then
               strcnt=1
            endif
         endif
         goto 71
      elseif(sym.eq.name) then
         if(strcnt.eq.0.and.eqid(syn,func)) then
            fcount=fcount+1
         elseif(strcnt.eq.0.and.eqid(syn,endfunc)) then
            if(fcount.eq.1) then 
               last=.true.
               n=lpt(3)-1-l4
c               lpt(4)=lpt(4)-1
               goto 72
            endif
            fcount=fcount-1
         endif
         goto 71
      elseif(sym.eq.eol) then
         n=lpt(4)-l4
         if (eof) then
c     .     endfunction omitted
            if(macr.eq.0) then
               last=.true.
               goto 72
            else
               call error(272)
               return
            endif
         endif
         goto 72
      elseif(strcnt.eq.0.and.sym.eq.cmt) then
         lpt(4)=lpt(6)
         n=lpt(4)-l4
         goto 72
      else
         goto 71
      endif

c     store a line
 72   continue
      if(last.and.n.le.0) goto 73
      if(ilp+nr+1.ge.ilw) then
c     .  allocate memory for rblock more rows
         ilw=ilw+rblock
         err=sadr(ilw)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      endif
      if(ilc+nc+n.ge.ilp) then
c     . allocate memory for cblock more characters
         ilp1=ilc+nc+n+cblock
         ilw=ilp1+(ilw-ilp)
         err=sadr(ilw)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(nr+1,istk(ilp),-1,istk(ilp1),-1)
         ilp=ilp1
      endif
      istk(ilp+nr+1)=istk(ilp+nr)+n
      call icopy(n,lin(l4),1,istk(ilc+nc),1)
      nc=nc+n
      nr=nr+1
      if(.not.last) then
         if(lpt(4).ge.lpt(6)) then
            call getlin(2,0)
            eof=fin.eq.-2
            l4=lpt(1)
         else
            lpt(4)=lpt(4)+1
            call getsym
            l4=lpt(4)
         endif
         goto 71
      endif
c
   73   continue
c      call getsym
      il=ilc
      ilp1=il+2
      ilc1=ilp1+nr+1
      ilp1=il+4 
      ilc1=ilp1+nr+1
      ilw1=ilw
      err=sadr(ilw1+nc)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call icopy(nc,istk(ilc),1,istk(ilw1),1)
      inc=-1
      if(ilp.ge.ilp1) inc=1
      call icopy(nr+1,istk(ilp),inc,istk(ilp1),inc)
      call icopy(nc,istk(ilw1),1,istk(ilc1),1)
      if(comp(1).ne.0) then
         istk(il)=26
         istk(il+1)=nr
         istk(il+2)=1
         istk(il+3)=0
         comp(1)=ilc1+nc
         call setlnb
      else
         top=top+1
         istk(il)=10
         istk(il+1)=nr
         istk(il+2)=1
         istk(il+3)=0
         lstk(top+1)=sadr(ilc1+nc)
      endif
c
      return
      end

