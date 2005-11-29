      DOUBLE PRECISION FUNCTION DNRM0(N,X,INCX)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C     Copyright:        Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                             MARZO 1988                               *
C                                                                      *
C***********************************************************************
C
C
C     OBJETIVO:
C        Este subprograma calcula la norma infinito de un vector.
C
C     LISTA DE LLAMADA
C
C        N      Variable que indica la dimension del vector  X.
C
C        X      Vector N-dimensional del cual se calculara su morma.
C
C        INCX   Incremento entre dos componentes consecutivas de X.
C
C
C        Esta subprograma trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: idamax
C     FUNCIONES FORTRAN INTRINSECAS: abs
C
C
      implicit double precision(a-h,o-z)
      dimension x(*)
      dnrm0=0
      if(n.lt.1) return
      i=idamax(n,x,incx)
      dnrm0=abs(x(i))
      end
