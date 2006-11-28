      subroutine dmpdsp(mp,d,nl,mm,nn,var,lvar,maxc,mode,ll,lunit,
     1    cw,iw)
c!but
c     dmpdsp ecrit une matrice polynomiale (ou un polynome) sous
c     la forme d'un tableau de polynomes, avec gestion automatique de
c     l'espace disponible.
c!liste d'appel
c     
c     subroutine dmpdsp(mp,d,nl,m,n,var,lvar,maxc,mode,ll,lunit,
c     1                  cw,iw)
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
c     mode : si mode =1 alors representation variable
c     si mode = 0 representation d(maxc).(maxc-7)
c     ll : longueur de ligne maximum admissible
c     lunit : etiquette logique du support d'edition
c     cw : chaine de caracteres de travail de longueur au moins 2*ll
c     iw : tableau de travail entier de taille au moins egale a
c     d(nl*n+1)-d(1) + m*n+1
c!
c     Copyright INRIA

      double precision mp(*),a
      integer d(*),iw(*),maxc,mode,fl,c1,c2,typ
      integer sl,sk
      logical first
      character var*(*),cw*(*),sgn*1,dl*1
      character*10 form(2),fexp,expo
c     
      m=abs(mm)
      n=abs(nn)

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
      lines=0
      lbloc=n
      lf=lbloc+2+n
      nbloc=1
      iw(lbloc+nbloc)=n
      sk=0
c     
      ldg=-nl
      ldef=lf
      k0=1
      do 21 k=1,n
         sl=0
         iw(k)=0
         ldg=ldg+nl
         do 20 l=1,m
c     
c     traitement du polynome (l,k)
            lp=d(ldg+l)-1
            np=d(ldg+l+1)-d(ldg+l)
            lgh=0
            first=.true.
            do 10 i=1,np
               a=abs(mp(lp+i))
               iw(ldef)=0
               if(a.eq.0.0d+0) goto 09
               first=.false.
c     determination du format devant representer a
               typ=1
               if(mode.eq.1) call fmt(a,maxc,typ,n1,n2)
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
c     determination de la longueur de la representation du monome,
c     cette longueur est a priori fl+2 (' '//sgn//rep(a)//var).
c     mais peut etre reduite dans des cas particulier
               lgh=lgh+fl+2
               if(n2.eq.0) then
                  lgh=lgh-1
                  if(i.ne.1.and.int(a+0.1).eq.1) lgh=lgh-1
               endif
               if(i.ne.1) lgh=lgh+lvar
 09            ldef=ldef+1
 10         continue
c     
c     cas particulier du dernier exposant du polynome
            nd=ifix(log10(0.5+np))+1
            lgh=lgh+nd
c     cas particulier d'un polynome reduit a 0
            if(first) lgh=4
c     
            iw(k)=max(iw(k),lgh)
            sl=sl+(lgh/(ll-2))+1
c     
 20      continue
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
            lines=lines+2*sl+m+2
         endif
 21   continue
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
         c2=2+ll
c     
         do 60 l=1,m
            l1=c1
            l2=c2
            if(iw(k1).gt.ll-2) ll1=ll
            do 50 k=k1,k2
               ldg=(k-1)*nl+l
               lp=d(ldg)-1
               np=d(ldg+1)-d(ldg)
               ldef=lf-1+d(ldg)-d(1)
               first=.true.
c     
               l0=l1
               do 45 j=1,np
                  ifmt=iw(ldef+j)
                  if(ifmt.eq.0) goto 45
                  sgn='+'
                  if(first) sgn=' '
                  first=.false.
                  if(mp(lp+j).lt.0.0d+0) sgn='-'
                  a=abs(mp(lp+j))
c     
                  if(ifmt.eq.1) then
                     fl=maxc
                     n2=1
                  elseif(ifmt.ge.0) then
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
                     if(l1.le.ll-1) cw(l1:ll-1)=' '
                     if(l2.le.c2+ll-3) cw(l2:c2+ll-3)=' '
                     cw(ll:ll)=dl
                     call basout(io,lunit,cw(c1-1:ll))
                     cw(c2+ll-2:c2+ll-2)=dl
                     call basout(io,lunit,cw(c2-1:c2+ll-2))
                     if(io.eq.-1) goto 99
                     cw(c2:c2+9)=' '
                     l2=c2+10
                     cw(c1:c1+9)=' '
                     l1=c1+10
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
 45            continue
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
 50         continue
            if(cw(c1:l1-1).ne.' ') then
c     write(6,'(a)') cw(1:l2)
c     write(6,'(''c1,c2,l1,l2 '',4i4)') c1,c2,l1,l2
               cw(l1:l1)=dl
               call basout(io,lunit,cw(c1-1:l1))
            endif
            cw(l2:l2)=dl
            call basout(io,lunit,cw(c2-1:l2))
            if(l.ne.m) then
               cw(c2:l2-1)=' '
               call basout(io,lunit,cw(c2-1:l2))
            endif
            if(io.eq.-1) goto 99
 60      continue
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
