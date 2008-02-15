c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine hmcreate(lw,nz,sz,typv,iflag,retval)
c     lw : number of the variable
c     nz : number of dimensions
c     sz : dimensions
c     typv : Matlab class
c     iflag = 0 or 1 (real or complex)

      include 'stack.h'
      integer nz,sz(nz),szv,retval
      integer typv
      integer hmsz
      integer int8,uint8,int16,uint16,int32,uint32
      parameter(int8=8,uint8=9,int16=10,uint16=11,int32=12,uint32=13)
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      retval=0
      l0=lstk(lw)

      if (lw.gt.intersiz) then 
         buf = 'createvar :too many arguments in the stack' //
     $        ' edit stack.h and enlarge intersiz'
         call error(998)
         goto 999
      endif
      iii=hmsz(nz,sz,typv,iflag)
      err=l0+iii-lstk(bot)
      if(err.gt.0) then
         call error(17)
         goto 999
      endif
      lstk(lw+1)=l0+iii
      retval=1
      ntypes(lw-top+rhs) = 36
      iwhere(lw-top+rhs)= lstk(lw)
c      lad(lw-top+rhs) = should point to numeric data

c     create the mlist header
      
      il=iadr(l0)
      istk(il)=17
      istk(il+1)=3
      istk(il+2)=1
c
c     set mlist type entry
      call hmtyp(n1,'size')
      istk(il+3)=istk(il+2)+n1
      l=sadr(il+6)
      call hmtyp(istk(iadr(l)),'set')
      l=l+n1
c     set the size field 
      ilsz=iadr(l)
      istk(ilsz)=8
      istk(ilsz+1)=1
      istk(ilsz+2)=nz
      istk(ilsz+3)=4
      l=sadr(ilsz+4)
c      call int2db(nz,sz,1,stk(l),1)  FD
      call icopy(nz,sz,1,istk(ilsz+4),1)
      l=l+nz
      istk(il+4)=istk(il+3)+nz+2
c
c     set the value field
c     1 - compute the number of elements
      nv=1
      do 10 i=1,nz
         nv=nv*sz(i)
 10   continue
      ilv=iadr(l)
c     2 - set the value header
      if(typv.eq.6) then
c     .  double
         istk(ilv)=1
         istk(ilv+1)=nv
         istk(ilv+2)=1
         istk(ilv+3)=iflag
         l=sadr(ilv+4)  
         call dset(nv*(iflag+1),0.0d0,stk(l),1)
         szv=nv*(iflag+1)+2
         istk(il+5)=istk(il+4)+szv
      elseif(typv.eq.4) then
c     .  string
         istk(ilv)=10
         istk(ilv+1)=1
         istk(ilv+2)=1
         istk(ilv+3)=0
         istk(ilv+4)=1
         istk(ilv+5)=1+nv
         l=ilv+6
         call iset(nv,40,istk(l),1)
         szv=sadr(l+nv)-sadr(l)
         istk(il+5)=istk(il+4)+szv
      else
         if(typv.eq.int8) then
            it=1
         elseif(typv.eq.uint8) then
            it=11
         elseif(typv.eq.int16) then
            it=2
         elseif(typv.eq.uint16) then
            it=12
         elseif(typv.eq.int32) then
            it=4
        elseif(typv.eq.uint32) then
            it=14
         endif
         istk(ilv)=8
         istk(ilv+1)=nv
         istk(ilv+2)=1
         istk(ilv+3)=it
         l=ilv+4
         call genset(it,nv,0,istk(l),1)
         szv=sadr(memused(it,nv)+4)
         istk(il+5)=istk(il+4)+szv
      endif
 999  continue
      return
      end


      subroutine hmtyp(ivt,job)
c     definition of first field  of tlist's type: hm
c     tlist fields are:
c     dims
c     entries
c
      integer ivt(*),l
      character*(*) job
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(job.eq.'size') then
c        size of the data structure
         ivt(1)=11
      elseif(job.eq.'nchar') then
c        number of chars defining the type field
         ivt(1)=13
      elseif(job.eq.'nfield') then
c        number of fields in the tlist
         ivt(1)=3
      elseif(job.eq.'ptr') then
c        pointers on individual strings
         ivt(1)=1
         ivt(2)=3
         ivt(3)=7
         ivt(4)=14
      else
c        Character string Variable header
         ivt(1)=10
         ivt(2)=1
         ivt(3)=3
         ivt(4)=0
         ivt(5)=1
         l=8
c        entry (1,1) = "hm"
         ivt(l+1)=17
         ivt(l+2)=22
         ivt(6)=ivt(5)+2
         l=l+2
c        entry (2,1) = "dims"
         ivt(l+1)=13
         ivt(l+2)=18
         ivt(l+3)=22
         ivt(l+4)=28
         ivt(7)=ivt(6)+4
         l=l+4
c        entry (3,1) = "entries"
         ivt(l+1)=14
         ivt(l+2)=23
         ivt(l+3)=29
         ivt(l+4)=27
         ivt(l+5)=18
         ivt(l+6)=14
         ivt(l+7)=28
         ivt(8)=ivt(7)+7
         l=l+7
      endif
      return
      end


      integer function hmsz(nz,sz,typv,iflag)
c     sz = dimensions

      include 'stack.h'
      integer nz,sz(nz)
      integer nv
      integer typv
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     mlist header size
      il=1
      l=sadr(il+6)

c     first field size
      call hmtyp(n1,'size')
      l=l+n1

c     set the size field 
      ilsz=iadr(l)
      l=sadr(ilsz+4)+nz
c
c     set the value field size
      nv=1
      do 10 i=1,nz
         nv=nv*sz(i)
 10   continue

      ilv=iadr(l)
      if(typv.eq.6) then
c     .  double
         l=sadr(ilv+4)+nv*(iflag+1)
      elseif(typv.eq.4) then
c     .  string
         l=sadr(ilv+6+nv)
      else
         if(typv.eq.8) then
c         if(typv.eq.'int8') then
            it=1
         elseif(typv.eq.9) then
c         elseif(typv.eq.'uint8') then
            it=11
         elseif(typv.eq.10) then
c         elseif(typv.eq.'int16') then
            it=2
         elseif(typv.eq.11) then
c         elseif(typv.eq.'uint16') then
            it=12
         elseif(typv.eq.12) then
c         elseif(typv.eq.'int32') then
            it=4
         elseif(typv.eq.13) then
c        elseif(typv.eq.'uint32') then
            it=14
         endif
         l=sadr(ilv+4)+memused(it,nv)
      endif
      hmsz=l-1
      return
      end

