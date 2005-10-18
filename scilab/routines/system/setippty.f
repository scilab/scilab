      subroutine setippty(n)
c     ippty: interfaces properties
      parameter (mxbyptr=40)
      integer byptr(mxbyptr),nbyptr
      common /ippty/ byptr,nbyptr

      if(n.eq.0) then 
c     .  initialization
         byptr(1)=6
         byptr(2)=13
         byptr(3)=16
         byptr(4)=19
         byptr(5)=21
         byptr(6)=23
         byptr(7)=41
         byptr(8)=42
         byptr(9)=50
         nbyptr=9
      elseif(n.gt.0) then 
c     .  add a  interface n in the list
         do 01 i=1,nbyptr
            if(byptr(i).eq.n) return
 01      continue
         if(nbyptr.ge.mxbyptr) then
            call error(261)
            return
         endif
         nbyptr=nbyptr+1
         byptr(nbyptr)=n
      else 
c     .  suppress interface abs(n)
         ifun=abs(n)
         do 10 i=1,nbyptr
            if(byptr(i).eq.ifun) then
               ifun=i
               goto 20
            endif
 10      continue
         return
 20      if(ifun.lt.nbyptr) then
            call icopy(nbyptr-ifun,byptr(ifun+1),1,byptr(ifun),1)
         endif
         nbyptr=nbyptr-1
      endif
      end
