      subroutine parse
c     ====================================================================
c     Scilab parsing function
c     ====================================================================
c     Copyright INRIA
      include '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      logical compil,eptover
c     
      logical iflag
      common /basbrk/ iflag
      integer semi,equal,eol,lparen,rparen,colon,dot
      integer blank,comma,left,right,less,great,not
      integer quote,percen
      integer name,num,insert,extrac
c     
      integer id(nsiz),ans(nsiz)
      integer pts,psym,excnt,p,r,topk
      integer pcount,strcnt,bcount,qcount,pchar,schar
      logical dotsep
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
c     
      save job

c
      itime = 10000
      call xscion(inxsci)
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
            call prompt(lct(4)/4)
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
      call getlin(job)
      if(fin .eq. -1) then
c     Continuation line handling when scilab is called as a routine
         fun=99
         return
      endif
      job=0
      err = 0
c     
c     Beginning of a new statement, clause expression or command
c------------------------------------------------------------
 15   continue
      if (inxsci.eq.1) then
         ntimer=stimer()/itime
         if (ntimer.ne.otimer) then
            call sxevents()
            otimer=ntimer
            if(ismenu().eq.1) goto 96
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
      call putid(id,ans)
c     
      call getsym
      if (sym.eq.right) call getsym
      if (sym.eq.colon) call getsym
      if (sym.eq.semi .or. sym.eq.comma .or. sym.eq.eol) goto 75
      if (sym.eq.name) goto 20
      if (sym.eq.left) goto 40
      goto 60
c     
c     lhs begins with name
c-------------------------
 20   call comand(syn)
      if (err .gt. 0) goto 98
      if (fun .eq. 99) return
      if (fun .ne. 0) goto 93
      if (fin .lt. 0) goto 80
      if (fin .eq. 2) goto 88
      if (fin .eq. 3) goto 16
      if (fin .eq. 4) goto 05
      if (fin .gt. 0) goto 75
 21   rhs = 0
c      fin=0
c      call funtab(syn,fin,1)
      fin=-5
