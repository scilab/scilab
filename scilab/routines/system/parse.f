      subroutine parse
c     ====================================================================
c     Scilab parsing function
c     ====================================================================
c     Copyright INRIA
      include '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2,nz3=nsiz-3)
      logical compil,eptover,eqid
c     
      logical iflag
      common /basbrk/ iflag
      integer semi,equal,eol,lparen,rparen,colon,dot
      integer blank,comma,left,right,less,great,not
      integer quote,percen
      integer name,num,insert,extrac
c     
      integer id(nsiz),ans(nsiz),varargout(nsiz)
      integer pts,psym,excnt,p,r,topk,where
      integer pcount,strcnt,bcount,qcount,pchar,schar
      logical dotsep,found
      character*50 tmp
c
      integer otimer,ntimer,stimer,ismenu
      external stimer,ismenu
      save otimer
      data otimer/0/
c     
      data blank/40/,semi/43/,equal/50/,eol/99/,comma/52/,colon/44/
      data lparen/41/,rparen/42/,left/54/,right/55/,less/59/,great/60/
      data not/61/
      data quote/53/,percen/56/,dot/51/
      data name/1/,num/0/,insert/2/,extrac/3/
      data ans/672929546,nz1*673720360/
      data varargout/169544223,504893467,673720349,nz3*673720360/
c     
      save job

