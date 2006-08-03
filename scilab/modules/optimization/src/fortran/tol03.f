      subroutine tol03(q,iq,r,ir,c,ic,d,a,ia,b,ci,cs,x,w,ipvt,n,m,mi,
     &                  mi1,nmd,io)
C     SUBROUTINE TOL03(Q,IQ,R,IR,C,IC,D,A,IA,B,CI,CS,X,W,IPVT,N,M,MI,
C    &                  MI1,NMD,IO)
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
C                            NOVIEMBRE 1988                            *
C***********************************************************************
C
C     OBJETIVO:
C        Esta  es  una subrutina  auxiliar a  OPTR03. Calcula una mejor
C        aproximacion de la solucion  obtenida  por  OPTR03  si  no  se
C        satisfacen las restricciones  con la tolerancia deseada.
C
C    LISTA DE LLAMADA:
C
C       Q      Matriz de dimension (IQ,N). Contiene la matriz ortogonal
C              de la factorizacion  QR  de las restricciones activas.
C
C       R      Matriz de dimension (IR,M). Almacena la matriz triangular
C              superior de la factorizacion  QR de las restricciones
C              activas.
C
C       C      Matriz de dimension (IC,MI+NMD-N). Referencia en  OPTR03.
C
C       D      Vector de dimension  MI+NMD-N. Referencia en OPTR03.
C
C       A      Matriz de dimension (IA,MIF+MDF). Referencia en OPTR03.
C
C       B      Vector de dimension MIF+MDF. Referencia en OPTR03.
C
C     CI,CS    Referencia en OPTR03.
C
C       X      Vector N-dimensional que almacena la solucion calculada.
C
C       W      Vector de trabajo de dimension N+M.
C
C       IPVT   Vector de dimension M. Referencia en  OPTR03.
C
C       N      Dimension del problema.
C
C       M      Numero de restricciones activas.
C
C       MI     Numero de restricciones de igualdad.
C
C       MI1    Numero  de  restricciones  de  igualdad  linealmente
C              independientes.
C
C       NMD    N+MD. Referencia en OPTR03.
C
C       IO     Numero de canal de salida de resultados.
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES:  anrs01,dadd,ddot,dmmul,d1mach
C     FUNCIONES FORTRAN INTRINSECAS:  abs,mod
C
C
      implicit double precision (a-h,o-z)
      dimension q(iq,*),r(ir,*),c(ic,*),d(*),a(ia,*),b(*),ci(*),cs(*),
     &          x(*),w(*),ipvt(*)
css      eps=d1mach(4)**0.75
      eps=dlamch('p')**0.75
      ind=0
      m1=m+1
      do 10 i=1,mi1
         j=ipvt(i)
         dj=d(j)
         s=dj-ddot(n,c(1,j),1,x,1)
         w(i)=s
         if(ind.eq.0) then
            s=abs(s)/(1+abs(dj))
            if(s.gt.eps) ind=1
         end if
10    continue
      do 20 i=mi1+1,m
         j=ipvt(i)
         if(j.lt.0) then
            j=-j
            dj=ci(j)
            s=x(j)-dj
         else if(j.le.n) then
            dj=cs(j)
            s=dj-x(j)
         else if(j.le.nmd) then
            j=mi+j-n
            dj=d(j)
            s=dj-ddot(n,c(1,j),1,x,1)
         else
            j=j-nmd
            dj=b(j)
            s=dj-ddot(n,a(1,j),1,x,1)
         end if
         w(i)=s
         if(ind.eq.0) then
            s=abs(s)/(1.d0+abs(dj))
            if(s.gt.eps) ind=1
         end if
20    continue
      if(ind.eq.0) return
      call anrs01(r,ir,m,w,w,ind,io)
      call dmmul(q,iq,w,m,w(m1),n,n,m,1)
      call dadd(n,w(m1),1,x,1)
      end
