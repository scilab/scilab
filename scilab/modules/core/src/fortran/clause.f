c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine clause
c     ======================================================================
c     gestion des structures de controle
c     ======================================================================
      include 'stack.h'
      integer while(nsiz),iff(nsiz),else(nsiz),ennd(nsiz)
      integer do(nsiz),thenn(nsiz),cas(nsiz),sel(nsiz)
      integer elsif(nsiz)
      integer semi,equal,eol,blank,comma,name,cmt
      integer lparen,rparen
      integer r,r1
      logical eqid,istrue,ok,first,eptover
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      parameter (iif=1,iwhile=2,iselect=3)
      data semi/43/,equal/50/,eol/99/,blank/40/
      data comma/52/,name/1/,cmt/2/
      data lparen/41/,rparen/42/
      data do/673716237,nz1*673720360/, else/236721422,nz1*673720360/
      data ennd/671946510,nz1*673720360/
      data iff/673713938,nz1*673720360/
      data thenn/386797853,nz1*673720360/
      data while/353505568,673720334,nz2*673720360/
      data cas/236718604,nz1*673720360/
      data sel/236260892,673717516,nz2*673720360/
      data elsif/236721422,673713938,nz2*673720360/
      



c     
      r = -fin-10
      fin = 0
      r1=0
      if(pt.gt.0) r1=rstk(pt)
