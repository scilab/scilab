c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1986-2008 - INRIA - Serge STEER
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine dmrdsp(mpn,dn,mpd,dd,nl,mm,nn,var,lvar,maxc,mode,ll,
     1    lunit,cw,iw)
c!but
c     dmpdsp ecrit une matrice polynomiale (ou un polynome) sous
c     la forme d'un tableau de polynomes, avec gestion automatique de
c     l'espace disponible.
c!liste d'appel
c
c     subroutine dmrdsp(mpn,dn,mpd,dd,nl,m,n,var,lvar,maxc,mode,ll,
c     1 lunit,cw,iw)
c
c     double precision mp(*)
c     integer d(nl*n+1),nl,m,n,lvar,maxc,mode,iw(*),ll,lunit
c     character var*(*),cw*(*)
c
c     pm : tableau reel contenant les coefficients des polynomes,
c     le coefficient de degre k du polynome pm(i,j) est range
c     dans pm( d(i + (j-1)*nl + k) )
c     pm doit etre de taille au moins d(nl*n+1)-d(1)
c     d : tableau entier de taille nl*n+1,  si k=i+(j-1)*nl alors
c     d(k)) contient  l'adresse dans pm du coeff de degre 0
c     du polynome pm(i,j). Le degre du polynome pm(i,j) vaut:
c     d(k+1)-d(k) -1
c     nl : entier definissant le rangement dans d
c     m : nombre de ligne de la matrice polynomiale
c     n : nombre de colonnes de la matrice polynomiale
c     var : nom de la variable muette
c     lvar : nombre de caracteres de var
c     maxc : nombre de caracteres maximum autorise pour
c     representer un nombre
c     mode : si mode = 1 alors representation variable
c     si mode = 0 alors representation d(maxc).(maxc-7)
c     ll : longueur de ligne maximum admissible
c     lunit : etiquette logique du support d'edition
c     cw : chaine de caracteres de travail de longueur au moins ll*2
c     iw : tableau de travail entier de taille au moins egale a
c     n*(4+m)+1+dn(n*m+1)+dd(n*m+1)

c!
c
      double precision mpn(*),mpd(*),a
      integer dd(*),dn(*),iw(*),maxc,mode
      integer fl,sk,sl,c1,c2,typ
      logical first
      character var*(*),cw*(*),sgn*1,dl*1
      character*10 form(2),fexp,expo
      integer nind
c
      data nind/5/
c
      m=abs(mm)
      n=abs(nn)
c
      cw=' '
      write(form(1),130) maxc,maxc-7
      dl=' '
      if(m*n.gt.1) dl=' '
c
c     phase d'analyse: pour chaque coefficient a representer on determine
c     format avec lequel on va l'editer, on en deduit la longueur
c     de la representation de chacun des polynomes.
c     les differents formats sont stockes sous forme codee dans iw
c     a partir de lf
c     la taille respective des representation des chacun des polynomes
c     est contenue dans iw a partir de 1 .
c
      lcol=1
      lbloc=lcol+n-1
      lfn=lbloc+n+2
      lfd=lfn+dn(n*m+1)
      ldelta=lfd+dd(n*m+1)
      ldeb=ldelta+m*n
      lfin=ldeb+n
c
      lines=0
      nbloc=1
      iw(lbloc+nbloc)=n
      sk=0
      ldefn=lfn
      ldg=-nl
      ldefd=lfd
      idelta=ldelta
c
      k0=1
      do 11 k=1,n
         sl=0
         iw(lcol-1+k)=0
         ldg=ldg+nl
         do 10 l=1,m
c
c     traitement du polynome (l,k)
            lpn=dn(ldg+l)-1
            npn=dn(ldg+l+1)-dn(ldg+l)
            lghn=0
            first=.true.
            do 05 i=1,npn
               a=abs(mpn(lpn+i))
               iw(ldefn)=0
               if(a.ne.0.0d+0) then
                  first=.false.
c     determination du format devant representer a
                  typ=1
                  if(mode.eq.1) call fmt(a,maxc,typ,n1,n2)
                  if(typ.eq.2) then
                     fl=n1
                     iw(ldefn)=n2+32*n1
                  elseif(typ.lt.0) then
                     iw(ldefn)=typ
                     n2=1
                     fl=3
                  else
                     iw(ldefn)=1
                     fl=maxc
                     n2=maxc-7
                  endif