c
      itime = 10000
      call checkevts(inxsci)
 01   r = 0
      if(pt.gt.0) r=rstk(pt)
      if(ddt.eq.4) then
         write(tmp(1:18),'(2i4,2i2,i6)') pt,r,icall,niv,err
         call basout(io,wte,' TOP    pt:'//tmp(1:4)//' rstk(pt):'
     +        //tmp(5:8)//' icall:'//tmp(9:10)//' niv:'//tmp(11:12)
     +        //' err:'//tmp(13:18))
      endif
c     
      if(icall.eq.5) goto 88
      if(pt.gt.0) goto 86
      if(err.gt.0) goto 98

c     initialization
c-------------------
 05   sym = eol
      job=0
      if(rio.eq.-1) job=-1
      top = 0
      fin=0
      macr=0
      paus=0
      icall=0
      rio = rte
      lct(3) = 0
      lct(4) = 2
      lpt(1) = 1
      
      if(job.eq.-1) goto 13
c     
 10   if(sym.ne.eol) goto 15
      if(comp(1).eq.0) goto 12
      if(lin(lpt(4)+1).eq.eol) goto 88
c     
c     get a new line
c-------------------
 12   continue
      if(lct(4).le.-10) then 
         lct(4)=-lct(4)-11      
      else
         if (mod(lct(4)/2,2).eq.1) then
            call prompt(lct(4)/4,iesc)
            if(iesc.eq.1) then
c     .        interrupted line acquisition (mode=7)
               iret=3
               goto 96
            endif
            lct(1)=0
            if(paus.eq.0.and.rio.eq.rte.and.macr.eq.0) then
               if(pt.ne.0) then
                  call msgs(30,0)
                  pt=0
               endif
               if(top.ne.0) then
                  call msgs(31,0)
                  top=0
               endif
            endif
         endif
      endif
 13   continue
      call tksynchro(paus)
      call getlin(job,1)
      call tksynchro(-1)
      if(fin .eq. -3) then
c     interrupted line acquisition
         iret=2
         goto 96
      elseif(fin .eq. -1) then
c     Continuation line handling when scilab is called as a routine
         fun=99
         return
      endif
      job=0
      err = 0
c
      if(pt.ne.0) goto 15
 14   call handleonprompt(where)
      if(err.gt.0) goto 98
      goto(85,88) where


c     Beginning of a new statement, clause expression or command
c------------------------------------------------------------
 15   continue
      if (inxsci.eq.1) then
         ntimer=stimer()/itime
         if (ntimer.ne.otimer) then
            call sxevents()
            otimer=ntimer
            if(ismenu().eq.1) then
               iret=1
               goto 96
            endif
         endif
      endif
      r = 0
      if(pt.gt.0) r=rstk(pt)
      if(ddt.eq.4) then
         write(tmp(1:20),'(3i4,i2,i6)') pt,r,top,niv,err
         call basout(io,wte,' parse  pt:'//tmp(1:4)//' rstk(pt):'
     +        //tmp(5:8)//' top:'//tmp(9:12)//' niv:'//tmp(13:14)
     +        //' err:'//tmp(15:20))
      endif
c     
      excnt = 0
      if(.not.iflag) goto 18

c     Handling of pauses
 16   if ( eptover(1,psiz))  goto 98
      pstk(pt)=rio
      ids(2,pt)=top
      rio=rte
      rstk(pt)=701
      iflag=.false.
      fin=2
      if(lct(4).le.-10) then
         fin=-1
         lct(4)=-lct(4)-11 
      endif
c     *call* macro
      goto 88
c     pauses termination
 17   rio=pstk(pt)
      top=ids(2,pt)
      pt=pt-1
      goto 15

 18   lhs = 1
      excnt=0
      call putid(id,ans)
c     preserve current character position 
      lpts = lpt(4)-1
      pts =  pt
c     
      call getsym
      if (sym.eq.right.or.sym.eq.rparen.or.
     $     sym.eq.less.or.sym.eq.great) then 
         call error(1)
         goto 98
      elseif (sym.eq.semi .or. sym.eq.comma .or. sym.eq.eol) then
         goto 77
      elseif (sym.eq.name) then
         lpts = lpt(3)-1
c     .  try to avoid the comand call whenever it is possible
         if (char1 .eq. equal) goto 25
c         if (char1 .eq. lparen) then
c           one can get this case with "if ( ) then"
c         endif
         if(lpt(4).ge.2) then
            if (lin(lpt(4)-2).eq.blank) goto 20
         endif
         if (char1.eq.dot) then
c     .     name.x
            schar=char1
            goto 30
         endif
         goto 20
      elseif (sym.eq.left) then
c     .  is there an explicit affectation
         lpts = lpt(3)-1
         lpt(4)=lpts
         lpt(3)=lpts
         char1=blank
         call findequal(found)
         if(err.gt.0) goto 98
         if(found) then
c     .     multiple lhs
            goto 40
         else
c     .     no ==> implicit lhs
            goto 50
         endif
      else
c     .  not lhs defined
c     .  set ans for lhs      
         if ( eptover(1,psiz))  goto 98
         call putid(ids(1,pt),ans)
         lhs=1
         pstk(pt) = 0
c     .  go to rhs analysis
         goto 60
      endif
c     
c     lhs begins with name
c-------------------------
c     check if it is a simple command like clear,...
 20   call comand(syn,0)
      if (err .gt. 0) goto 98
      if (fin.eq.0) goto 21

      if (fun .eq. 99) return
      if (fun .ne. 0) goto 93
      if (fin .lt. 0) goto 80
      if (fin .eq. 2) goto 88
      if (fin .eq. 3) goto 16
      if (fin .eq. 4) goto 05
      if (fin .gt. 0) goto 77

c     name is not a command
 21   rhs = 0
      fin=-5
c IL y a p avec fin=-5 (on ne trouve pas les macros parce que l'on ne
c veut pas que les macros sans arg soient vues comme des commandes
c mais pourquoi pas il suffirait de dire que pour visualiser une macro
c il faut faire disp()
      fun=0
      call funs(syn)
      if (fin .gt. 0) then
c        name is a builtin name
         if(char1.eq.equal) then
c           fun=expr is not allowed
            call putid(ids(1,pt+1),syn)
            call error(25)
            goto 98
         endif
c        skip lhs analysis
         if ( eptover(1,psiz))  goto 98
         call putid(ids(1,pt),id)
         lhs=1
         pstk(pt)=0
c     .  go to rhs analysis
         goto 60
      endif
c     
c     peek one character ahead
      if (char1.eq.semi .or. char1.eq.comma .or. char1.eq.eol)
     $     call putid(id,syn)
      if (char1 .eq. lparen) then
         schar=char1
         goto 30
      endif
c     instruction is just "name", skip lhs analysis
c     record name as lhs
      if ( eptover(1,psiz))  goto 98
      call putid(ids(1,pt),id)
      lhs=1
      pstk(pt)=0
c     go to rhs analysis
      goto 60
c     
c     name = expr  or name == expr syntax
c----------------------------------------
 25   call putid(id,syn)
      lpts=lpt(2)
      call getsym
      if(char1.eq.equal) then 
c     .  name == expr syntax ==> implicit lhs 
         goto 50
      else
c     .  lhs found
c     .  record it
         if ( eptover(1,psiz))  goto 98
         call putid(ids(1,pt),id)
         lhs=1
         pstk(pt)=0
         call getsym
c     .  go to rhs analysis
         goto 60
      endif
c     
c     lhs is name(...) or name.x...
c-----------------------------------
 30   lpt(5) = lpt(4)
      call putid(id,syn)
c     
c     looking for equal to check if it is really an lhs
      call findequal(found)
      if(err.gt.0) goto 98
      if(.not.found) goto 50
c     It is really a lhs (insertion syntax)


 32   continue
      lpt(4)=lpt(5)
      char1=schar
c
c35     call parseindexlist(excnt)
c     if(err.gt.0) goto 98

      if (compil(21,0,0,0,0)) then 
         if (err.gt.0) return
      endif

c     begin the index lists
      icount=0

      call getsym
c
 33   continue
c     begin a new index list (.,..) or .name
      icount=icount+1
c
      dotsep=sym.eq.dot
      call getsym
      if(dotsep) then
c     .  --> new index list is .name
         if(sym.ne.name) then
            call error(21)
            if(err.gt.0) return
         endif
c     
         if(comp(1).ne.0) then
            if(compil(23,syn,0,0,0)) then
               if(err.gt.0) return
            endif
         else
            call name2var(syn)
         endif
         call getsym
c         icount=icount+1
         if (sym .eq. dot) goto 33
         dotsep=.false.
         excnt=1
         goto 36
      endif

c     --> new index list is (.,..)
 34   continue
c     add a new index in index list (i,...)
      excnt = excnt+1

      if ( eptover(1,psiz)) goto 98
      call putid(ids(1,pt), id)
      pstk(pt) = excnt+1000*icount
      rstk(pt) = 702
c     *call* expr
      goto 81
 35   continue

      call putid(id,ids(1,pt))
      icount=int(pstk(pt)/1000)
      excnt = pstk(pt)-1000*icount
      pt = pt-1
c
      if (sym .eq. comma) then
c     .  current syntax is (i,j,..)
         call getsym
         goto 34
      endif
c
      if (sym .eq. rparen) then
c     .  end of the current index list
         call getsym
      else
         call error(3)
         if (err .gt. 0) goto 98
      endif
c
 36   if(sym.eq.lparen.or.sym.eq.dot) then
c     .  begining of a a new index list 

c     . first memorize the previous one
         if(excnt.gt.1) then
c     .     previously analysed syntax is (i,j,..)(
            if(comp(1).eq.0) then
c     .     form  list with individual indexes i,j,..
               call mkindx(0,excnt)
               if(err.gt.0) return
            else
               if (compil(19,0,excnt,0,0)) then 
                  if (err.gt.0) return
               endif
            endif
            excnt=1
         endif

c     .  open a new index list
         excnt=0
c         icount=icount+1
         goto 33
      endif

c     end of all the index lists
      if(icount.gt.1) then
c     .  form  list with individual indexes
         if(comp(1).eq.0) then
            call mkindx(icount,excnt)
            if(err.gt.0) return
         else
            if (compil(19,icount,excnt,0,0)) then 
               if (err.gt.0) return
            endif
         endif
         excnt=1
      endif
c     end of code for recursive index

      if(rstk(pt).eq.711) goto 42
c     
      if(sym.eq.equal) then
c     .  name(...) = expr syntax 
c     .  record name as lhs
         if ( eptover(1,psiz)) goto 98
         call putid(ids(1,pt),id)
         pstk(pt) = excnt
         lhs=1
         call getsym
c     .  go to rhs analysis
         goto 60
      else
c     . this should never happen. this case has been detected above 
c     . when lookin for equal sign
c     .  name(...) syntax ==> really an rhs
         if ( eptover(1,psiz))  goto 98
         call putid(ids(1,pt),ans)
         lhs=1
         pstk(pt) = 0
         goto 60
      endif

c     
c     multiple lhs [a,...]= or [a(..),..]
c----------------------------------------
 40   lpt(4)= lpts
      lpt(3)= lpts
      char1 = blank
      lhs   = 0
      call getsym
c     start lhs arguments list
      call getsym


 41   continue
c     begin analysis of a new lhs argument
      if (sym .ne. name) then
         call error(274)
         goto 98
      endif
      if (eqid(syn,varargout)) then
         call error(275)
         goto 98
      endif

      call putid(id,syn)
      excnt=0

      next=lin(lpt(4)-2)
      if (next.eq.blank.or.char1.eq.comma.or.char1.eq.right) then
c     .  argument followed by a blank, a comma or a ] ==> it is a simple name
         call getsym
         goto 44
      elseif (char1.ne.lparen.and.char1.ne.dot) then
c     .  invalid lhs
         call error(273)
         goto 98
      endif
c     lhs argument is name(..) or name.xx
      if ( eptover(1,psiz)) goto 98
      rstk(pt)=711
      lpt(5) = lpt(4)
      schar=char1
      goto 32
c     *parse* index
 42   continue
      pt=pt-1
      goto 44


 44   continue
c     record current lhs arg      
      if ( eptover(1,psiz)) goto 98
      call putid(ids(1,pt),id)
      lhs = lhs+1
      pstk(pt) = excnt
      rstk(pt) = 0

c     end analysis of a current lhs arg
      if (sym .eq. right) goto 46
      if (sym .eq. comma) call getsym

c     loop on lhs args
      goto 41
c
 46   call getsym
      if (sym .eq. equal.and.char1.ne.equal) then
c        really found a lhs go to  the rhs analysis part
         call getsym
         goto 60
      endif

c     lhs revealed to be an rhs
c-------------------------------------
 50   continue
c     no equal symbol 
      goto 51

 51   continue
c     lhs is in fact an  rhs
c     1 - reinititialise the parser at the instruction beginning
      pt = pts
      lpt(4)=lpts
      lpt(3)=lpts
      char1=blank
 52   call getsym
c      if(sym.eq.comma.or.sym.eq.semi) goto 52
c     2 - make "ans" the lhs
      if ( eptover(1,psiz))  goto 98
      call putid(ids(1,pt),ans)
      pstk(pt) = 0
      lhs = 1
c     3 - go to the rhs analysis part
      goto 60

c     
c     lhs finished, start rhs
c----------------------------
 60   continue
      rstk(pt) = 703
c     *call* expr
      goto 81
 65   continue
      if (rstk(pt-lhs).eq.313) then 
c     store  new variable as "named" at the top of the stack
         if (sym.eq.rparen .or. sym.eq.comma) then
            call mrknmd()
            if(err.gt.0) goto 98
            goto 83
         else
            call error(40)
            goto 98
         endif
      endif
      if (sym.eq.semi .or. sym.eq.comma .or. sym.eq.eol) goto 70
      call error(276)
      goto 98
c     
c     store results
c-------------------
 70   continue
      lhs=max(lhs,1)
      if (compil(29,lhs,sym,0,0)) then 
         if (err.gt.0) return
         pt=pt-lhs
         lhs=0
         goto 77
      endif
      ndel=0
 71   rhs = pstk(pt)
      lastindpos=(top-lhs-ndel)
      if(err1.ne.0) goto 76

      if(rhs.eq.0) then
c     .  goto simple affectation
         call stackp(ids(1,pt),0)
         if (err .gt. 0 ) goto 98
         if(err1.gt.0) then
            pt=pt-1
            lhs=lhs-1
            goto 98
         endif
c     .  topk points on the newly saved variable
         topk=fin
c     .  go to print
         goto 73
      endif

c     partial variable affectation (insertion)
      if(lastindpos+1.ne.top) then
c     .  create reference variables to get index1,...,indexn, value at
C     .  the top of the stack in this order
c     .  create reference variables pointing to the  indices
         do ir=1,rhs
            call createref1(lastindpos-rhs+ir)
         enddo
c     .  create reference variable pointing to the value
         call createref1(top-rhs)
c     .  remind to remove the original indices
         ndel=ndel+rhs
      endif
      lastindpos=lastindpos-rhs

c     put a reference to the lhs variable
      fin=-3
      call stackg(ids(1,pt))
      if (err .gt. 0) goto 98
c     perform insertion operation
c     index1,...,indexn, value ==> updated lhs value (or pointer to)
      if ( eptover(1,psiz))  goto 98
      pstk(pt)=lhs
      call putid(ids(1,pt),ids(1,pt-1))
      ids(1,pt-1)=ndel
      ids(2,pt-1)=lastindpos
      rstk(pt)=704
      rhs=rhs+2
      fin=insert
c     *call* allops(insert)
      goto 91
 72   lhs=pstk(pt)
      ndel=ids(1,pt-1)
      lastindpos=ids(2,pt-1)
      call putid(ids(1,pt-1),ids(1,pt))
      pt=pt-1

c     store the updated value 
      call stackp(ids(1,pt),0)
      if (err .gt. 0 ) goto 98
      if(err1.gt.0) then
         pt=pt-1
         lhs=lhs-1
         goto 98
      endif
c     topk points on the newly saved variable
      topk=fin

c     remove variable containing the value if required
      if (lastindpos.ne.top) top=top-1



 73   continue
c     print if required
c----------------------
      if(lct(4).lt.0.or.fin.eq.0) goto 76
      if(.not.((sym.ne.semi.and.lct(3).eq.0).or.
     &     (sym.eq.semi.and.lct(3).eq.1))) goto 76
 74   call print(ids(1,pt),topk,wte)
      if (err .gt. 0) goto 98
      if(topk.eq.0) goto 76
c     overloaded display, call a macro
      if ( eptover(1,psiz))  goto 98
      rstk(pt)=708
      pstk(pt)=sym
      ids(1,pt)=sym
      ids(2,pt)=ndel
      ids(3,pt)=lhs
      if(fun.eq.0) goto 88
      goto 85
 75   continue
      sym=pstk(pt)
      ndel=ids(2,pt)
      lhs=ids(3,pt)
      pt=pt-1
      goto 74
c     
 76   pt   = pt-1
      lhs  = lhs-1
      if (lhs .gt. 0) goto 71
      top=top-ndel
c     
c     finish statement
c---------------------
 77   fin = 0
      p = 0
      r = 0
      if (pt .gt. 0) p = pstk(pt)
      if (pt .gt. 0) r = rstk(pt)
      if (ddt .eq. 4) then
         write(tmp(1:22),'(4i4,3i2)') pt,r,p,lpt(1),niv,macr,paus
         call basout(io,wte,
     +        ' finish pt:'//tmp(1:4)//' rstk(pt):'//tmp(5:8)//
     +        ' pstk(pt):'//tmp(9:12)//' lpt(1):'//tmp(13:16)//
     +        ' niv:'//tmp(17:18)//' macr:'//tmp(19:20)//
     +        ' paus:'//tmp(21:22))
      endif
      if(err1.ne.0) then
c     .  a catched error has occured
         if (ids(1,pt-1).ne.0) then
c     .    execution is explicitly required to be stopped
            if(rstk(pt).eq.502.and.rstk(pt-1).eq.903) then
c     .        in an execstr(...,'errcatch') instruction
               goto 88
            elseif(rstk(pt).eq.502.and.rstk(pt-1).eq.909) then
c     .        in an exec(function,'errcatch') instruction
               goto 88
            elseif (rstk(pt).eq.503.and.rstk(pt-1).eq.902) then 
c     .        in an exec(file,'errcatch') instruction
               goto 88
            endif
         endif
         if(err2.eq.0) err2=err1
         err1=0
         imode=abs(errct/100000)
         if(imode-8*int(imode/8).eq.2) iflag=.true.
      endif
      toperr=top
c     fin instruction
      if(sym.ne.eol) goto 15
c     gestion des points d'arrets dynamiques
      if(wmac.ne.0) then
         call whatln(lpt(1),lpt(2)-1,lpt(6),nlc,l1,ifin)
         
         do  ibpt=lgptrs(wmac),lgptrs(wmac+1)-1
            if(lct(8)-nlc.eq.bptlg(ibpt)) then
               call cvname(macnms(1,wmac),buf(1:nlgh),1)
               write(buf(nlgh+2:nlgh+7),'(i5)') lct(8)-nlc
               call msgs(32,0)
               call cvstr(ifin-l1+1,lin(l1),buf,1)
               call basout(io,wte,buf(1:ifin-l1+1))
               iflag=.true.
               goto 79
            endif
         enddo
      endif
 79   continue
c
      if(comp(1).ne.0) then
         call seteol
      endif

c     EOL
      if(int(r/100).ne.8) goto 10
c     end of an instruction or a clause
      if(comp(1).ne.0) then
         k=lpt(6)
         if(lin(k-1).eq.eol.and.lin(k).eq.eol) then
            call error(47)
            goto 98
         endif
      endif
      if(lpt(4).eq.lpt(6))  then
c         call getlin(1,0)
         goto 13
      else
         lpt(4)=lpt(4)+1
         call getsym
      endif
      goto 15
c     
c     simulate recursion
c-----------------------
 80   icall=0
      call clause
      if (err .gt. 0) goto 98
      goto(81,82,83,91,88,90,15,80,85),icall
      if (pt .le. 0) goto 15
      r = rstk(pt)/100
      goto(81,82,83,91,88,90,92,80,85),r
      goto 99
c     
 81   icall=0
      call expr
      if (err .gt. 0) goto 98
      goto(81,82,83,91,88,90,15,80,85),icall
      r = rstk(pt)/100
      goto(81,82,83,91,88,90,92,80,85),r
      goto 99
c     
 82   icall=0
      call terme
      if (err .gt. 0) goto 98
      goto(81,82,83,91,88,90,15,80,85),icall
      r = rstk(pt)/100
      goto(81,82,83,91,88,90,92,80,85),r
      goto 99
c     
 83   icall=0
      call fact
      if (err .gt. 0) goto 98
      goto(81,82,83,91,88,90,15,80,85),icall
      r = rstk(pt)/100
      goto(81,82,83,91,88,90,92,80,85),r
      goto 99
c     
 85   icall=0
      if(fun.eq.krec) then
         if(fun.gt.0) then
            call error(22)
            goto 98
         endif
      endif
      
      if(err1.ne.0) then
         if(int(rstk(pt)/100).eq.9) then
            if(rstk(pt).ge.901.and.rstk(pt).le.909) then
c              *call* matfns
               return
            else
               pt=pt-1
               goto 86
            endif
         else
            goto 86
         endif
      endif
c     compilation matfns: <100*fun rhs lhs fin>
      if (compil(100*fun,rhs,lhs,fin,0)) then 
         if (err.gt.0) goto 98 
         goto 86
      else
c     *call* matfns
         return
      endif
 86   if (err .gt. 0) goto 98
      goto(81,82,83,91,88,90,15,80,85),icall
      r = rstk(pt)/100
      goto(81,82,83,91,88,90,92,80,85),r
      goto 99
      
c     
 88   icall=0
      call macro
      if (err .gt. 0) goto 98
      goto(81,82,83,91,88,90,10,80,85),icall
      r = rstk(pt)/100
      goto(81,82,83,91,88,90,92,80,85,89),r
      goto 99
 89   fun=99
      return
c     
 90   icall=0
      call  run
      if (err .gt. 0) goto 98
      if (fun .eq. 99) return
c     last label is used to handle return from abort
      goto(81,82,83,91,88,90,15,80,85,05),icall
      r = rstk(pt)/100
      goto(81,82,83,91,88,90,92,80,85,89),r
      goto 99
c     
 91   icall=0
      call allops
      if (err .gt. 0) goto 98
      goto(81,82,83,91,88,90,15,80,85),icall
      r = rstk(pt)/100
      goto(81,82,83,91,88,90,92,80,85),r
      goto 99
c     
 92   goto(17,35,65,72,65,97,94,75,99,14) rstk(pt)-700
      goto 99
c     
 93   continue
c     command like function and macro call

c     store ans as lhs
      if ( eptover(1,psiz)) goto 98
      call putid(ids(1,pt),ans)
      pstk(pt)=0
c
      rstk(pt)=707
      if (fun .gt. 0)  then
c        *call* matfns
         goto 85
      else
         if (comp(1).ne.0)  then
            rhs=rhs+1
            fin=extrac
c           *call* allops(extrac)
            goto 91
         else
            fin=lstk(fin)
c           *call* macro
            goto 88
         endif
      endif
 94   continue
c     go to store code
      goto 70
c
 96   continue
c     asynchronous events handling
      call getmen(buf,lb,nentry)
      call bexec(buf(1:lb),lb,ierr)
      if(ierr.ne.0) goto 15
      if ( eptover(1,psiz)) goto 98
      pstk(pt)=top
      rstk(pt)=706
      ids(1,pt)=iret
c     *call* macro
      goto 88
 97   top=pstk(pt)-1
      iret=ids(1,pt)
      pt=pt-1
      if(iret.eq.1) then
         goto 15
      elseif(iret.eq.2) then
          if(lpt(6).eq.lpt(1)) then
             job=0
          else
c     .      go ahead with interrupted continuation line
             job=3
          endif
         sym = eol
         goto 13
      elseif(iret.eq.3) then
         job=0
         goto 12
      endif
      
 98   continue
c     error recovery
c-------------------
      imode=abs(errct)/100000
      imode=imode-8*int(imode/8)
      if(imode.eq.3) then
         fun=99
         return
      endif
c     
c     error in an external (niv), during compilation (comp) or in a pause
      if(pt.ne.0) then
         if(rstk(pt).eq.503.and.rio.eq.rte) then
            comp(1)=0
            goto 12
         endif
c     added 16/04/97
         if(err1.ne.0.and.rstk(pt).eq.502) then
c     catched error while compiling
            goto 88
         endif
      endif
c     error in an external
      if(niv.gt.0) then
         return
      else
c     error in a  pause
         comp(1)=0
         goto 05
      endif
c     
 99   call error(22)
      goto 01
c

      end

      subroutine handleonprompt(where)
c     checks if an implicit execution is required on the prompt
c     where : returned indicator
c     where = 0 : no implicit execution is required
c     where = 1 : implicit execution is a primitive
c     where = 2 : implicit execution is a Scilab function

c     Copyright INRIA
      include '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2,nz3=nsiz-3)

      integer where,onprompt(nsiz)
      data onprompt/420943928,420878363,673720349,nz3*673720360/
      where=0
      if(pt.gt.0) then
c     .  back from %onprompt
         errct=pstk(pt)
         pt=pt-1
         err2=0
         top=0
         fin=0
         fun=0
      else
c     .  on prompt implicit execution
         fun=0
         call funs(onprompt)
         if(err.gt.0) return
         if(fun.le.0.and.fin.eq.0) return
c     .  %onprompt function exists
         rhs=0
         lhs=1
         pt=pt+1
         pstk(pt)=errct
         rstk(pt)=710
c     .  set error catch with mode continue
         errct=-100001
         errpt=1
         if(fun.gt.0) then
c     .  %onprompt is a primitive *call* matfns
            where=1
         else
c     .  %onprompt is a Scilab function *call*  macro
            fin=lstk(fin)
            where=2
         endif
      endif
      return
      end
c     












