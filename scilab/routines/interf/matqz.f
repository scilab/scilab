      subroutine matqz
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      double precision t,eps
      logical fail,macro
      integer top0,top2,iadr,sadr,tops
      character*(nlgh+1) namef
      common/cschur/namef
      external fschur,bschur
      integer         iero
      common /ierinv/ iero

c     
c     fin    1       2     3        4
c     gschur  gspec  ereduc   fstair
c     
      sadr(l)=(l/2)+1
      iadr(l)=l+l-1
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matqz '//buf(1:4))
      endif
c     
      tops=top
      eps=stk(leps)
c     
      if(fin.ge.3) goto 973
      err=0
      if(fin.eq.1.and.rhs.eq.3) goto 30
c     gschur et spectre
c     
      if(rhs.ne.2) then
         call error(39)
         return
      endif
      ile=iadr(lstk(top))
      if(istk(ile).ne.1) then
         if(fin.eq.1) then
            call putfunnam('gschur',top)
         else
            call putfunnam('gspec',top)
         endif
         fun=-1
         return
      endif
      if(istk(ile+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      me=istk(ile+1)
      ne=istk(ile+2)
      if(me.ne.ne) then
         err=2
         call error(20)
         return
      endif
      le=sadr(ile+4)
c     
      top=top-1
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         if(fin.eq.1) then
            call putfunnam('gschur',top)
         else
            call putfunnam('gspec',top)
         endif
         fun=-1
         top=tops
         return
      endif
      if(istk(il+3).ne.0) then
         err=1
         call error(52)
         return
      endif
      ma=istk(il+1)
      na=istk(il+2)
      if(ma.ne.na) then
         err=1
         call error(20)
         return
      endif
      nn=na*na
      la=sadr(il+4)
c     
      if(na.ne.ne) then
         call error(60)
         return
      endif
c     
      goto(10,20) fin
 10   continue
c     gschur

c     generalized schur form: transformations q,z not required
c-------------------------------------------------------------------
c     Three steps in the reduction of a - s*e:
c     1. qhesz: reduces a to upper hessenberg form and e to upper
c     triangular form
c     2. qitz:  further reduces a to quasi upper triangular form
c     where the 2x2 blocks may correspond to pairs of
c     real eigenvalues
c     3. qvalz: further reduces a so that all 2x2 blocks correspond 
c     to pairs of complex conjugate eigenvalues
      
      if(lhs.ne.2) goto 11
      top=top+1
      lr=le+nn+2
      li=lr+na
      lb=li+na+2
      err=lb+na-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call qhesz(na,na,stk(la),stk(le),.false.,t,.false.,t)
      call qitz(na,na,stk(la),stk(le),eps,.false.,t,.false.,t,
     x     ierr)

      if (ierr.gt.2)  call msgs(1,ierr)

      call qvalz(na,na,stk(la),stk(le),eps,stk(lr),stk(li),
     x     stk(lb),.false.,t,.false.,t)
      return
c     
c     generalized schur form with transformations q,z
c-------------------------------------------------------------------
 11   if(lhs.ne.4) then
         call error(41)
         return
      endif
      lq=le+nn+sadr(5)-1
      lz=lq+nn+sadr(5)-1
      lr=lz+nn+2
      li=lr+na
      lb=li+na+2
      err=lb+na-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call qhesz(na,na,stk(la),stk(le),.true.,stk(lq),.true.,stk(lz))
      call qitz(na,na,stk(la),stk(le),eps,.true.,stk(lq),.true.,
     x     stk(lz),ierr)
c     
      if (ierr.gt.2)  call msgs(1,ierr)
c     
      call qvalz(na,na,stk(la),stk(le),eps,stk(lr),stk(li),
     x     stk(lb),.true.,stk(lq),.true.,stk(lz))


c     affectation de q
      top=top+2
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=na
      istk(il+2)=na
      istk(il+3)=0
      lstk(top+1)=sadr(il+4)+nn
c     
c     affectation de z
      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=na
      istk(il+2)=na
      istk(il+3)=0
      lstk(top+1)=sadr(il+4)+nn
      return

c     
c     generalized spectrum
c---------------------------------------------
 20   continue
      if(na.eq.0) then
         top=top+1
         if(lhs.eq.3) then
            top=top+1
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
         endif
         return
      endif
      if(lhs.eq.3) goto 21
      if(lhs.ne.2) then
         call error(41)
         return
      endif
c     
      lr=le+nn+2
      li=lr+na
      lb=li+na+2
      err=lb+na-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call qhesz(na,na,stk(la),stk(le),.false.,t,.false.,t)
      call qitz (na,na,stk(la),stk(le),eps,.false.,t,.false.
     x     ,t,ierr)

c     
      if (ierr.gt.2)  call msgs(1,ierr)

      call qvalz(na,na,stk(la),stk(le),eps,stk(lr),stk(li),
     x     stk(lb),.false.,t,.false.,t)
c     affectation des resultats
c     numerateur (complexe)
      it=1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=na
      istk(il+2)=1
      istk(il+3)=it
      call unsfdcopy(na*(it+1),stk(lr),1,stk(la),1)
      lstk(top+1)=sadr(il+4)+na*(it+1)
c     denominateur (reel)
      it=0
      top=top+1
      il=iadr(lstk(top))
      l=sadr(il+4)
      istk(il)=1
      istk(il+1)=na
      istk(il+2)=1
      istk(il+3)=it
      call unsfdcopy(na,stk(lb),1,stk(l),1)
      lstk(top+1)=sadr(il+4)+na*(it+1)
      return
c     
c     spectre generalise avec vecteurs propres a droite
c-----------------------------------------------------
 21   continue
      lr=le+nn+2
      li=lr+na
      lb=li+na
      lz=lb+na
      err=lz+nn*2-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call qhesz(na,na,stk(la),stk(le),.false.,t,.true.,stk(lz))
      call qitz (na,na,stk(la),stk(le),eps,.false.,t,.true.
     x     ,stk(lz),err)
      if(err.ne.0) then
         call error(24)
         return
      endif
      call qvalz(na,na,stk(la),stk(le),eps,stk(lr),stk(li),
     x     stk(lb),.false.,t,.true.,stk(lz))
      call qvecz(na,na,stk(la),stk(le),eps,stk(lr),stk(li),
     x     stk(lb),stk(lz))
c     affectation des resultats
c     numerateur (complexe)
      it=1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=na
      istk(il+2)=1
      istk(il+3)=it
      call unsfdcopy(na*(it+1),stk(lr),1,stk(la),1)
      lstk(top+1)=sadr(il+4)+na*(it+1)
c     denominateur (reel)
      it=0
      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      l=sadr(il+4)
      istk(il+1)=na
      istk(il+2)=1
      istk(il+3)=it
      call unsfdcopy(na,stk(lb),1,stk(l),1)
      lstk(top+1)=sadr(il+4)+na*(it+1)
c     vecteurs propres generalises (complexes)
      it=1
      top=top+1
      il=iadr(lstk(top))
      l=sadr(il+4)
      istk(il)=1
      istk(il+1)=na
      istk(il+2)=na
      istk(il+3)=it
      kk=1
      do 22 k=1,na
         lzr=lz+(kk-1)*na
         lzr2=lzr+na
         lzi=lzr+nn
         lzi2=lzi+na
         if(abs(stk(li+kk-1)).le.stk(leps)) goto 24
c     paire complexe
         call unsfdcopy(na,stk(lzr2),1,stk(lzi),1)
         do 23 j=1,na
            stk(lzi2+j-1)=-stk(lzi+j-1)
 23      continue
         call unsfdcopy(na,stk(lzr),1,stk(lzr2),1)
         kk=kk+2
         goto 25
 24      continue
c     cas reel
         call dset(na,0.0d+0,stk(lzi),1)
         kk=kk+1
 25      continue
         if(kk.gt.na) goto 26
 22   continue
 26   continue
      call unsfdcopy(nn*(it+1),stk(lz),1,stk(l),1)
      lstk(top+1)=sadr(il+4)+nn*(it+1)
      return

c     
c     stabl :   sous espace de deflation stable a droite
c--------------------------------------------------------
 30   continue
      top0=top+1-rhs
      if(rhs.ne.3) then
         call error(39)
         return
      endif
      if(lhs.ne.2.and.lhs.ne.4) then 
         call error(59)
         return
      endif
      ilcd=iadr(lstk(top))
      if(istk(ilcd).gt.10) then
         macro=.true.
      elseif(istk(ilcd).eq.10) then
         macro=.false.
         if(istk(ilcd+1)*istk(ilcd+2).ne.1) then
            err=3
            call error(89)
            return
         endif
         nc=istk(ilcd+5)-1
         namef=' '
         call cvstr(nc,istk(ilcd+6),namef,1)
         namef(nc+1:nc+1)=char(0)
         call setfschur(namef,irep)
         if ( irep.eq.1) then 
            buf = namef
            call error(50)
            return
         endif
         top=top-1
         icd=abs(istk(ilcd+6))
      else
         err=rhs
         call error(44)
      endif
      top2=top
      lw=lstk(top+1)

c     acquisition de la matrice   e
      ile=iadr(lstk(top0+1))
      if(istk(ile).ne.1.or.istk(ile+3).ne.0) then
         if(fin.eq.1) then
            call putfunnam('gschur',top0+1)
         else
            call putfunnam('gspec',top0+1)
         endif
         fun=-1
         top=tops
         return
      endif
      me=istk(ile+1)
      ne=istk(ile+2)
      if(me.ne.ne) then
         err=2
         call error(20)
         return
      endif
      le=sadr(ile+4)
c     
c     acquisition de la matrice a
      ila=iadr(lstk(top0))
      if(istk(ila).ne.1.or.istk(ila+3).ne.0) then
         if(fin.eq.1) then
            call putfunnam('gschur',top0)
         else
            call putfunnam('gspec',top0)
         endif
         fun=-1
         top=tops
         return
      endif
      ma=istk(ila+1)
      na=istk(ila+2)
      if(ma.ne.na) then
         err=1
         call error(20)
         return
      endif
      nn=na*na
      ita=istk(ila+3)
      la=sadr(ila+4)
c     
c     test de coherence des tailles
      if(ma.ne.me) then
         call error(60)
         return
      endif
c     
c     on implante les tableau resultats et de travail
      ilz=iadr(lw)
      lz=sadr(ilz+4)
      ild=iadr(lz+nn)
      ld=sadr(ild+4)
      lr=ld+1
      li=lr+na
      lb=li+na
      ilw=iadr(lb+na)
      lfree=sadr(ilw+na)
c     
c     verification de l'existence de l'espace necessaire
      err=lfree - lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
c     appel du programme
      call qhesz(na,na,stk(la),stk(le),.false.,t,.true.,stk(lz))
      call qitz (na,na,stk(la),stk(le),eps,.false.,t,.true.
     x     ,stk(lz),ierr)
c     
      if (ierr.gt.2) call msgs(1,ierr)

      call qvalz(na,na,stk(la),stk(le),eps,stk(lr),stk(li),
     x     stk(lb),.false.,t,.true.,stk(lz))

      if(.not.macro) then
         call dsubsp(na,na,stk(la),stk(le),stk(lz),fschur
     x        ,eps,ndim,fail,istk(ilw))
      else
c     on ferme le tableau de travail...
         lstk(top+1)=lfree
c     creation d'une variable bidon de type scalaire pour stockage de la
c     valeur retournee par l'external
         top=top+1
         il9=iadr(lstk(top))
         istk(il9)=1
         istk(il9+1)=3
         istk(il9+2)=1
         istk(il9+3)=0
         lvar=sadr(il9+4)
         kvtop=top
         lstk(top+1)=lvar+3
c     creation d'une structure pour l'external
         top=top+1
         ilwe=iadr(lstk(top))
         istk(ilwe)=1
         istk(ilwe+1)=ilwe+2
         istk(ilwe+2)=top2
         istk(ilwe+3)=kvtop
         lstk(top+1)=lstk(top)+3
         call dsubsp(na,na,stk(la),stk(le),stk(lz),bschur
     x        ,eps,ndim,fail,istk(ilw))
         if(iero.ne.0) then
            err=1
            return
         endif
      endif
      if(fail) then
         call error(24)
         return
      endif
      top=top0+1
      if(lhs.eq.2) then
         call unsfdcopy(nn,stk(lz),1,stk(la),1)
         stk(le)=ndim
         istk(ile+1)=1
         istk(ile+2)=1
         return
      endif
c     creation de la variable z
      top=top+1
      ilz=iadr(lstk(top))
      istk(ilz)=1
      istk(ilz+1)=na
      istk(ilz+2)=na
      istk(ilz+3)=0
      l=sadr(ilz+4)
      if(macro) call unsfdcopy(na*na,stk(lz),1,stk(l),1)
      lstk(top+1)=l+nn
c     creation de la variable d
      top=top+1
      ild=iadr(lstk(top))
      istk(ild)=1
      istk(ild+1)=1
      istk(ild+2)=1
      istk(ild+3)=0
      ld=sadr(ild+4)
      stk(ld)=ndim
      lstk(top+1)=ld+1
      return
 973  continue
      if (fin .eq.3) then
c     
c     SCILAB function : ereduc
c     --------------------------
         lw = lstk(top+1)
         l0 = lstk(top+1-rhs)
         if (rhs .ne. 2) then
            call error(39)
            return
         endif
         if (lhs .gt. 5) then
            call error(41)
            return
         endif
c     checking variable e (number 1)
c     
         il1 = iadr(lstk(top-rhs+1))

         if (istk(il1) .ne. 1) then
            call putfunnam('ereduc',top-rhs+1)
            fun=-1
            return
         endif
         n1 = istk(il1+1)
         m1 = istk(il1+2)
         l1 = sadr(il1+4)
c     checking variable tol (number 2)
c     
         il2 = iadr(lstk(top-rhs+2))
         if (istk(il2) .ne. 1) then
            err = 2
            call error(53)
            return
         endif
         if (istk(il2+1)*istk(il2+2) .ne. 1) then
            err = 2
            call error(89)
            return
         endif
         l2 = sadr(il2+4)
c     
c     cross variable size checking
c     
c     
c     cross formal parameter checking
c     not implemented yet
c     
c     cross equal output variable checking
c     not implemented yet
         lw4=lw
         lw=lw+n1*n1
         lw5=lw
         lw=lw+m1*m1
         lw6=lw
         lw=lw+n1
         lw7=lw
         lw=lw+1
         err=lw-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     
         call ereduc(stk(l1),n1,m1,stk(lw4),stk(lw5),stk(lw6),stk(lw7)
     $        ,stk(l2))
         if (err .gt. 0) return
c     
         top=top-rhs
         lw0=lw
         mv=lw0-l0
c     
         if(lhs .ge. 1) then
c     
c     output variable: e
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+n1*m1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il1),1,istk(ilw),1)
            lw=sadr(ilw+4)
            call unsfdcopy(n1*m1,stk(l1),1,stk(lw),1)
            lw=lw+n1*m1
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 2) then
c     
c     output variable: q
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+n1*n1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=n1
            istk(ilw+2)=n1
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call unsfdcopy(n1*n1,stk(lw4),1,stk(lw),1)
            lw=lw+n1*n1
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 3) then
c     
c     output variable: z
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+m1*m1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=m1
            istk(ilw+2)=m1
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call unsfdcopy(m1*m1,stk(lw5),1,stk(lw),1)
            lw=lw+m1*m1
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 4) then
c     
c     output variable: stair
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+n1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=n1
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call int2db(n1,stk(lw6),1,stk(lw),1)
            lw=lw+n1
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 5) then
c     
c     output variable: rk
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+5-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=1
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call int2db(1,stk(lw7),1,stk(lw),1)
            lw=lw+1
            lstk(top+1)=lw-mv
         endif
