      subroutine whatln(lpt1,lpt2,lpt6,nct,idebut,ifin)
c!but
c     whatln permet de retrouver dans quelle ligne d'instruction 
c     se trouve le caractere designe par le pointeur lpt2 sachant 
c     que les lignes d'instructions considerees se situent entre 
c     lpt1 et lpt6 dans le tableau lin.
c
c     en retour:
c     nct : nombre de lignes qui suivent la ligne "contenant" le 
c           caractere designe par lpt2
c     idebut : pointeur sur le debut de la ligne
c     ifin   : pointeur sur la fin de la ligne
c!
c
c     Copyright INRIA
      include '../stack.h'
      integer eol
      data eol/99/
c
      nct=0
      l2=lpt2
      if(lin(l2).eq.eol) l2=l2-1
      ifin=lpt6-1
      ii=lpt6
 10   ii=ii-1
      if(ii.le.l2) goto 20
      if(lin(ii).eq.eol) then
         nct=nct+1
         ifin=ii-1
      endif
      goto 10
c
 20   continue
      idebut=lpt1
 21   ii=ii-1
      if(ii.le.lpt1) return
      if(lin(ii).ne.eol) goto 21
      idebut=ii+1
      return
c
      end
