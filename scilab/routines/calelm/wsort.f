      subroutine wsort(countr,counti,n,index,test)
c
c!purpose
c     wsort sort double precision array according to rule specified by
c      external test
c     maintaining an index array 
c
c!calling sequence
c     subroutine wsort(countr,counti,n,index,test)
c     integer n,index(n)
c     double precision count(n)
c     integer test
c     external test
c
c     count(r,i)   : array to be sorted
c     n       :size of count and index
c     index   : array containing on return index of sorted array
c     test    : external integer function which define formal order for
c               records 
c               test(r1,i1,r2,i2)
c               where
c               r1,i1 are real and imag part of first complex number
c               r2,i2 are real and imag part of second complex number
c               returns
c                1 :if 1 is greater than 2
c               -1 :if 1 is less than 2
c                0 :if 1 is equal to 2
c
c!method
c     quick sort metjod is used
c!restriction
c     n must be less than 2**(50/2) ! due to lengh of work space mark
c!
c     Copyright INRIA
      dimension mark(50),index(n)
      double precision countr(n),counti(n),avr,avi,xr,xi
      integer test
      external test
c
c  set index array to original order .
      do 10 i=1,n
      index(i)=i
   10 continue
c  check that a trivial case has not been entered .
      if(n.eq.1)goto 200
      if(n.ge.1)go to 30
      goto 200
c  'm' is the length of segment which is short enough to enter
c  the final sorting routine. it may be easily changed.
   30 m=12
c  set up initial values.
      la=2
      is=1
      if=n
      do 190 mloop=1,n
c  if segment is short enough sort with final sorting routine .
      ifka=if-is
      if((ifka+1).gt.m)goto 70
c********* final sorting ***
c  ( a simple bubble sort )
      is1=is+1
      do 60 j=is1,if
      i=j
   40 it=test(countr(i-1),counti(i-1),countr(i),counti(i))
      if(it.eq.1) goto 60
      if(it.eq.-1) goto 50
      if(index(i-1).lt.index(i))goto 60
   50 avr=countr(i-1)
      avi=counti(i-1)
      countr(i-1)=countr(i)
      counti(i-1)=counti(i)
      countr(i)=avr
      counti(i)=avi
      int=index(i-1)
      index(i-1)=index(i)
      index(i)=int
      i=i-1
      if(i.gt.is)goto  40
   60 continue
      la=la-2
      goto 170
c             *******  quicksort  ********
c  select the number in the central position in the segment as
c  the test number.replace it with the number from the segment's
c  highest address.
   70 iy=(is+if)/2
      xr=countr(iy)
      xi=counti(iy)
      intest=index(iy)
      countr(iy)=countr(if)
      counti(iy)=counti(if)
      index(iy)=index(if)
c  the markers 'i' and 'ifk' are used for the beginning and end
c  of the section not so far tested against the present value
c  of x .
      k=1
      ifk=if
c  we alternate between the outer loop that increases i and the
c  inner loop that reduces ifk, moving numbers and indices as
c  necessary, until they meet .
      do 110 i=is,if
         it=test(xr,xi,countr(i),counti(i))
         if(it.lt.0) goto 110
         if(it.gt.0) goto 80
         if(intest.gt.index(i))goto 110
 80      if(i.ge.ifk)goto 120
         countr(ifk)=countr(i)
         counti(ifk)=counti(i)
         index(ifk)=index(i)
         k1=k
         do 100 k=k1,ifka
            ifk=if-k
            it=test(countr(ifk),counti(ifk),xr,xi)
            if(it.lt.0) goto 100
            if(it.gt.0) goto 90
            if(intest.le.index(ifk))goto 100
 90         if(i.ge.ifk)goto 130
            countr(i)=countr(ifk)
            counti(i)=counti(ifk)
            index(i)=index(ifk)
            go to 110
 100     continue
         goto 120
 110  continue
c  return the test number to the position marked by the marker
c  which did not move last. it divides the initial segment into
c  2 parts. any element in the first part is less than or equal
c  to any element in the second part, and they may now be sorted
c  independently .
  120 countr(ifk)=xr
      counti(ifk)=xi
      index(ifk)=intest
      ip=ifk
      goto 140
  130 countr(i)=xr
      counti(i)=xi
      index(i)=intest
      ip=i
c  store the longer subdivision in workspace.
  140 if((ip-is).gt.(if-ip))goto 150
      mark(la)=if
      mark(la-1)=ip+1
      if=ip-1
      goto 160
  150 mark(la)=ip-1
      mark(la-1)=is
      is=ip+1
c  find the length of the shorter subdivision.
  160 lngth=if-is
      if(lngth.le.0)goto 180
c  if it contains more than one element supply it with workspace .
      la=la+2
      goto 190
  170 if(la.le.0)goto 200
c  obtain the address of the shortest segment awaiting quicksort
  180 if=mark(la)
      is=mark(la-1)
  190 continue
  200 return
      end
      integer function rptest(r1,i1,r2,i2)
      double precision r1,i1,r2,i2
      if(r1.gt.r2) then
         rptest=1
      elseif(r1.lt.r2) then
         rptest=-1
      else
         rptest=0
      endif
      end
      integer function modtest(r1,i1,r2,i2)
      double precision r1,i1,r2,i2,a1,a2
      a1=r1**2+i1**2
      a2=r2**2+i2**2
      if(a1.gt.a2) then
         modtest=1
      elseif(a1.lt.a2) then
         modtest=-1
      else
         modtest=0
      endif
      end
