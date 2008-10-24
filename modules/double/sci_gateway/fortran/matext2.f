
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine matext2
c     
c     B=A(i,j)

      include 'stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1

      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      mn3=m3*n3
      top=top-1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c     
c     arg3(arg1,arg2)
 82   if(rhs.gt.3) then
         call error(36)
         return
      endif
      if(mn3.eq.0) then 
c     .  arg3=[]
         if(m1.le.0.or.m2.le.0) then
            il1=iadr(lstk(top))
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         else
c     should be an error but a lot of code uses this feature
c     we will change it just after 3.1 release see also matext1

c           call error(21)
            il1=iadr(lstk(top))
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)

         endif
         return
      elseif(m3.lt.0) then
c     .arg3=eye
         call error(14)
         return
      endif
c     check and convert indices variables
      call indxg(il1,m3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mxi.gt.m3) then
         call error(21)
         return
      endif
      call indxg(il2,n3,ilj,nj,mxj,lw,1)
      if(err.gt.0) return
      if(mxj.gt.n3) then
         call error(21)
         return
      endif
c
 90   mn=mi*nj
      if(mn.eq.0) then 
c     .  arg1=[] or arg2=[] 
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
c     get memory for the result
      il1=iadr(lstk(top))
      l1=sadr(il1+4)
      if(sadr(ili-1).le.l1+(it3+1)*mi*nj) then
         lr=lw
         lw=lr+(it3+1)*mi*nj
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      else
c     .  the result may be installed at its final place
         lr=l1
      endif
c     perform extraction
      l=lr
      do 94 j = 0, nj-1
         do 93 i = 0, mi-1
            ind=istk(ili+i)-1+(istk(ilj+j)-1)*m3
            stk(l) = stk(l3+ind)
            if(it3.eq.1) stk(l+mn) = stk(l3+mn3+ind)
            l=l+1
 93      continue
 94   continue
c     form the resulting variable
      istk(il1)=1
      istk(il1+1)=mi
      istk(il1+2)=nj
      istk(il1+3)=it3
      if(lr.ne.l1) call unsfdcopy(mn*(it3+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mn*(it3+1)
      return
      end

c			================================================
