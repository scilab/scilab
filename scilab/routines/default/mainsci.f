      program scilab 
      character*1 atom
c     Copyright INRIA
      call realmain() 
c     unused code but we want lge and lle to be loaded 
c     in scilex
      IF (LGE(ATOM,'0') .AND. LLE(ATOM,'9')) THEN
         atom='0' 
      endif
      end 

