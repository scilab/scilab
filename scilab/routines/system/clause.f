      subroutine clause
c     ======================================================================
c     gestion des structures de controle
c     ======================================================================
c     Copyright INRIA
      include '../stack.h'
      integer while(nsiz),iff(nsiz),else(nsiz),ennd(nsiz)
      integer do(nsiz),thenn(nsiz),cas(nsiz),sel(nsiz)
      integer elsif(nsiz)
      integer semi,equal,eol,blank,comma,name
      integer r,r1
      logical eqid,istrue,ok,first,eptover
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      data semi/43/,equal/50/,eol/99/,blank/40/
      data comma/52/,name/1/
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
      go to (02,30,30,55,30,55,55),r
c     
 01   r = rstk(pt)
      ir=r/100
      if(ir.ne.8) goto 99
      goto(05,15,40,45,55,65,46),r-800
      goto 99
c     
c     for
c     
 02   call getsym
      if (sym .ne. name) then
         call error(34)
         return
      endif
      rstk(pt+1)=0
      if ( eptover(2,psiz))  return
      call putid(ids(1,pt),syn)
      call getsym
      if (sym .ne. equal) then
         call error(34)
         return
      endif
      call getsym
      if(comp(1).ne.0) then
         rstk(pt) = 800
         call compcl
         if(err.gt.0) return
      endif
      ids(4,pt-1) = toperr
      rstk(pt) = 801
      icall=1
c     *call* expr
      return
 05   if(comp(1).ne.0) call compcl
      if(err.gt.0) return
      toperr=top
      pstk(pt-1) = 0
      ids(1,pt-1)=top
      if (eqid(syn,do)) then
         sym=comma
         if(char1.eq.eol) call getsym
      endif
      if(sym.eq.comma.or.sym.eq.semi) then
         sym = semi
         pstk(pt) = lpt(4) - 1
      elseif( sym.eq.eol) then
         sym=semi
         pstk(pt) = lpt(4)
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
 15   if(comp(1).eq.0) goto 10
      call compcl
      if(err.gt.0) return
c     
c     fin for
 20   continue
      toperr = ids(4,pt-1)
      pt = pt-2
      icall=7
      char1 = blank
      return
c     
c     while  if  select/case or if/elseif
c   
 30   if ( eptover(1,psiz)) return
      call putid(ids(1,pt),syn)
      pstk(pt) = lpt(4)-1
      if(eqid(while,ids(1,pt))) then
c     on recherche le "end" pour s'assurer que toutes les lignes relatives 
c     sont chargee (pb des matrices sur plusieurs lignes)
         call skpins(1)
         if(err.gt.0) return
      endif
 35   lpt(4) = pstk(pt)
      pstk(pt)=lpt(4)
      char1 = blank
      call getsym
      rstk(pt)=803
      if(comp(1).ne.0) call compcl
      if(err.gt.0) return
      goto 37
 36   rstk(pt) = 803
      call getsym
 37   icall=1
c     *call* expr
      return
 40   if (.not.eqid(ids(1,pt),sel)) goto 46
 41   continue
      if(sym.eq.comma.or.sym.eq.semi) then
         call getsym
         goto 41
      elseif(sym.eq.eol) then
         if(macr.gt.0.and.lin(lpt(4)+1).eq.eol) then
            call error(47)
            return
         endif
         if(comp(1).ne.0) call seteol()
c     get the following line
         if(lpt(4).eq.lpt(6))  then
            call getlin(1)
         else
            lpt(4)=lpt(4)+1
            char1=blank
         endif
         call getsym
         goto 41
      elseif(sym.eq.name.and.eqid(syn,cas)) then
         rstk(pt)=807
         if(comp(1).ne.0) then
            call compcl
            if(err.gt.0) return
         endif
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
            if(.not.eqid(iff,ids(1,pt))) then
               call error(34)
               return
            endif
            goto 36
         endif
         if(eqid(syn,cas)) then
            if(.not.eqid(sel,ids(1,pt))) then
               call error(34)
               return
            endif
            goto 42
         endif
         if(eqid(syn,ennd)) goto 66
      endif
 48   rstk(pt)=804
      call  compcl
      if(err.gt.0) return
c     
c     then
c     --------
 50   toperr=top
      rstk(pt) = 805
      icall=7
c     *call* parse
      return
 55   if(comp(1).eq.0) then
         if (eqid(ids(1,pt),while)) go to 35
         if(.not.eqid(syn,ennd)) then
            call skpins(1)
            if(err.gt.0) return
         endif
      else
         call compcl
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
         call compcl
         if(err.gt.0) return
      endif
      goto 66
c     
c     fin if ou while ou select
c------------------------------
 66   if(eqid(ids(1,pt),sel).and.comp(1).eq.0) top=top-1
      pt=pt-1
      icall=7
      return
c
 99   call error(22)
      if (err .gt. 0) return
      return
      end
