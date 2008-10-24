c
c @TODO : Remove that file.
c
c This routine has been rewrritten.
c see src/c/matmult.c
c

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

  	  subroutine matmult
c
c     matrix/vector multiplications

      include 'stack.h'
c
      double precision sr,si
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
      itr=max(it1,it2)
c

      if(mn1.eq.0.or.mn2.eq.0) then
c     .  []*a , a*[]
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
      elseif (mn1 .eq. 1) then
c     .  cst*a
         sr = stk(l1)
         si=0.0d+0
         if(it1.eq.1) si = stk(l1+1)
         if (m1.lt.0) then
            if(mn2.eq.1) then
c     .     eye*cst
               istk(il1+1)=m1
               istk(il1+2)=n1
               istk(il1+3)=itr
            else
               call error(14)
               return
            endif
         else
            istk(il1+1)=m2
            istk(il1+2)=n2
            istk(il1+3)=itr
         endif
         call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     le scalaire et la matrice sont reel
            call dscal(mn2,sr,stk(l1),1)
         elseif(it21.eq.1) then
c     .     la matrice est complexe le scalaire est reel
            call dscal(mn2,sr,stk(l1),1)
            call dscal(mn2,sr,stk(l1+mn2),1)
         elseif(it21.eq.2) then
c     .     la matrice est reelle, le scalaire est complexe
            lstk(top+1)=l1+mn2*(itr+1)
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn2,stk(l1),1,stk(l1+mn2),1)
            call dscal(mn2,sr,stk(l1),1)
            call dscal(mn2,si,stk(l1+mn2),1)
         elseif(it21.eq.3) then
c     .     la matrice et le scalaire sont complexes
            call wscal(mn2,sr,si,stk(l1),stk(l1+mn2),1)
         endif
         lstk(top+1)=l1+mn2*(itr+1)
      elseif (mn2 .eq. 1) then
c     .  a*cst
         if(m2.lt.0) then
            call error(14)
            return
         endif
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     la matrice et le scalaire sont reel
            call dscal(mn1,stk(l2),stk(l1),1)
         elseif(it21.eq.1) then
c     .     la matrice est reelle le scalaire est complexe
            sr = stk(l2)
            si = stk(l2+1)
            lstk(top+1)=l1+mn1*(itr+1)
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn1,stk(l1),1,stk(l1+mn1),1)
            call dscal(mn1,si,stk(l1+mn1),1)
            call dscal(mn1,sr,stk(l1),1)
            istk(il1+3)=itr
         elseif(it21.eq.2) then
c     .     la matrice est complexe, le scalaire est reel
            sr = stk(l2)
            call dscal(mn1,sr,stk(l1),1)
            call dscal(mn1,sr,stk(l1+mn1),1)
         elseif(it21.eq.3) then
            sr = stk(l2)
            si = stk(l2+1)
c     .     la matrice et le scalaire sont complexes
            call wscal(mn1,sr,si,stk(l1),stk(l1+mn1),1)
         endif
      else
c     .  matrix*matrix
         if (n1 .ne. m2) then
            call error(10)
            return
         endif
         lr=l2+mn2*(it2+1)
c     .  m1*n2 may overflow
         temp=float(lr)+float(m1)*n2*(itr+1)-lstk(bot)
         if(temp.gt.0.0d0) then
            err=int(temp)
            call error(17)
            return
         endif
         if(it1*it2.ne.1) then
*           remplacement de dmmul par dgemm (Bruno le 31/10/2001)
            call dgemm('n','n',m1,n2,n1,1.d0,stk(l1),m1,stk(l2),m2,
     $           0.d0,stk(lr),m1)
            if(it1.eq.1) call dgemm('n','n',m1,n2,n1,1.d0,stk(l1+mn1),
     $           m1,stk(l2),m2,0.d0,stk(lr+m1*n2),m1)
            if(it2.eq.1) call dgemm('n','n',m1,n2,n1,1.d0,stk(l1),m1,
     $           stk(l2+mn2),m2,0.d0,stk(lr+m1*n2),m1)
         else
c     .     a et a2 sont complexes
            call wmmul(stk(l1),stk(l1+mn1),m1,stk(l2),stk(l2
     $           +mn2),m2,stk(lr),stk(lr+m1*n2),m1,m1,n1,n2)
         endif
         call unsfdcopy(m1*n2*(itr+1),stk(lr),1,stk(l1),1)
         istk(il1+2)=n2
         istk(il1+3)=itr
         lstk(top+1)=l1+m1*n2*(itr+1)
      endif
      return
      end

c			================================================
