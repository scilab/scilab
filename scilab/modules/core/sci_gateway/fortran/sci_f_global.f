c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intglobal()
      include 'stack.h'
      logical checklhs,getsmat,checkval
      integer topk,vol
      integer id(nsiz),blank,percen,ch
      integer iadr,sadr
      data blank/40/,percen/56/
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.le.0) then
         call error(39)
         return
      endif
      if(.not.checklhs('global',1,1)) return
      topk=top
      do 10 i=1,rhs
c     .  get the variable name
         if(.not.getsmat('global',topk,top,m,n,1,1,lr,nn)) return
         if(.not.checkval('global',m*n,1)) return
c     .  check if it is a correct variable name
         ch=abs(istk(lr))
         if(.not.(ch.eq.percen.or.(ch.gt.9.and.ch.lt.blank))) then
            err=rhs+1-i
            call error(116)
            return
         endif
         if(nn.gt.1) then
            do 01 k=1,nn-1
               ch=abs(istk(lr+k))
               if(ch.ge.blank) then
                  err=rhs+1-i
                  call error(116)
                  return
               endif
 01         continue
         endif
         call namstr(id,istk(lr),nn,0)
c     .  check if the variable is already defined in global area
         fin=-5
         call stackg(id)
         if(err.gt.0) return
         if (gtop+2.gt.isizt) then
            call error(262)
            return
         endif
         if(fin.eq.0) then
c     .     the variable is not defined yet in global area 
c     .     check if it is defined in local area
            fin=-6
            call stackg(id)
            if(err.gt.0) return
            if(fin.eq.0) then
c     .        no, create an empty variable in the global area
               vol=5
               if (lstk(gtop+1)+vol.gt.lstk(gbot)) then
c     .           not enough memory, realloc
                  mem=lstk(gbot)-lstk(isiz+2)+max(vol+1,10000)
                  call reallocglobal(mem)
                  if(err.gt.0) return
               endif
               gtop=gtop+1
               call putid(idstk(1,gtop),id)
               infstk(gtop)=0
               il=iadr(lstk(gtop))
               istk(il)=1
               istk(il+1)=0
               istk(il+2)=0
               istk(il+3)=0
               lstk(gtop+1)=sadr(il+4)
            else
c     .        yes, move it to the global area
               vol=lstk(fin+1)-lstk(fin)
               if (lstk(gtop+1)+vol+10.ge.lstk(gbot)) then
c     .           max(vol+1,100000) to avoid too many reallocation
                  mem=lstk(gbot)-lstk(isiz+2)+max(vol+1,10000)
                  call reallocglobal(mem)
                  if(err.gt.0) return
               endif

               gtop=gtop+1

               call putid(idstk(1,gtop),id)
               infstk(gtop)=0
               lstk(gtop+1)=lstk(gtop)+vol
               call unsfdcopy(vol,stk(lstk(fin)),1,stk(lstk(gtop)),1)
            endif
            kg=gtop 
         else
c     .    the variable is  already defined  in the global area
            kg=fin
         endif
c     .  create a link to the created or moved variable with the same
c     .  name in the local area
         top=top-1
         call createref(iadr(lstk(kg)),kg,lstk(kg+1)-lstk(kg))
         call stackp(id,0)
         if(err.gt.0) return
         infstk(fin)=2
 10   continue
c     return a null matrix
      top=top+1
      il=iadr(lstk(top))
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      return
      end
     
