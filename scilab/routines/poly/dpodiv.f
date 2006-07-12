      subroutine dpodiv(a,b,na,nb  )
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension a(*),b(*)
c
c     1. a: avant execution c'est le tableau des coefficients du
c           polynome dividende range suivant les puissances croissantes
c           de la variable (ordre na+1).
c           apres execution,contient dans les nb premiers elements
c           le tableau des coefficients du reste ordonne suivant les
c           puissances croissantes, et dans les (na-nb+1) derniers elements,
c           le tableau des coefficients du polynome quotient range suivant
c           les puissances croissantes de la variable.
c     2. b: tableau des coefficients du polynome diviseur range suivant
c           les puissances croissantes de la variable (ordre nb+1).
c     3. na: degre du polynome a.
c     4. nb: degre du polynome b.
c
      l=na-nb+1
    2 if (l .le. 0) then 
         goto 5
      else
         goto 3
      endif
    3 n=l+nb
      q=a(n)/b(nb+1)
      nb1=nb+1
      do 4 i=1,nb1
      n1=nb-i+2
      n2=n-i+1
   4  a(n2)=a(n2)-b(n1)*q
      a(n)=q
      l=l-1
      goto 2
   5  continue
      return
      end
