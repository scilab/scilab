      subroutine mexfunction(NLHS,PLHS, NRHS, PRHS)
c     simple example of fortran mex: use %VAL to pass arguments
c     [a,b,c,d,e]=mexf2(1,2)    see builder.sce

      INTEGER NLHS, NRHS

C     Use integer*8 on Dec 64 bits machines
      INTEGER*4 PLHS(*),PRHS(*),ptrC,ptrD,ptrE,A,B,C,D
C
C      INTEGER*8 PLHS(*),PRHS(*),ptrC,ptrD,ptrE,A,B,C,D
C
      character*(20) string1,str2
c
      if (NRHS.ne.2) then
         call mexerrmsgtxt('Needs Two inputs!')
      endif

      if (NLHS.gt.5) then
         call mexerrmsgtxt('Too many outputs!')
      endif

      A =mxGetPr(prhs(1))
      mA=mxGetM((prhs(1)))
      nA=mxGetN(prhs(1))

      B =mxGetPr(prhs(2))
      mB=mxGetM((prhs(2)))
      nB=mxGetN(prhs(2))

      ptrC=mxCreateFull(mA,nA,0)
      C=mxGetPr(ptrC)

      ptrD=mxCreateFull(mB,nB,0)
      D=mxGetPr(ptrD)
 
      string1='scimex' 
      ptrE=mxCreateString(string1)

      call mexfjob(mA*nA,%VAL(A),nB*mB,%VAL(B),
     &       iz,%VAL(C),iw,%VAL(D))
c     Return variables 
      PLHS(1)=PRHS(1)
      PLHS(2)=PRHS(2)
      PLHS(3)=ptrC
      PLHS(4)=ptrD
      PLHS(5)=ptrE
      return 
      end

********************************************************************
      subroutine mexfjob(ix,x,iy,y,iz,z,iw,w)
      double precision x(*),y(*),z(*),w(*)
      integer ix,iy,iz,iw
      do 10 i=1,ix 
         z(i) = 2* x(i) 
 10   continue 
      do 20 i=1,iy 
         w(i) = 3* y(i) 
 20   continue 
      return
      end

*******************************************************************