c     
c     putting in order the stack
c     
         call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
         return
      endif
c     
      if (fin .eq. 4) then
c     
c     SCILAB function : fstair
c     --------------------------
         lw = lstk(top+1)
         l0 = lstk(top+1-rhs)
         if (rhs .ne. 7) then
            call error(39)
            return
         endif
         if (lhs .gt. 10) then
            call error(41)
            return
         endif
c     checking variable a (number 1)
c     
         il1 = iadr(lstk(top-rhs+1))
         if (istk(il1) .ne. 1) then
            if (istk(il1) .ne. 1) then
               call putfunnam('fstair',top-rhs+1)
               fun=-1
               return
            endif
         endif
         n1 = istk(il1+1)
         m1 = istk(il1+2)
         l1 = sadr(il1+4)
c     checking variable e (number 2)
c     
         il2 = iadr(lstk(top-rhs+2))
         if (istk(il2) .ne. 1) then
            call putfunnam('fstair',top-rhs+2)
            fun=-1
            return
         endif
         n2 = istk(il2+1)
         m2 = istk(il2+2)
         l2 = sadr(il2+4)
c     checking variable q (number 3)
c     
         il3 = iadr(lstk(top-rhs+3))
         if (istk(il3) .ne. 1) then
            call putfunnam('fstair',top-rhs+3)
            fun=-1
            return
         endif
         if (istk(il3+1) .ne. istk(il3+2)) then
            err = 3
            call error(20)
            return
         endif
         n3 = istk(il3+1)
         m3 = istk(il3+2)
         l3 = sadr(il3+4)
