      FUNCTION OPVF03(R,IR,A,IA,P,B,X,W,ALFA,ND,N,MIF,MDF)
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
C        Este subprograma calcula el valor del funcional de  OPTR03  en
C     un punto.
C
C     LISTA DE LLAMADA:
C
C        R      Matriz de dimension  (IR,N)  que contiene la matriz del
C               termino cuadratico.
C
C        A      Matriz de dimension  (IA,MIF+MDF). Referencia en OPTR03
C
C        P      Vector de dimension  N que contiene los coeficientes del
C               termino lineal del funcional.
C
C        B      Vector de dimension  MIF+MDF.Referencia en  OPTR03.
C
C        X      Vector de dimension  N  que contiene  el punto donde se
C               calculara el valor del funcional.
C
C        W      Vector de trabajo de dimension  ND+2*N-1.
C
C        ALFA   (Referencia en  OPTR03).
C
C        ND     Variable de trabajo. (Referencia en  OPTR03).
C
C
C        Esta subprograma trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES:  ddot,dadd,dscal
C     FUNCIONES FORTRAN INTRINSECAS:  abs,mod
C
      implicit double precision(a-h,o-z)
      dimension r(ir,*),a(ia,*),p(*),b(*),x(*),w(*)
      ni=nd+n-1
      do 290 i=1,n
         i1=i+1
         ii=ni+i
         w(ii)=ddot(i,r(i,1),ir,x,1)
         if(i.lt.n) w(ii)=w(ii)+ddot(n-i,r(i1,i),1,x(i1),1)
290   continue
      s1=0.5d0
      call dscal(n,s1,w(ni+1),1)
      call dadd(n,p,1,w(ni+1),1)
      opvf03=ddot(n,x,1,w(ni+1),1)
      if(alfa.ne.1.d0) opvf03=opvf03*alfa
      do 300 i=1,mif
300   opvf03=opvf03+abs(ddot(n,a(1,i),1,x,1)-b(i))
      do 310 i=mif+1,mif+mdf
         s1=ddot(n,a(1,i),1,x,1)-b(i)
         if(s1.gt.0.d0) opvf03=opvf03+s1
310   continue
      end
