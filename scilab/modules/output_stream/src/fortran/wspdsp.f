c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) - INRIA - Serge Steer
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine wspdsp(ne,ind,xr,xi,m,n,maxc,mode,ll,lunit,cw)
C     !but
C     wspdsp visualise une matrice  creuse complexe
C     !liste d'appel
C     
C     subroutine wspdsp(ne,ind,xr,xi,m,n,maxc,mode,ll,lunit,cw)
C     
C     double precision xr(ne),xi(ne)
C     integer ind(*)
C     integer nx,m,n,maxc,mode,ll,lunit
C     character cw*(*)
C     
C     c : nombre d'elements nons nuls de la matrice
C     ind : indices specifiant la position des elements non nuls
C     xr,xi : tableaux contenant les parties reelles et imaginaires des
C     elements non nuls
C     m : nombre de ligne de la matrice
C     n : nombre de colonnes de la matrice
C     maxc : nombre de caracteres maximum autorise pour
C     representer un nombre
C     mode : si mode=1 representation variable
C     si mode=0 representation d(maxc).(maxc-7)
C     ll : longueur de ligne maximum admissible
C     lunit : etiquette logique du support d'edition
C     cw : chaine de caracteres de travail de longueur au moins ll

      double precision xr(ne),xi(ne),a,a1,a2,fact,eps,dlamch,ar,ai
      integer maxc,mode,fl,typ
      integer ind(*)
      character cw*(*),sgn*1,dl*1
      character*10 form(2)
C    
      if (ne .eq. 0) then
        write (cw,'(''('',i5,'','',i5,'') zero sparse matrix'')') m, n
        call basout(io,lunit,cw(1:32))
        call basout(io,lunit,' ')
        goto 99
      else
        write (cw,'(''('',i5,'','',i5,'') sparse matrix'')') m, n
        call basout(io,lunit,cw(1:27))
        call basout(io,lunit,' ')
        if (io .eq. -1) goto 99
      endif
      ilr = 1
      ilc = m + 1
      nx = 1
      eps = dlamch('p')
      cw = ' '
      write (form(1),130) maxc, maxc-7
      dl = ' '
      if (m*n .gt. 1) dl = ' '
C     
C     facteur d'echelle
C     
      fact = 1.0d+0
      a1 = 0.0d+0
      if (ne .eq. 1) goto 10
      a2 = abs(xr(1)) + abs(xi(1))
      do 5 i = 1,ne
        a = abs(xr(i)) + abs(xi(i))
        if (a.eq.0.0d+0 .or. a.gt.dlamch('o')) goto 5
        a1 = max(a1,a)
        a2 = min(a2,a)
 5    continue
      imax = 0
      imin = 0
      if (a1 .gt. 0.0d+0) imax = int(log10(a1))
      if (a2 .gt. 0.0d+0) imin = int(log10(a2))
      if (imax*imin .le. 0) goto 10
      imax = (imax+imin) / 2
      if (abs(imax) .ge. maxc-2) fact = 10.0d+0**(-imax)
 10   continue
      eps = a1 * fact * eps
C     
      if (fact .ne. 1.0d+0) then
        write (cw(1:12),'(1x,1pd9.1,'' *'')') 1.0d+0/fact
        call basout(io,lunit,cw(1:12))
        call basout(io,lunit,' ')
        if (io .eq. -1) goto 99
      endif
      i0 = 0
      i1 = i0
      l = 1
      do 20 k = 1,ne
        cw = ' '
 11     i0 = i0 + 1
        if (i0-i1 .gt. ind(l)) then
          i1 = i0
          l = l + 1
          goto 11
        endif
        i = l
        j = ind(ilc-1+k)
        write (cw,'(''('',i5,'','',i5,'')'')') i, j
        l1 = 18
        ar = xr(k) * fact
        ai = xi(k) * fact
c        if (abs(ar).lt.eps .and. mode.ne.0) ar = 0.0d+0
c        if (abs(ai).lt.eps .and. mode.ne.0) ai = 0.0d+0
        if (ar.eq.0.0d0 .and. ai.ne.0.0d0) goto 16
C     determination du format devant representer a
        sgn = ' '
        if (ar .lt. 0.0d+0) sgn = '-'
        ar = abs(ar)

        typ = 1
        if(mode.eq.1) then
           call fmt(ar,maxc,typ,n1,n2)
        else
           if (isanan(ar).eq.1) then
              typ=-2
           elseif (ar.gt.dlamch('o')) then
              typ=-1
           endif
        endif
        if (typ .eq. 2) typ=n2 + 32*n1

        cw(l1:l1+1) = ' ' // sgn
        l1 = l1 + 2
        call formatnumber(ar,typ,maxc,cw(l1:),fl)
        l1 = l1 + fl
        if (ll .eq. 2) then
          cw(l1:l1) = 'i'
          l1 = l1 + 1
        endif
 16     continue
        if (ai .eq. 0.0d0) goto 17
        sgn = '+'
        if (ai .lt. 0.0d+0) sgn = '-'
        ai = abs(ai)
        typ = 1
        if(mode.eq.1) then
           call fmt(ai,maxc,typ,n1,n2)
           if (typ .eq. 2) typ=n2 + 32*n1
        else
           if (isanan(ai).eq.1) then
              typ=-2
           elseif (ai.gt.dlamch('o')) then
              typ=-1
           endif
        endif

        cw(l1:l1+1) = ' ' // sgn
        l1 = l1 + 2
        call formatnumber(ai,typ,maxc,cw(l1:),fl)
        l1 = l1 + fl
        cw(l1:l1) = 'i'
        l1 = l1 + 1
 17     continue
        call basout(io,lunit,cw(1:l1))
        if (io .eq. -1) goto 99
 20   continue
 99   continue
C     
 130  format ('(1pd',i2,'.',i2,')')
      end

