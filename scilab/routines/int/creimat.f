      logical function creimat(fname,lw,it,m,n,lr)
c     ----------------------------------------------------------
C     verifie que l'on peut stocker une matrice entiere [it,m,n]
C     a la position lw en renvoyant .true. ou .false. suivant la reponse
C     Entree :
C       lw : position (entier)
C       it : type 0 ou 1
C       m, n dimensions

C!
C      implicit undefined (a-z)
c     ----------------------------------------------------------
      logical creimatI
      character fname*(*)
      integer lw,it,m,n,lr
      include '../stack.h'
      if ( lw+1.ge.bot) then
         call error(18)
         creimat=.false.
         return
      endif
      creimat = creimatI(fname,lstk(lw),it,m,n,lr,.true.)
      if (creimat) then 
         lstk(lw+1)= sadr(lr +(m*n)/(4/it)+1)
      endif
      return
      end 

      logical function creimatI(fname,stlw,it,m,n,lr,flag)
c     ---------------------------------------------------------
C     internal function used by cremat and listcremat
c     comme cremat mais la position ou il faut creer la matrice 
c     est donnee par sa position dans lstk directement 
c     ----------------------------------------------------------
C      implicit undefined (a-z)
      character fname*(*)
      integer stlw,it,m,n,lr,il
      integer iadr, sadr
      logical flag 
      include '../stack.h'
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      il=iadr(stlw)
      err=sadr(il+4+(m*n)/(4/it)+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         creimatI=.false.
         return
      else
         creimatI=.true.
         if ( flag) then 
            istk(il)=8
c     si m*n=0 les deux dimensions sont mises a zero.
            istk(il+1)=min(m,m*n)
            istk(il+2)=min(n,m*n) 
            istk(il+3)=it
         endif
         lr=il+4
         return
      endif
      end