c     
      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') pt,r1,r
         call basout(io,wte,' clause pt:'//buf(1:4)//' rstk(pt):'//
     &        buf(5:8)//' -fin-10:'//buf(9:12))
      endif
c     
      if(r.le.0.and.pt.le.0) then
         call error(34)
         return
      endif
      go to (02,30,30,55,30,55,55,70,80),r
c     
      r = rstk(pt)
      ir=r/100
      if(ir.ne.8) goto 99
      goto(05,15,40,45,55,65,46,75),r-800
      goto 99
c     
c     for
c     
 02   call getsym
      if ( eptover(2,psiz))  return
      pstk(pt-1)=0
      rstk(pt-1)=0
      if (sym .ne. name) then
         if (sym. eq. lparen) then
c     .  for matlab compatiblity  for (k=1:n)
            call getsym
            pstk(pt-1)=1
         else
            call error(34)
            return
         endif
      endif
      call putid(ids(1,pt),syn)
      call getsym
      if (sym .ne. equal) then
         call error(34)
         return
      endif
      call getsym
      if(comp(1).ne.0) then
         rstk(pt) = 800
         call compcl(0)
         if(err.gt.0) return
      endif
      rstk(pt) = 801
      icall=1
c     *call* expr
      return
 05   continue
      if(err1.gt.0) goto 20
      if(comp(1).ne.0) call compcl(0)
      if(err.gt.0) return
      if (pstk(pt-1).eq.1) then
c     .  for matlab compatiblity: for (k=1:n)
         if (sym. eq. rparen) then
            call getsym
         else
            call error(3)
            return
         endif
      endif
      pstk(pt-1) = 0
      ids(1,pt-1)=top
      if (eqid(syn,do)) then
         sym=comma
         if(char1.eq.eol) call getsym
      endif
      if(sym.eq.comma.or.sym.eq.semi) then
         sym = semi
         pstk(pt) = lpt(4) - 1
      elseif( sym.eq.cmt) then
         call parsecomment
         sym=semi
         pstk(pt) = lpt(4)
      elseif( sym.eq.eol) then
         sym=semi
         pstk(pt) = lpt(4)
      elseif(lin(lpt(3)-2).eq.blank) then
         sym=semi
         lpt(4)=lpt(3)-1
         pstk(pt) = lpt(4)
         char1=blank
      else
         call error(34)
         return
      endif
c     on recherche le "end" pour s'assurer que toutes les lignes relatives 
c     sont chargee (pb des matrices sur plusieurs lignes)
      call skpins(1)
      if(err.gt.0) return
      first=.true.
 10   if(top.ne.ids(1,pt-1)) then 
         call error(115)
         return
      endif
      call nextj(ids(1,pt),pstk(pt-1))
      if(err.gt.0) return
      if(pstk(pt-1).eq.0) goto  20
      first=.false.
      lpt(4) = pstk(pt)
      char1 = blank
      rstk(pt) = 802
      icall=7
c     *call* parse
      return
 15   continue
      if(err1.gt.0.and.catch.eq.0) then
        
c     .  remove variable associated to the expression and skip to the end
         top=ids(1,pt-1)-1
         goto 20
      endif
      if(comp(1).eq.0) goto 10
      call compcl(0)
      if(err.gt.0) return
c     
c     fin for
 20   continue
      pt = pt-2
      icall=7
c      char1 = blank
      return
c     
c     while  if  select/case or if/elseif
c   
 30   if ( eptover(1,psiz)) return
      if (eqid(syn,while))  then
         ids(1,pt)=iwhile
      elseif(eqid(syn,sel))  then
         ids(1,pt)=iselect
      else
         ids(1,pt)=iif
      endif
      if(char1.eq.eol.or.char1.eq.comma.or.char1.eq.semi) then
         call error(34)
         return
      endif
C      call putid(ids(1,pt),syn)

      pstk(pt) = lpt(4)-1

      if(ids(1,pt).eq.iwhile) then
c     .  while, look for the end to be sure all lines are loaded
         call skpins(1)
         if(err.gt.0) return
      endif
 35   lpt(4) = pstk(pt)
      pstk(pt)=lpt(4)
      char1 = blank
      call getsym
      rstk(pt)=803
      if(comp(1).ne.0) call compcl(0)
      if(err.gt.0) return
      goto 37
 36   rstk(pt) = 803
      call getsym
 37   icall=1
c     *call* expr
      return
 40   if (ids(1,pt).ne.iselect) goto 46
c     select expression evaluated
 41   continue
c     skip following commas or semi columns if any
      if(sym.eq.comma.or.sym.eq.semi) then
         call getsym
         goto 41
      endif
c     end of line reached?
      if(sym.eq.eol.or.sym.eq.cmt) then
         if(sym.eq.cmt) call parsecomment
         if(macr.gt.0.and.lin(lpt(4)+1).eq.eol) then
            call error(47)
            return
         endif
         if(comp(1).ne.0) call seteol()
c    .    get the following line
         if(lpt(4).eq.lpt(6))  then
            call getlin(1,0)
         else
            lpt(4)=lpt(4)+1
            char1=blank
         endif
         call getsym
         goto 41
      endif
c     looking for the "case" keyword

      if(sym.eq.name) then
         if (eqid(syn,cas)) then
            rstk(pt)=807
            if(comp(1).ne.0) then
               call compcl(0)
               if(err.gt.0) return
            endif
         else
            call error(35)
            return
         endif    
      elseif(lin(lpt(3)-2).eq.blank) then
         sym=semi
         lpt(4)=lpt(3)-1
         char1=blank
         goto 41
      else
         call error(35)
         return
      endif   

 42   if(comp(1).eq.0) then
c     recopie de la premiere expression
         l=lstk(top)
         l1=lstk(top+1)
         if(top+2.ge.bot) then
            call error(18)
            if(err.gt.0) return
         endif
         err=lstk(top+1)+l1-l-lstk(bot)
         if(err.gt.0) then
            call error(17)
            if(err.gt.0) return
         endif
         call unsfdcopy(l1-l,stk(l),1,stk(l1),1)
         top=top+1
         lstk(top+1)=lstk(top)+l1-l
      endif
 43   call getsym
      rstk(pt) = 804
      icall=1
c     *call* expr
      return
 45   continue
      if(comp(2).ne.0) goto 46
      rstk(pt)=807
      fin = equal
      rhs=2
      icall=4
c     *call* allops(==)
      return
 46   if (eqid(syn,do) .or. eqid(syn,thenn)
     &     .or.sym.eq.comma.or.sym.eq.semi.or.sym.eq.eol) then
         sym = semi
      elseif(sym.eq.cmt) then
         call parsecomment
         sym = semi
      elseif(lin(lpt(3)-2).eq.blank) then
c     .  added for matlab compatibility (then or , is not mandatory)
         sym=semi
         lpt(4)=lpt(3)-1
         char1=blank
      else
         call error(35)
         return
      endif
      if(comp(1).ne.0) goto 48
c     comparaison ...
      ok=istrue(1)
      if(err.gt.0) return
      if(ok) then
         goto 50
      else
         call skpins(0)
         if(err.gt.0) return
         if(eqid(syn,else)) goto 60
         if(eqid(syn,elsif)) then
            if(ids(1,pt).ne.iif) then
               call error(34)
               return
            endif
            goto 36
         endif
         if(eqid(syn,cas)) then
            if(ids(1,pt).ne.iselect) then
               call error(34)
               return
            endif
            goto 42
         endif
         if(eqid(syn,ennd)) goto 66
      endif
 48   rstk(pt)=804
      call  compcl(0)
      if(err.gt.0) return
c     
c     then
c     --------
 50   toperr=top
      rstk(pt) = 805
      icall=7
c     *call* parse
      return
 55   continue
      if(rstk(pt).ne. 805) then
         call error(34)
         return
      endif
      if(comp(1).eq.0) then
         if (ids(1,pt).eq.iwhile) go to 35
         if(.not.eqid(syn,ennd)) then
            call skpins(1)
            if(err.gt.0) return
         endif
      else
         call compcl(0)
         if(err.gt.0) return
         if(eqid(syn,else)) goto 60
         if(eqid(syn,elsif)) goto 36
         if(eqid(syn,cas))  goto 43
      endif
      goto 66
c     
c     else
c     ---------
 60   rstk(pt) = 806
      icall=7
c     *call parse*
      return
 65   if(comp(1).ne.0) then
         call compcl(0)
         if(err.gt.0) return
      endif
      goto 66
c     
c     fin if ou while ou select
c------------------------------
 66   if(ids(1,pt).eq.iselect.and.comp(1).eq.0) top=top-1
      pt=pt-1
      icall=7
      return
c     
c     try
c---------
 70   continue
      if ( eptover(1,psiz)) return
      rstk(pt)=808
      if(comp(1).ne.0) then
         ids(1,pt)=0
         call compcl(1)
         if(err.gt.0) return
      else
c     .  set error control mode
         pstk(pt)=top
         ids(2,pt)=errct
         ids(3,pt)=err2
         ids(4,pt)=err1
         ids(5,pt)=errpt
         ids(6,pt)=(lct(4)+100)+10000*sym
         errpt=pt
         ids(1,pt)=1
         imode=1
         imess=1
         errct=-((8*imess+imode)*100000+1)
      endif
      
      ic=char1
      isym=sym
      call getsym
c     end of line reached?
      if(sym.eq.eol.or.sym.eq.cmt) then
         if(sym.eq.cmt) call parsecomment
         if(comp(1).ne.0) call seteol()
c    .    get the following line
         if(lpt(4).eq.lpt(6))  then
            call getlin(1,0)
         else
            lpt(4)=lpt(4)+1
            char1=blank
         endif
      else
c     ones symbol back
         lpt(4)=lpt(3)
         sym=isym
         char1=ic
      endif
      icall=7

c     *call* parse (for the try instructions)
      return
 75   continue

c     end of try reached or an error occurred
      if (max(err2,err1).gt.0) then
c     .  an error occured in the try part 
c     .  skip remaining instructions up to catch or end keywords
         errct=-1
         call skpins(0)
         errct=ids(2,pt)
         err2=ids(3,pt)
         err1=ids(4,pt)
         errpt=ids(5,pt)
         sym=ids(6,pt)/10000
         lct(4)=ids(6,pt)-10000*sym-100
         top=toperr
         if(eqid(syn,ennd)) then
c     .     no catch keyword  "try catch" finished
            goto 76
         endif
c     .  execute catch  instructions
         icall=7
c     * call* parse
         return
      else
c     .  end of try without error (or end of catch)
         if(comp(1).eq.0) then
            errct=ids(2,pt)
            err2=ids(3,pt)
            err1=ids(4,pt)
            errpt=ids(5,pt)
            sym=ids(6,pt)/10000
            lct(4)=ids(6,pt)-10000*sym-100
         else
            call compcl(3)
            if(err.gt.0) return
         endif
      endif
 76   pt=pt-1
      icall=7
c     *call* parse
      return

c     
c     catch
c---------
 80   continue
      if(rstk(pt).ne.808) then
         call error(34)
         return
      endif
      if(comp(1).ne.0) then
         call compcl(2)
         if(err.gt.0) return
         icall=7
c     *call* parse
         return
      endif

C       if (max(err2,err1).gt.0) then
C c     .  an error occured in the try part, execute next instructions
C          errct=ids(2,pt)
C          err2=ids(3,pt)
C          err1=ids(4,pt)
C          errpt=ids(5,pt)
C          icall=7

C c     *call* parse
C          return
C       else

c     .  no error occured in the try part, skip next instructions
         if(.not.eqid(syn,ennd)) then
            call skpins(1)
            if(err.gt.0) return
            goto 75
         endif
C       endif

c
 99   call error(34)
      if (err .gt. 0) return
      return
      end
