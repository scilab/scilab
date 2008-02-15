c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine fsom(a,n,b,m,c)
      real a(*),b(*),c(*)
      kk=min(n,m)
      do 2 k=1,m
 2       c(k)=1.0
      do 1 k=1,kk
 1       c(k)=c(k)+b(k)+a(k)
      end


