      subroutine sp2col(m,n,ind,nel,indr)
c     given a sparse matrix storage information sp2col compute the  
c     storage of the column conversion of the original matrix
c!parameters
c     m : number of rows of original matrix
c     n : number of columns of original matrix
c     ind : 
c          ind(i) contains the number of non zero elements in the 
c                 original matrix ith row
c          ind(m+j) contains the column number of jth non zero elements
c                   (non zeros elements are counted row by row)
c     nel : total number of non zero elements
c     indr:
c          ind(i) contains the number of non zero elements in the 
c                 resulting vector ith row (must be 0 or 1)
c          indr(m*n+j) contains the old position of the jth non zero 
c                      element
c     Copyright INRIA
      integer ind(*),indr(*)
c
      call iset(m*n,0,indr,1)
      k0=0
      k1=0
      do 20 i=1,m
         ni=ind(i)
         if (ni.gt.0) then
            do 10 k=1,ni
               j=ind(m+k0+k)
               indr(i+(j-1)*m)=1
               k1=k1+1
               indr(m*n+k0+k)=k1
 10         continue
            k0=k0+ni
         endif
 20   continue
      return
      end
