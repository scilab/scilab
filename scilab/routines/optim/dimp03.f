      subroutine dimp03(x,w,ire,ipvt,s,i1,i2,i3,i4,i5,i6,i7,i8,i9,ind,
     &                  imp,io,iter)
C     SUBROUTINE DIMP03(X,W,IRE,IPVT,S,I1,I2,I3,I4,I5,I6,I7,I8,I9,IND,
C    &                  IMP,IO,ITER)
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
C                           SEPTIEMBRE 1989                            *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta es una subrutina  auxiliar a   OPTR03.  Se  encarga  de la
C        impresion  de salida  de resultados  segun el nivel dado por el
C        usuario.
C
C     LISTA DE LLAMADA:
C
C        X      Vector N-dimensional. (Referencia en  OPTR03).
C
C        W      Vector de trabajo. (Referencia en OPTR03 ).
C
C        IRE    Vector IRE asociado a las restricciones del funcional
C               (Referencia en OPTR03).
C
C        IPVT   Vector de dimension  M. (Referencia en  OPTR03).
C
C        S      Variable de trabajo.
C
C     I1,I2,I3
C     I4,I5,I6  Variables enteras de trabajo.
C     I7,I8,I9
C
C        IND    Variables que indica el motivo por el cual se ha de
C               imprimir:
C                  1  : Se ha superado  el limite  de iteraciones fijado
C                       por el programa.
C                  0  : Se ha finalizado el proceso sin problemas.
C                 -1  : El funcional no esta acotado inferiormente.
C                 -2  : Se  encuentra  un punto  degenerado  con ciclaje
C                       indefinido.
C                 -3  : Posible problema no acotado.  La distancia entre
C                       los puntos  de dos iteraciones  consecutivas  es
C                       "demasiado grande".
C                 -4  : Variables enteras suministradas incorrectamente.
C                -11  : Incompatibilidad en el sistema de restricciones
C                       de igualdad en  OPTR01.
C                -12  : No existen puntos admisibles en  OPTR01 para las
C                       restricciones ajenas al funcional.
C                -13  : Se encuentra  un punto  degenerado  con  ciclaje
C                       indefinido en  OPTR01.
C                -14  : Se  ha  realizado  el limite  de iteraciones  en
C                       OPTR01 sin encontrar un punto admisible para las
C                       restricciones ajenas al funcional.
C                -24  : Cotas incorrectas sobre las variables.
C                -34  : Vector IRE suministrado incorrectamente.
C
C        IMP    Variable entera (Referencia en OPTR03).
C
C        IO    Numero de canal de salida de resultados
C
C        ITER  Numero de iteraciones realizadas.
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: dnrm0
C
      Implicit double precision(a-h,o-z)
      character*30 car
      dimension x(*),w(*),ipvt(*),ire(*)
      car='END OF OPTR03.'
      if(ind.eq.2) then
         if(imp.ge.10) write(io,5000) 'POINT COMPUTED: ',
     &                 (x(i),i=1,i1)
         write(io,4000)  iter
         if(i2.ne.0) then
            ii2=i2
            if(i2.gt.i1) ii2=i2+i9
            write(io,3000) 'DELETED CONSTRAINT: ',ii2
         end if
         write(io,3000) 'NUMBER OF ACTIVE CONSTRAINTS:',i3
         do 10 i=1,i9
 10      ipvt(i)=ipvt(i)+i1
         do 20 i=i9+1,i3
            j=ipvt(i)
            if(j.gt.i1) ipvt(i)=j+i8
 20      continue
         write(io,8000) 'ACTIVE CONSTRAINTS:',(ipvt(i),i=1,i3)
         do 30 i=1,i9
 30      ipvt(i)=ipvt(i)-i1
         do 40 i=i9+1,i3
            j=ipvt(i)
            if(j.gt.i1) ipvt(i)=j-i8
 40      continue
         if(i4.gt.0) write(io,8000)
     &    '-CONSTRAINTS ASSOCIATED TO THE OBJECTIVE FUNCTION:',
     &    (ire(i),i=1,i4)
         if(i5.eq.1) then
            write(io,3000)
     &'A DESCENT DIRECTION OF POSITIVE CURVATURE HAS BEEN COMPUTED.'
         else if(i5.eq.0) then
            write(io,3000)
     &'A DESCENT DIRECTION OF NULL CURVATURE HAS BEEN COMPUTED.'
         else
            write(io,3000)
     &'A DESCENT DIRECTION OF NEGATIVE CURVATURE HAS BEEN COMPUTED.'
         end if
         if(i6.ne.0) write(io,3000)
     &               'A DEGENERATED POINT HAS BEEN COMPUTED.'
         if(i7.ne.0) then
            ii7=i7
            if(ii7.gt.i1) ii7=ii7+i9
            write(io,3000) 'ADDED CONSTRAINT: ',ii7
         end if
      else if(ind.eq.0) then
         if(imp.ge.10) write(io,5000) 'POINT COMPUTED: ',
     &                 (x(i),i=1,i1)
         if(i5.ne.0) then
            write(io,4000)  iter
            ii2=i5
            if(i5.gt.i1) ii2=i5+i9
            write(io,3000) 'DELETED CONSTRAINT: ',ii2
         end if
         write(io,1000) car, 'A LOCAL MINIMUM HAS BEEN FOUND.'
         if(imp.ge.8) then
            write(io,3000) 'NUMBER OF ITERATIONS: ',iter
            write(io,5000) 'POINT COMPUTED: ',(x(i),i=1,i1)
            write(io,7000) 'NORM OF THE KUHN-TUCKER VECTOR: ',s
            write(io,3000) 'NUMBER OF ACTIVE CONSTRAINTS:',i2
            do 50 i=1,i9
 50         ipvt(i)=ipvt(i)+i1
            do 60 i=i9+1,i2
               j=ipvt(i)
               if(j.gt.i1) ipvt(i)=j+i8
 60         continue
            write(io,8000) 'ACTIVE CONSTRAINTS:',(ipvt(i),i=1,i2)
            do 70 i=1,i9
 70         ipvt(i)=ipvt(i)-i1
            do 80 i=i9+1,i2
               j=ipvt(i)
               if(j.gt.i1) ipvt(i)=j-i8
 80         continue
            write(io,9000) 'LAGRANGE MULTIPLIERS:',
     &                      (w(i),i=i3,i3+i2-1)
            if(i4.ne.0) write(io,7000) 'OBJECTIVE FUNCTION: ',w(i4)
         end if
      else if(ind.eq.-4) then
         if(io.le.0) then
            print 1000,car,
     &      'INVALID NUMBER FOR THE OUTPUT CHANEL NUMBER.'
         else if(imp.ge.7) then
            write(io,1000) car,'INVALID INTEGER VARIABLES.'
         end if
      else if(ind.eq.-24) then
         write(io,2000) car,'CI(I).GT.CS(I) FOR I= ',i1
      else if(ind.eq.-34) then
         write(io,1000) car,'INCORRECT VECTOR IRE.'
      else if(ind.lt.-10) then
         write(io,1000) car,   'OPTR01 HAS NOT FOUND A FEASIBLE POINT.
     &IND OF','OPTR01=',ind+10
      else if(ind.eq.-1) then
         write(io,1000) car, 'PROBLEM UNBOUNDED FROM BELOW'
      else if(ind.eq.-2) then
         write(io,1000) car,
     &   'A DEGENERATED POINT CYCLING HAS BEEN FOUND.'
         if(imp.ge.8)write(io,3000)'NUMBER OF ITERATIONS:',iter
      else if(ind.eq.-3) then
         write(io,1000) car,
     &  'THE DISTANCE BETWEEN THE POINTS OF TWO CONSECUTIVE ITERATIONS '
     & ,'IS "TOO BIG".'
          write(io,3000) 'PROBABLY PROBLEM UNBOUNDED FROM BELOW.'
         if(imp.ge.8) then
            s1=dnrm0(i1,x,1)
            write(io,6000) '-NORM OF THE POINT COMPUTED:',s1
            write(io,6000) '-OBJECTIVE FUNCTION:',w(i2)
            write(io,3000) 'NUMBER OF ITERATIONS:',iter
         end if
      else if(ind.eq.1) then
         write(io,1000)    car,
     & 'THE LIMIT FOR THE ITERATION NUMBER HAS BEEN PASSED.'
         if(imp.ge.8)write(io,3000) 'NUMBER OF ITERATIONS:',iter
      end if
1000  format(////,80('*'),///,10X,A,2(/,10X,A),I5)
2000  format(////,80('*'),///,10X,A,/,10X,A,I5)
3000  format(///,10X,A,I4)
4000  format(////,80('-'),///,10X,'ITERATION: ',I4)
5000  format(//,10X,A,(T31,SP,E22.16))
6000  format(/,10X,A,T41,E22.16)
7000  format(/,10X,A,E22.16)
8000  format(//,10X,A,//,(10X,10(2X,I4),/))
9000  format(/,10X,A,/,(10X,4(2X,E14.8)))
      end
