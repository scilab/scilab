      subroutine addevs(tevts,evtspt,nevts,pointi,t,evtnb,ierr)
C 
c     Copyright INRIA
      double precision tevts(nevts)
      integer evtspt(nevts)
      integer nevts
      integer pointi
      double precision t
      integer evtnb
      integer ierr
C 
C.. Local Scalars .. 
      integer i,j
C 
C
      ierr = 0
      if (evtspt(evtnb).ne.-1) then
         ierr=1
         return
      else
         evtspt(evtnb)=0
         tevts(evtnb)=t
      endif
      if(pointi.eq.0) then
         pointi=evtnb
         return
      endif
      if (t.lt.tevts(pointi)) then
         evtspt(evtnb)=pointi
         pointi=evtnb
         return
      endif
      i = pointi
C     
 100  if(evtspt(i).eq.0) then
         evtspt(i)=evtnb
         return
      endif
      if (t.ge.tevts(evtspt(i))) then
        j = evtspt(i)
        if(evtspt(j).eq.0) then
           evtspt(j)=evtnb
           return
        endif
        i=j
        goto 100
      else 
         evtspt(evtnb)=evtspt(i)
         evtspt(i)=evtnb
      endif
      end 

      subroutine putevs(tevts,evtspt,nevts,pointi,t,evtnb,ierr)
C 
      double precision tevts(nevts)
      integer evtspt(nevts)
      integer nevts
      integer pointi
      double precision t
      integer evtnb
      integer ierr
C 
C.. Local Scalars .. 
      integer i,j
C 
C
      ierr = 0
      if (evtspt(evtnb).ne.-1) then
         ierr=1
         return
      else
         evtspt(evtnb)=0
         tevts(evtnb)=t
      endif
      if(pointi.eq.0) then
         pointi=evtnb
         return
      endif
      evtspt(evtnb)=pointi
      pointi=evtnb
      return
      end 
