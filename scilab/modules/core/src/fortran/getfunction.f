c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine getfunction 
c     getfunction is used by the parser to acquire an inline function.
c
c     Supported syntax are
c     function <lhs>=fun <rhs> <instr1>
c      <instri>
c      ....
c     endfunction
c 
c     or
c      function <lhs> fun <rhs> <instr1>,endfunction
c    
c     <lhs> must be
c        - a simple variable name followed by an =, 
c        - a comma separated sequence of variable names enclosed in
C           brackets  (the sequence may be empty)and followed by an  "="
c        -  nothing.
c     <rhs> must be
c        - a comma separeted sequence of variable names enclosed in
C          parenthesis (the sequence may be empty)  
c        - nothing.
c     <instr1> must be a sequence (may be empty) of Scilab instructions
c          with no carriage return preceeded by a comma or a semi column
C          if the sequence of instructions is empty or limmited to a comment
C          the comma or semi column can be omitted.
c     <instri> must be a sequence (may be empty) of Scilab instructions
c          with no carriage return 
c
c     getfunction creates a character string vector stored in the Scilab
c     stack, the first row contains <lhs> fun <rhs>. The second one
c     contains the  <instr1> instruction or just a blank if <instr1> is empty.
c     the next rows contains the sequence of <instri>.
c
c     The result of getfunction is transformed into a function data type
C     by the getfun routine.

      include 'stack.h'
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
      call getsym1(1)
      if(sym.eq.name) then
c     .  a=func(..) or func(..) syntaxes
         if(char1.eq.equal) then
c     .     a=func(..) 
            call getsym1(1)
            call getsym1(1)
         endif
      elseif(sym.eq.left) then
c    .   [..]=func()
 41      call getsym1(1)
         if(sym.ne.name) goto  42
         call getsym1(1)
         if(sym.eq.comma) goto  41
 42      if(sym.ne.right) goto 50
c     
         call getsym1(1)
         if(sym.ne.equal) goto 50
         call getsym1(1)
      else
         goto 50
      endif
c     lhs analyzed
      if(sym.ne.name) goto 50
c     
      call getsym1(1)
      if(sym.eq.eol.or.sym.eq.cmt.or.sym.eq.semi.or.sym.eq.comma) then
         lpt(4)=lpt(3)
         char1=blank
         goto 46
      endif
      if(sym.ne.lparen) goto 50
 44   call getsym1(1)
      if(sym.ne.name) goto  45
      call getsym1(1)
      if(sym.eq.comma) goto  44
 45   if(sym.ne.rparen) goto 50
c     ok, check if next sym is valid
      call getsym1(1)
      if(sym.ne.eol.and.sym.ne.semi.and.
     $     sym.ne.comma.and.sym.ne.cmt) goto 50
      lpt(4)=lpt(3)
      char1=blank
 46   continue
c     rhs analyzed
      lf=lpt(4)
      if(sym.ne.eol) lf=lf-1
      n=lf-l4
      goto 60
 50   continue
c     invalid syntax
      if(rio.ne.rte) then
         err=lct(8)
      else
         err=-1
      endif

      call error(37)
      lct(8)=1
      return

 60   continue
      if(comp(1).ne.0) then 
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
      ncont=0
      eof=.false.

      if (sym.eq.cmt) lpt(4)=lpt(4)-1
      l4=lpt(4)

      psym=sym
 71   continue
      psym=sym
      call getsym1(1)
      if(sym.eq.quote) then
         if(strcnt.ne.0) then
            qcount=0
 311        qcount=qcount+1
            if(abs(char1).ne.quote) goto 312
            call getsym1(1)     
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
               n=lpt(3)-l4
 313           n=n-1
               if(n.gt.0.and.lin(l4-1+n).eq.blank) goto 313
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
c     .  looking for eol (needed when function is called in a for
c     .  because in this case all the for content has already be put
C     .   into lin
         l=lpt(4)-1
 314     l=l+1
         if (l.lt.lpt(6).and.lin(l).ne.eol) goto 314
         lpt(4)=l
         char1=blank
         call getsym1(1) 
         n=lpt(4)-l4
         goto 72
      else
         goto 71
      endif

c     store a line
 72   continue
      if(ilp+nr+1+ncont.ge.ilw) then
c     .  allocate memory for rblock more rows
         ilw=ilw+rblock
         err=sadr(ilw)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      endif
      if(ilc+nc+n+ncont.ge.ilp) then
c     . allocate memory for cblock more characters
         ilp1=ilc+nc+n+ncont+cblock
         ilw=ilp1+(ilw-ilp)
         err=sadr(ilw)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(nr+1,istk(ilp),-1,istk(ilp1),-1)
         ilp=ilp1
      endif
      if (ncont.gt.0) then
c     .  add ncont empty lines before the logical line
         do ic1=1,ncont
            istk(ilp+nr+1)=istk(ilp+nr)+1
            istk(ilc+nc)=blank
            nc=nc+1
            nr=nr+1
         enddo
         ncont=0
      endif
      if(last.and.n.le.0) then
c     .  add an empty line if endfunction is at the beginning of a line
        istk(ilp+nr+1)=istk(ilp+nr)+1
         istk(ilc+nc)=blank
         n=1
      else
         istk(ilp+nr+1)=istk(ilp+nr)+n
c     .  write(6,'(30(i2,1x))') (lin(l4+ii),ii=0,n-1)
         call icopy(n,lin(l4),1,istk(ilc+nc),1)
      endif
      nc=nc+n
      nr=nr+1
      if(.not.last) then
         if(lpt(4).ge.lpt(6)) then
            lct8=lct(8)
            call getlin(2,0)
            eof=fin.eq.-2
            l4=lpt(1)
            ncont=lct(8)-lct8-1
         else
            lpt(4)=lpt(4)+1
            l4=lpt(4)
            call getsym1(1)
         endif
         goto 71
      endif
c
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

