      subroutine funnam(id,fct,il1)
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer id(nsiz)

      character*(*) fct
      integer name(nlgh),name1(nlgh)
      integer percen,under
      data percen/56/,under/36/
      name(1)=percen
      i=2
      call typ2cod(il1,name(i),n1)
      i=i+n1
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
      INCLUDE '../stack.h'
      integer iadr
      iadr(l)=l+l-1
      call funnam(ids(1,pt+1),fct,iadr(lstk(k)))
      return
      end

      subroutine setfunnam(id,fct,nf)
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer id(nsiz)
      character*(*) fct
      integer name(nlgh),name1(nlgh)

      n1=min(nf,24)
      call cvstr(n1,name,fct,0)
      call namstr(id,name,n1,0)
      return
      end
