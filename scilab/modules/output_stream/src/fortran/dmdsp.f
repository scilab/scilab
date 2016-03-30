c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C)  - INRIA - Serge Steer
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine dmdsp(x,nx,mm,nn,maxc,mode,ll,lunit,cw,iw)
c!but
c     dmdsp ecrit une matrice  (ou un scalaire) sous
c     la forme d'un tableau s, avec gestion automatique de
c     l'espace disponible.
c!liste d'appel
c
c     subroutine dmdsp(x,nx,m,n,maxc,mode,ll,lunit,cw,iw)
c
c     double precision x(*)
c     integer nx,m,n,maxc,mode,iw(*),ll,lunit
c     character cw*(*)
c
c     x : tableau contenant les coefficients de la matrice x
c     nx : entier definissant le rangement dans x
c     m : nombre de ligne de la matrice
c     n : nombre de colonnes de la matrice
c     maxc : nombre de caracteres maximum autorise pour
c            representer un nombre
c     mode : si mode=1 representation variable
c            si mode=0 representation d(maxc).(maxc-7)
c     ll : longueur de ligne maximum admissible
c     lunit : etiquette logique du support d'edition
c     cw : chaine de caracteres de travail de longueur au moins ll
c     iw : tableau de travail entier de taille au moins egale a
c          m*n + 2*n
c!
      double precision x(*),a,a1,a2,fact,eps,dlamch
      integer iw(*),maxc,mode,fl,s,typ
      character cw*(*),sgn*1,dl*1
      character temp*10
c
      eps=dlamch('p')
      cw=' '
      dl=' '
      m=abs(mm)
      n=abs(nn)
      if(m*n.gt.1) dl=' '
c
c facteur d'echelle
c
      fact=1.0d+0
      a1=0.0d+0
      if(m*n.eq.1) goto 10
      a2=abs(x(1))
      l=-nx
      do 05 j=1,n
         l=l+nx
         do 05 i=1,m
            a=abs(x(l+i))
            if(a.eq.0.0d+0.or.a.gt.dlamch('o')) goto 05
            a1=max(a1,a)
            a2=min(a2,a)
 05   continue
      imax=0
      imin=0
      if(a1.gt.0.0d+0) imax=int(log10(a1))
      if(a2.gt.0.0d+0) imin=int(log10(a2))
      if(imax*imin.le.0) goto 10
      imax=(imax+imin)/2
      if(abs(imax).ge.maxc-2)  fact=10.0d+0**(-imax)
 10   continue
      eps=a1*fact*eps
c     
c phase d'analyse: pour chaque coefficient a representer on determine
c       format avec lequel on va l'editer, on en deduit la longueur
c       de la representation de chacun des coefficients.
c       les differents formats sont stockes sous forme codee dans iw
c       a partir de lf
c       la taille respective des representation des chacun des coeff
c       est contenue dans iw a partir de 1 .
c
c n is the number of columns of the matrix
      lbloc=n
      lf=lbloc+n+1
      nbloc=1
      iw(lbloc+nbloc)=n
c iw(k) contient la longueur maxi necessaire pour representer un
c       element de colonne k
c iw(lbloc+nbloc) contient le numero de la derniere colonne
c        representable dans le bloc courant
c
      lp=-nx
      ldef=lf
      s=0
      do 20 k=1,n
      iw(k)=0
      lp=lp+nx
      do 15 l=1,m
c
c     traitement du coeff (l,k)
      a=abs(x(lp+l))*fact
c     c_ex      if(a.lt.eps) a=0.0d+0
c     jpc: add of isanan for msvc++
c      if(isanan(a).ne.1.and.a.lt.eps.and.mode.ne.0) a=0.0d+0
c     determination du format devant representer a
      typ=1

      if(mode.eq.1) then
         call fmt(a,maxc,typ,n1,n2)
      else
         if (isanan(a).eq.1) then
            typ=-2
         elseif (a.gt.dlamch('o')) then
            typ=-1
         endif
      endif

      if(typ.eq.2) then
         fl=n1
         iw(ldef)=n2+32*n1
      elseif(typ.lt.0) then
         iw(ldef)=typ
         fl=4
      else
         iw(ldef)=1
         fl=maxc
         n2=maxc-7
      endif
c
c     determination de la longueur de la representation du monome,
c          cette longueur est a priori fl+2 (' '//sgn//rep(a)).
c           mais peut etre reduite si la representation est entiere
      lgh=fl+3
      ldef=ldef+1
c
      iw(k)=max(iw(k),lgh)
   15 continue
      s=s+iw(k)
      if(s.gt.ll-2) then
         iw(lbloc+nbloc)=k-1
         nbloc=nbloc+1
         iw(lbloc+nbloc)=n
         s=iw(k)
      endif
c
   20 continue
c
      l1=1
      if(fact.ne.1.0d+0) then
         write(temp,'(i4)'),imax
         k1=0
 22      k1=k1+1
         if(temp(k1:k1).eq.' ') goto 22
         if(imax.lt.0) then
            cw(l1:l1+12-k1)=' 10^('//temp(k1:4)//') *'
            l1=l1+13-k1
         else
            cw(l1:l1+10-k1)=' 10^'//temp(k1:4)//' *'
            l1=l1+11-k1
         endif

      endif
      if(mm.lt.0) then
         write(cw(l1:l1+4),'(''eye *'')') 
         l1=l1+5
      endif
      if(l1.gt.1) then
         call basout(io,lunit,cw(1:l1-1))
         call basout(io,lunit,' ')
         if(io.eq.-1) goto 99
      endif
c phase d'edition : la chaine de caractere representant la ligne des coeff
c       est constituee puis imprimee.
c
      k1=1
      do 70 ib=1,nbloc
         k2=iw(lbloc+ib)
         if(nbloc.ne.1) then
            call blktit(lunit,k1,k2,io)
            if (io.eq.-1) goto 99
         endif
c
      cw(1:1)=dl
      do 60 l=1,m
      ldef=lf+l-1+(k1-1)*m
      l1=2
      do 50 k=k1,k2
      a=x(l+(k-1)*nx)*fact
cc_ex      if(abs(a).lt.eps) a=0.0d+0
c      if(abs(a).lt.eps.and.mode.ne.0) a=0.0d+0
c
      l0=l1
      ifmt=iw(ldef)
      sgn=' '
C      if(a.lt.0.0d+0.and.typ.ne.-2) sgn='-'
      if ( isanan(a).ne.1 .and. a .lt. 0 ) sgn='-'     
      a=abs(a)
c
      cw(l1:l1+1)=' '//sgn
      l1=l1+2
c     
      call formatnumber(a,ifmt,maxc,cw(l1:),fl)
      l1=l1+fl
c
c      if(n2.eq.0) l1=l1-1
      nl1=l0+iw(k)-1
      cw(l1:nl1)=' '
      l1=nl1+1
      ldef=ldef+m
   50 continue
      cw(l1:l1)=dl
      call basout(io,lunit,cw(1:l1) )
      if (io.eq.-1) goto 99
   60 continue
      k1=k2+1
   70 continue
c
   99 return
c
      end
