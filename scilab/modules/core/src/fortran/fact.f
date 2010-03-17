c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine fact
c     ======================================================================
c     analyseur de facteurs
c     ======================================================================
c     
      include 'stack.h'
c     
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      logical eptover
      integer r,excnt,psym,p,count,v
      integer id(nsiz),op,fun1
      integer star,dstar,semi,eol,blank,percen
      integer comma,lparen,rparen,hat,dot,equal
      integer quote,left,right,colon,not
      integer num,name,cmt
      integer cconc,extrac,rconc
      logical recurs,compil,dotsep,nullarg,ok
      integer setgetmode
      integer minus,plus
      integer iadr,sadr
      
      data star/47/,dstar/62/,semi/43/,eol/99/,blank/40/,percen/56/
      data comma/52/,lparen/41/,rparen/42/, hat/62/,dot/51/,equal/50/
      data quote/53/,left/54/,right/55/,colon/44/,not/61/
      data minus/46/,plus/45/
      data num/0/,name/1/,cmt/2/
      data cconc/1/,extrac/3/,rconc/4/
      
      
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      r = rstk(pt)
c     
      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') pt,r,sym
         call basout(io,wte,' factor pt:'//buf(1:4)//' rstk(pt):'//
     &        buf(5:8)//' sym:'//buf(9:12))
      endif
c     
      nullarg=.true.
      dotsep=.false.
      ir=r/100
      if(ir.ne.3) goto 01
      goto(25,26,91,29,99,52,43,48,55,62,65,66,41),r-300
      goto 99
c     
 01   continue
c     next test added to handle syntax like a*-b... or a*--+b....  for Matlab
c     compatiblity
      count=0
 02   continue
      if (sym.eq.minus) then
         count=-count+1
         call getsym
         goto 02
      elseif (sym.eq.plus) then
         call getsym
         goto 02
      endif
      if(count.ne.0) then
c     .  memorize sign change to be applied at the end of the factor
         if ( eptover(1,psiz-1))  return
         rstk(pt)=303
      endif

      if (sym.eq.left) go to 20
      if (sym.eq.quote) go to 15
      if (sym.eq.num) go to 10
      excnt = 0
      if (sym .eq. name) go to 30
      if (sym .eq. colon) then
         call getsym
c     call eye()
         if(comp(1).ne.0) then
            if (compil(21,0,0,0,0)) then 
               if (err.gt.0) return
            endif
         endif
         fun=6
         fin=13
         rhs=0
         goto 53
      endif
      id(1) = blank
      if (sym .eq. lparen) go to 36

      if(err1.gt.0) then
c     .  check for error in short circuit skip mode
         p=pt-5
         if(p.gt.0.and.rstk(p).eq.106) then
c     .     in short circuit skip mode
            if(int(abs(-errct)/100000).ne.0.and.err1.ne.9191919) then
c     .        while in errcatch mode (see expr.f)
               call error(2)
c               pt=p
               return
            else
c     .        errcatch mode off, really send an error
               err1=0
               call error(2)
               return
            endif
         endif
         p=pt+1
 03      p=p-1
         if(p.le.0) goto 04
         r=rstk(p)
         if(int(r/100).ne.3) goto 03
         pt=p
         goto(25,26,99,29,99,52,43,48,55,62,65,66),r-300
      endif
 04   continue
      call error(2)
c      if (err .gt. 0) return
      return
c     
c     put something on the stack
      
c     --- single number, getsym stored it in stk(vsiz)
c     
 10   call getnum
      if(err.gt.0) return
      psym=num
      call getsym
      go to 60
c     
c     --- string
c     
 15   call getstr
      if(err.gt.0) return
      call getsym
      go to 60
c     
c     --- matrix defined by bracket operators
c     
 20   continue
      if (eptover(0,psiz-3))  return
      pt=pt+1
      rstk(pt)=0
      pstk(pt)=0
c     
 21   continue
      pt=pt+1
      pstk(pt)=0
      call getsym
c     
 22   if (sym.eq.cmt) call parsecomment
      if (sym.eq.semi .or. sym.eq.eol .or.sym.eq.right) go to 27
      if (sym .eq. num .and.char1.eq.dot.and.
     $     lin(lpt(4)-2).ne.blank) then
c     .  to return an error on [1.000.3,...] 
         lpt(2)=lpt(4)+1
         call error(276)
         return
      endif

      if (sym .eq. comma) then
         call getsym
c        comment next line to disallow Matlab syntax like [1,2,,;3,4]  
         goto 22
      endif
 
      rstk(pt) = 301
c     get next entry or block
      icall=1
c     *call* expr
      return
c     catenate  entry or block with previous on the same row
 25   continue
      if(err1.ne.0) goto 22
      pstk(pt)=pstk(pt)+1
      if(pstk(pt).le.1) goto 22
      pstk(pt)=1
      rstk(pt)= 302
      fin=cconc
      rhs=2
      icall=4
c     *call* allops(cconc)
      return
 26   go to 22
 27   pt=pt-1
      if (sym.eq.semi .and. char1.eq.eol) call getsym
c     catenate row with previous rows
      if(err1.ne.0) goto 29
      if(pstk(pt+1).gt.0) pstk(pt)=pstk(pt)+1
      if(pstk(pt).le.1) goto 29
      pstk(pt)=1
      rstk(pt)= 304
      fin=rconc
      rhs=2
      icall=4
c     *call* allops(rconc)
      return
 29   if (sym .eq. eol) then
         if(comp(1).ne.0) call seteol
         if(lpt(4).eq.lpt(6))  then
            call getlin(0,0) 
         else
            lpt(4)=lpt(4)+1
            call getsym
         endif
      endif
      if (sym.eq.eol.and.err1.ne.0) then
         pt=pt-1 
         go to 60
      endif
      if (sym .ne. right) go to 21
      if (pstk(pt).le.0) then
         call defmat
         if(err.gt.0) return
      endif
      pt=pt-1
      call getsym
      go to 60
c     
c     --- named variable, function evaluation or matrix element   x(...)
c     
 30   call putid(id,syn(1))
      call getsym
      if (sym .eq. lparen) then
c     .     check for blank separator in matrix definition
         if(abs(lin(lpt(3)-2)).ne.blank.or.rstk(pt-2).ne.301) then
c     .     it is really x(....)
            dotsep=.false.
            goto 36
         endif
      endif
      if (sym.eq.dot.and.(abs(char1) .lt. blank.or.
     $     char1.eq.percen)) then
         dotsep=.true.
         goto 36
      endif
      fin=0
      rhs = 0
c     
c     -- put a named variable in the stack
c     check for indirect loading
      fin=setgetmode(id)
      call stackg(id)
      if (err .gt. 0) return
      if(fin.ne.0.or.err1.ne.0) goto 60
c     
      if(comp(1).eq.0) then
         fun1=fun
         call funs(id)
         if(err.gt.0) return
         if (fun .gt. 0) then
            call varfunptr(id,fun,fin)
            goto 60
         endif
c     this should never happen???
         fun=fun1
         fin=setgetmode(id)
         call stackg(id)
         if (err .gt. 0) return
         if (fin .eq. 0) then
            if(err1.ne.0) goto 60
            call  putid(ids(1,pt+1),id)
            call error(4)
            if (err .gt. 0) return
         endif
         go to 60
      endif
      
c     
 36   continue
c     --- function evaluation or variable element   x(...)
      if ( eptover(1,psiz-1))  return
c     x(...) :store object or function name

      rstk(pt)=0
      pstk(pt)=lhs
      call putid(ids(1,pt),id)
c     %% added for runtime set rhs args by list extraction (ss)
      fun1=fun
      fun=0
      if(id(1).ne.blank) then
         fin=-2
         call funs(id)
         if (fun .ne. 0) then
c     .     name is a function name, check if it is a call or a reference
         else
c     .     allow indirect reference to variables
            fun=-1
         endif
         lhs=1

c       %% next lines added  for runtime set rhs args number
         if(comp(1).ne.0) then
            if (compil(21,0,0,0,0)) then 
               if (err.gt.0) return
            endif
         endif
      else
         lhs=1
      endif


c     
c     eval function or variable arguments
 38   continue
      call getsym
      if(.not.dotsep.and.sym.eq.rparen) then
         if(char1.eq.dot.or.char1.eq.lparen) then
            call error(250)
            return
         endif
c     .  function has no input parameter
         if(rstk(pt).lt.0) then
c     .    a(...)()
            goto 461
         endif

         excnt=-1
         goto 45
      endif
c
      if(dotsep) then
         if (sym.ne.name) then
            call error(2)
            if (err .gt. 0) return
         endif
c     .  create a string variable containing name syn
         icount=icount+1
         if(comp(1).ne.0) then
            if(compil(23,syn,0,0,0)) then
               if(err.gt.0) return
            endif
         else
            call name2var(syn)
         endif
         dotsep=.false.
         excnt=excnt+1
         goto 45
      endif

      fun1=fun
      if(sym.eq.name.and.char1.eq.lparen) then
c     . check for a function name
         fun=0
         fin=-2
         call funs(syn)
         if (fun .ne. 0) then
c     .     name is a function name, check if it is a call or a reference

         else
            fun=fun1
         endif
      endif

      excnt = excnt+1
c
      if(sym.ne.comma) goto 40

c     args(, ..) or (.. ,, ..) or (.. ,) syntax
 39   if (nullarg) then 
c     .  default argument allowed
         if(comp(1).eq.0) then
            top=top+1
            call objvide(' ', top)
         else
            if(compil(24,0,0,0,0)) then
               if(err.gt.0) return
            endif
         endif
         if(sym.eq.rparen) then
c     .     (.. ,) syntax
            goto 45
         endif
         goto 44
      else
         lpt(2)=lpt(4)
         call error(46)
         return
      endif

 40   continue
      if(sym.ne.name.or.char1.ne.equal) goto 42
c     next lines to manage named arguments (..,a=..)

      fun=fun1
      lpt4=lpt(4)
      call fortrangetch
      if(char1.eq.equal) then
c     check for a==
         lpt(4)=lpt4
         goto 42
      endif
      if(ids(1,pt).eq.blank) then 
c     .  (x=2) syntax for a a factor ->(x==2)
         lpt(4)=lpt4
         char1=equal
         goto 42
      endif
c     it is really a named argument
      if ( eptover(1,psiz-1))  return
      ids(1,pt)=rhs
      ids(2,pt)=lhs
      ids(3,pt)=lct(4)
      ids(4,pt)=fun
      lct(4)=-1
      rstk(pt)=313
      lpt(4)=lpt(2)
      pstk(pt)=excnt
      char1=blank
      icall=7
      return
c     *call* parse
 41   continue
      rhs=ids(1,pt)
      lhs=ids(2,pt)
      lct(4)=ids(3,pt)
      excnt = pstk(pt)
      fun=ids(4,pt)
      pt = pt-1
c     end of special code to manage named argument
      goto 44
c
 42   continue
c     argument is a standard expression
      if ( eptover(1,psiz-1))  return
      pstk(pt) = excnt
      ids(1,pt)=fun
      ids(2,pt)=fun1
      rstk(pt) = 307
      icall=1
c     *call* expr
      return
 43   excnt = pstk(pt)
      fun=ids(2,pt)
      pt = pt-1
c
 44   continue
c     one more argument ?
      if (sym .eq. comma) then
         if (char1.eq.rparen) then
            call getsym
            excnt=excnt+1
            goto 39
         endif
         go to 38
      endif
c     end of argument sequence or recursive extraction ?
      if (sym .ne. rparen) then
         lpt(2)=lpt(3)+1
         call error(3)
         return
      endif
c     
 45   continue
c     end of argument sequence or recursive extraction 

      call getsym
      recurs=.false.
      if( sym .eq. dot.and.(abs(char1) .lt. blank.or.
     $     char1.eq.percen)) then
         dotsep=.true.
c     .  recursive extraction
         if(excnt.gt.1) then
            if(comp(1).eq.0) then
c     .     form  list with individual indexes
               call mkindx(0,excnt)
               if(err.gt.0) return
            else
               if (compil(19,0,excnt,0,0)) then 
                  if (err.gt.0) return
               endif
            endif
            excnt=1
            recurs=.true.
         endif
         if(excnt.ge.0) rstk(pt)=rstk(pt)-1
         excnt=0
c     .  get one more argument of the recursive extraction
         goto 38
      elseif(sym.eq.lparen) then
c        . check for blank separator in matrix definition
         if(abs(lin(lpt(3)-2)).eq.blank.and.rstk(pt-3).eq.301) then
c     .     end of argument sequence
            goto 46
         endif
c     .  recursive extraction
         if(excnt.gt.1) then
            if(comp(1).eq.0) then
c     .     form  list with individual indexes
               call mkindx(0,excnt)
               if(err.gt.0) return
            else
               if (compil(19,0,excnt,0,0)) then 
                  if (err.gt.0) return
               endif
            endif
            excnt=1
            recurs=.true.
         endif
         if(excnt.ge.0) rstk(pt)=rstk(pt)-1
         excnt=0
c     .  get one more argument of the recursive extraction
         goto 38
      elseif(rstk(pt).lt.0) then
c     .  all args of the recursive extraction have been computed
c     .  store them into a list
         if(comp(1).eq.0) then
c     .     form  list with individual indexes
            call mkindx(1-rstk(pt),excnt)
            if(err.gt.0) return
            excnt=1
            recurs=.true.
         else
            if (compil(19,1-rstk(pt),excnt,0,0)) then 
               if (err.gt.0) return
               excnt=1
            endif
         endif
      endif

 46   continue

c     all arguments evaluated
      call putid(id,ids(1,pt))
      lhs=pstk(pt)
      pt=pt-1
 
      
      if (id(1) .eq. blank) then
         if(lhs.ne.excnt) then
            call error(41)
            if(err.gt.0) return
         endif
         if (recurs) then
            call error(250)
            return
         endif
         go to 60
      endif
c     skip empty argument list portion
      goto 463


c     empty argument list   a(...)()
 461  excnt=excnt+1
      call getsym
c      lhs=pstk(pt)
      call putid(id,ids(1,pt))
      if ( eptover(1,psiz-1))  return
      rstk(pt)=314
c      pstk(pt)=lhs
      pstk(pt)=pstk(pt-1)

 463  rhs = excnt
c     get function or variable to be evaluated for computed arguments
      fin=0
      fin=-2
      call stackg(id)
      if(err.gt.0) return
      if(comp(1).ne.0) goto 47
      if(fin.ge.0) then
c     .  id is not a standard variable
         if (recurs) then
            call error(250)
            return
         endif
         if(err1.gt.0) goto 60
C next line added serge 06/08/08: it is not useful to scan funtab
         if(fin.gt.0) fin=-4
         call funs(id)
         if(err.gt.0) return
         if(fun.gt.0)  goto 53
         fin=-2
         call stackg(id)
         if(err.gt.0) return
         if(fin.eq.0) then
            if(err1.gt.0) goto 60
            call error(4)
            if(err.gt.0) return
         endif
      endif
      if(fin.gt.0) goto 50
      if(rstk(pt+1).ge.0.and.rhs.eq.0) goto 60

      call isafunptr(top,id,ifun,ifin)
 
      if(ifun.ne.0) then
         top=top-1
         fun=ifun
         fin=ifin
         goto 53
      endif
c     
c     --- variable is a matrix or list :extraction
c     
 47   rhs=rhs+1
      if ( eptover(1,psiz-1)) return
      rstk(pt)=308
      fin=extrac
      icall=4
c     *call* allops(extrac)
      return
 48   pt=pt-1
      if(rstk(pt).eq.314) then
c     .  a(...)()
c     .  a(...) has been put on the top of the stack
         lhs=pstk(pt)
         pt=pt-2
         rhs=0
         call isafunptr(top,id,ifun,ifin)
         if(ifun.ne.0) then
c     .     a(...) is a funpr
            top=top-1
            fun=ifun
            fin=ifin
            goto 53
         endif

         il=iadr(lstk(top))
         if (istk(il).eq.-13.or.istk(il).eq.-11) then
c     .    a(...) is a reference to a macro
            fin=istk(il+1)
            top=top-1
            goto 50
         endif
c     next if for bug 6730 fix
         if (istk(il).eq.13.or.istk(il).eq.11) then
c     .    a(...) is a macro 
c     .     use negative value to indicate that this variable must be
c     .     removed after the function has been evaluated
            fin=-top
            goto 50
         endif

      endif
      goto 60
c     
c     --- variable is macro : execution
c     
 50   if ( eptover(1,psiz-1)) return
c     next if for bug 6730 fix
c      call putid(id,ids(1,pt))
      if(fin.lt.0) then
         ids(1,pt)=top
         fin=lstk(top)
      else
         ids(1,pt)=0
      endif
      rstk(pt)=306
      pstk(pt)=wmac
      icall=5
c     *call* macro
      return
 52   wmac=pstk(pt)
c     next if for bug 6730 fix
      if(ids(1,pt).ne.0) then
c     .  shift storage up to remove the macro
         mtop=ids(1,pt)
         v=lstk(mtop+1)-lstk(mtop)
         call dcopy(lstk(top+1)-lstk(mtop+1),stk(lstk(mtop+1)),1
     $        ,stk(lstk(mtop)),1) 
         do  ib = mtop,top-1
            call putid(idstk(1,ib),idstk(1,ib+1))
            infstk(ib)=infstk(ib+1)
            lstk(ib+1) = lstk(ib+2)-v
         enddo
         top=top-1
      endif
      pt=pt-1
      go to 60
c     
c     evaluate function
 53   if ( eptover(1,psiz-1))  return
      rstk(pt) = 309
      icall=9
c     *call* matfns
      return
 55   pt = pt-1
      go to 60
c     
 60   continue
c     check for ', .'  **,  ^ and .^

      if (sym .ne. quote) go to 63
      if (lin(lpt(3)-1).ne.quote) then
c     .  not to admit " as a transposition operator 
C     .  (remove the equivalence ' ")
         go to 63
      endif
      i = lpt(3) - 2
      if (abs(lin(i)) .eq. blank) go to 90
      fin=quote
 61   rhs=1
      if ( eptover(1,psiz-1))  return
      rstk(pt)=310
      icall=4
c     *call* allops(quote) or allops(dot+quote)
      return
 62   pt=pt-1
      psym=sym
      call getsym

 63   continue
      if(sym.eq.hat) then
         op=dstar
      elseif(sym.eq.dot.and.char1.eq.hat) then
         call getsym
         op=dstar+dot
      elseif(sym.eq.star.and.char1.eq.star) then
         call getsym
         op=dstar
      elseif(sym.eq.dot.and.char1.eq.quote) then
         call getsym
         fin=dot+quote
         goto 61
      elseif(sym.eq.not.and.char1.ne.equal) then
         call error(276)
         return
      elseif(sym.eq.name.and.psym.eq.num) then
         i = lpt(3) - 2
         if (abs(lin(i)) .ne. blank) then
            ok=.false.
c     .     next line to restrict to mfile2sci compilation only
c     .     ok=(comp(1).ne.0.and.comp(3).eq.2) 
            if (ok) then
c     .        make 2m be evaluated as 2*m
               op=star
               goto 64
            else
c     .        issue an error
               lpt(2)=lpt(3)+1
               call error(276)
               if (err.gt.0) return
            endif
         endif
         goto 90
      elseif (sym.eq.num.and.abs(lin(lpt(3) - 2)) .ne. blank) then 
c     .   issue an error
         lpt(2)=lpt(3)+1
         call error(276)
         if (err.gt.0) return
      else
         goto 90
      endif
      call getsym
 64   if ( eptover(1,psiz-1))  return
      rstk(pt) = 311
      pstk(pt) = op
      icall=3
c     *call* factor
      go to 01
 65   rstk(pt)=312
      fin=pstk(pt)
      rhs=2
      icall=4
c     *call* allops(dstar)
      return
 66   pt=pt-1
      goto 90

 90   continue
c     check for unary minus
      if (rstk(pt).ne.303) return
      ids(1,pt) = rhs
      ids(2,pt) = lhs
      pstk(pt) = fin
      fin = minus
      icall = 4
      rhs=1
      return
c     *call* allops(minus) 
 91   continue
      rhs = ids(1,pt)
      lhs = ids(2,pt)
      fin = pstk(pt)
      pt=pt-1
      return
c     
 99   call error(22)
      if (err .gt. 0) return
      return
      end

      subroutine name2var(id)
c     Copyright INRIA
      include 'stack.h'
c     given a variable name code in id, creates a string variable 
c     on the top of the stack
      integer id(nsiz)
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (err1.gt.0) return
      top=top+1
      il=iadr(lstk(top))
      err=sadr(il+6+nlgh)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      ilp=il+4
      istk(ilp)=1
      call namstr(id,istk(ilp+2),n,1)
      istk(ilp+1)=n+1
      lstk(top+1)=sadr(ilp+2+n)
      return
      end
