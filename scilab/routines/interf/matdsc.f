      subroutine matdsc
C ====================================================================
C
C     evaluate functions involving   eigenvalues and eigenvectors
C
C ====================================================================
C
c     Copyright INRIA
      include '../stack.h'
C
      double precision sr,si,powr,powi,t,rmax,eps,tt(1,1)
      logical herm,vect,fail
      integer iadr,sadr
C
      sadr(l) = (l/2) + 1
      iadr(l) = l + l - 1
C
      if (ddt .eq. 4) then
        write (buf(1:4),'(i4)') fin
        call basout(io,wte,' matdsc '//buf(1:4))
      endif
C
C     functions/fin
C     1       2       3       4       5       6      
C 0 hess    schur  spectre  blocdia         balanc
C
      if (top+lhs-rhs .ge. bot) then
        call error(18)
        return
      endif
      if (rhs .le. 0) then
        call error(39)
        return
      endif
C
      if (istk(iadr(lstk(top+1-rhs))) .ne. 1) then
         if(fin.eq.1) then
            call putfunnam('hess',top-rhs+1)
         elseif(fin.eq.2) then
            call putfunnam('schur',top-rhs+1)
         elseif(fin.eq.3) then
            call putfunnam('spec',top-rhs+1)
         elseif(fin.eq.4) then
            call putfunnam('bdiag',top-rhs+1)
         elseif(fin.eq.6) then
            call putfunnam('balanc',top-rhs+1)
         else
            err = 1
            call error(53)
            return
         endif
         fun=-1
         return
      endif
C
      lw = lstk(top+1)
      eps = stk(leps)
C
      if (fin .eq. 6) goto 310
C
      if (fin.eq.2 .and. rhs.eq.2) then
        call error(43)
        return
      endif
C
      vect = (lhs.eq.2.and.fin.ne.3)
      it2 = 0
      if (rhs .eq. 1) goto 5
      if (rhs.lt.1.or.rhs.gt.2.or.fin.ne.2.and.fin.ne.4) then 
         call error(39)
         return
      endif
      il = iadr(lstk(top))
      if (istk(il+1)*istk(il+2) .ne. 1) then
        call error(30)
        return
      endif
      l = sadr(il+4)
      it2 = istk(il+3)
      powi = 0.0d+0
      powr = stk(l)
      if (it2 .eq. 1) powi = stk(l+1)
      top = top - 1
 5    continue
C acquisition des parametre de la matrice
      il = iadr(lstk(top))
      m = istk(il+1)
      n = istk(il+2)
      l = sadr(il+4)
      mn = m * n
      if (mn .ne. 0) goto 6
C
C     matrice de taille nulle
C
      if (fin.ne.3 .or. lhs.gt.1) then
        err = 1
        call error(89)
        return
      endif
      return
C
 6    continue
C
C
C test si la matrice est carree
      ld = l
      if (m .ne. n) then
        err = 1
        call error(20)
        return
      endif
      nn = n * n
      if (fin .eq. 4) goto 200
C
C decomposition spectrale de la matrice
C
C la matrice est-elle symetrique?
      herm = .false.
      if (n .eq. 1) goto 21
      do 20 j = 2,n
        j1 = j - 1
        do 20 i = 1,j1
          ls = l + (i-1) + j1*n
          ll = l + (i-1)*n + j1
          sr = abs(stk(ll)-stk(ls))
          si = abs(stk(ll+nn)+stk(ls+nn))
          if (stk(ll)+sr.gt.stk(ll) .or. stk(ll+nn)+si.gt.stk(ll+nn))
     &      goto 23
 20   continue
 21   do 22 j = 1,n
        ll = l + (j-1) + (j-1)*n
        if (stk(ll)+abs(stk(ll+nn)) .gt. stk(ll)) goto 23
 22   continue
      herm = .true.
 23   continue
      if (herm) goto 100
      if (fin .gt. 3) goto 900
C
C equilibrage
      low = 1
      igh = n
      if (fin .ne. 3) goto 24
      lw = l + nn + nn
      err = lw + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      call cbal(n,n,stk(l),stk(l+nn),low,igh,stk(lw))
C
C calcul de la forme de hessenberg
 24   lv = l
      if (vect) l = lstk(top+1)
      if (lhs .eq. 1) goto 25
C on cree une nouvelle variable
      top = top + 1
      il = iadr(lstk(top))
      istk(il) = 1
      istk(il+1) = n
      istk(il+2) = n
      istk(il+3) = 1
      l = sadr(il+4)
      lstk(top+1) = l + nn*2
 25   continue
      lw = l + nn*2
      err = lw + n*2 - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      if (vect) call unsfdcopy(nn*2,stk(lv),1,stk(l),1)
      call corth(n,n,low,igh,stk(l),stk(l+nn),stk(lw),stk(lw+n))
      if (vect)
     &  call cortr(n,n,low,igh,stk(l),stk(l+nn),stk(lw),stk(lw+n),
     &             stk(lv),stk(lv+nn))
      if (fin .ne. 1) goto 40
C fin hess
      if (n .lt. 3) goto 31
      do 30 j = 3,n
        call dset(j-2,0.0d+0,stk(l+j-1),n)
 30   call dset(j-2,0.0d+0,stk(l+nn+j-1),n)
 31   continue
      goto 999
C
C calcul de la forme de schur
 40   job = 0
      if (vect) job = 1
      lsr = lw
      lsi = lw
      if (fin.eq.2 .or. fin.eq.3) job = job + 10
      lsi = lsr + n
      err = lsi + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      call comqr3(n,n,low,igh,stk(l),stk(l+nn),stk(lsr),stk(lsi),
     &            stk(lv),stk(lv+nn),ierr,job)
      if (ierr .gt. 1) call msgs(2,ierr)
C
      if (fin .eq. 3) goto 44
C
C fin schur
      if (n .lt. 2) goto 999
      do 42 i = 2,n
        call dset(i-1,0.0d+0,stk(l-1+i),n)
        call dset(i-1,0.0d+0,stk(l+nn-1+i),n)
 42   continue
      goto 999
C
 44   continue
C fin spectre et root
      call unsfdcopy(2*n,stk(lsr),1,stk(l),1)
      istk(il+1) = n
      istk(il+2) = 1
      istk(il+3) = 1
      lstk(top+1) = l + 2*n
      goto 999
C
C fin cas general
C cas d'une matrice hermitienne
 100  continue
C calcul de la forme de hessenberg(tridagonale)
      lv = l
      if (vect) l = lstk(top+1)
      if (lhs .eq. 1) goto 108
C     on cree une nouvelle variable
      top = top + 1
      il = iadr(lstk(top))
      istk(il) = 1
      istk(il+1) = n
      istk(il+2) = n
      l = sadr(il+4)
      istk(il+3) = 0
      lstk(top+1) = l + n*n
 108  continue
      ld = l + nn*2
      le = ld + n
      le2 = le + n
      lw = le2 + n
      err = lw + 2*n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      if (vect) call unsfdcopy(nn*2,stk(lv),1,stk(l),1)
      call htridi(n,n,stk(l),stk(l+nn),stk(ld),stk(le),stk(le2),stk(lw))
      if (fin .ne. 1) goto 120
C fin hess
      if (.not. vect) goto 109
      call dset(nn,0.0d+0,stk(lv),1)
      call dset(n,1.0d+0,stk(lv),n+1)
      call htribk(n,n,stk(l),stk(l+nn),stk(lw),n,stk(lv),stk(lv+nn))
 109  istk(il+3) = 0
      lstk(top+1) = l + nn
      call dset(nn,0.0d+0,stk(l),1)
      call unsfdcopy(n,stk(ld),1,stk(l),n+1)
      if (n .le. 1) goto 999
      call unsfdcopy(n-1,stk(le+1),1,stk(l+1),n+1)
      call unsfdcopy(n-1,stk(le+1),1,stk(l+n),n+1)
      goto 999
C
C calcul de la forme diagonale
 120  continue
      job = 0
      if (.not. vect) goto 121
      job = 1
      call dset(nn,0.0d+0,stk(lv),1)
      call dset(n,1.0d+0,stk(lv),n+1)
 121  continue
      if (vect) job = 1
      call imtql3(n,n,stk(ld),stk(le),stk(lv),ierr,job)
      if (ierr .gt. 1) call msgs(2,ierr)
      if (vect)
     &  call htribk(n,n,stk(l),stk(l+nn),stk(lw),n,stk(lv),stk(lv+nn))
      mn = n
C
      if (fin .eq. 3) goto 123
C
C fin schur et jordan
      call dset(nn,0.0d+0,stk(l),1)
      call unsfdcopy(n,stk(ld),1,stk(l),n+1)
      istk(il+3) = 0
      lstk(top+1) = l + nn
      goto 999
C
 123  continue
C fin spectre
      if (lhs .ne. 1) then
        call error(41)
        return
      endif
      call unsfdcopy(n,stk(ld),1,stk(l),1)
      istk(il+1) = n
      istk(il+2) = 1
      istk(il+3) = 0
      lstk(top+1) = l + n
      goto 999
C
C bloc diagonalisation
C
 200  continue
      if (rhs .gt. 2) then
        call error(39)
        return
      endif
      if (rhs .eq. 1) goto 201
C     rmax est en argument
      rmax = powr
      if (powi .ne. 0.0d+0) then
        err = 2
        call error(52)
        return
      endif
      goto 202
C   calcul de rmax par defaut:norme l1
 201  rmax = 0.0d+0
      lj = l - 1
      do 203 j = 1,n
        t = 0.0d+0
        do 204 i = 1,n
          t = t + abs(stk(lj+i)) + abs(stk(lj+nn+i))
 204    continue
        if (t .gt. rmax) rmax = t
        lj = lj + n
 203  continue
 202  continue
C     preparation de la pile
      top = top + 1
C
C     changement de base
      ilx = iadr(lstk(top))
      istk(ilx) = 1
      istk(ilx+1) = n
      istk(ilx+2) = n
      istk(ilx+3) = 1
      lx = sadr(ilx+4)
      lstk(top+1) = lx + 2*nn
C    structure des blocs
      top = top + 1
      ilbs = iadr(lstk(top))
      lbs = sadr(ilbs+4)
      illbs = ilbs + 4
C    er,ei:valeurs propres (tbl de travail)
      ler = lbs + n
      lei = ler + n
      ilb = iadr(lei+n)
      lw = sadr(ilb+n)
      err = lw + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      call wbdiag(n,n,stk(l),stk(l+nn),rmax,stk(ler),stk(lei),istk(ilb),
     &            stk(lx),stk(lx+nn),tt,tt,stk(lw),0,fail)
C     sorties
C   structure des blocs
      nbloc = 0
      ln = lbs - 1
      do 222 k = 1,n
        if (istk(ilb+k-1) .lt. 0) goto 222
        nbloc = nbloc + 1
        ln = ln + 1
        stk(ln) = dble(istk(ilb+k-1))
 222  continue
      lstk(top+1) = sadr(ilbs+4) + nbloc
      istk(ilbs) = 1
      istk(ilbs+1) = nbloc
      istk(ilbs+2) = 1
      istk(ilbs+3) = 0
      if (lhs .eq. 2) top = top - 1
      if (lhs .eq. 1) top = top - 2
      goto 999
C
C equilibrage (balanc)
C
 310  continue
      if (lhs .ne. 2) then
        call error(41)
        return
      endif
      if (rhs .ne. 1) then
        call error(42)
        return
      endif
      il = iadr(lstk(top))
      m = istk(il+1)
      n = istk(il+2)
      it = istk(il+3)
      l = sadr(il+4)
C     test si la matrice est carree
      if (m .ne. n) then
        err = 1
        call error(20)
        return
      endif
      nn = n * n
      if (nn .eq. 0) then
        err = 1
        call error(89)
        return
      endif
C     equilibrage
      low = 1
      igh = n
      ilv = iadr(lw)
      lv = sadr(ilv+4)
      lw = lv + nn
      err = lw + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      call cbal(n,n,stk(l),stk(l+nn),low,igh,stk(lw))
      call dset(nn,0.0d+0,stk(lv),1)
      call dset(n,1.0d+0,stk(lv),n+1)
      call balbak(n,n,low,igh,stk(lw),n,stk(lv))
      istk(ilv) = 1
      istk(ilv+1) = n
      istk(ilv+2) = n
      istk(ilv+3) = 0
      top = top + 1
      lstk(top+1) = lv + nn
      goto 999
C
 999  return
 900  call error(43)
      return
      end

