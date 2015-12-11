c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
