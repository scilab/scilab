c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

       subroutine fcalc9(a,b,c,lc,d,nd)
       integer a(*),d(*)
       double precision b(*)
       character c*(*)
       do 10 i=1,nd
          d(i)=2*d(i)
 10    continue
       do 20 i=1,lc
          if (c(i:i).eq.'u') c(i:i)='o'
 20    continue
       return
       end
