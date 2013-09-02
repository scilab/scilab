c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2006 - INRIA - Allan CORNET
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine adjuststacksize(mem,offset)
c     ajuste la stacksize apres allocation
c     routine for intstacksize
      INCLUDE 'stack.h'
      integer offset,p
      integer iadr,sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      lbot=lstk(isiz)-lstk(bot)
      ltop=lstk(top+1)-lstk(1)

      offset=offset+1

      l1=lstk(1)
      l2=lstk(bot)
c     copy the top of the stack
      if (top.gt.0) call unsfdcopy(ltop,stk(l1),1,stk(offset),1)
      kd1=offset-l1
      do 30 k=1,top+1
         lstk(k)=lstk(k)+kd1
 30   continue

c     copy the bottom of the stack
      call unsfdcopy(lbot,stk(l2),1,stk(offset+mem-lbot),1)
      kd2=offset+mem-lbot-l2
      do 40 k=bot,isiz
         lstk(k)=lstk(k)+kd2
 40   continue

      call freeStackLastMemory()
c     update reference variables
      do 41 i=1,top
         if(infstk(i).ne.2) then
            il=iadr(lstk(i))
            if(istk(il).lt.0) istk(il+1)=istk(il+1)+kd1
         endif
 41   continue
      do 42 i=bot,isiz-1
         if(infstk(i).ne.2) then
            il=iadr(lstk(i))
            if(istk(il).lt.0) istk(il+1)=istk(il+1)+kd2
         endif
 42   continue

c     rebuild asolute pointers if necessary see macro.f  and run.f
      if(macr.gt.0) then
         lpt1=lpt(1)
c     Check if a compiled macro is running
         p=pt+1
 45      p=p-1
         if((rstk(p).ge.601.and.rstk(p).le.603).or.rstk(p).eq.605) then
            call adjuststkptr(pstk(p),kd1,kd2,l2)
         elseif(rstk(p).eq.604.or.rstk(p).eq.606) then
            call adjuststkptr(ids(1,p),kd1,kd2,l2)
            if(rstk(p).le.606) call adjuststkptr(ids(2,p),kd1,kd2,l2)
         elseif(rstk(p).ge.611.and.rstk(p).le.616) then
             call adjuststkptr(ids(1,p),kd1,kd2,l2)
             if(rstk(p).ge.614) call adjuststkptr(pstk(p),kd1,kd2,l2)
         elseif(rstk(p).eq.501.or.rstk(p).eq.502.or.rstk(p).eq.503) then
            k = lpt1 - (13+nsiz)
            lpt1 = lin(k+1)
            if(rstk(p).eq.501.or.rstk(p).eq.502) then
               call adjuststkptr(lin(k+6),kd1,kd2,l2)
               call adjuststkptr(lin(k+7),kd1,kd2,l2)
               call adjuststkptr(ids(3,p),kd1,kd2,l2)
            endif
         endif
         if(p.gt.0) goto 45
      endif

      leps=sadr(iadr(lstk(isiz-5)) +4)
      end
c ====================================================================
      subroutine adjustgstacksize(mem,offset,l)
      include 'stack.h'
      integer offset
      integer l
      integer iadr
      logical eqid
c
      iadr(l)=l+l-1

      offset=offset+1
      call unsfdcopy(l,stk(lstk(isiz+2)),1,stk(offset),1)
      kd=offset-lstk(isiz+2)
      do 05 k=isiz+2,gtop+1
         lstk(k)=lstk(k)+kd
 05   continue
      call freeGlobalStackLastMemory()
      lstk(gbot)=lstk(isiz+2)+mem
c     following lines are necessary because of use of il=iadr(istk(il+1))
c     to get variable pointed by an indirect variable.
c     it should be no more useful with il=iadr(lstk(istk(il+2)))
      do 09 i = isiz+2, gtop
c     update pointers in variables which refer this global var
      do 07 j=bot,isiz-1
         if(infstk(j).eq.2) then
            if(eqid(idstk(1,j),idstk(1,i))) then
c     .        variable j refers this global var
               ilj=iadr(lstk(j))
               istk(ilj+1)=lstk(i)
               istk(ilj+2)=i
            endif
         endif
 07   continue
 09   continue
      end
c ====================================================================
      subroutine adjuststkptr(p,topoffset,botoffset,lbot)
c     routine for adjuststacksize
      integer p,topoffset,botoffset,lbot
      if(p.ge.2*lbot) then
         p=p+2*botoffset
      else
         p=p+2*topoffset
      endif
      end
c ====================================================================
