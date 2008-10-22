c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine cvname(id,str,job)
c     =====================================
c     Scilab internal coding of vars to string 
c     =====================================
      include 'stack.h'
      integer id(nsiz),name(nlgh),ch,blank,star
      character*(*) str
      data blank/40/,star/47/
c
      if(job.ne.0) then 
         i1=1
         do 15 l=1,nsiz
            idl=id(l)
            do 10 i=i1,i1+3
               k=(idl+128)/256
               if(k.lt.0) k=k-1
               ch=idl-256*k
               idl=k
               if(abs(ch).ge.csiz) ch=star
               if(ch.gt.0) then
                  str(i:i)=alfa(ch+1)
               else
                  str(i:i)=alfb(-ch+1)
               endif
 10         continue
            i1=i1+4
 15      continue
      else
         ln=min(nlgh,len(str))
         call cvstr(ln,name,str,0)
         if(ln.lt.nlgh) call iset(nlgh-ln,blank,name(ln+1),1)
         i1=1
         do 30 l=1,nsiz
            id(l)=0
            do 20 i=1,4
               ii=i1+4-i
               id(l)=256*id(l)+name(ii)
 20         continue
            i1=i1+4
 30      continue
      endif
      return
      end

      subroutine cvnamel(id,str,job,l)
c     =====================================
c     Scilab internal coding of vars to string 
c      plus elimination of trailing blank
c     =====================================
      include 'stack.h'
      integer id(nsiz),job,l
      character*(*) str
      call cvname(id,str,job)
      if (job.eq.1) then 
         l=1+nlgh
 42      l=l-1
         if(str(l:l).eq.' ') goto 42
      endif
      return
      end







