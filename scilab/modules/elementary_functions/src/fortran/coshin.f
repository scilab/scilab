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
C/MEMBR ADD NAME=COSHIN,SSI=0
c
c fonction:  coshin
c fonction cosinus hyperbolique inverse de x
c en double precision
c acheve le 05/12/85
c ecrit par philippe touron
c
c
c sous programmes appeles: aucun
c
      double precision function coshin(x)
      double precision x
      coshin=log(x+sqrt(x*x-1.0d+0))
      return
      end
