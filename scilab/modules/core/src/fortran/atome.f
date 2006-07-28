      subroutine atome(chaine,n,type,sign)
c!
c analyse une chaine de caracteres pour reconnaitre s'il
c s'agit ou non d'un atome
c
c si chaine<>atome type=0
c si chaine=atome type=1
c ce sous programme retourne aussi une chaine modifiee:
c     on supprime tous les blancs et les parentheses inutiles
c sign indique quel est le signe en tete de chaine :
c    sign=1  : +
c    sign=-1 : -
c    sign=0  : pas de signe
c attention n est modifie aussi
c!
c reference externe : icopy
c!
c     Copyright INRIA
      integer n
      integer chaine(*),type,sign
c
      integer count,iok,k,l
      integer rparen,lparen,plus,minus,star,blanc,slash,bslash
      logical isexpr
      data rparen/42/,lparen/41/,plus/45/,minus/46/,star/47/,blanc/40/
      data slash/48/,bslash/49/
c
      l=1
      do 10 k=1,n
         if(chaine(k).ne.blanc) then
            chaine(l)=chaine(k)
            l=l+1
         endif
         n=l-1
 10   continue
      isexpr=.false.
      if ( n.le.0 ) return
c     
c     on elimine d'eventuelles parentheses en tete et en fin
c     
 20   continue
      if(chaine(n).eq.rparen.and.chaine(1).eq.lparen) then
         count=0
         do 21 k=1,n
            if (chaine(k).eq.lparen) count=count+1
            if(chaine(k).eq.rparen) count=count-1
            if(count.eq.0.and.k.lt.n)  goto 26
            if(count.eq.1.and. (chaine(k).eq.plus.or.
     $           chaine(k).eq.minus)) isexpr=.true.
 21      continue
         if(count.eq.0.and..not.isexpr) then
            call icopy(n-2,chaine(2),1,chaine(1),1)
            n=n-2
            iok=1
         else
            iok=0
         endif
      else
         iok=0
      endif
      if(iok.eq.1) goto 20
c     s'il y a un signe plus en tete on l'elimine
      iok=0
      if(chaine(1).eq.plus) then
         call icopy(n-1,chaine(2),1,chaine(1),1)
         n=n-1
         iok=1
      endif
      if(iok.eq.1) goto 20
c     
c     s'il y a -(term) on elimine les ()
 22   iok=0
      if(chaine(1).eq.minus.and.chaine(2).eq.lparen
     +     .and.chaine(n).eq.rparen) then
         count=1
         k=2
 23      k=k+1
         if(k.gt.n) goto 24
         if(chaine(k).eq.lparen) count=count+1
         if(chaine(k).eq.rparen) count=count-1
         if(count.gt.1) goto 23
         if(chaine(k).eq.plus.or.chaine(k).eq.minus) goto 24
         goto 23
 24      iok=0
         if(k.gt.n) then
            call icopy(n-2,chaine(3),1,chaine(2),1)
            n=n-2
            iok=1
         endif
      endif
      if(iok.eq.1) goto 22
c     
c     est-ce un atome
c     
 26   k=1
 25   k=k+1
      if(k.gt.n) goto 30
      if(chaine(k).ne.plus.and.chaine(k).ne.minus.and.
     +     chaine(k).ne.star.and.chaine(k).ne.slash.and.
     +     chaine(k).ne.bslash) goto 25
c     ce n'est pas un atome
      type=0
      goto 47
 30   continue
c     c'est un atome
      type=1
c     on traite le signe
 47   if(chaine(1).eq.plus) then
         sign=1
      elseif(chaine(1).eq.minus) then
         sign=-1
      else
         sign=0
      endif
c     
      end