c     checking variable z (number 4)
c     
         il4 = iadr(lstk(top-rhs+4))
         if (istk(il4) .ne. 1) then
            call putfunnam('fstair',top-rhs+4)
            fun=-1
            return
         endif
         if (istk(il4+1) .ne. istk(il4+2)) then
            err = 4
            call error(20)
            return
         endif
         n4 = istk(il4+1)
         m4 = istk(il4+2)
         l4 = sadr(il4+4)
c     checking variable stair (number 5)
c     
         il5 = iadr(lstk(top-rhs+5))
         if (istk(il5) .ne. 1) then
            call putfunnam('fstair',top-rhs+5)
            fun=-1
            return
         endif
         if (istk(il5+1) .ne. 1) then
            err = 5
            call error(89)
            return
         endif
         m5 = istk(il5+2)
         l5 = sadr(il5+4)
c     checking variable rk (number 6)
c     
         il6 = iadr(lstk(top-rhs+6))
         if (istk(il6) .ne. 1) then
            err = 6
            call error(53)
            return
         endif
         if (istk(il6+1)*istk(il6+2) .ne. 1) then
            err = 6
            call error(89)
            return
         endif
         l6 = sadr(il6+4)
c     checking variable tol (number 7)
c     
         il7 = iadr(lstk(top-rhs+7))
         if (istk(il7) .ne. 1) then
            err = 7
            call error(53)
            return
         endif
         if (istk(il7+1)*istk(il7+2) .ne. 1) then
            err = 7
            call error(89)
            return
         endif
         l7 = sadr(il7+4)
