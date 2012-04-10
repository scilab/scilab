c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


      subroutine isbrk(l)
c ====================================================================
c
c       retourne la valeur de l'indicateur d'interruption
c
      logical iflag,interruptible
      common /basbrk/ iflag,interruptible
      l=0
      if(iflag) l=1
      return
      end