c IL y a p avec fin=-5 (on ne trouve pas les macros parce que l'on ne
c veut pas que les macros sans arg soient vues comme des commandes
c mais pourquoi pas il suffirait de dire que pour visualiser une macro
c il faut faire disp()
      fun=0
      call funs(syn)
      if (fin .gt. 0) then
c        name est le nom d'une primitive
         if(char1.eq.equal) then
            call putid(ids(1,pt+1),syn)
            call error(25)
            goto 98
         endif
c        name is a function, must be rhs
         goto 60
      endif
c     
c     peek one character ahead
      if (char1.eq.semi .or. char1.eq.comma .or. char1.eq.eol)
     $     call putid(id,syn)
      if (char1 .eq. equal) goto 25
      if (char1 .eq. lparen.or.char1.eq.dot) then
         schar=char1
         goto 30
      endif
      goto 60
c     
c     lhs is simple variable
c---------------------------
 25   call putid(id,syn)
      lpts=lpt(2)
      call getsym
      if(char1.eq.equal) then
c     logical equality
         lpt(4)=lpts
         lpt(2)=lpts
         lpt(3)=lpts
         call putid(id,ans)
         char1=blank
         sym=blank
      endif
      call getsym
      goto 60
c     
c     lhs is name(...) or name.x...
c---------------------
 30   lpt(5) = lpt(4)
      call putid(id,syn)
c     
c     looking for equal to check if it is really an lhs
      pcount=0
      strcnt=0
      bcount=0
 31   psym=sym
      call getsym
      if(strcnt.ne.0) then
         if(sym.eq.eol) then
            call error(3)
            goto 98
         endif
         if(sym.eq.quote) then
            qcount=0
 311        qcount=qcount+1
            if(abs(char1).ne.quote) goto 312
            call getsym
            goto 311
 312        continue
            if(2*int(qcount/2).ne.qcount)  strcnt=0
         endif
      else if(sym.eq.lparen) then
         pcount=pcount+1
      else if(sym.eq.rparen) then
         pcount=pcount-1
         if(pcount.lt.0) then
            call error(2)
            goto 98
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
            goto 98
         endif
      else if(pcount.eq.0) then
         if(sym.eq.equal) then
            if(char1.eq.equal) then
               call getsym
            else
               if(psym.ne.less.and.psym.ne.great.and.
     $              psym.ne.not)  goto 32
            endif
         endif
         if(sym.eq.eol .or. sym.eq.comma .or. sym.eq.semi) goto 50
c     .  next line for recursive index
         if(sym.eq.lparen) goto 31
      else if(sym.eq.eol) then
         if(bcount.eq.0) then
            call error(3)
            goto 98
         else
            if(lpt(4).eq.lpt(6))  then
               call getlin(1)
               if(err.gt.0) goto 98
            else
               lpt(4)=lpt(4)+1
               call getsym
            endif
         endif
      endif
      goto 31
c     
 32   continue
c     It is really a lhs (insertion syntax)
      lpt(4)=lpt(5)
      char1=schar
c
c35     call parseindexlist(excnt)
c     if(err.gt.0) goto 98

      if(comp(1).ne.0) then
         if (compil(21,0,0,0,0)) then 
            if (err.gt.0) return
         endif
      endif
c     begin the index lists
      icount=0

      call getsym
c
 33   continue
c     begin a new index list (...) or .name
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

c     --> new index list is (...)
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
      goto 60
c     
c     multiple lhs
c-----------------
 40   lpt(5) = lpt(4)
      pts = pt
      call getsym
 41   if (sym .ne. name) goto 43
      call putid(id,syn)
      call getsym
      if (sym .eq. right) goto 42
      if (sym .eq. comma) call getsym
      if ( eptover(1,psiz)) goto 98
      lhs = lhs+1
      pstk(pt) = 0
      rstk(pt) = 0
      call putid(ids(1,pt),id)
      goto 41
 42   call getsym
      if (sym .eq. equal) then
         if(char1.eq.equal) goto 43
         goto 60
      endif
 43   lpt(4) = lpt(5)
      pt = pts
      lhs = 1
      sym = left
      char1 = lin(lpt(4)-1)
      call putid(id,ans)
      goto 60
c     
c     
c     lhs is really rhs
c-----------------------
 50   lpt(4)=lpt(5)
      char1=schar
      sym=name
      call putid(syn,id)
      call putid(id,ans)
      goto 61
c     
c     lhs finished, start rhs
c----------------------------
 60   if (sym .eq. equal) call getsym
c      fun=0
c      if(sym.eq.name) then
c     check if name is a function
c         fin=-2
c         call funs(syn)
c      endif

 61   if ( eptover(1,psiz))  goto 98
      call putid(ids(1,pt),id)

      pstk(pt) = excnt
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
      call error(40)
      goto 98
c     
c     store results
c-------------------
 70   rhs = pstk(pt)
      if(err1.ne.0) goto 74
      if(rhs.eq.0) goto 72
      fin=-3
      call stackg(ids(1,pt))
      if (err .gt. 0) goto 98
      rhs=rhs+2
      pstk(pt)=lhs
      rstk(pt)=704
      fin=insert
c     *call* allops(insert)
      goto 91
 71   lhs=pstk(pt)
 72   call stackp(ids(1,pt),0)
      if (err .gt. 0 ) goto 98
      if(err1.gt.0) then
         pt=pt-1
         lhs=lhs-1
         goto 98
      endif
c     topk points on the newly saved variable
      topk=fin
c     print if required
c----------------------
      if(lct(4).lt.0.or.fin.eq.0) goto 74
      if(.not.((sym.ne.semi.and.lct(3).eq.0).or.
     &     (sym.eq.semi.and.lct(3).eq.1))) goto 74
 73   call print(ids(1,pt),topk,wte)
      if (err .gt. 0) goto 98
      if(topk.eq.0) goto 74
c     overloaded display, call a macro
      if ( eptover(1,psiz))  goto 98
      rstk(pt)=708
      pstk(pt)=sym
      ids(1,pt)=sym
      if(fun.eq.0) goto 88
      goto 85
 731  continue
      sym=pstk(pt)
      pt=pt-1
      goto 73
c     
 74   pt = pt-1
      lhs = lhs-1
      if (lhs .gt. 0) goto 70

c     
c     finish statement
c---------------------
 75   fin = 0
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
         
         do 76 ibpt=lgptrs(wmac),lgptrs(wmac+1)-1
            if(lct(8)-nlc.eq.bptlg(ibpt)) then
               call cvname(macnms(1,wmac),buf(1:nlgh),1)
               write(buf(nlgh+2:nlgh+7),'(i5)') lct(8)-nlc
               call msgs(32,0)
               call cvstr(ifin-l1+1,lin(l1),buf,1)
               call basout(io,wte,buf(1:ifin-l1+1))
               iflag=.true.
               goto 77
            endif
 76      continue
      endif
 77   continue
c
      if(comp(1).ne.0) then
         call seteol
      endif

c     fin ligne
      if(int(r/100).ne.8) goto 10
c     fin d'une instruction dans une clause
      if(comp(1).ne.0) then
         k=lpt(6)
         if(lin(k-1).eq.eol.and.lin(k).eq.eol) then
            call error(47)
            goto 98
         endif
      endif
      if(lpt(4).eq.lpt(6))  then
         call getlin(1)
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
      goto(81,82,83,91,88,90,92,80,85),r
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
 92   goto(17,35,65,71,65,97,94,731) rstk(pt)-700
      goto 99
c     
 93   continue
c     command like function and macro call
      if ( eptover(1,psiz)) goto 98
      pstk(pt)=1
      call putid(ids(1,pt),ans)
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
 94   goto 71
c
 96   continue
c     asynchronous events handling
      call getmen(buf,lb,nentry)
      call bexec(buf(1:lb),lb,ierr)
      if(ierr.ne.0) goto 15
      if ( eptover(1,psiz)) goto 98
      pstk(pt)=top
      rstk(pt)=706

c     *call* macro
      goto 88
 97   top=pstk(pt)-1
      pt=pt-1
      goto 15
      
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














