      subroutine wmdsp(xr,xi,nx,mm,nn,maxc,mode,ll,lunit,cw,iw)
c!but
c     wmdsp ecrit une matrice complexe  (ou un scalaire) sous
c     la forme d'un tableau, avec gestion automatique de
c     l'espace disponible.
c!liste d'appel
c
c     subroutine wmdsp(x,nx,m,n,maxc,mode,ll,lunit,lct,cw,iw)
c
c     double precision xr(*),xi(*)
c     integer nx,m,n,maxc,mode,iw(*),ll,lunit,lct(2)
c     character cw*(*)
c
c     x : tableau contenant les coefficients de la matrice x
c     nx : entier definissant le rangement dans x
c     m : nombre de ligne de la matrice
c     n : nombre de colonnes de la matrice
c     maxc : nombre de caracteres maximum autorise pour
c            representer un nombre
c     mode : si  mode = 1 format variable
c            si mode = 0 format d(maxc).(maxc-7)
c     ll : longueur de ligne maximum admissible
c     lunit : etiquette logique du support d'edition
c     cw : chaine de caracteres de travail de longueur au moins ll
c     iw : tableau de travail entier de taille au moins egale a
c          m*n + 2*n
c
c     Copyright INRIA
      double precision xr(*),xi(*),a,a1,a2,fact
      integer iw(*),maxc,mode,fl,s,typ
      character cw*(*),sgn*1,dl*1,var*4
      character*10 form(2)
c
      var='i'
      lvar=1
c
      m=abs(mm)
      n=abs(nn)

      cw=' '
      write(form(1),130) maxc,maxc-7
      dl=' '
      if(m*n.gt.1) dl=' '
c
c facteur d'echelle
c
      fact=1.0d+0
      if(m*n.eq.1) goto 2
      a1=0
      a2=abs(xr(1))+abs(xi(1))
      l=-nx
      do 1 j=1,n
      l=l+nx
      do 1 i=1,m
      a=abs(xr(l+i))+abs(xi(l+i))
      if(a.eq.0.0d+0) goto 1
      a1=max(a1,a)
      a2=min(a2,a)
    1 continue
      imax=0
      imin=0
      if(a1.gt.0.0d+0) imax=int(log10(a1))
      if(a2.gt.0.0d+0) imin=int(log10(a2))
      if(imax*imin.le.0) goto 2
      imax=(imax+imin)/2
      if(abs(imax).ge.maxc-2) fact=10.0d+0**(-imax)
    2 continue
c
c phase d'analyse: pour chaque coefficient a representer on determine
c       format avec lequel on va l'editer, on en deduit la longueur
c       de la representation de chacun des coefficients.
c       les differents formats sont stockes sous forme codee dans iw
c       a partir de lf
c       la taille respective des representation des chacun des coeff
c       est contenue dans iw a partir de 1 .
c
      lbloc=n
      lf=lbloc+1+n
      nbloc=1
      iw(lbloc+nbloc)=n
      s=0
c
      lp=-nx
      ldef=lf
      do 21 k=1,n
      lp=lp+nx
      iw(k)=0
      do 20 l=1,m
c
c     traitement du coeff (l,k)
c
      lgh=0
      do 10 i=1,2
      a=xr(lp+l)*fact
      if(i.eq.2) a=xi(lp+l)*fact
      iw(ldef)=0
      if(a.eq.0.0d+0) goto 10
c
c     determination du format devant representer a
      typ=1
      if(mode.eq.1) call fmt(abs(a),maxc,typ,n1,n2)
      if(typ.eq.2) then
         fl=n1
         iw(ldef)=n2+32*n1
      elseif(typ.lt.0) then
         iw(ldef)=typ
         fl=3
      else
         iw(ldef)=1
         fl=maxc
         n2=maxc-7
      endif
c
c     determination de la longueur de la representation du coefficient,
c          cette longueur est a priori fl+2 (' '//sgn//rep(a)).
c           mais peut etre reduite si la representation est entiere
   05 lgh=fl+2+lgh
c      if(n2.eq.0) lgh=lgh-1
   10 ldef=ldef+1
c
      if(iw(ldef-1).ne.0) lgh=lgh+lvar
      if(lgh.eq.0) lgh=4
      lgh=lgh+1
      iw(k)=max(iw(k),lgh)
c
   20 continue
      s=s+iw(k)
      if(s.gt.ll-2) then
         iw(lbloc+nbloc)=k-1
         nbloc=nbloc+1
         iw(lbloc+nbloc)=n
         s=iw(k)
      endif
   21 continue
      l1=1
      if(fact.ne.1.0d+0) then
         write(cw(l1:l1+11),'(1x,1pd9.1,'' *'')')  1.0d+0/fact
         l1=l1+12
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
c
c phase d'edition : la chaine de caractere representantla ligne des coeff
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
      ldef=lf+2*(l-1+(k1-1)*m)
      l1=2
      do 50 k=k1,k2
      lp=(k-1)*nx+l
      li=(k-1)*m+l
      l0=l1
c
      do 42 i=1,2
c
      ifmt=iw(ldef+i-1)
      if(ifmt.eq.0) goto 42
c
      a=xr(lp)
      if(i.eq.2) a=xi(lp)
      sgn=' '
      if(i.eq.2.and.iw(ldef).ne.0) sgn='+'
      if(a.lt.0.0d+0) sgn='-'
      a=abs(a)*fact
c
      a=abs(a)
c
      cw(l1:l1+1)=' '//sgn
      l1=l1+2
c
      call formatnumber(a,ifmt,maxc,cw(l1:),fl)
      l1=l1+fl

   42 continue
c
      if(iw(ldef+1).eq.0) goto 43
      if(fl.eq.3.and.cw(l1-2:l1-1).eq.'1.') l1=l1-2
      cw(l1:l1-1+lvar)=var(1:lvar)
      l1=l1+lvar
      goto 44
   43 if(iw(ldef).ne.0) goto 44
      cw(l1:l1+3)='   0.'
      l1=l1+4
c
   44 continue
      nl1=l0+iw(k)-1
      cw(l1:nl1)=' '
      l1=nl1+1
      ldef=ldef+2*m
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
  120 format('(f',i2,'.',i2,')')
  130 format('(1pd',i2,'.',i2,')')
c
      end
