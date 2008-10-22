c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Francois VOGEL, May      2004 - Bug 718 fixed
c Serge Steer,    May      2004 - Bug 719 fixed
c Francois VOGEL, January  2005 - Bug 1187 fixed
c Francois VOGEL, February 2005 - Vector argument now allowed
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intsetbpt
      include 'stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,getsmat,getrvect,checkval,eqid
      integer topk

      topk=top

      if(.not.checklhs('setbpt',1,1)) return
      if(.not.checkrhs('setbpt',1,2)) return

c check line number(s) argument
      if(rhs.eq.2) then
         if(.not.getrvect('setbpt',topk,top,m,n,lv)) return
         nbp=m*n
         do 310 i=1,nbp
            lnb=stk(lv+i-1)
            if (int(stk(lv+i-1)).ne.stk(lv+i-1).or.lnb.lt.1) then
               buf='Breakpoint line number must be a positive integer'
               call error(9997)
               return
            endif
 310     continue
c silently remove duplicate line numbers in vector argument
 312     do 313 i=0,nbp-2
            do 314 j=i+1,nbp-1
               if(stk(lv+i).eq.stk(lv+j)) then
                  stk(lv+j)=stk(lv+nbp-1)
                  nbp=nbp-1
                  goto 312
               endif
 314        continue
 313     continue
         lnb=stk(lv)
         top=top-1
      else
c rhs is 0 or 1 (no line number provided)
         lnb=1
         nbp=1
      endif

c check macro name argument
      if(.not.getsmat('setbpt',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('setbpt',m*n,1) ) return
      call namstr(id,istk(l),n1,0)

      if(nmacs.gt.0) then
         do 323 kmac=1,nmacs
            if(eqid(macnms(1,kmac),id)) goto 324
 323     continue
      endif

c required macro has no breakpoint yet
      if (nmacs.ge.maxdb) then
         buf='Too many functions contain breakpoints'
         call error(9999)
         return
      endif
      nmacs=nmacs+1
      call putid(macnms(1,nmacs),id)
      if (lgptrs(nmacs)+nbp.gt.maxbpt) then
         buf='Too many defined breakpoints'
         call error(9998)
         return
      endif
  
      lgptrs(nmacs+1)=lgptrs(nmacs)+nbp
      if(nbp.eq.1) then
         bptlg(lgptrs(nmacs))=lnb
      else
         do 3232 i=0,nbp-1
            bptlg(lgptrs(nmacs)+i)=stk(lv+i)
 3232    continue
      endif
      goto 330

c required macro already has breakpoints defined
c    remove duplicate line requests from vector argument
 324  if (rhs.lt.2) then
         do 3244 kk=lgptrs(kmac),lgptrs(kmac+1)-1
            if (bptlg(kk).eq.lnb) goto 330
 3244    continue
      else
 3243    do 3241 kk=lgptrs(kmac),lgptrs(kmac+1)-1
            do 3242 j=0,nbp-1
               if (bptlg(kk).eq.stk(lv+j)) then
                  stk(lv+j)=stk(lv+nbp-1)
                  nbp=nbp-1
                  if (nbp.eq.0) goto 330
                  goto 3243
               endif
 3242       continue
 3241    continue
      endif

 325  if(kmac.eq.nmacs) then
c    required macro is the last in the list,
c    just add the new breakpoints at the end of the list
         nae=lgptrs(nmacs+1)-lgptrs(nmacs)
         lgptrs(nmacs+1)=lgptrs(nmacs+1)+nbp
         if (lgptrs(nmacs+1)-1.gt.maxbpt) then
            buf='Too many defined breakpoints'
            call error(9998)
            return
         endif
         if(rhs.lt.2) then
            bptlg(lgptrs(nmacs+1)-1)=lnb
         else
            do 3251 i=0,nbp-1
               bptlg(lgptrs(nmacs)+nae+i)=stk(lv+i)
 3251       continue
         endif
      else
c    required macro is not the last in the list,
c    first move the existing breakpoints, and then
c    add the new breakpoints
        if (lgptrs(nmacs+1)+nbp-1.gt.maxbpt) then
            buf='Too many defined breakpoints'
            call error(9998)
            return
         endif
         do 326 kk=nmacs,kmac,-1
            l0=lgptrs(kk)
            call icopy(lgptrs(kk+1)-l0,bptlg(l0),-1,bptlg(l0+nbp),-1)
            lgptrs(kk+1)=lgptrs(kk+1)+nbp
 326     continue
         if(rhs.lt.2) then
            bptlg(lgptrs(kmac))=lnb
         else
            do 327 i=0,nbp-1
               bptlg(lgptrs(kmac)+i)=stk(lv+i)
 327        continue
         endif
      endif

c the end...
 330  continue
      call objvide('setbpt',top)
      return
      end

