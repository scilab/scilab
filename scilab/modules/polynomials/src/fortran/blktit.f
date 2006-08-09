      subroutine blktit(lunit,k1,k2,io)
c!purpose
c     generates and outputs a matrix bloc title line of the form :
c     <empty line>
c            column k1 to k2
c     <empty line>
c!
c     Copyright INRIA
      integer k1,k2,io
c
      character*4 fmt2
      character*70 fmt1,cw
c
c     number of digit necessary to represent k2
      lft=int(log10(dble(k2)+0.4))+1
      write(fmt2,'(''i'',i2)') lft

      call basout(io,lunit,' ')
      if(io.eq.-1) return

      if(k1.eq.k2) then
         fmt1='(''         column '','//fmt2//')'
         write(cw(1:16+lft),fmt1) k1
         call basout(io,lunit,cw(1:16+lft))
      else
         fmt1='(''         column '','//fmt2//','' to '','//fmt2//')'
         write(cw(1:(20+2*lft)),fmt1) k1,k2
         call basout(io,lunit,cw(1:(20+2*lft)))
      endif
      if (io.eq.-1) return
      call basout(io,lunit,' ')
      if (io.eq.-1) return
      end
