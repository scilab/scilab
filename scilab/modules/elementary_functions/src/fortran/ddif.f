c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
C/MEMBR ADD NAME=DDIF,SSI=0
c
      subroutine ddif(n,a,na,b,nb)
c!but
c     ddif effectue l'operation vectorielle b=b-a
c!liste d'appel
c     subroutine ddif(n,a,na,b,nb)
c     double precision a(*),b(*)
c     integer n,na,nb
c
c     n : nombre d'elements des vecteurs a et b
c     a : tableau contenant a
c     na : increment entre deux elements consecutifs de a
c          na > 0
c     b,nb : definitions similaires a celles de a et na
c!
      double precision a(*),b(*)
      ia=1
      ib=1
      do 10 i=1,n
      b(ib)=b(ib)-a(ia)
      ia=ia+na
      ib=ib+nb
   10 continue
      return
      end
