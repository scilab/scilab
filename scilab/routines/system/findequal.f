      subroutine findequal(found)
c     look for a single equal sign exists between the current char and
c     the end of the current instruction (== <= >=) are excluded
c
c     the result is returned in found
c     on return the current char is the last analyzed character
c
c     Author Serge Steer, Copyright INRIA
      include '../stack.h'
      logical found

      integer semi,equal,eol,lparen,rparen,dot,quote
      integer blank,comma,left,right,less,great,not
      integer name,num,cmt
c     
      integer psym
      integer pcount,strcnt,bcount,qcount,pchar
c     
      data blank/40/,semi/43/,equal/50/,eol/99/,comma/52/
      data lparen/41/,rparen/42/,left/54/,right/55/,less/59/,great/60/
      data quote/53/,dot/51/,not/61/
      data name/1/,num/0/,cmt/2/

c     
c initialize counters (parenthesis, quote and brackets)      
      pcount=0
      strcnt=0
      bcount=0

 10   psym=sym
      call getsym
      if(strcnt.ne.0) then
         if(sym.eq.eol) then
            call error(3)
            return
         endif
         if(sym.eq.quote) then
            qcount=0
 11        qcount=qcount+1
            if(abs(char1).ne.quote) goto 12
            call getsym
            goto 11
 12        continue
            if(2*int(qcount/2).ne.qcount)  strcnt=0
         endif
      else if(sym.eq.lparen) then
         pcount=pcount+1
      else if(sym.eq.rparen) then
         pcount=pcount-1
         if(pcount.lt.0) then
            call error(2)
            return
         endif
      else if(sym.eq.quote) then
c     .  check if transpose or beginning of a string
         pchar=lin(lpt(3)-2)
         if(abs(pchar).eq.blank) then
            strcnt=1
         elseif(psym.ne.num.and.psym.ne.name.and.psym.ne.rparen.and.
     $           psym.ne.right.and.psym.ne.dot.and.psym.ne.quote) then
            strcnt=1
         endif
      else if(sym.eq.left) then
         bcount=bcount+1
      else if(sym.eq.right) then
         bcount=bcount-1
         if(bcount.lt.0) then
            call error(2)
            return
         endif
      else if(pcount.eq.0.and.bcount.eq.0) then
         if(sym.eq.equal) then
            if(char1.eq.equal) then
               call getsym
            else
               if(psym.ne.less.and.psym.ne.great.and.psym.ne.not)  then
c     .           single equal sign found
                  found=.true.
                  return
               endif
            endif
         endif
         if(sym.eq.eol.or.sym.eq.comma.or.
     $        sym.eq.semi.or.sym.eq.cmt) then
c     .           single equal sign not found
                  found=.false.
                  return
               endif

c     .  next line for recursive index
         if(sym.eq.lparen) goto 10
      else if(sym.eq.eol.or.sym.eq.cmt) then
         found=.false.
         return
C          if(bcount.eq.0) then
C c     .  end of line reached before all brakets are closed
C             found=.false.
C c           call error(3)
C             return
C          else
C             if(lpt(4).eq.lpt(6))  then
C                call getlin(1,0)
C                if(err.gt.0) return
C             else
C                lpt(4)=lpt(4)+1
C                call getsym
C             endif
C          endif
      endif
      goto 10

      end
