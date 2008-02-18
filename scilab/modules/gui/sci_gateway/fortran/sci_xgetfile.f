c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intsxgetfile(fname)
C     ================================================================
c     SCILAB function : xgetfile, fin = 1
C     ================================================================
       character*(*) fname
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cresmat2,getsmat,checkval,bufstore
       logical crepointer,crestring,isoptlw,getwsmat
       include 'stack.h'
       character name*(nlgh)
c
       integer iadr, sadr
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,3)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable a (number 1)
c       
       if(rhs .le. 0) then
        top = top+1
        rhs = rhs+1
        nlr1 = 3
        if(.not.cresmat2(fname,top,nlr1,lr1)) return
        call cvstr(nlr1,istk(lr1),'*.*',0)
       endif
c       checking variable dirname (number 2)
c       
       if(rhs .le. 1) then
        top = top+1
        rhs = rhs+1
        nlr2 = 1
        if(.not.cresmat2(fname,top,nlr2,lr2)) return
        call cvstr(nlr2,istk(lr2),'.',0)
       endif

       if(rhs .le. 2) then
        top = top+1
        rhs = rhs+1
        nlr3 = 9
        if(.not.cresmat2(fname,top,nlr3,lr3)) return
        call cvstr(nlr3,istk(lr3),'File menu',0)
       endif

       if(.not.crepointer(fname,top+1,lw3)) return


       if (rhsk.eq.3) then 
          idir=1
          if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1))
     $         return
          if(.not.checkval(fname,m1*n1,1)) return
          if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return

          if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $         return
          if(.not.checkval(fname,m2*n2,1)) return
          if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr2,nlr2)) return

          if (.not.getwsmat(fname,top,top-rhs+3,m3,n3,il3,ild3)) return

          call xgetfile(buf(lbufi1:lbuff1),buf(lbufi2:lbuff2),
     $         stk(lw3),ne5,err,idir,istk(il3),istk(ild3),m3*n3)
       endif
       if (rhsk.eq.2) then 
          if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1))
     $         return
          if(.not.checkval(fname,m1*n1,1)) return
          if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return

          if (isoptlw(top,top-rhs+2,name)) then 
             if (name.ne.'title') then 
                buf = fname // 'optional argument must be title='
                call error(999)
                return
             endif
             idir = 0
             if (.not.getwsmat(fname,top,top-rhs+2,m2,n2,il2,ild2))
     $            return
             call xgetfile(buf(lbufi1:lbuff1),'.',
     $            stk(lw3),ne5,err,idir,istk(il2),istk(ild2),m2*n2)
          else
             idir = 1
             if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $            return
             if(.not.checkval(fname,m2*n2,1)) return
             if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr2,nlr2)) return

             if (.not.getwsmat(fname,top,top-rhs+3,m3,n3,il3,ild3))
     $            return
             call xgetfile(buf(lbufi1:lbuff1),buf(lbufi2:lbuff2),
     $            stk(lw3),ne5,err,idir,istk(il3),istk(ild3),m3*n3)
          endif
       endif
       if ( rhsk.eq.1) then 
          idir = 0
          if (isoptlw(top,top-rhs+1,name)) then 
             if (name.ne.'title') then 
                buf = fname // ' optional argument must be title='
                call error(999)
                return
             endif
             if (.not.getwsmat(fname,top,top-rhs+1,m1,n1,il1,ild1))
     $            return
             call xgetfile('*.*','.',
     $            stk(lw3),ne5,err,idir,istk(il1),istk(ild1),m1*n1)
          else
             if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1))
     $            return
             if(.not.checkval(fname,m1*n1,1)) return
             if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return

             if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $            return
             if(.not.checkval(fname,m2*n2,1)) return
             if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr2,nlr2)) return

             if (.not.getwsmat(fname,top,top-rhs+3,m3,n3,il3,ild3))
     $            return
             call xgetfile(buf(lbufi1:lbuff1),buf(lbufi2:lbuff2),
     $            stk(lw3),ne5,err,idir,istk(il3),istk(ild3),m3*n3)
          endif
       endif
       if (rhsk.le.0 ) then 
          idir=0
          if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1))
     $         return
          if(.not.checkval(fname,m1*n1,1)) return
          if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
          
          if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $         return
          if(.not.checkval(fname,m2*n2,1)) return
          if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr2,nlr2)) return
          
          if (.not.getwsmat(fname,top,top-rhs+3,m3,n3,il3,ild3))
     $         return
          call xgetfile(buf(lbufi1:lbuff1),buf(lbufi2:lbuff2),
     $         stk(lw3),ne5,err,idir,istk(il3),istk(ild3),m3*n3)
       endif
C
C      the result is a string and xgetfile 
C      will have to create ( malloc ) space for storing the result
C      we will keep track of this allocated string through lw3.

       if(err .gt. 0) then 
        buf = fname // 'Internal Error' 
        call error(999)
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.crestring(fname,top,ne5,ilrs)) return
        call ccharf(ne5,stk(lw3),istk(ilrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
C     ================================================================