c     
c     cross variable size checking
c     
         if (n1 .ne. n2) then
            call error(42)
            return
         endif
         if (n1 .ne. n3) then
            call error(42)
            return
         endif
         if (n1 .ne. m3) then
            call error(42)
            return
         endif
         if (n1 .ne. m5) then
            call error(42)
            return
         endif
         if (m1 .ne. m2) then
            call error(42)
            return
         endif
         if (m1 .ne. n4) then
            call error(42)
            return
         endif
         if (m1 .ne. m4) then
            call error(42)
            return
         endif
c     
c     cross formal parameter checking
c     not implemented yet
c     
c     cross equal output variable checking
c     not implemented yet
         call entier(m5,stk(l5),istk(iadr(l5)))
         call entier(1,stk(l6),istk(iadr(l6)))
         lw10=lw
         lw=lw+1
         lw11=lw
         lw=lw+m1
         nn12=n1+1
         lw12=lw
         lw=lw+nn12
         lw13=lw
         lw=lw+m1
         lw14=lw
         lw=lw+nn12
         nn15=4
         lw15=lw
         lw=lw+nn15
         lw16=lw
         lw=lw+m1
         lw17=lw
         lw=lw+m1

         err=lw-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     
         call fstair(stk(l1),stk(l2),stk(l3),stk(l4),n1,m1
     $        ,istk(iadr(l5)),istk(iadr(l6)),stk(l7),stk(lw10)
     $        ,stk(lw11),stk(lw12),stk(lw13),stk(lw14),stk(lw15)
     $        ,stk(lw16),stk(lw17),ierr)
         if (ierr.ne.0) then
            buf='fstair failed !'
            call error(9999)
            return
         endif
