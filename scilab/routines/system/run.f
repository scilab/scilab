       subroutine run
c ======================================================================== 
C     Execution of a compiled macro 
c ======================================================================== 
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision x
      integer op,equal,r,ix(2),m
      equivalence (x,ix(1))
      logical logops,ok,iflag,istrue,ptover,cremat,cresmat,
     $     vcopyobj, eptover
      integer p,lr,nlr,lcc,id(nsiz)
      integer extrac,insert,semi
      common /basbrk/ iflag
      integer otimer,ntimer,stimer,ismenu,getendian,gettype
      integer tref,t,fclock
      integer iadr,sadr
      external stimer,ismenu,getendian
      save otimer
      data extrac /3/,insert/2/,semi/43/
      data otimer/0/
      data equal/50/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      tref = 0
      itime = 10000
      call checkevts(inxsci)
      if (ddt .eq. 4) then
         write(buf(1:8),'(2i4)') pt,rstk(pt)
         call basout(io,wte,' run pt:'//buf(1:4)//' rstk(pt):'
     &        //buf(5:8))
      endif
c     
      l0=0
      nc=0
c     
      if ( ptover(0,psiz-2)) return
c     
      r=rstk(pt)
      ir=r/100
      if(ir.ne.6) goto 01
      goto(33,66,82,92,58,116,232),r-600
c     
c     debut d'une macro compilee
 01   continue
      tref=fclock()
      toperr=top
      k=lpt(1)-(13+nsiz)
      lc=lin(k+7)
c     
 10   if(err.gt.0) return
      if(iflag) then
         iflag=.false.
         goto 91
      endif
      if(err1.ne.0) then
c     .  errcatch in exec(function,'errcatch') 
c     .  or catched error in an external
c     .  or errcatch in execstr('foo()','errcatch')
         if(rstk(pt-1).eq.903.or.rstk(pt-1).eq.909.or.
     $        rstk(pt).eq.1001) return
      endif
      if(lc-l0.ne.nc) goto 11
      r=rstk(pt)-610
      goto(46,47,52,56,57,61),r
c     
c     nouvelle 'operation'
 11   continue
      op=istk(lc)
c     label 49 retains to be able issue a compatibility error message
      goto(20, 25, 40, 42, 30, 41, 45, 49, 49, 55,
     &     15, 90, 95, 100,105,110,120,130,140,150,
     &     160,170,180,190,200,210,220, 97,230,240) ,op
c     matfns
      if(op.ge.100) goto 80
c     return
      if(op.eq.99) then
c     .  check if "return" occured in a for loop
         p=pt+1
 12      p=p-1
         if(rstk(p).eq.612) then
c     .     yes, remove the for loop variable
            top=top-1
            goto 12
         else if(rstk(p).ne.501) then
            goto 12
         endif
 13      fin = 2
         goto 998
      endif
c     
      if(op.le.0) then
c     nop
         lc=lc+istk(lc+1)
         goto 11
      endif
 15   continue
      call error(60)
      return
c     
 20   continue
c     retained for 2.7 and earlier versions compatibility
      call stackp(istk(lc+1),0)
c     store info if printing is required see code 22
      call putid(id,istk(lc+1))
      kid=fin
      lc=lc+1+nsiz
      goto 10
c     
 25   fin=istk(lc+nsiz+1)
      rhs=istk(lc+nsiz+2)
      lname=lc+1
 26   call stackg(istk(lname))
      if(err.gt.0) then 
         lc=lc+nsiz+3
         goto 10
      endif
      if(fin.ne.0) goto 28
      call funs(istk(lc+1))
      if(err.gt.0) return
      if(fun.ne.-2) then
         call putid(ids(1,pt+1),istk(lc+1))
         if(fun.eq.0) then
            call error(4)
            if(err.gt.0) then
               top=top-1
               return
            endif
         else
c     .     referenced name was function at compile time it is now a
c     .     primitive. Modify the code for further use

c     .     change current  opcode to nop 
            istk(lc)=0
            istk(lc+1)=nsiz+3
            lc=lc+nsiz+3
c     .     change the following to matfn
            
            op=fun*100
            istk(lc)=op
            istk(lc+1)=istk(lc+2)-1
            istk(lc+2)=istk(lc+3)
            istk(lc+3)=fin
            goto 80
c           call error(110)
         endif
         lc=lc+nsiz+3
         goto 10   
      endif
      fin=istk(lc+nsiz+1)
      goto 26
 28   continue
      if (rhs.eq.0.and.istk(lc+nsiz+1).eq.-2.and.fin.eq.-1) then
         lc=lc+nsiz+3
c     .  instruction reduced to <name> with name not a function, replace
C     .  next two op code by a single store 
c     .  skip extract op-code <5 3 1 1>
         if (istk(lc).ne.5.or.istk(lc+1).ne.3) then
            buf='Unexpected opcode, please report'
            call error(9999)
            return
         endif
         lc=lc+4
c     .  skip assignment op_code <29 43 ans 0>   
         if (istk(lc).ne.29) then
            buf='Unexpected opcode, please report'
            call error(9999)
            return
         endif
         lc=lc+nsiz+4
c     .  store
         call stackp(istk(lname),0)
         if (err .gt. 0 ) return
         goto 10
      endif
      lc=lc+nsiz+3
      if(fin.gt.0) goto 65
      goto 10
c     
c     allops 
 30   fin=istk(lc+1)
      rhs=istk(lc+2)
      lhs=istk(lc+3)
      lc=lc+4
      if(fin.eq.extrac) then
         call isafunptr(top,id,ifun,ifin)
         if(ifun.ne.0) then
            top=top-1
            rhs=rhs-1
            fun=ifun
            fin=ifin
            call adjustrhs
            goto 81
         endif
      endif

      if(fin.eq.extrac.or.fin.eq.insert) call adjustrhs
      pt=pt+1
      rstk(pt)=601
      ids(1,pt)=tref
      ids(3,pt)=l0
      ids(4,pt)=nc
      icall=4
c     pstk(pt) is used by allops to get the name of output variable (insertion)
      pstk(pt)=lc
c     *call* allops
      return
 33   continue
      tref=ids(1,pt)
      l0=ids(3,pt)
      nc=ids(4,pt)
      lc=pstk(pt)
      pt=pt-1
      goto 70
c     
c     string
 40   n=istk(lc+1)
      if(err1.le.0) then
         top=top+1
         if (cresmat("run",top,1,1,n)) then 
            call getsimat("run",top,top,mm1,nn1,1,1,lr,nlr)         
            call icopy(n,istk(lc+2),1,istk(lr),1)
         endif
      endif
      lc=lc+n+2
      goto 10
c
c     num
 41   if(err1.le.0) then
         if(getendian().eq.1) then
            ix(1)=istk(lc+1)
            ix(2)=istk(lc+2)
         else
            ix(2)=istk(lc+1)
            ix(1)=istk(lc+2)
         endif
         top=top+1
         if (cremat("run",top,0,1,1,lr,lcc)) then 
            stk(lr)=x
         endif
      endif
      lc=lc+3
      goto 10
c     
 42   call defmat
      lc=lc+1
      goto 10
c     
c     for
 45   nc=istk(lc+1)
      lc=lc+2
      l0=lc
      if ( ptover(1,psiz)) then 
         lc=lc+nc
         lc=lc+nsiz+istk(lc)
         goto 10
      endif
      rstk(pt)=611
      ids(1,pt)=l0
      ids(2,pt)=nc
      goto 10
c     
 46   nc=istk(lc)
      l0=lc+1+nsiz
      rstk(pt)=612
      pstk(pt)=0
      ids(1,pt)=l0
      ids(2,pt)=lct(8)
      ids(3,pt)=top
      ids(4,pt)=toperr
      toperr=top
 47   lc=l0
      if(top.ne.ids(3,pt)) then 
         call error(115)
         return
      endif
      call nextj(istk(l0-nsiz),pstk(pt))
      if(pstk(pt).ne.0) then
         lct(8)=ids(2,pt)
         if (inxsci.eq.1) then
            ntimer=stimer()/itime
            if (ntimer.ne.otimer) then
               call sxevents()
               otimer=ntimer
               if (ismenu().eq.1) goto 115
            endif
         endif
         goto 10
      endif
c     fin for
      lc=lc+nc
      toperr=ids(4,pt)
      pt=pt-1
      goto 70
c     
c     Very old if - while (removed)
 49   continue
      if(istk(lc+1).lt.0) goto 55
 52   continue
      buf='Functions compiled with very old versions are'//
     $     ' no more handled'
      call error(997)
      return
c     
c     "select- case"  or  "if elseif else end"
 55   continue
      if ( ptover(1,psiz)) then 
         lc=lc+abs(istk(lc+1))
         goto 10
      endif
      pstk(pt)=lc
      ids(3,pt)=toperr
c     
      if(istk(lc+1).gt.0) then
c     premiere expression
         nc=istk(lc+3)
         rstk(pt)=614
         lc=lc+4
         l0=lc
         ids(1,pt)=l0
         ids(2,pt)=nc
         goto 10
      else
         lc=lc+4
      endif
c     
c     expri
 56   continue
      if(istk(pstk(pt)).eq.10) then
c     recopie de la premiere expression 
         if (.not.vcopyobj("run",top,top+1)) return
         top=top+1
      endif
c     
      nc=istk(lc)
      rstk(pt)=615
      lc=lc+1
      l0=lc
      ids(1,pt)=l0
      ids(2,pt)=nc
      goto 10
c     
c     instructions i
 57   continue
      if(nc.eq.0) then
c     si nc=0 l'instruction i correspond a l'isntruction associee au  else
         ok=.true.
         if(istk(pstk(pt)).eq.10) top=top-1
         goto 59
      elseif(istk(pstk(pt)).ne.10) then
         ok=istrue(1)
         if(err.gt.0) return
         goto 59
      endif

      pt=pt+1
      fin=equal
      rhs=2
      lhs=1
      rstk(pt)=605
      icall=4
      pstk(pt)=lc
      ids(1,pt)=tref
c     *call* allops(equal)
      return
c
 58   continue
      lc=pstk(pt)
      tref=ids(1,pt)
      pt=pt-1
      ok=istrue(1)
      if(err.gt.0) return
 59   nc=istk(lc)
      toperr=top
      if(ok) then
         lc=lc+1
         if(istk(pstk(pt)).eq.10) top=top-1
         l0=lc
         ids(1,pt)=l0
         ids(2,pt)=nc
         rstk(pt)=616
         if (inxsci.eq.1) then
            ntimer=stimer()/itime
            if (ntimer.ne.otimer) then
               call sxevents()
               otimer=ntimer
               if (ismenu().eq.1) goto 115
            endif
         endif
         goto 10
      else
         if(istk(pstk(pt)).eq.9) goto 62
         lc=lc+nc+1
         goto 56
      endif
c     
 61   continue
c     fin if while select/case
      l0=pstk(pt)
      if(istk(pstk(pt)).eq.9) then
         lc=l0+4
         goto 56
      endif
 62   l0=pstk(pt)
      lc=l0+abs(istk(l0+1))
      toperr=ids(3,pt)
      pt=pt-1
      goto 70
c     
c     macro
 65   rhs=max(istk(lc+2)-1,0)
      call adjustrhs
      lhs=istk(lc+3)
c
      lc=lc+4
c     
      if ( ptover(1,psiz)) goto 10 
      rstk(pt)=602
      pstk(pt)=lc
      ids(1,pt)=wmac
      ids(2,pt)=tref
      ids(3,pt)=l0
      ids(4,pt)=nc
      icall=5
      fun=0
c     *call* macro
      return
 66   lc=pstk(pt)
      wmac=ids(1,pt)
      tref=ids(2,pt)
      l0= ids(3,pt)
      nc=ids(4,pt)
      pt=pt-1
      goto 70
c     
 70   continue
      if (inxsci.eq.1) then
         ntimer=stimer()/itime
         if (ntimer.ne.otimer) then
            call sxevents()
            otimer=ntimer
            if (ismenu().eq.1) goto 115
         endif
      endif
      r=rstk(pt)-610
      goto(74,71,72,73,73,73) ,r
      goto 10
c     retour d'une boucle interne ou d'une macro vers un for
 71   j=pstk(pt)
      l0=ids(1,pt)
      nc=istk(l0-1-nsiz)
      goto 10
c     retour  d'une boucle interne ou d'une macro vers un if/while
 72   li=ids(1,pt)
      kc=ids(2,pt)
      nc=istk(li+2)
      l0=li+5
      if(kc.eq.0) goto 10
      l0=l0+nc
      nc=istk(li+3)
      if(kc.eq.1) goto 10
      l0=l0+nc
      nc=istk(li+4)
      goto 10
c     retour d'une boucle interne vers une clause select
 73   l0=ids(1,pt)
      nc=ids(2,pt)
      goto 10
c     
 74   l0=ids(1,pt)
      nc=ids(2,pt)
      goto 10
c     
 80   fun=op/100
      rhs=istk(lc+1)
      call adjustrhs
      lhs=istk(lc+2)
      fin=istk(lc+3)
      lc=lc+4
c     
 81   pt=pt+1
      rstk(pt)=603
      pstk(pt)=lc
      icall=9
      ids(2,pt)=0
      ids(3,pt)=tref
      ids(4,pt)=l0
      ids(5,pt)=nc
c     *call* matfns
      return
 82   continue
c     warning if builtin is "resume" control is passed to macro and not here
      lc=pstk(pt)
      tref=ids(3,pt)
      l0=ids(4,pt)
      nc=ids(5,pt)
      pt=pt-1
      goto 70
c     
c     pause
 90   lc=lc+1
 91   continue
      if ( ptover(1,psiz)) goto 10
      pstk(pt)=rio
      rio=rte
      fin=2
      if(lct(4).le.-10) then
         fin=-1
         lct(4)=-lct(4)-11 
      endif
      ids(1,pt)=lc
      ids(2,pt)=top
      ids(3,pt)=tref
      ids(4,pt)=l0
      ids(5,pt)=nc
      
      rstk(pt)=604
      icall=5
c     *call* macro
      return
 92   lc=ids(1,pt)
      top=ids(2,pt)
      tref=ids(3,pt)
      l0=ids(4,pt)
      nc=ids(5,pt)
      rio=pstk(pt)
      pt=pt-1
      goto 70
c     
c     break
 95   continue
      p=pt+1
 96   p=p-1
      if(p.eq.0) then
         lc=lc+1
         goto 10
      endif
      if(rstk(p).eq.612) then
c     break in a for
         l0=ids(1,p)
         lc=l0+istk(l0-1-nsiz)
         pt=p-1
         top=top-1
         goto 70
      elseif(rstk(p).eq.616.and.istk(pstk(p)).eq.9) then
c     break in a while 
         l0=pstk(p)
         lc=l0+abs(istk(l0+1))
         pt=p-1
         goto 70
      elseif(rstk(p).eq.501.or.rstk(p).eq.502.or.rstk(p).eq.503) then
c     going outside a function an exec (break ignored)
         lc=lc+1
         goto 10
      else
         goto 96
      endif

c     continue
 97   continue
      p=pt+1
 98   p=p-1
      if(p.eq.0) then
         lc=lc+1
         goto 10
      endif
      if(rstk(p).eq.612) then
c     continue in a  for
         l0=ids(1,p)
         pt=p
         goto 47
      elseif(rstk(p).eq.616.and.istk(pstk(p)).eq.9) then
c     continue in a while 
         l0=pstk(p)
         lc=l0
         pt=p
         goto 57
      else
         goto 98
      endif

c     abort
 100  continue
      pt=pt+1
 101  pt=pt-1
      if(pt.eq.0) goto 102
      if(int(rstk(pt)/100).eq.5) then
         k = lpt(1) - (13+nsiz)
         lpt(1) = lin(k+1)
         lpt(2) = lin(k+2)
         lpt(3) = lin(k+3)
         lpt(4) = lin(k+4)
         lct(4) = lin(k+6)
         lpt(6) = k
         if(rstk(pt).le.502) bot=lin(k+5)
      endif
      goto 101
 102  continue
      icall=10
      top=0
      comp(1)=0
      if(niv.gt.1)  err=9999999
      return
c
 105  continue

c     eol
c     la gestion de la recuperation des erreurs devrait plutot se trouver
c     a la fin de l'instruction (mais il n'y a pas actuellement d'indicateur
c     de fin d'instruction dans les macros
      if(err1.ne.0) then
        if(err2.eq.0) err2=err1
        err1=0
        imode=abs(errct/100000)
        if(imode-8*int(imode/8).eq.2) iflag=.true.
      endif

c     gestion des points d'arrets dynamiques
      if(wmac.ne.0) then
         do 106 ibpt=lgptrs(wmac),lgptrs(wmac+1)-1
            if(lct(8).eq.bptlg(ibpt)) then
               call cvname(macnms(1,wmac),buf(1:nlgh),1)
               write(buf(nlgh+2:nlgh+7),'(i5)') lct(8)
               call msgs(32,0)
               iflag=.true.
               goto 107
            endif
 106      continue
      endif
 107  continue
c
      if (mod(lct(4)/2,2).eq.1) then
         call prompt(lct(4)/4,iesc)
      endif
c
      lct(8)=lct(8)+1
      lc=lc+1
      if (inxsci.eq.1) then
         ntimer=stimer()/itime
         if (ntimer.ne.otimer) then
            call sxevents()
            otimer=ntimer
            if (ismenu().eq.1) goto 115
         endif
      endif
      goto 10
c     
c     set line number. Au debut de chaque expression liee a un then et a 
c     la fin de chaque clause, le compilateur (compcl) inscrit
c     la valeur de la ligne. ceci permet de mettre rapidement a jour le
c     compteur de ligne sans avoir a analyser la suite des codes operatoires
 110  continue
      lct(8)=istk(lc+1)
      lc=lc+2
      goto 10

c
c     gestion des evements asynchrones "interpretes"
c
 115  continue
      call getmen(buf,lb,nentry)
      call bexec(buf(1:lb),lb,ierr)
      if(ierr.ne.0) goto 10
      pt=pt+1
      ids(1,pt)=lc
      ids(2,pt)=l0
      ids(3,pt)=nc
      ids(4,pt)=tref
      rstk(pt)=606
      icall=5
c     *call* macro
      return
 116  lc=ids(1,pt)
      l0=ids(2,pt)
      nc=ids(3,pt)
      tref=ids(4,pt)
      top=top-1
      pt=pt-1
      r=rstk(pt)-610
      goto(74,71,72,73,73,73) ,r
      goto 10
c
c     quit
c
 120  continue 
      fun=99
      return
c
c     named variable
c
 130  continue
      infstk(top)=1
      call putid(idstk(1,top),istk(lc+1))
      lc=lc+1+nsiz
      goto 10
c
c     form recursive extraction list
c
 140  continue
      m=istk(lc+2)
      if(rstk(pt).eq.617) then
c     .  runtime arg count (list extraction)
         m=m+pstk(pt)
         pstk(pt)=0
      endif
      call mkindx(istk(lc+1),m)
      lc=lc+3
      goto 10
c
c     exit
c
 150  continue
      lc=lc+1
      if (niv.gt.0) then
         call sciquit
         stop
      endif
      fun = 99
      goto 10

c
c     begrhs - for run time rhs value computation
c              syntax like: l=list(...); a(l(:))
c
 160  continue
      lc=lc+1
      pt=pt+1
      rstk(pt)=617
      pstk(pt)=0
      goto 10
c
c     printmode
c
 170  continue
c     print stored variable
      if(lct(4).ge.0.and.istk(lc+1).ne.semi.and.kid.ne.0) then
         call print(id,kid,wte)
      endif
      lc=lc+2
      goto 10

 180  continue
c     name2var
      call name2var(istk(lc+1))
      lc=lc+nsiz+1
      goto 10
c
 190  continue
c     deffnull
      lc=lc+1
      top=top+1
      call objvide(' ', top)
      goto 10
c
 200  continue
c     profile
      istk(lc+1)=istk(lc+1)+1
      t=fclock()
      istk(lc+2)=istk(lc+2)+t-tref
      tref=t
      lc=lc+3
      goto 10
c
 210  continue
c     character string vector
      if(err1.le.0) then
         n=istk(lc+1)*istk(lc+2)
         nc=istk(lc+4+n)-1
         top=top+1
         il=iadr(lstk(top))
         err=sadr(il+5+n+nc)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(5+n+nc,istk(lc),1,istk(il),1)
         istk(il)=10
         lstk(top+1)=sadr(il+5+n+nc)
      endif
      lc=lc+5+n+nc
      goto 10


 220  continue
c     varfun
      call varfunptr(istk(lc+3),istk(lc+1),istk(lc+2))
      lc=lc+3+nsiz
      goto 10

 230  continue
c     affectation
      lhs=istk(lc+1)
      ip=istk(lc+2)
      li=lc+3
      lc=li+(nsiz+1)*lhs
c     following code is an adaptation of corresponding code in parse.f
      ndel=0
 231  rhs = istk(li+nsiz)

      lastindpos=(top-lhs-ndel)
      if(err1.ne.0) goto 233

      if(rhs.eq.0) then
c     .  goto simple affectation
         call stackp(istk(li),0)
         if (err .gt. 0 ) return
         if(err1.gt.0) goto 233
c     .  fin points on the newly saved variable
         if(lct(4).ge.0.and.ip.ne.semi.and.fin.ne.0) then
            call print(istk(li),fin,wte)
         endif
         goto 233
      endif

c     take rhs (number of indices) computed at runtime into account
      call adjustrhs

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
      call stackg(istk(li))
      if (err .gt. 0) return
c     perform insertion operation
c     index1,...,indexn, value ==> updated lhs value (or pointer to)


      if ( eptover(1,psiz))  return
c     pstk(pt) is used by allops to get the name of output variable
      pstk(pt)=li
      ids(1,pt)=ndel
      ids(2,pt)=lastindpos
      ids(3,pt)=tref
      ids(4,pt)=l0
      ids(5,pt)=lhs
      ids(6,pt)=nc
      rstk(pt)=607
      rhs=rhs+2
      lhs=1
      icall=4
      fin=insert
c     *call* allops(insert)
      return
 232  li=pstk(pt)
      ndel=ids(1,pt)
      lastindpos=ids(2,pt)
      tref=ids(3,pt)
      l0=ids(4,pt)
      lhs=ids(5,pt)
      nc=ids(6,pt)
      pt=pt-1

c     store the updated value 
      call stackp(istk(li),0)
      if (err .gt. 0 )  return
      if(err1.gt.0) goto 233
c     fin points on the newly saved variable
      if(lct(4).ge.0.and.ip.ne.semi.and.fin.ne.0) then
         call print(istk(li),fin,wte)
      endif

c     remove variable containing the value if required
      if (lastindpos.ne.top) top=top-1
 233  li=li+nsiz+1
      lhs=lhs-1
      if (lhs .gt. 0) goto 231
      top=top-ndel
      lc=li
      goto 10
c     
c     logical expression shortcircuit
 240  continue
      if(istk(lc+1).eq.1) then
c     .  | case
         if(gettype(top).ne.8 .and. istrue(0)) lc=lc+istk(lc+2)
      else
c     .  & case
         if(gettype(top).ne.8 .and..not.istrue(0)) lc=lc+istk(lc+2)
      endif
      lc=lc+3
      goto 10
c
 998  continue
      lhs=0
 999  continue
      if(rstk(pt).ne.501) then
         pt=pt-1
         goto 999
      endif
      fun=0
      return
c     
      end
      subroutine adjustrhs
c     to adjust rhs in the case it is only fixed at run time
c     example  l=list(....); foo(a,l(2:3)).
c     the parser supposes that the rhs for foo is 2. at run time it is
C     really 3. See begrhs
      include '../stack.h'
c     adjust rhs
      if(rstk(pt).eq.617) then
         rhs=rhs+pstk(pt)
         pt=pt-1
      elseif(rstk(pt).eq.501) then
c     retained for 2.4.1 compatiblity 
         rhs=rhs+ids(5,pt)
         ids(5,pt)=0
      endif
      return
      end
