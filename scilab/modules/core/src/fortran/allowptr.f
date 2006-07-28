      logical function allowptr(ifun)
c     this function returns true if the interface given by the index
c     ifun allow to pass the input arguments by address.
      integer ifun
      logical r
      parameter (mxbyptr=40)
      integer byptr(mxbyptr),nbyptr
      common /ippty/ byptr,nbyptr
      r=.false.
      do 10 i=1,nbyptr
         if(byptr(i).eq.ifun) then
            r=.true.
            goto 20
         endif
 10   continue
 20   allowptr=r
      end
