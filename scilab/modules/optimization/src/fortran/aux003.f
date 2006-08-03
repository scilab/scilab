      subroutine aux003(a,ia,x,b,q,iq,r,ir,w,ire,ipvt,nmd,mif,mdf,midf,
     &                  n,m,ind,io)
C     SUBROUTINE AUX003(A,IA,X,B,Q,IQ,R,IR,W,IRE,IPVT,NMD,MIF,MDF,MIDF,
C    &                  N,M,IND,IO)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C      Copyright:       Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                              AGOSTO  1987                            *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta es una subrutina auxiliar a OPTR03. Calcula el vector IRE
C        para las restricciones presentes en el funcional o lo modifica
C
C     LISTA DE LLAMADA:
C
C        A      Matriz de dimension (IA,MIF+MDF). (Referencia en OPTR03)
C
C        X      Vector  N-dimensional. Contiene al punto de la iteracion
C               correspondiente.
C
C        B      Vector de dimension  MIF+MDF. (Referencia en OPTR03).
C
C        Q      Matriz de dimension  (IQ,N). (Referencia en OPTR03).
C
C        R      Matriz de dimension  (IR,M). (Referencia en OPTR03).
C
C        W      Vector de dimension  N+MIF+MDF.
C
C        IRE    Vector de dimension MIF+MDF. (Referencia en OPTR03).
C
C        IPVT   Vector de dimension  min(M+MIF+MDF,N). Si el IND toma el
C               valor 1 no necesita referenciarse.
C
C        IND    Variable que indica segun los valores:
C                  0  : Se apaden al conjunto activo las restricciones
C                       del  funcional  no  activas, recalculandose  la
C                       factorizacion  QR  y los vectores IRE e IPVT.
C                  1  : Se modifica el vector IRE de entrada.
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES:  anfm01,daxpy,dcopy,ddot,dnrm2,dscal,
C                               d1mach
C     FUNCIONES FORTRAN INTRINSECAS:  abs,mod,sign,sqrt
C
      implicit double precision(a-h,o-z)
      dimension a(ia,*),x(*),b(*),q(iq,*),r(ir,*),w(*),ire(*),ipvt(*)
css      eps=d1mach(4)**0.75
      eps=dlamch('p')**0.75
      mf=midf+1
      info=1
      do 10 i=1,mif
         if(ire(i).ne.1) then
            s=ddot(n,a(1,i),1,x,1)-b(i)
            if(abs(s).lt.eps) then
               if(m.lt.n .and. ind.eq.0) then
                  m1=m+1
                  call anfm01(q,iq,r,ir,a(1,i),w(mf),n,m1,ind,io)
                  if(ind.lt.0) then
                     ire(i)=0
                     ind=0
                  else
                     m=m1
                     ipvt(m)=nmd+i
                     ire(i)=1
                  end if
               else
                  if(ire(i).ne.0) info=0
                  ire(i)=0
               end if
            else if(s.ge.eps) then
               ire(i)=2
               w(i)=s
            else
               ire(i)=-2
               w(i)=s
            end if
         end if
10    continue
      do 20 i=1,mdf
         ni=mif+i
         if(ire(ni).ne.1) then
            s=ddot(n,a(1,ni),1,x,1)-b(ni)
            if(s.gt.eps) then
               ire(ni)=2
            else if(s.lt.-eps .or. ind.eq.1) then
               if(ire(ni).ne.0) info=0
               ire(ni)=0
            else
               if(m.lt.n) then
                  m1=m+1
                  call anfm01(q,iq,r,ir,a(1,ni),w(mf),n,m1,ind,io)
                  if(ind.lt.0) then
                     ire(ni)=0
                     ind=0
                  else
                     m=m1
                     ipvt(m)=nmd+ni
                     ire(ni)=1
                  end if
               else
                  ire(ni)=0
               end if
            end if
            w(ni)=s
         end if
20    continue
      if(ind.eq.1) ind=info
      end
