c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine basnms(ids,nc,istk,ni)
c!but
c     traduit un nom de variable codee scilab en une variable scilab de
c     type chaine 
c!    
      integer nsiz
      parameter (nsiz=6)
      integer ids(nsiz,nc),istk(*),nc,ni
c
      il=1
      l=il+5+nc
c     header d'une variable de type chaine

      if(nc.eq.0) then 
         istk(il)=1
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         ni=4
         return
      endif
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=nc
      istk(il+3)=0
c
      istk(il+4)=1
      do 10 i=1,nc
         call namstr(ids(1,i),istk(l),nstr,1)
         l=l+nstr
         istk(il+4+i)=istk(il+3+i)+nstr
 10   continue
      ni=l-1
      end
