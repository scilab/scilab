c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      double precision function arcosh(x)
c!but
c calcule l'arcosinus hyperbolique d'un double precision
c!liste d'appel
c     double precision function arcosh(x)
c     double precision x
c
      double precision x
      if (x.lt.1.0d+0) go to 10
      arcosh = log(x+sqrt(x*x-1.0d+0))
      return
10    arcosh = 0.0d+0
      return
      end
      double precision function arsinh(x)
c!but
c calcule l'arcsinus hyperbolique d'un double precision
c!liste d'appel
c     double precision function arsinh(x)
c     double precision x
c!
      double precision x
c
      arsinh = log(x+sqrt(x*x+1.0d+0))
      return
      end
