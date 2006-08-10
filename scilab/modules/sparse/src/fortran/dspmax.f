      subroutine dspmax(nr,nc,a,nela,inda,b,nelb,indb,c,nelc,indc,ierr)
c!pupose
c     computes the sparse matrix formed with maximum elementwise of two 
c     sparse matrices.
c!parameters     
c     a,b,c : arrays. 
c             Contain non zero elements of first,second and sum matrices.
c     nr : integer: row dimension of a b c matrices
c     nc : integer: column dimension of a b c matrices
c     nela :integer: number of non zero elements of a
c     nelb :integer: number of non zero elements of b
c     nelc :integer: 
c            on entry maximum number  of non zero elements of c
c            on return number of non zero elements of c
c     inda : a matrix control data:
c            inda(i) 1<=i<=nr contains the number of ith row non zero elements
c            of a
c            inda(m+i) 1<=i<=nela column index of each non zero element
c     indb : b matrix control data:
c            indb(i) 1<=i<=nr contains the number of ith row non zero elements
c            of b
c            indb(m+i) 1<=i<=nelb column index of each non zero element
c
c     indc : on return contains c matrix control data:
c            indc(i) 1<=i<=nr contains the number of ith row non zero elements
c            of c
c            indc(m+i) 1<=i<=nelb column index of each non zero element
c     ierr : if non zero initial value of nelc is to small
c!
c     Copyright INRIA
      double precision a(*),b(*),c(*)
      integer nr,nc,nela,inda(*),nelb,indb(*),nelc,indc(*),ierr
c
      integer jc,ka,kb,jb,kf,i,ka1,ja,j1,j2,nold
c     
      nelmx=nelc
      ierr=0
c     clear indc.
      do 1 i = 1,nr
         indc(i) = 0
 1    continue
c     jc counts elements of c.
      jc     = 1
c     ka,kb are numbers in first i rows of a,b.
      ka     = 0
      kb     = 0
c     kf is number of control data in a,b or c.
      kf     = nr
c     jb counts elements of b.
      jb     = 1
c     i counts rows of a,b,c.
      do 15 i=1,nr
         kb      = kb+indb(i)
c     nira is number in row i of a.
         nira    = inda(i)
         if (nira.eq.0) go to 12
         ka1     = ka+1
         ka      = ka+nira
c     ja counts elements of a.
         do 11 ja= ka1,ka
 6          j1     = inda(ja+kf)
c     at end of b-row transfer rest of a-row.
            if (jb.gt.kb) then
               if(a(ja).gt.0.0d0) then
                  if (jc.gt.nelmx) go to 16
                  c(jc)  = a(ja)
                  indc(jc+kf)=j1
                  jc     = jc+1
               endif
               goto 11
            endif
            j2     = indb(jb+kf)
            if(j1.lt.j2) then
c     if a-index less than b-index transfer a-element to c.
               if(a(ja).gt.0.0d0) then
                  if (jc.gt.nelmx) go to 16
                  c(jc)  = a(ja)
                  indc(jc+kf)=j1
                  jc     = jc+1
               endif
            elseif(j1.eq.j2) then
               if (jc.gt.nelmx) go to 16
               c(jc)  = max(a(ja),b(jb))
               jb     = jb+1
               indc(jc+kf)=j1
               jc     = jc+1
            else
c     if a-index greater than b-index transfer b-element to c.
               if(b(jb).gt.0.0d0) then
                  if (jc.gt.nelmx) go to 16
                  c(jc)  = b(jb)
                  indc(jc+kf)=j2
                  jc     = jc+1
               endif
               jb     = jb+1
               go to 6
            endif
 11      continue
c     end of row of a.  transfer rest of row of b.
 12      if (jb.gt.kb) go to 13
         if(b(jb).gt.0.0d0) then
            if (jc.gt.nelmx) go to 16
            c(jc) = b(jb)
            j2    = indb(jb+kf)
            indc(jc+kf)=j2
            jc    = jc+1
         endif
         jb      = jb+1
         go to 12
 13      if (i.gt.1) go to 14
         nold  = jc-1
c     nirc is number in row i of c.
         nirc  = jc-1
         go to 15
 14      nirc   = jc-1-nold
         nold  = jc-1
 15      indc(i)=nirc
         nelc  = jc-1
         return
c     error messages.
 16      ierr=1
c     no more place for c

         return 
         end
