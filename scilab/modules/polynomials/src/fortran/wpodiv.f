c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine wpodiv(ar,ai,br,bi,na,nb,ierr)
      implicit double precision (a-h,o-z)
      dimension ar(*),br(*),ai(*),bi(*)
c     division euclidienne de deux polynomes a coefficients complexes
c     le suffixe r designe la partie reelle et i la partie imaginaire
c
c     1. a: avant execution c'est le tableau des coefficients du
c    polynome dividende range suivant les puissances croissantes
c    de la variable (ordre na+1).
c    apres execution,contient dans les nb premiers elements
c    le tableau des coefficients du reste ordonne suivant les
c    puissances croissantes, et dans les (na-nb+1) derniers elements,
c    le tableau des coefficients du polynome quotient range suivant
c    les puissances croissantes de la variable.
c     2. b: tableau des coefficients du polynome diviseur range suivant
c    les puissances croissantes de la variable (ordre nb+1).
c     3. na: degre du polynome a.
c     4. nb: degre du polynome b.
c
      ierr=0
      l=na-nb+1
    2 if (l .le. 0) then
         goto 5
      else
         goto 3
      endif
    3 n=l+nb
c      q=a(n)/b(nb+1)
      call wwdiv(ar(n),ai(n),br(nb+1),bi(nb+1),qr,qi,ierr)
      if(ierr.ne.0) then
         return
      endif
      nb1=nb+1
      do 4 i=1,nb1
      n1=nb-i+2
      n2=n-i+1
c   4  a(n2)=a(n2)-b(n1)*q
      call wmul(br(n1),bi(n1),qr,qi,wr,wi)
      ar(n2)=ar(n2)-wr
      ai(n2)=ai(n2)-wi
    4 continue
      ar(n)=qr
      ai(n)=qi
      l=l-1
      goto 2
   5  continue
      return
      end
