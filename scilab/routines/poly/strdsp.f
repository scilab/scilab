      subroutine strdsp(mat,d,lig,col,ll,lunit,iw,cw)
c!but 
c     
c     Cette subroutine fait le display d'une matrice de chaines de 
c     caracteres donnes par des codes entiers.
c     
c     La subroutine qui fait la conversion des codes entier en caracteres
c     est cvstr.
c     
c!Sequence d'appel:
c     
c     call strdsp(mat,d,lig,col,ll,lunit,iw,cw)
c     
c!Parametres:
c     
c     mat: matrice entiere, contenant les codes des caracateres de 
c     l'ensemble de la matrice de chaine
c     
c     d: matrice entiere, elle indique les deplacement des
c     adresses d'implantation par rapport au debut de mat.
c     
c     lig: entier, nombre de lignes de mat.
c     
c     col: entier, nombre de colonnes de mat.
c     
c     ll: entier, lleur de la ligne de dloyement.
c     
c     lunit: entier, indique l'etiquette logique du dispositif
c     de sortie.
c     
c     iw: vecteur entier de longueur col. Zone de travail.
c     
c     cw: caracter de longueur egale  a ll
c     
c!auteur:
c     s Steer (inria), 18sept.1985. corrige 1992
c!    
c     
c     Copyright INRIA
      integer mat(*),d(*),lig,col,ll,lunit,iw(*)
      character cw*(*),dl*1
      integer lines,sl,sk,c1,nind
c     
      data nind/5/
c     
      dl=' '
      if(lig*col.gt.1) dl='!'
c     
      lcol=1
      lines=0
      lbloc=lcol+col-1
      nbloc=1
      iw(lbloc+nbloc)=col
      sk=0
c     
c     cas d'une matrice vide
      if (col.eq.0.or.lig.eq.0) return
c     
      l=1
      k0=1
      do 11 k=1,col
         sl=0
         iw(k)=0
         do 10 i=1,lig
            lgh=d(l+1)-d(l)+2
            iw(k)=max(iw(k),lgh)
            sl=sl+(lgh/(ll-2))+1
            l=l+1
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
            iw(lbloc+nbloc)=col
c     lines=lines+sl+lig+2
         endif
 11   continue
      nbloc=min(nbloc,col)
c     
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
c     
         do 60 l=1,lig
            l1=c1
            do 50 k=k1,k2
               l0=l1
               ldg=(k-1)*lig+l
               lp=d(ldg)
               np=d(ldg+1)-d(ldg)
c     
               ll1=0
               indent=0
 40            np1=min(np,ll-2-indent)
               call cvstr(np1,mat(lp),cw(l1:l1+np1-1),1)
               l1=l1+np1
               if(np1.ne.np) then
                  ll1=ll
                  if(l1.le.ll-1) cw(l1:ll-1)=' '
                  cw(ll:ll)=dl
                  call basout(io,lunit,cw(c1-1:ll))
                  if(io.eq.-1) goto 99
                  cw(c1:c1+nind-1)=' '
                  l1=c1+nind
                  indent=nind
                  lp=lp+np1
                  np=np-np1
                  if(np.gt.0) goto 40
               endif
               il=min(iw(k),ll-2)
               if(l0+il.ge.l1) then
                  cw(l1:l0+il)=' '
                  l1=l0+il
               endif
 50         continue
            if(ll1.eq.ll) then
               if(l1.le.ll) then
                  cw(l1:ll)=' '
                  l1=ll
               endif
            endif
            cw(l1:l1)=dl
            call basout(io,lunit,cw(c1-1:l1))
            if(io.eq.-1) goto 99
            if(l.ne.lig) then
               cw(c1:l1-1)='  '
               call basout(io,lunit,cw(c1-1:l1))
               if(io.eq.-1) goto 99
            endif
 60      continue
         k1=k2+1
 70   continue
c     
 99   return
 110  format('(i',i2,')')
 120  format('(f',i2,'.',i2,')')
 130  format('(d',i2,'.',i2,')')
      end
