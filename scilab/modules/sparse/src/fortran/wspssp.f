      subroutine wspssp(nr,nc,ar,ai,nela,inda,br,bi,nelb,indb,cr,ci,
c     Copyright INRIA
     $     nelc,indc,ia,ib,ierr)
c!pupose
c     c=a-b for sparse complex matrices
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
c     ia   : on input specifies if a has an imaginary part
c            ia=0 : no imaginary part
c            ia=1 : the imaginary part of a is stored in ai
c     ib   : on input specifies if b has an imaginary part
c            ib=0 : no imaginary part
c            ib=1 : the imaginary part of b is stored in bi
c!
      double precision ar(nela),ai(nela),br(nelb),bi(nelb),cr(*),ci(*)
      integer nr,nc,nela,inda(*),nelb,indb(*),nelc,indc(*),ierr
c
      integer jc,ka,kb,jb,kf,i,ka1,ja,j1,j2,nold
      double precision tr,ti
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
            if (jb.gt.kb) go to 7
            j2     = indb(jb+kf)
            CRES=j1-j2
            if (CRES .lt. 0) then
               goto 7
            elseif (CRES .eq. 0) then
               goto 9
            else
               goto 10
            endif
c     if a-index less than b-index transfer a-element to c.
 7          if (jc.gt.nelmx) go to 16
            cr(jc)  = ar(ja)
            if(ia.eq.0) then 
               ci(jc)  = 0.0d0
            else
               ci(jc)  = ai(ja)
            endif
 8          continue
            indc(jc+kf)=j1
            jc     = jc+1
            go to 11
c     if a-index equals b-index add elements ,place sum in c.
 9          tr      = ar(ja)-br(jb)
            ti=0.0d0
            if(ia.ne.0) ti=ti+ai(ja)
            if(ib.ne.0) ti=ti-bi(jb)
c     ignore sum element if zero.
            jb     = jb+1
            if (tr.eq.0.0d0.and.ti.eq.0.0d0) go to 11
            if (jc.gt.nelmx) go to 16
            cr(jc)  = tr
            ci(jc)  = ti
            go to 8
c     if a-index greater than b-index transfer b-element to c.
 10         if (jc.gt.nelmx) go to 16
            cr(jc)  = -br(jb)
            if(ib.eq.0) then
               ci(jc)  = 0.0d0
            else
               ci(jc)  = -bi(jb)
            endif
            indc(jc+kf)=j2
            jb     = jb+1
            jc     = jc+1
            go to 6
 11      continue
c     end of row of a.  transfer rest of row of b.
 12      if (jb.gt.kb) go to 13
         if (jc.gt.nelmx) go to 16
         cr(jc) = -br(jb)
         if(ib.eq.0) then
            ci(jc)  = 0.0d0
         else
            ci(jc) = -bi(jb)
         endif
         j2    = indb(jb+kf)
         indc(jc+kf)=j2
         jc    = jc+1
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
