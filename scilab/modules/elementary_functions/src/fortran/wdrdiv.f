c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine wdrdiv(ar,ai,ia,br,ib,rr,ri,ir,n,ierr)
c!    purpose
c     computes r=a./b with a real vector and b complex vector
c     
c     ia,ib,ir : increment between two consecutive element of vectors a
c                b and r
c     ar,ai    : arrays containing a real and imaginary parts
c     br       : array  containing b vector
c     rr,ri    : arrays containing r real and imaginary parts
c     n        : vectors length
c     ierr     : returned error flag:
c                o   : ok
c                <>0 : b(ierr)=0
c
      double precision ar(*),ai(*),br(*),rr(*),ri(*)
      integer ia,ib,ir,n
c     wr, wi used because rr, ri may share same mem as ar,ai or br,bi
      double precision wr,wi
      jr=1
      jb=1
      ja=1
      ierr=0
      if (ia.eq.0) then
         do 10 k=1,n
            call wddiv(ar(ja),ai(ja),br(jb),wr,wi,ierr1)
            rr(jr)=wr
            ri(jr)=wi
            if(ierr1.ne.0) then
               ierr=k
c               return
            endif
            jr=jr+ir
            jb=jb+ib
 10      continue
      elseif(ib.eq.0) then
         if(br(jb).eq.0.0d0) then
            ierr=1
c            return
         endif
         do 11 k=1,n
            call wddiv(ar(ja),ai(ja),br(jb),wr,wi,ierr1)
            rr(jr)=wr
            ri(jr)=wi
            jr=jr+ir
            ja=ja+ia
 11      continue
      else
         do 12 k=1,n
            call wddiv(ar(ja),ai(ja),br(jb),wr,wi,ierr1)
            rr(jr)=wr
            ri(jr)=wi
            if(ierr1.ne.0) then
               ierr=k
c               return
            endif
            jr=jr+ir
            jb=jb+ib
            ja=ja+ia
 12      continue
      endif
      end
