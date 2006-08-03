      subroutine dipvtf(r,ir,ipvt,n,i1,i2)
C     SUBROUTINE DIPVTF(R,IR,IPVT,N,I1,I2)
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
C                            FEBRERO 1987                              *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta  subrutina  es  auxiliar de las subrutinas  que realizan o
C        modifican  la factorizacion de Cholesky  con pivotacion:ANFM03
C        y  ANFM06. Realiza el intercambio  de elementos de las columnas
C        I1  e  I2  producido por la pivotacion mencionada anteriormente
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        R      Matriz de dimension (IR,*). Contiene a la matriz donde
C               se  realiza  el  intercambio  de  elementos.  La parte
C               subdiagonal (de la caja inferior) no es utilizada.
C
C        IR     Primera dimension de la matriz R. IR >= N.
C
C        IPVT   Vector que indica el orden de las columnas de la matriz
C               R  en entrada.
C
C        N      Numero de elementos (por columna) a intercambiar.
C
C        I1,I2  Indican  las columnas de  R  en las que  se  produce  el
C               intercambio.
C
C     DE SALIDA:
C
C        R      En  R  se guarda la matriz modificada por la permutacion
C
C        IPVT   Vector que recoge el orden de las columnas despues de la
C               permutacion.
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C
      implicit double precision(a-h,o-z)
      dimension r(ir,*),ipvt(*)
      if(i1.eq.i2) return
      call dswap(n,r(1,i1),1,r(1,i2),1)
      i=ipvt(i1)
      ipvt(i1)=ipvt(i2)
      ipvt(i2)=i
      end
