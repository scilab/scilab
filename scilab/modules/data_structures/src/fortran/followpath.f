c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine followpath(indtop,listtop,ilindi,voli,ilp,voll,ind,
     $     count,info,lwork,job)
c =============================================================
c     given
c     - a path stored in a "linear" index list ind stored in the
c     variable number indtop
c     and
c     - a list L begining at position stored in the
c     variable number listtop

c     - a current pointer on index list entries count

c     returns:
c     - the pointer ilp on the deapest sublist of L on the path indtop
c       available just by following the list structure of L.
c       i.e. ilp is the pointer on the sublist of L pointed to by
c       the path indtop(1:count)
c     - the vector of numerical indexes corresponding to indtop(1:count)

c     - voll (in stk worlds) of the designed sublist,
c      and the size (in stk worlds) voli of the designed subindex
c
c     lw is a pointer on the beginning of the free area in stk
c     info
c         1 : unknown named field
c         2 : current index is a matrix index of an mlist
c         3 : current index is not a single number
c         4 : current index points to a leaf of the  L list
c         5 : end of index list reached
c         6 : current index is 0 or greater than current sublist # of
c             fields
c     job
c     if job is true and end of index list is reached, the last index is
c     a name and the parent of the deapest object is a tlist or an mlist
c     the procedure returns the pointer and path of the parent instead
c     of the object itself (null insertion).

c =============================================================
c
      include 'stack.h'
c
      logical job
      integer indtop,listtop,ind(*)
      integer typi,count,oldcount,voll,voli

      integer strpos
      external strpos
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      oldcount=count
c
      ilind=iadr(lstk(indtop))
      if(istk(ilind).lt.0) then
         itop=istk(ilind+2)
         voli=istk(ilind+3)
         ilind=iadr(istk(ilind+1))
      else
         itop=indtop
         voli=lstk(itop+1)-lstk(itop)
      endif
c
      illist=iadr(lstk(listtop))
      if(istk(ilind).ne.15) then
c     .  special case if ind is not a list
         nlist=1
         count=1
         ilindi=ilind
         ilindir=ilindi
         illistir=illist
         if(istk(illist).lt.0) illist=iadr(istk(illist+1))
         illisti=illist
         goto 12

      endif

c     nlist is the size of the path index list
      nlist=istk(ilind+1)
      ll=sadr(ilind+3+nlist)

      illistir=illist
      illisti=illistir
      if(istk(illisti).lt.0)  illisti=iadr(istk(illisti+1))
c
 10   continue
c     next index
      count=count+1
c     move pointer to next entry in index list
      ilindi=iadr(ll+istk(ilind+1+count)-1)
      voli=istk(ilind+2+count)-istk(ilind+1+count)
      ilindir=ilindi
      if(istk(ilindi).lt.0) ilindi=iadr(istk(ilindi+1))
c     extract infos out of the current sublist
 12   typi=istk(illisti)
      mi=istk(illisti+1)
      llisti=sadr(illisti+mi+3)
      voll=istk(illisti+mi+2)-1+sadr(3+mi)
c     go ahead along the path

      if(typi.eq.13.or.typi.eq.11.or.typi.eq.130) then
c     .  not an extraction but a function call
         info=2
         goto 50
      endif

c
      if(istk(ilindi).eq.10) then
c     .  current element index is a name
         if (istk(ilindi+1)*istk(ilindi+2).ne.1) then
            info=3
            goto 50
c            call error(21)
c            return
         endif
         ilname=ilindi+6
         nname=istk(ilindi+5)-istk(ilindi+4)
c     .  check if it is an explicit field name
         if  (typi.ne.16.and.typi.ne.17) then
c     .     list with no explicit fields names
            info=1
            goto 50
         else
c     .     first field may contain the fields names
            ilfn=iadr(llisti)
            nn=istk(ilfn+1)*istk(ilfn+2)
            ilptr=ilfn+5
            lfn=ilptr+nn
c     .     look for corresponding index if any
            n=strpos(istk(ilptr),nn-1,istk(lfn),istk(ilname),nname)
            if(n.le.0) then
c     .        no such name in the field names
               info=1
               goto 50
            elseif(job.and.count.eq.nlist) then
c     .        null assignment into a named sublist of an tlist or mlist
               info=1
               goto 50
            endif
            n=n+1
         endif
      elseif(istk(ilindi).eq.15) then
c     .  current index is a multi dimensional matrix index
         info=2
         goto 50
      else
c     .  current index is a standard index
         if(typi.eq.17.or.(typi.ne.15.and.typi.ne.16)) then
c     .     matrix index
c     .     can be improved for some special mlist types for which
c     .     matrix entry addressing is predefined
            info=2
            goto 50
         endif
         lw=lwork
         call indxg(ilindi,mi,ili,nl,mx,lw,10)
         if(err.gt.0) return
         if(nl.ne.1) then
c     .     index is a vector or []
            info=3
            goto 50
         endif
         n=istk(ili)
         if(n.lt.0) then
            call error(21)
            return
         endif
      endif
c     n is the numerical index of the  sublist

      if(n.eq.0.or.n.gt.mi) then
         if (typi.eq.15.and.count.lt.nlist) then
c     .    current index is out of definable list indices and is not the
C     .    last of the index list
            call error(21)
            return
         endif
         ind(count)=n
         info=6
         goto 50
      endif

c     move pointer to indexed sub-list of L
      lti=sadr(illisti+3+mi)+istk(illisti+1+n)-1
      voll=istk(illisti+2+n)-istk(illisti+n+1)
      illisti=iadr(lti)
      illistir=illisti
c      if(voll.eq.0) then
c     .  undefined field found
c         info=7
c         goto 50
c
c     endif
      if(istk(illisti).lt.0) then
         kk=istk(illisti+2)
c         voll=lstk(kk+1)-lstk(kk)
         illisti=iadr(istk(illisti+1))
      endif
c
      ind(count)=n
      if(count+1.gt.nlist) then
c     .   end of index list reached
         info=5
         goto 50
      endif
      if(istk(illisti).ge.15.and.istk(illisti).le.17) goto 10

c     a leaf of L has been found
c     move pointer to next entry in index list
      count=count+1
      ilindi=iadr(ll+istk(ilind+1+count)-1)
      voli=istk(ilind+2+count)-istk(ilind+1+count)
      ilindir=ilindi
      if(istk(ilindi).lt.0) ilindi=iadr(istk(ilindi+1))
      typi=istk(illisti)
      info=4
      goto 50


 50   continue
c     end
      ilp=illistir
      ilindi=ilindir

      return
      end