c     
         top=top-rhs
         lw0=lw
         mv=lw0-l0
c     
         if(lhs .ge. 1) then
c     
c     output variable: a
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+n1*m1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il1),1,istk(ilw),1)
            lw=sadr(ilw+4)
            call unsfdcopy(n1*m1,stk(l1),1,stk(lw),1)
            lw=lw+n1*m1
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 2) then
c     
c     output variable: e
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+n1*m1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il2),1,istk(ilw),1)
            lw=sadr(ilw+4)
            call unsfdcopy(n2*m2,stk(l2),1,stk(lw),1)
            lw=lw+n2*m2
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 3) then
c     
c     output variable: q
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+n1*n1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il3),1,istk(ilw),1)
            lw=sadr(ilw+4)
            call unsfdcopy(n3*m3,stk(l3),1,stk(lw),1)
            lw=lw+n3*m3
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 4) then
c     
c     output variable: z
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+m1*m1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il4),1,istk(ilw),1)
            lw=sadr(ilw+4)
            call unsfdcopy(n4*m4,stk(l4),1,stk(lw),1)
            lw=lw+n4*m4
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 5) then
c     
c     output variable: nblcks
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+5-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=1
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call int2db(1,stk(lw10),1,stk(lw),1)
            lw=lw+1
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 6) then
c     
c     output variable: muk
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+m1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=m1
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call int2db(m1,stk(lw11),1,stk(lw),1)
            lw=lw+m1
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 7) then
c     
c     output variable: nuk
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+nn12-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=nn12
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call int2db(nn12,stk(lw12),1,stk(lw),1)
            lw=lw+nn12
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 8) then
c     
c     output variable: muk0
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+m1-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=m1
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call int2db(m1,stk(lw13),1,stk(lw),1)
            lw=lw+m1
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 9) then
c     
c     output variable: nuk0
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+nn12-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=nn12
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call int2db(nn12,stk(lw14),1,stk(lw),1)
            lw=lw+nn12
            lstk(top+1)=lw-mv
         endif
c     
         if(lhs .ge. 10) then
c     
c     output variable: mnei
c     
            top=top+1
            ilw=iadr(lw)
            err=lw+4+nn15-lstk(bot)
            if (err .gt. 0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=nn15
            istk(ilw+2)=1
            istk(ilw+3)=0
            lw=sadr(ilw+4)
            call int2db(nn15,stk(lw15),1,stk(lw),1)
            lw=lw+nn15
            lstk(top+1)=lw-mv
         endif
c     
c     putting in order the stack
c     
         call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
         return
      endif
c     
      end


