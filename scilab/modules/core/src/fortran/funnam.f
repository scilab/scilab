c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine funnam(id,fct,il1)
      INCLUDE 'stack.h'
      integer id(nsiz)

      character*(*) fct
      integer name(nlgh)
      integer percen,under
      data percen/56/,under/36/
      name(1)=percen
      i=2
      if (il1 .ne. 0) then
         call typ2cod(il1,name(i),n1)
         i=i+n1
      endif
      name(i)=under
      i=i+1
      n1=min(len(fct),11)
      call cvstr(n1,name(i),fct,0)
      i=i+n1
      call namstr(id,name,i-1,0)
      return
      end

      subroutine putfunnam(fct,k)
      character*(*) fct
      integer k
      INCLUDE 'stack.h'
      integer iadr
      iadr(l)=l+l-1
      if (k .le. 0) then
         call funnam(ids(1,pt+1),fct,0)
      else
         call funnam(ids(1,pt+1),fct,iadr(lstk(k)))
      endif
      return
      end

      subroutine setfunnam(id,fct,nf)
c     Copyright INRIA
      INCLUDE 'stack.h'
      integer id(nsiz)
      character*(*) fct
      integer name(nlgh)

      n1=min(nf,24)
      call cvstr(n1,name,fct,0)
      call namstr(id,name,n1,0)
      return
      end
