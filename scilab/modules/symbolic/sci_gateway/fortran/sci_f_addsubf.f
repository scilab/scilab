c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine sciaddfsubf(fname)
c     ----------------------------
      character*(*) fname
      include 'stack.h'
      
      logical isnum
      integer plus,minus
      integer iadr,sadr
      data plus/45/,minus/46/

      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      isg=1
      if(fin.eq.2) isg=-1
      ilptr1=iadr(lstk(top+1))+1
c     le "+1" est du a expsum qui peut renvoyer une chaine 1 carac plus long 
c     que la chaine donnee
      il1=iadr(lstk(top-1))
      il=il1
      if(istk(il1).ne.10) then
         err=1
         call error(55)
         return
      endif
      il2=iadr(lstk(top))
      if(istk(il2).ne.10) then
         err=2
         call error(55)
         return
      endif
      is=plus
c     
      if(istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(36)
         return
      endif
      n1=istk(il1+5)-1
      il1=il1+6
      il0=il1
      maxnp=iadr(lstk(bot))-ilptr1
      call expsum(1,istk(il1),n1,istk(ilptr1),np1,maxnp,err)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      ilptr2=ilptr1+np1+1
      if(istk(il2+1)*istk(il2+2).ne.1) then
         err=2
         call error(36)
         return
      endif
      n2=istk(il2+5)-1
      il2=il2+6
      top=top-1
      maxnp=iadr(lstk(bot))-ilptr2
      call expsum(isg,istk(il2),n2,istk(ilptr2),np2,maxnp,err)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      inum=0
      do 1005 i1=1,np1
         it1=il1-1+istk(ilptr1-1+i1)
         nt1=istk(ilptr1+i1)-istk(ilptr1-1+i1)
         if(istk(it1+1).eq.0.and.n1.eq.2) then
            call icopy(n2,istk(il2),1,istk(il1),1)
            il1=il1+n2
            goto 1010
         endif
         if(isnum(istk(it1+1),nt1-1,inum1)) then
            if(istk(it1).eq.minus) inum1=-inum1  
            inum=inum+inum1
            istk(it1)=0
         endif
         i2=0
 1004    i2=i2+1
         if(i2.gt.np2) goto 1005
         it2=il2-1+istk(ilptr2-1+i2)
         if(istk(it2).eq.0) goto 1004
         nt2=istk(ilptr2+i2)-istk(ilptr2-1+i2)
         if(istk(it2+1).eq.0.and.n2.eq.2) then
            if (istk(it1).ne.0) then
               il1=il1+n1
               goto 1010
            else
               ilw=il1
               goto 1008
            endif
         endif
         if(isnum(istk(it2+1),nt2-1,inum2)) then
            if(istk(it2).eq.minus) inum2=-inum2
            inum=inum+inum2
            istk(it2)=0
            goto 1004
         endif
         if (istk(it1).ne.istk(it2).and.nt1.eq.nt2) then
c     on regarde si les termes sont egaux
            do 1002 k=2,nt1
               if(istk(it1-1+k).ne.istk(it2-1+k)) goto 1004
 1002       continue
            istk(it2)=0
            istk(it1)=0
            goto 1005
         endif
         goto 1004
 1005 continue
c     
      ilw=il1
      do 1006 i1=1,np1
         it1=il1-1+istk(ilptr1-1+i1)
         nt1=istk(ilptr1+i1)-istk(ilptr1-1+i1)
         if(istk(it1).ne.0) then
            call icopy(nt1,istk(it1),1,istk(ilw),1)
            ilw=ilw+nt1
         endif
 1006 continue
      do 1007 i2=1,np2
         it2=il2-1+istk(ilptr2-1+i2)
         nt2=istk(ilptr2+i2)-istk(ilptr2-1+i2)
         if(istk(it2).ne.0) then
            call icopy(nt2,istk(it2),1,istk(ilw),1)
            ilw=ilw+nt2
         endif
 1007 continue
c
 1008 if(inum.ne.0) then
         if(inum.lt.0) then
            istk(ilw)=minus
            ilw=ilw+1
            inum=-inum
         elseif(ilw.ne.il0) then
            istk(ilw)=plus
            ilw=ilw+1
         endif
         call intstr(inum,istk(ilw),ni,0)
         ilw=ilw+ni
      endif
c     
      il1=ilw
      if(il1.eq.il0) then
         istk(il1)=0
         il1=il1+1
      endif
c     
 1010 n1=il1-il0
      if(istk(il0).eq.plus) then
         call icopy(n1-1,istk(il0+1),1,istk(il0),1)
         n1=n1-1
         il1=il1-1
      endif
      istk(il+5)=n1+1
      lstk(top+1)=sadr(il1+1)
      goto 9999
      
 9999 return
      end
      
