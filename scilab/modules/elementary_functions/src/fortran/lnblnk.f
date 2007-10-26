      integer function lnblnk(str)
c     Copyright INRIA
      character*(*) str
      n=len(str)+1
 10   continue
      n=n-1
      if (n.eq.0) then
         lnblnk=0
         return
      else
         if (str(n:n).ne.' ') then
            lnblnk=n
            return
         endif
      endif
      goto 10
      end
