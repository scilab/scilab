      subroutine matdsr
C
C ====================================================================
C
C     evaluate functions involving eigenvalues and eigenvectors
C
C ====================================================================
C
c     Copyright INRIA
      include '../stack.h'
C
      double precision sr,t,rmax,tt(1,1)
      double precision eps
      logical herm,vect,fail,chain,macro
      integer fschur,bschur
      external fschur, bschur
      integer top2,tope,topf
      integer iadr,sadr
      character*(nlgh+1) namef
      common /cschur/ namef
      integer iero
      common /ierinv/ iero
C
      sadr(l) = (l/2) + 1
      iadr(l) = l + l - 1
C
      if (ddt .eq. 4) then
        write (buf(1:4),'(i4)') fin
        call basout(io,wte,' matdsr '//buf(1:4))
      endif
C
C     functions/fin
C        1       2       3       4       5       6       7       8
C     0 hess    schur  spec  bdiag         balanc
      if (top+lhs-rhs .ge. bot) then
        call error(18)
        return
      endif
      if (rhs .le. 0) then
        call error(39)
        return
      endif
C
      lw = lstk(top+1)
      eps = stk(leps)
      tope = top - rhs + 1
      if (istk(iadr(lstk(tope))) .ne. 1) then
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
      if (fin .eq. 6) goto 310
C
      ireg = 0
      vect = (lhs.ge.2.and.fin.ne.3)
      if (rhs .eq. 1) goto 5
      if (rhs.lt.1.or.rhs.gt.2.or.fin.ne.2.and.fin.ne.4) then 
         call error(39)
         return
      endif
      il = iadr(lstk(top))
      mn2 = istk(il+1) * istk(il+2)
      if (istk(il).eq.10 .or. istk(il).eq.11 .or. istk(il).eq.13) goto 1
      if (istk(il) .ne. 1) then
        err = rhs
        call error(44)
        return
      endif
      it2 = istk(il+3)
      l2 = sadr(il+4)
      top = top - 1
      goto 5
 1    continue
C schur ordonne
      ireg = 1
      if (lhs.ne.2 .and. lhs.ne.3) then
        call error(41)
        return
      endif
      chain = .false.
      macro = .false.
      top2 = top
      topf = top - rhs + lhs
C      
      if (istk(il) .gt. 10) then
        macro = .true.
      else
        chain = .true.
        nc = istk(il+5) - 1
        namef = ' '
        call cvstr(nc,istk(il+5+mn2),namef,1)
        namef(nc+1:nc+1)=char(0)
        call setfschur(namef,irep)
        if ( irep.eq.1) then 
           buf = namef
           call error(50)
           return
        endif
        top = top - 1
      endif
 5    continue
C acquisition des parametre de la matrice
      il = iadr(lstk(tope))
      m = istk(il+1)
      n = istk(il+2)
      l = sadr(il+4)
      mn = m * n
      if (mn .ne. 0) goto 6
C     matrice de taille nulle
      if (fin.ne.3 .or. lhs.gt.1) then
        err = 1
        call error(89)
        return
      endif
      top = tope
      return
C
C test si la matrice est carree
C
 6    ld = l
      if (m .ne. n) then
        err = 1
        call error(20)
        return
      endif
      nn = n * n
C
      if (fin .eq. 4) goto 200
C
C ... decomposition spectrale de la matrice
C
C la matrice est-elle symetrique?
C
      herm = .false.
      if (ireg .ne. 0) goto 21
      do 20 j = 1,n
        do 19 i = 1,j
          ls = l + (i-1) + (j-1)*n
          ll = l + (i-1)*n + (j-1)
          sr = abs(stk(ll)-stk(ls))
          if (stk(ll)+sr .gt. stk(ll)) goto 21
 19     continue
 20   continue
      herm = .true.
 21   continue
      if (herm) goto 100
C
      if (fin .gt. 3) goto 900
C
C equilibrage
C
      low = 1
      igh = n
      if (fin .ne. 3) goto 22
      err = lw + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      call balanc(n,n,stk(l),low,igh,stk(lw))
C
C calcul de la forme de hessenberg
C
 22   lv = l
      if (vect) l = lw
      if (lhs .eq. 3) then
C     on cree la  variable d
        top = top + 1
        ild = iadr(lstk(top))
        istk(ild) = 1
        istk(ild+1) = 1
        istk(ild+2) = 1
        istk(ild+3) = 0
        ld = sadr(ild+4)
        lstk(top+1) = ld + 1
      endif
      if (lhs .gt. 1) then
C     on cree la  variable s
        top = top + 1
        il = iadr(lstk(top))
        istk(il) = 1
        istk(il+1) = n
        istk(il+2) = n
        istk(il+3) = 0
        l = sadr(il+4)
        lstk(top+1) = l + nn
      endif
C
      lw = l + nn
      err = lw + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      if (vect) call unsfdcopy(nn,stk(lv),1,stk(l),1)
      call orthes(n,n,low,igh,stk(l),stk(lw))
      if (vect) call ortran(n,n,low,igh,stk(l),stk(lw),stk(lv))
      if (fin .ne. 1) goto 40
C fin hess
      if (n .ge. 3) then
        do 30 j = 3,n
          call dset(j-2,0.0d+0,stk(l+j-1),n)
 30     continue
      endif
      goto 999
C
C calcul de la forme de schur
C
 40   job = 10
      if (vect) job = 11
      lsr = lw
      lsi = lw
      if (fin .eq. 3) then
        job = job + 10
        lsi = lsr + n
      endif
      err = lsi + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      call hqror2(n,n,low,igh,stk(l),stk(lsr),stk(lsi),stk(lv),ierr,job)
C
      if (ierr .gt. 1) call msgs(2,ierr)
C
      if (ireg .eq. 0) goto 42
C
C schur ordonne
C
      if (chain) then
        call inva(n,n,stk(l),stk(lv),fschur,eps,ndim,fail,istk(iadr(lw))
     &           )
      elseif (macro) then
C     on ferme le tableau de travail...
        lwn = lw + n
        err = lwn - lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
        lstk(top+1) = lwn
C     creation d'une variable bidon de type scalaire pour stockage de la
C     valeur retournee par l'external
        top = top + 1
        il9 = iadr(lstk(top))
        istk(il9) = 1
        istk(il9+1) = 3
        istk(il9+2) = 1
        istk(il9+3) = 0
        lvar = sadr(il9+4)
        kvtop = top
        lstk(top+1) = lvar + 3
C     creation d'une structure pour l'external
        top = top + 1
        ilw = iadr(lstk(top))
        istk(ilw) = 1
        istk(ilw+1) = ilw + 2
        istk(ilw+2) = top2
        istk(ilw+3) = kvtop
        lstk(top+1) = lstk(top) + 3
        call inva(n,n,stk(l),stk(lv),bschur,eps,ndim,fail,istk(iadr(lw))
     &           )
        if (iero .ne. 0) then
          err = 1
          return
        endif
        top = top - 3
      endif
      if (fail) then
        call msgs(2,0)
        call error(24)
        return
      endif
      if (n .ge. 3) then
        do 41 i = 3,n
          call dset(i-2,0.0d+0,stk(l-1+i),n)
 41     continue
      endif
      if (lhs .eq. 2) then
        il = iadr(lstk(top))
        istk(il) = 1
        istk(il+1) = 1
        istk(il+2) = 1
        istk(il+3) = 0
        l = sadr(il+4)
        stk(l) = dble(ndim)
        lstk(top+1) = l + 1
      elseif (lhs .eq. 3) then
        stk(ld) = dble(ndim)
      endif
      goto 999
C
 42   continue
      if (fin .eq. 3) goto 44
C fin schur
      if (lhs .gt. 2) then
        call error(41)
        return
      endif
      if (n .ge. 3) then
        do 43 i = 3,n
          call dset(i-2,0.0d+0,stk(l-1+i),n)
 43     continue
      endif
      goto 999
C
 44   continue
C fin spectre et root
      if (lhs .ne. 1) then
        call error(41)
        return
      endif
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
      l1 = l
      if (vect) l = lw
      if (lhs .eq. 1) goto 101
C     on cree une variable
      top = top + 1
      il = iadr(lstk(top))
      istk(il) = 1
      istk(il+1) = n
      istk(il+2) = n
      istk(il+3) = 0
      l = sadr(il+4)
      lstk(top+1) = l + nn
C
 101  if(vect) then
         job=1
      else
         lv=l1
         job=0
      endif
      ld = l + nn
      le = ld + n
      err = le + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      call tred2(n,n,stk(l1),stk(ld),stk(le),stk(lv))
      if (fin .ne. 1) goto 120
C fin hess
      call dset(nn,0.0d+0,stk(l),1)
      call unsfdcopy(n,stk(ld),1,stk(l),n+1)
      if (n .le. 1) goto 999
      call unsfdcopy(n-1,stk(le+1),1,stk(l+1),n+1)
      call unsfdcopy(n-1,stk(le+1),1,stk(l+n),n+1)
      goto 999
C
C calcul de la forme diagonale
 120  continue
      call tql2(n,n,stk(ld),stk(le),stk(lv),job,ierr)
C
      if (ierr .gt. 1) call msgs(2,ierr)
      mn = n
C
      if (fin .eq. 3) goto 121
C
C fin schur , jordan et bdiag
      call dset(nn,0.0d+0,stk(l),1)
      call unsfdcopy(n,stk(ld),1,stk(l),n+1)
      goto 999
C
 121  continue
C fin spectre
      call unsfdcopy(n,stk(ld),1,stk(l),1)
      istk(il+1) = n
      istk(il+2) = 1
      lstk(top+1) = l + n
      goto 999
C
C     bloc diagonalisation
C
 200  continue
      if (rhs .gt. 2) then
        call error(39)
        return
      endif
      if (rhs .eq. 1) goto 201
C     rmax est en argument
      rmax = stk(l2)
      if (it2 .eq. 1) then
        err = 1
        call error(52)
        return
      endif
      goto 202
C     calcul de rmax par defaut:norme l1
 201  rmax = 1.0d+0
      lj = l - 1
      do 203 j = 1,n
        t = 0.0d+0
        do 204 i = 1,n
          t = t + abs(stk(lj+i))
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
      istk(ilx+3) = 0
      lx = sadr(ilx+4)
      lstk(top+1) = lx + nn
C     structure des blocs
      top = top + 1
      ilbs = iadr(lstk(top))
      lbs = sadr(ilbs+4)
C     er,ei:valeurs propres (tbl de travail)
      ler = lbs + n
      lei = ler + n
      ilb = iadr(lei+n)
      lw = sadr(ilb+n)
      err = lw + n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
      call bdiag(n,n,stk(l),0.d0,rmax,stk(ler),stk(lei),istk(ilb),
     &           stk(lx),tt,stk(lw),0,fail)
C
      if (fail) then
        call msgs(2,0)
        call error(24)
        return
      endif
C
C     sorties
C structure des blocs
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
      if (rhs .eq. 2) goto 320
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
      call balanc(n,n,stk(l),low,igh,stk(lw))
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
 320  continue
C faisceau
      if (lhs .ne. 4) then
        call error(41)
        return
      endif
      if (rhs .ne. 2) then
        call error(42)
        return
      endif
      il = iadr(lstk(top))
      m = istk(il+1)
      n = istk(il+2)
      l = sadr(il+4)
      il1 = iadr(lstk(top-1))
      m1 = istk(il1+1)
      n1 = istk(il1+2)
      l1 = sadr(il1+4)
C     test if square
      if (m .ne. n) then
        err = 2
        call error(20)
        return
      endif
      if (m1 .ne. n1) then
        err = 1
        call error(20)
        return
      endif
C test of dimensions
      if (n.ne.n1 .or. m.ne.m1) then
        buf = 'balanc:-->parameters with uncompatible dimensions!'
        call error(9999)
      endif
      nn = n * n
      if (nn*n1*m1 .eq. 0) then
        call error(89)
        return
      endif
C     equilibrage
      low = 1
      igh = n
      ilv1 = iadr(l+nn)
      lv1 = sadr(ilv1+4)
      ilv2 = iadr(lv1+nn)
      lv2 = sadr(ilv2+4)
      lcs = lv2 + nn
      lcp = lcs + n
      lw = lcp + n
      err = lw + 6*n - lstk(bot)
      if (err .gt. 0) then
        call error(17)
        return
      endif
c   l1-->E
c   l -->A
c  SUBROUTINE DGGBAL('B', N, E, LDE, A, LDA, LOW, IGH, LSCALE,
c$                   RSCALE, WORK, INFO )
      call dggbal('B',n,stk(l1),n,stk(l),n,low,igh,stk(lcs),
     &             stk(lcp),stk(lw),info)
c SUBROUTINE DGGBAK('B','R', N, LOW, IGH, LSCALE, RSCALE, N, V,
c$                   LDV, INFO )
      call dset(nn,0.0d+0,stk(lv2),1)
      call dset(n,1.0d+0,stk(lv2),n+1)
      call dggbak('B','R',n,low,igh,stk(lcs),stk(lcp),n,stk(lv2),
     &             n,info)
c     lv2--->Y
      call dset(nn,0.0d+0,stk(lv1),1)
      call dset(n,1.0d+0,stk(lv1),n+1)
      call dggbak('B','L',n,low,igh,stk(lcs),stk(lcp),n,stk(lv1),
     &             n,info)
c     lv1--->X
      istk(ilv1) = 1
      istk(ilv1+1) = n
      istk(ilv1+2) = n
      istk(ilv1+3) = 0
      top = top + 1
      lstk(top+1) = lv1 + nn
      istk(ilv2) = 1
      istk(ilv2+1) = n
      istk(ilv2+2) = n
      istk(ilv2+3) = 0
      top = top + 1
      lstk(top+1) = lv2 + nn
      goto 999
 999  return
 900  call error(43)
      return
      end

