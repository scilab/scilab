      subroutine xchar(line,k)
c     ======================================================================
c     routine systeme dependente  pour caracteres speciaux
c     ======================================================================
c     fin de ligne : retourner k=eof
c     ignorer le caractere : retourner k=0
c
c     Copyright INRIA
      include '../stack.h'
c
      integer blank,eof
      integer k,ic
      character line*(*)
      data blank/40/,eof/99/
c
      ic=ichar(line(1:1))
      if(ic.eq.0) then
c     prise en compte de la marque de fin de chaine C
c     dans le cas d'un appel de scilab par un programme C
         k=eof
      elseif(ic.eq.9) then
c     tab remplace par un blanc
         k=blank+1
      elseif(ic.eq.10) then
c     \n remplace par un eol
         k=eof
      else
         k=eof+1+ic+1
      endif
      return 
      end
