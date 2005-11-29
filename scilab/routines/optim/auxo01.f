      subroutine auxo01 (c,ic,ci,cs,b,x,w,ire,ira,n,md,ind,fun,iv)
C     SUBROUTINE AUXO01 (C,IC,CI,CS,B,X,W,IRE,IRA,N,MD,IND,FUN,IV)
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
C        Esta es una subrutina auxiliar de OPTR01. Comprueba si un punto
C        dado es admisible para las restricciones no activas, calcula el
C        valor del funcional de restricciones violadas y el gradiente de
C        dicho funcional cambiado de signo  ( si el  indicador  toma  el
C        valor adecuado)
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        C      Matriz de dimension (IC,MD).Contiene los coeficientes de
C               las restricciones de desigualdad.
C
C        IC     Primera dimension de la matriz C. IC >= N.
C
C        B      Vector MD-dimensional. Contiene los coeficientes de los
C               " terminos independientes " de las restricciones de
C               desigualdad.
C
C        X      Vector N-dimensional. En el se tiene el punto en el que
C               se estudian las restricciones.
C
C        CI,CS  Vectores N-dimensionales de cotas.(Referencia en OPTR01)
C
C        IRE    Vector N+MD-dimensional.(Referencia en OPTR01).
C
C        IRA    Variable  que  indica  si  existen  restricciones  de
C               acotacion. (Referencia en  OPTR01).
C
C        N      Dimension del problema.
C
C        MD     Numero de columnas de C, dimension de B.
C
C        IND    Variable que toma los valores:
C                  0  : Se estudia la admisibilidad.
C                  1  : Ademas se calcula el valor del funcional de las
C                       restricciones que se violen y el vector IRE.
C     DE SALIDA:
C
C        W      Vector de trabajo de dimension MD si IND=0, y 3*N+MD si
C               IND=1. Si IND=1, en las primeras coordenas contiene El
C               vector gradiente.
C
C        IRE    Vector que indica de que tipo son las restricciones,a la
C               salida, (segun los valores indicados en OPTR01).
C
C        FUN    Variable que contiene, si IND=1, el valor del funcional
C               de restricciones violadas.
C
C        IV     Variable que indica segun los valores:
C                  0  : No existen restricciones violadas en X.
C                  1  : Existen restricciones violadas en X.
C
C     Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                -Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: ddot,d1mach
C     FUNCIONES FORTRAN INTRINSECAS: abs,mod,sqrt
C
C
      implicit double precision (a-h,o-z)
      dimension c(ic,*),ci(*),cs(*),b(*),x(*),w(*),ire(*)
      if(ind.eq.1) fun=0
      iv=0
css   eps=d1mach(4)**0.75
css   gigant=d1mach(2)
      eps=dlamch('p')**0.75
      gigant=dlamch('o')

      gig1=sqrt(gigant)
      if(ind.eq.1) then
         do 5 i=1,n
5        w(i)=0
         nw=n*3
      else
         nw=0
      end if
      if(ira.gt.0) then
         do 10 i=1,n
            xi=x(i)
            ij=0
            ia=abs(ire(i))
            if(ira.ne.2) then
               cii=ci(i)
               if(cii.ge.-gig1  .and. ia.ne.1) then
                  if(xi.lt.(cii-eps)) then
                     iv=1
                     if(ind.eq.1) then
                        fun=fun+cii-xi
                        ire(i)=-2
                        w(i)=1
                        ij=1
                     else
                        return
                     end if
                  else if(ind.eq.1) then
                     ire(i)=0
                  end if
               end if
            end if
            if(ira.ge.2) then
               csi=cs(i)
               if(csi.le.gig1 .and. ij.eq.0 .and. ia.ne.1)then
                  if(xi.gt.(csi+eps)) then
                     iv=1
                     if(ind.eq.1) then
                        fun=fun+xi-csi
                        ire(i)=2
                        w(i)=-1
                     else
                        return
                     end if
                  else if(ind.eq.1) then
                     ire(i)=0
                  end if
               end if
            end if
10       continue
      end if
      if(md.gt.0) then
         do 20 i=1,md
            nwi=nw+i
            ni=n+i
            if(ire(ni).ne.1) then
               w(nwi)=ddot(n,c(1,i),1,x,1)-b(i)
               if(w(nwi).gt.eps) then
                  iv=1
                  if(ind.eq.1) then
                     ire(ni)=2
                     call ddif(n,c(1,i),1,w,1)
                     fun=fun+w(nwi)
                  else
                     return
                  end if
               else if(ind.eq.1) then
                  ire(ni)=0
               end if
            end if
20       continue
      end if
      end
