c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine storeglobal(id,k)
      INCLUDE 'stack.h'
      logical update
      integer iadr
      integer id(nsiz)
c
      logical eqid,new
      integer v,vk
c
      iadr(l)=l+l-1   

   
      il=iadr(lstk(k))
      kg=istk(il+2)
      if (.not.eqid(idstk(1,kg),id)) then
c     .  global variable had moved look for it by name (is it possible?)
         k=vsiz+1
 10      continue
         k=k+1
         if(k.gt.gtop) then
            call error(4)
            return
         endif
         if (.not.eqid(idstk(1,k),id)) goto 10
         kg=k
      endif
c
      if(istk(iadr(lstk(top))).eq.0) then
c     replace null variable by an empty matrix
         top=top-1
         call defmat
      endif
c
      vk=lstk(kg+1)-lstk(kg)
      v=lstk(top+1)-lstk(top)
c
      update=.false.
      if (v.ne.vk) then
c     .  new variable does not fit the size of the old one
         if (kg .lt. gtop) then
c     .     make room to install new variable
            ls = lstk(kg+1)
            ll = ls + v - vk
            if(v.gt.vk) then
c     .        new is bigger, move bottom down
               if (lstk(gtop+1)+v-vk.gt.lstk(gbot)) then
c     .           not enought memory, realloc
                  mem=lstk(gbot)-lstk(isiz+2)+max(v+1,10000)
                  call reallocglobal(mem)
                  if(err.gt.0) return
                  ls=lstk(kg+1)
                  ll = ls + v - vk
               endif
               call unsfdcopy(lstk(gtop+1)-lstk(kg+1),stk(ls),-1,
     $              stk(ll),-1)
c     .        update pointer               
            else
c     .        new is smaller, move bottom up
               call unsfdcopy(lstk(gtop+1)-lstk(kg+1),stk(ls),1,
     $              stk(ll),1)
            endif
            update=.true.
         else
            if(v.gt.vk) then
               if (lstk(gtop+1)+v-vk.gt.lstk(gbot)) then
c     .        not enought memory, realloc
                  mem=lstk(gbot)-lstk(isiz+2)+max(v+1,10000)
                  call reallocglobal(mem)
                  if(err.gt.0) return
               endif
            endif
         endif

c     .  update pointers on variables
         do 20 i=kg+1,gtop+1
            lstk(i)=lstk(i) + v - vk
 20      continue

        if(update) then


c     .     following lines are necessary because of use of 
c     .     il=iadr(istk(il+1)) to get variable pointed by an indirect
c     .     variable.
c     .     it should be no more useful with il=iadr(lstk(istk(il+2)))

           do 22 i = kg+1, gtop
c     .        update pointers in variables which refer this global var
              do 21 j=bot,isiz-1
                 if(infstk(j).eq.2) then
                    if(eqid(idstk(1,j),idstk(1,i))) then
c     .                 variable j refers this global var
                       ilj=iadr(lstk(j))
                       istk(ilj+1)=lstk(i)
                       istk(ilj+2)=i
                    endif
                 endif
 21           continue
 22        continue
        endif

      endif

c     copy new value
 25   call unsfdcopy(v,stk(lstk(top)),1,stk(lstk(kg)),1)
c     update type of the local pointer
      istk(il)=-abs(istk(iadr(lstk(top))))
      fin=kg
      top=top-1
      return
      end


      subroutine reallocglobal(mem)
c     Copyright INRIA
      INCLUDE 'stack.h'
      integer offset
      logical eqid
      integer iadr
c
      iadr(l)=l+l-1
c
      l=lstk(gtop+1)-lstk(isiz+2)
      call scigmem(mem+1,offset)
      if(offset.eq.0) then
         call error(112)
         return
      endif
      offset=offset+1
      call unsfdcopy(l,stk(lstk(isiz+2)),1,stk(offset),1)
      kd=offset-lstk(isiz+2)
      do 05 k=isiz+2,gtop+1
         lstk(k)=lstk(k)+kd
 05   continue 
      call freegmem()
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
      return
      end
