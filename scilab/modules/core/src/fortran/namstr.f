c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine namstr(id,str,n,job)
c     
c     convertit un nom code scilab en une chaine scilab
c     
c
      include 'stack.h'
      integer blank
      parameter (blank=40)
      integer id(nsiz),ch
      integer str(nlgh)
c     
      if(job.eq.0) goto 20
      i1=1
      do 15 l=1,nsiz
         idl=id(l)
         do 10 i=i1,i1+3
            k=(idl+128)/256
            if(k.lt.0) k=k-1
            ch=idl-256*k
            if(ch.eq.blank) then
               n=i-1
               return
            endif
            idl=k
            str(i)=ch
 10      continue
         i1=i1+4
 15   continue
      n=nlgh
      return
c     
 20   continue
      i1=1
      do 30 l=1,nsiz
         id(l)=0
         do 22 i=1,4
            ii=i1+4-i
            if(ii.le.n) then
               id(l)=256*id(l)+str(ii)
            else
               id(l)=256*id(l)+blank
            endif
 22      continue
         i1=i1+4
 30   continue
      return
      end