c
c     determination de la longueur de la representation du monome,
c     cette longueur est a priori fl+2 (' '//sgn//rep(a)//var).
c     mais peut etre reduite dans des cas particulier
                  lghn=lghn+fl+2
                  if(n2.eq.0) then
                     lghn=lghn-1
                     if(i.ne.1.and.int(a+0.1).eq.1) lghn=lghn-1
                  endif
                  if(i.ne.1) lghn=lghn+lvar
               endif
               ldefn=ldefn+1
 05         continue

c
c     cas particulier du dernier exposant du polynome
            nd=ifix(log10(0.5+npn))+1
            lghn=lghn+nd
c     cas particulier d'un polynome reduit a 0
            if(first) lghn=4
c
            lpd=dd(ldg+l)-1
            npd=dd(ldg+l+1)-dd(ldg+l)
            lghd=0
            first=.true.
            do 08 i=1,npd
               a=abs(mpd(lpd+i))
               iw(ldefd)=0
               if(a.ne.0.0d+0) then
                  first=.false.
c     determination du format devant representer a
                  typ=1
                  if(mode.eq.1) call fmt(a,maxc,typ,n1,n2)
                  if(typ.eq.2) then
                     fl=n1
                     iw(ldefd)=n2+32*n1
                  elseif(typ.lt.0) then
                     iw(ldefd)=typ
                     n2=1
                     fl=3
                  else
                     iw(ldefd)=1
                     fl=maxc
                     n2=maxc-7
                  endif
c     determination de la longueur de la representation du monome,
c     cette longueur est a priori fl+2 (' '//sgn//rep(a)//var).
c     mais peut etre reduite dans des cas particulier
                  lghd=lghd+fl+2
                  if(n2.eq.0) then
                     lghd=lghd-1
                     if(i.ne.1.and.int(a+0.1).eq.1) lghd=lghd-1
                  endif
                  if(i.ne.1) lghd=lghd+lvar
               endif
               ldefd=ldefd+1
 08         continue
c
c     cas particulier du dernier exposant du polynome
            nd=ifix(log10(0.5+npd))+1
            lghd=lghd+nd
c     cas particulier d'un polynome reduit a 0
            if(first) lghd=4
c
            iw(k)=max(iw(k),lghn,lghd)
            sl=sl+(lghn/(ll-10))+(lghd/(ll-10))+2
            iw(idelta)=min(lghn,ll-2)-min(lghd,ll-2)
            idelta=idelta+1
c
 10      continue
         sk=sk+iw(k)
         if(sk.gt.ll-2) then
            if(k.eq.k0) then
               iw(lbloc+nbloc)=k
               sk=0
               k0=k+1
            else
               iw(lbloc+nbloc)=k-1
               sk=iw(k)
               k0=k
            endif
            nbloc=nbloc+1
            iw(lbloc+nbloc)=n
c     lines=lines+2*sl+m+2
         endif
 11   continue
      nbloc=min(nbloc,n)
c
      l1=1
      if(mm.lt.0) then
         write(cw(l1:l1+4),'(''eye *'')')
         l1=l1+5
         call basout(io,lunit,cw(1:l1-1))
         call basout(io,lunit,' ')
         if(io.eq.-1) goto 99
      endif

c
c     phase d'edition : les deux chaines de caracteres representant
c     la ligne des exposants et la ligne des coefficients,sont
c     constituees puis imprimees.
c
      k1=1
      do 70 ib=1,nbloc
         k2=iw(lbloc+ib)
         ll1=0
         if(nbloc.ne.1) then
            call blktit(lunit,k1,k2,io)
            if (io.eq.-1) goto 99
         endif
c
         cw(1:1)=dl
         c1=2
         cw(1+ll:1+ll)=dl
         c2=max(3+ll,nind+maxc+15)
c
         do 60 l=1,m
c     numerateur
            l1=c1
            l2=c2
            if(iw(k1).gt.ll-2) ll1=ll
            do 45 k=k1,k2
               l0=l1
               idelta=ldelta-1+l+(k-1)*m
               ndelta=0
               if(iw(idelta).lt.-1) then
                  ndelta=-iw(idelta)/2
                  cw(l1:l1+ndelta-1)=' '
                  cw(l2:l2+ndelta-1)=' '
                  l1=l1+ndelta
                  l2=l2+ndelta
               endif
c
               ldg=(k-1)*nl+l
               lpn=dn(ldg)-1
               npn=dn(ldg+1)-dn(ldg)
               ldefn=lfn-1+dn(ldg)-dn(1)
               first=.true.
c
               iw(ldeb-1+k)=l2
               iw(lfin-1+k)=0
               do 40 j=1,npn
                  ifmt=iw(ldefn+j)
                  if(ifmt.eq.0) goto 40
                  sgn='+'
                  if(first) sgn=' '
                  first=.false.
                  if(mpn(lpn+j).lt.0.0d+0) sgn='-'
                  a=abs(mpn(lpn+j))
c
                  if(ifmt.eq.1) then
                     nf=1
                     fl=maxc
                     n2=1
                  elseif(ifmt.ge.0) then
                     nf=2
                     n1=ifmt/32
                     n2=ifmt-32*n1
                     fl=n1
                  elseif(ifmt.lt.0) then
c     Inf/Nan
                     fl=3
                     n2=1
                  endif

c
                  nd=0
                  if(j.gt.2) nd=ifix(log10(0.5+j))+1
                  if(l2+fl+2+lvar+nd.gt.c2+ll-2) then
c     gestion des lignes suites
                     if(l1.le.ll-1) then
                        cw(l1:ll-1)=' '
                        l1=ll
                     endif
                     if(l2.le.c2+ll-2) then
                        cw(l2:c2+ll-2)=' '
                        l2=c2+ll-2
                     endif
                     iw(lfin-1+k)=l2-1
                     cw(l1:l1)=dl
                     call basout(io,lunit,cw(c1-1:l1))
                     cw(l2:l2)=dl
                     cw(c2-1:c2-1)=dl
                     call basout(io,lunit,cw(c2-1:l2))
                     if(io.eq.-1) goto 99
                     cw(c2-1:c2+nind-1)=' '
                     cw(c2-1:c2-1)=dl
                     l2=c2+nind
                     cw(c1-1:c1+nind-1)=' '
                     cw(c1-1:c1-1)=dl
                     l1=c1+nind
                  endif
c     representation du monome
                  cw(l2:l2+1)=' '//sgn
                  l2=l2+1
                  call formatnumber(a,ifmt,maxc,cw(l2+1:),fl)
                  l2=l2+fl
                  if(n2.eq.0) l2=l2-1
                  if(j.gt.1) then
                     if(n2.eq.0.and.int(a+0.1).eq.1) l2=l2-1
                     cw(l2+1:l2+lvar)=var(1:lvar)
                     l2=l2+lvar
                  endif
                  nl1=l2+c1-c2
                  cw(l1:nl1)=' '
                  if(j.gt.2) then
                     write(fexp,110) nd
                     write(expo,fexp) j-1
                     cw(nl1+1:nl1+nd)=expo(1:nd)
                     l1=nl1+nd
                  endif
                  l1=l1+1
                  l2=l2+1
 40            continue
               if(first) then
c     cas particulier du polynome nul
                  cw(l1:l1+3)=' '
                  cw(l2:l2+3)='   0'
                  l1=l1+4
                  l2=l2+4
                  nd=0
               endif
               if(iw(lfin-1+k).eq.0) iw(lfin-1+k)=l2
               if(nd.ne.0) cw(l2:l2+nd-1)=' '
               nl1=l0+iw(k)
               if(ll1.eq.ll) nl1=ll-1
               cw(l1:nl1)=' '
               l1=nl1+1
               cw(l2:c2+nl1-c1)=' '
               l2=c2+nl1-c1+1
 45         continue
            if(cw(c1:l1-1).ne.' ') then
               cw(l1:l1)=dl
               call basout(io,lunit,cw(c1-1:l1))
            endif
            cw(l2:l2)=dl
            cw(c2-1:c2-1)=dl
            call basout(io,lunit,cw(c2-1:l2))
            if(io.eq.-1) goto 99
c
c     trait de fraction
            cw(c2:l2-1)=' '
            jjb1=c2
            do 47 k=k1,k2
               idelta=ldelta-1+l+(k-1)*m
               ndelta=max(0,-iw(idelta)/2)
               ideb=max(jjb1,iw(ldeb-1+k)-ndelta+2)
               ifin=iw(lfin-1+k)+ndelta-2
               if(ifin-ideb+1.eq.2) ideb=ideb-1
               do 46 i=ideb,ifin
                  cw(i+1:i+1)='-'
 46            continue
               jjb1=iw(lfin-1+k)+1
 47         continue
            cw(l2:l2)=dl
            call basout(io,lunit,cw(c2-1:l2))
            if(io.eq.-1) goto 99
c
c     denominateur
            l1=c1
            l2=c2
            do 55 k=k1,k2
               l0=l1
               idelta=ldelta-1+l+(k-1)*m
               ndelta=0
               if(iw(idelta).gt.1) then
                  ndelta=iw(idelta)/2
                  cw(l1:l1+ndelta-1)=' '
                  cw(l2:l2+ndelta-1)=' '
                  l1=l1+ndelta
                  l2=l2+ndelta
               endif
c
               ldg=(k-1)*nl+l
               lpd=dd(ldg)-1
               npd=dd(ldg+1)-dd(ldg)
               ldefd=lfd-1+dd(ldg)-dd(1)
               first=.true.
c
               do 50 j=1,npd
                  ifmt=iw(ldefd+j)
                  if(ifmt.eq.0) goto 50
                  sgn='+'
                  if(first) sgn=' '
                  first=.false.
                  if(mpd(lpd+j).lt.0.0d+0) sgn='-'
                  a=abs(mpd(lpd+j))
c
                  if(ifmt.eq.1) then
                     nf=1
                     fl=maxc
                     n2=1
                  elseif(ifmt.ge.0) then
                     nf=2
                     n1=ifmt/32
                     n2=ifmt-32*n1
                     fl=n1
                  elseif(ifmt.lt.0) then
c     Inf/Nan
                     fl=3
                     n2=1
                  endif
c
                  nd=0
                  if(j.gt.2) nd=ifix(log10(0.5+j))+1
                  if(l2+fl+2+lvar+nd.gt.c2+ll-2) then
c     gestion des lignes suites
                     if(l1.le.ll-1) then
                        cw(l1:ll-1)=' '
                        l1=ll
                     endif
                     if(l2.le.c2+ll-2) then
                        cw(l2:c2+ll-2)=' '
                        l2=c2+ll-2
                     endif
                     cw(l1:l1)=dl
                     call basout(io,lunit,cw(c1-1:l1))
                     cw(l2:l2)=dl
                     call basout(io,lunit,cw(c2-1:l2))
                     if(io.eq.-1) goto 99
                     cw(c2:c2-1+nind)=' '
                     cw(c2-1:c2-1)=dl
                     l2=c2+nind
                     cw(c1:c1-1+nind)=' '
                     cw(c1-1:c1-1)=dl
                     l1=c1+nind
                  endif
c     representation du monome
                  cw(l2:l2+1)=' '//sgn
                  l2=l2+1
                  call formatnumber(a,ifmt,maxc,cw(l2+1:),fl)
                  l2=l2+fl
                  if(n2.eq.0) l2=l2-1
                  if(j.gt.1) then
                     if(n2.eq.0.and.int(a+0.1).eq.1) l2=l2-1
                     cw(l2+1:l2+lvar)=var(1:lvar)
                     l2=l2+lvar
                  endif
                  nl1=l2+c1-c2
                  cw(l1:nl1)=' '
                  if(j.gt.2) then
                     write(fexp,110) nd
                     write(expo,fexp) j-1
                     cw(nl1+1:nl1+nd)=expo(1:nd)
                     l1=nl1+nd
                  endif
                  l1=l1+1
                  l2=l2+1
 50            continue
               if(first) then
c     cas particulier du polynome nul
                  cw(l1:l1+3)=' '
                  cw(l2:l2+3)='   0'
                  l1=l1+4
                  l2=l2+4
                  nd=0
               endif
               if(nd.ne.0) cw(l2:l2+nd-1)=' '
               nl1=l0+iw(k)
               if(ll1.eq.ll) nl1=ll-1
               cw(l1:nl1)=' '
               l1=nl1+1
               cw(l2:c2+nl1-c1)=' '
               l2=c2+nl1-c1+1
 55         continue
            if(cw(c1:l1-1).ne.' ') then
               cw(l1:l1)=dl
               call basout(io,lunit,cw(c1-1:l1))
            endif
            cw(l2:l2)=dl
            cw(c2-1:c2-1)=dl
            call basout(io,lunit,cw(c2-1:l2))
            cw(c1:l1-1)=' '
            cw(l1:l1)=dl
            if(io.eq.-1) goto 99
            if(l.ne.m) then
               call basout(io,lunit,cw(c1-1:l1))
               if(io.eq.-1) goto 99
            endif
 60      continue
c
         k1=k2+1
 70   continue
c
 99   return
c
 110  format('(i',i2,')')
 120  format('(f',i2,'.',i2,')')
 130  format('(1pd',i2,'.',i2,')')
c
      end
