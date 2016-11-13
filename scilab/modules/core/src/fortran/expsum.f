c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1999 - INRIA - Serge Steer
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine expsum(isg,chaine,n,iptr,np,maxnp,ierr)
c!but
c     Etant donnee une expression cette subroutine la decompose
c     en  somme et differences de sous-expressions et en change
c     eventuellement le signe
c!liste d'appel
c     subroutine expsum(isg,chaine,n,iptr,np,maxnp,ierr)
c     integer isg,chaine(*),n,iptr(*),np,maxnp,ierr
c
c     isg : flag de changement de signe de l'expression
c           isg>0 pas de changement de signe
c           isg<0 changement de signe
c     chaine:vecteur d'entiers contenant les codes  des
c            caracteres de l'expression.
c            en retour chaine contient les codes de l'expression
c            la longueur peut etre modifiee (voir augmentee de 1)
c     n: longueur de "chaine"
c     iptr : vecteur des pointeurs sur le debut de chaque sous
c            expressions (signe initial compris) dans chaine.
c     np : nombre de sous expressions
c     maxnp : taille maxi de iptr (limite du nombre de sous
c             expressions
c     ierr : retour d'erreur si np>maxnp (ierr=1)
c!origine
c!
      integer chaine(*),iptr(*)
c     
      integer plus,minus,sign,lparen,rparen
      data plus/45/,minus/46/,lparen/41/,rparen/42/
c     
      ierr=0
c     on elimine des parentheses superflues   
      sign=isg
      l1=1
 01   n0=n
      if(chaine(l1).eq.plus.or.chaine(l1).eq.minus) l1=l1+1
      if(chaine(l1).eq.lparen) then
         l1=l1+1
         icount=1
 03      l1=l1+1
         if(l1.gt.n) goto 04
         if(chaine(l1).eq.lparen) icount=icount+1
         if(chaine(l1).eq.rparen) icount=icount-1
         if(icount.ne.0) goto 03
         if(l1.ne.n) goto 06         
         if(chaine(1).eq.minus) sign=-sign
         call icopy(n-3,chaine(3),1,chaine(1),1)
         n=n-3
      endif
 04   if(chaine(1).eq.lparen) then
         l1=2
         icount=1
 05      l1=l1+1
         if(l1.gt.n) goto 06
         if(chaine(l1).eq.lparen) icount=icount+1
         if(chaine(l1).eq.rparen) icount=icount-1
         if(icount.ne.0) goto 05
         if(l1.ne.n) goto 06
         if(chaine(1).eq.minus) sign=-sign
         call icopy(n-2,chaine(2),1,chaine(1),1)
         n=n-2
      endif
 06   if(n.lt.n0) goto 01
c on  rajoute eventuellement le premier signe
      if(chaine(1).ne.plus.and.chaine(1).ne.minus) then
         call icopy(n,chaine(1),-1,chaine(2),-1)
         chaine(1)=plus
         n=n+1
      endif

      np=0
c     
c on recherche chacun des termes et l'on change eventuellement les signes
      l1=0
      icount=0
 10   l1=l1+1
      if(l1.gt.n) goto 30
      if(chaine(l1).eq.lparen) icount=icount+1
      if(chaine(l1).eq.rparen) icount=icount-1
      if(icount.eq.0) then
         if(chaine(l1).eq.minus) then
            if(sign.eq.-1) chaine(l1)=plus
            np=np+1
            if(np.gt.maxnp) goto 100
            iptr(np)=l1
         elseif(chaine(l1).eq.plus) then
            if(sign.eq.-1) chaine(l1)=minus
            np=np+1
            if(np.gt.maxnp) goto 100
            iptr(np)=l1
         endif
      endif
      goto 10
      
 30   if(np+1.gt.maxnp) goto 100
      iptr(np+1)=l1
      return
 100  ierr=1
      return
      end
