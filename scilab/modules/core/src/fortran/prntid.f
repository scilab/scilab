c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine prntid(id,argcnt,lunit)
c
c     print variable names
c
c
      include 'stack.h'
      integer id(nsiz,*),argcnt
      character name*(nlgh)
      logical printed
      integer base,esp
      parameter (base=8,esp=2)
      integer equal
      parameter (equal=50)


c
      if (argcnt .eq. -1) then
c     a  =
         l = 2
         buf(1:1)=' '
         call cvname(id(1,1),buf(l:l+nlgh-1),1)
         l1=l+nlgh
 02      l1=l1-1
         if(l1.eq.0) goto 03
         if(buf(l1:l1).eq.' ') goto 02
         l=l1+1
         buf(l1+1:l1+3) = '  '//alfa(equal+1)
         call basout(io,lunit,buf(1:l1+3))
 03      continue
      else
c     display d'une liste de noms

         buf(1:1)=' '
         l = 2
         j = 0

 10      j = j+1
         if(j.gt.argcnt) goto 20

         call cvname(id(1,j),name,1)
         ll=nlgh+1
 16      ll=ll-1
         if(ll.eq.0) then
            ll=1
            goto 17
         endif
         if(name(ll:ll).eq.' ') goto 16

 17      n1=(ll+esp)/(base+esp)
         if(n1*(base+esp).lt.ll+esp) n1=n1+1
         ll1=n1*(base+esp)
         if(l+ll1.gt.lct(5)) then
            if(l.gt.2) then
               call basout(io,lunit,buf(1:l-1))
               printed=.true.
               if(io.eq.-1)  return
               l=2
            endif
         endif
         buf(l:l+ll1-1)=name(1:ll)
         l=l+ll1
         printed=.false.
         goto 10
 20      if(.not.printed) call basout(io,lunit,buf(1:l-1))
      endif
      return
      end
