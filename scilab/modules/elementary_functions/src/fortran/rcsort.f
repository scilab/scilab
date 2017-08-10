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
      subroutine rcsort(test,isz,iptr,iv,n,index)
c
c!purpose
c     rcsort sort a set of integer records ,maintaining an index array
c
c!calling sequence
c     subroutine rcsort(test,isz,iptr,iv,n,index)
c     integer n,index(n),iv(*),isz(n)
c     integer iptr(n+1)
c
c     test    : external integer function which define formal order for
c               records 
c               test(r1,l1,r2,l2)
c               where
c               l1 is the length or record r1
c               l2 is the length or record r2
c               returns
c                1 :if record r1 is greater than r2
c               -1 :if record r1 is less than r2
c                0 :if record r1 is equal to r2
c     isz     : vector of records sizes
c     iptr    : table of records adresses in iv
c     iv      : table of records values
c     n       : size of vector of record and index
c     index   : array containing on return index of sorted array
c
c!method
c     quick sort method is used
c!restriction
c     n must be less than 2**(50/2) ! due to lengh of work space mark
c
      dimension mark(50),index(n)
      integer iptr(*),isz(n),iv(*),av,x,it,s,as
      integer test
      external test
c
c  set index array to original order .
      do 10 i=1,n
         index(i)=i
 10   continue
c  check that a trivial case has not been entered .
      if(n.eq.1) goto 200
      if(n.ge.1) goto 30
      goto 200
c     'm' is the length of segment which is short enough to enter
c     the final sorting routine. it may be easily changed.
 30   m=12
c     set up initial values.
      la=2
      is=1
      if=n
      do 190 mloop=1,n
c     if segment is short enough sort with final sorting routine .
         ifka=if-is
         if((ifka+1).gt.m) goto 70
c*********final sorting ***
c     ( a simple bubble sort )
         is1=is+1
         do 60 j=is1,if
            i=j
 40         continue
            it=test(iv(iptr(i-1)),isz(i-1),iv(iptr(i)),isz(i))
            if(it.eq.1) goto 60
            if(it.eq.-1) goto 50
            if(index(i-1).lt.index(i)) goto 60
 50         av=iptr(i-1)
            iptr(i-1)=iptr(i)
            iptr(i)=av
c     
            as=isz(i-1)
            isz(i-1)=isz(i)
            isz(i)=as
c     
            int=index(i-1)
            index(i-1)=index(i)
            index(i)=int
c     
            i=i-1
            if(i.gt.is) goto  40
 60      continue
         la=la-2
         goto 170
c     *******  quicksort  ********
c     select the number in the central position in the segment as
c     the test number.replace it with the number from the segment's
c     highest address.
 70      iy=(is+if)/2
         x=iptr(iy)
         intest=index(iy)
         s=isz(iy)

         iptr(iy)=iptr(if)
         isz(iy)=isz(if)
         index(iy)=index(if)
c     the markers 'i' and 'ifk' are used for the beginning and end
c     of the section not so far tested against the present value
c     of x .
         k=1
         ifk=if
c     we alternate between the outer loop that increases i and the
c     inner loop that reduces ifk, moving numbers and indices as
c     necessary, until they meet .
         do 110 i=is,if
            it=test(iv(x),s,iv(iptr(i)),isz(i))
            if(it.lt.0) goto 110
            if(it.gt.0) goto 80
            if(intest.gt.index(i)) goto 110
 80         if(i.ge.ifk) goto 120
            iptr(ifk)=iptr(i)
            index(ifk)=index(i)
            isz(ifk)=isz(i)
            k1=k
            do 100 k=k1,ifka
               ifk=if-k
               it=test(iv(iptr(ifk)),isz(ifk),iv(x),s)
               if(it.lt.0) goto 100
               if(it.gt.0) goto 90
               if(intest.le.index(ifk)) goto 100
 90            if(i.ge.ifk) goto 130
               iptr(i)=iptr(ifk)
               index(i)=index(ifk)
               isz(i)=isz(ifk)
               goto 110
 100        continue
            goto 120
 110     continue
c     return the test number to the position marked by the marker
c     which did not move last. it divides the initial segment into
c     2 parts. any element in the first part is less than or equal
c     to any element in the second part, and they may now be sorted
c     independently .
 120     iptr(ifk)=x
         index(ifk)=intest
         isz(ifk)=s
         ip=ifk
         goto 140
 130     iptr(i)=x
         index(i)=intest
         isz(i)=s
         ip=i
c     store the longer subdivision in workspace.
 140     if((ip-is).gt.(if-ip)) goto 150
         mark(la)=if
         mark(la-1)=ip+1
         if=ip-1
         goto 160
 150     mark(la)=ip-1
         mark(la-1)=is
         is=ip+1
c     find the length of the shorter subdivision.
 160     lngth=if-is
         if(lngth.le.0) goto 180
c     if it contains more than one element supply it with workspace .
         la=la+2
         goto 190
 170     if(la.le.0) goto 200
c     obtain the address of the shortest segment awaiting quicksort
 180     if=mark(la)
         is=mark(la-1)
 190  continue
 200  return
      end
