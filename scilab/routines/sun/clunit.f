      subroutine clunit( lunit, name, mode)
c ====================================================================
c
c     system dependent routine to allocate files
c
c ====================================================================
c
c     lunit   logical unit number
c     name    file name
c     mode    attach mode
c             recl = mode(2) ( direct file )
c             mode(1) definit le mode d'ouverture
c             le chiffre des centaines definit le format
c                0  ->  formatted
c             le chiffre des dizaines  definit l' acces
c                0  ->  sequential
c             le chiffre des unites    definit le status
c                0  ->  new
c                1  ->  old
c                2  ->  scratch
c                3  ->  unknown
c             si mode est negatif, on ouvre en read seulement (vax/vms)
c
c
c sur apollo, pour les fichiers binaires sequentiels, on met un recl a
c 250000 si celui-ci n'est pas donne par l'utilisateur. Pour ce faire,
c il suffit de remplacer la chaine 'cap' par trois blancs.
c
c
c reference externe : cluni0
c           fortran : len
c
c ====================================================================
c
c     Copyright INRIA
      include '../stack.h'
      integer       lunit,mode(*)
      character*(*) name
c
c      integer        nunit,unit(50)
c      common /units/ nunit,unit
c
      integer       iacc,ifor,ista,k,rec
      character*11  for,sta,acc
      character*800  nomfic
      double precision res
c
      if ( lunit.eq.rte) then
c     attach units  rte   to terminal in
         call addfile (lunit,1,0,1,001,char(0),ierr)
         if(ierr.ne.0) then
            call error(112)
            return
         endif
         goto 100
      elseif(lunit.eq.wte ) then
c     attach units   wte  to terminal out
         call addfile (lunit,1,0,1,000,char(0),ierr)
         if(ierr.ne.0) then
            call error(112)
            return
         endif
         goto 100
      endif

c
c ----------
c close file
c ----------
c
 
      if ( lunit.lt.0 ) then
c     .  preserve permanent files
         if (lunit.eq.-rte.or.lunit.eq.-wte) goto 100
c     .  close file and put it out of the table
         call getfiletype(-lunit,ltype,info)
         if(info.eq.0) then
            if(ltype.eq.1) then
               close( -lunit )
               call delfile(-lunit)
            else
               call mclose(-lunit,res)
            endif
         endif
      else
c
c ---------
c open file
c ---------
c
         k = abs(mode(1))
c
c ------------------------
c definition des attributs
c ------------------------
c
         rec  = mode(2)
         ifor = k / 100
         k    = k - 100 *ifor
         iacc = k / 10
         ista = k - 10  *iacc
c
         if ( ifor.eq.0 ) then
            for='formatted'
         else
            for='unformatted'
         endif
c
         if ( ista.eq.0 ) then
            sta='new'
         elseif ( ista.eq.1 ) then
            sta='old'
         elseif ( ista.eq.2 ) then
            sta='scratch'
         elseif ( ista.eq.3 ) then
            sta='unknown'
         else
            err = 67
            goto 100
         endif
         if ( iacc.ne.0 ) then
            acc='direct'
         else
            acc='sequential'
         endif
c
         if ( lunit.ne.0 ) then
c     .     file is defined directly by its logical unit
            call getfileinfo(lunit,ifa,iswap,ltype,mode,buf,lb,info)
            if(info.eq.2) then
c     .        unit is free
               call addfile(lunit,1,0,1,mode(1),char(0),ierr)
               if(ierr.ne.0) then
                  call error(112)
                  return
               endif
            elseif(info.eq.1) then
c     .        unit is out of bounds
               call error(66)
               return
            else
c     .        unit is not free
               goto 40
            endif

            if ( iacc.ne.0 ) then
               open(lunit,form=for,status=sta,access=acc,recl=rec,
     $              err=30)
            else
               open(lunit,form=for,status=sta,access=acc,err=30)
            endif
         else
c     .     file is defined by its name
            call getfiledesc(lunit) 
            if(lunit.lt.0) then
               err = 66
               return
            endif
c     .     get full file name
            call cluni0(name, nomfic, k)    
            if ( iacc.ne.0 ) then
               open( lunit, file=nomfic(1:k), form=for,
     1              access=acc , status=sta,recl=rec, err=30)
            else
               open( lunit, file=nomfic(1:k), form=for,
     1              access=acc ,status=sta, err=30)
            endif
         endif
         call addfile(lunit,1,0,1,mode(1),nomfic(1:k)//char(0),ierr)
         if(ierr.ne.0) then
            call error(112)
            return
         endif

         if(ista.ne.0.and.iacc.eq.0) rewind(lunit)
      endif
c
      goto 100
c
c ----------
c error open
c ----------
c
   30 continue
      if(mode(1).ge.0) then
         err = 240
      else
         err = 241
      endif
      goto 100
c
   40 continue
      err = 65
      goto 100
c
c --------------
c end of program
c --------------
c
  100 continue
      end
