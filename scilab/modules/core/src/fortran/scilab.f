      subroutine scilab(nos)
c     Copyright INRIA
      character*100  bu1
      character*256 path
      integer  nc,nos,ierr
      common /comnos/ nos1,mem
      common /starfi/ path
c     ----------
c     initialize 
c     ----------
      call inisci(-1,mem, ierr)
      if(ierr.gt.0) return
c     ----------------------------------------
c     get startup instruction and start parser
c     ----------------------------------------
      if(nos1.eq.0) then
c     .  get initial instruction  if required
         bu1=' '
         call inffic( 2, bu1,nc)
         nc=max ( 1 , nc )
      else
         bu1=' '
         nc=1
      endif
      if(path.eq.' ') then
         call scirun(bu1(1:nc))
      else
         bu1=bu1(1:nc)//';quit'
         nc=nc+5
         call scirun(bu1(1:nc))
c     strip leading and trailin blanks in path
         l1=0
 10      l1=l1+1
         if(path(l1:l1).eq.' ') goto 10
         l2=len(path)+1
 11      l2=l2-1
         if(path(l2:l2).eq.' ') goto 11
         call scirun('exec('''//path(l1:l2)//''',-1)')
      endif
c     --------
c     cleaning 
c     --------
      call sciquit
      return 
      end
