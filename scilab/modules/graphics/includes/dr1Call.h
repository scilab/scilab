/*------------------------------------------------------------------------*/
/* file: dr1Call.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Djalel ABDEMOUCHE, Jean-Baptiste Silvy                       */
/* desc : Old Graphics Fonctions                                          */
/*------------------------------------------------------------------------*/

#ifndef _DR1_CALL_H_
#define _DR1_CALL_H_

#include "Graphics.h" 

void Xrect ( char          * fname    ,
             unsigned long   fname_len,
             double        * x        ,
             double        * y        ,
             double        * width    ,
             double        * height    ) ;

void Xarc ( char          * fname        ,
            unsigned long   fname_len,
            integer       * a1           ,
            integer       * a2           ,
            double        * x            ,
            double        * y            ,
            double        * width        ,
            double        * height        ) ;

void Xrects ( char          * fname    ,
              unsigned long   fname_len,
              integer       * vect1    ,
              integer         n        ,
              double        * vect2     ) ;

void Xarcs ( char          * fname    ,
             unsigned long   fname_len,
             integer       * vect1    ,
             integer         n        ,
             double        * vect2     ) ;

void Xfarcs( char          * fname    ,
             unsigned long   fname_len,
             integer       * vect1    ,
             integer         n        ,
             double        * vect2     ) ;

void Xpoly ( char          * fname    ,
             unsigned long   fname_len,
             integer         n        ,
             integer         close    ,
             double        * x        ,
             double        * y         ) ;

void Xfpoly ( integer   rempl ,
              integer   closed,
              double  * x     ,
              double  * y      ) ;

void Xpolys ( integer * vect,
              integer   n   ,
              integer   m   ,
              double  * x   ,
              double  * y    ) ;

void Xfpolys ( integer * vect,
               integer   val1,
               integer   val2,
               integer   n   ,
               integer    m   ,
               double  * x   ,
               double  * y   ) ;

void Xsegs ( integer * style,
             integer   flag ,
             integer   n    ,
             double  * x    ,
             double  * y    ,
             double    arsize ) ;

void Xstring ( char          * fname    ,
               unsigned long   fname_len,
               integer         str      ,
               double          x        ,
               double          y        ,
               double          angle    ,
               double        * box       ) ;

void Xtitle ( char * str, int n ) ;

void Xdrawaxis ( char      dir      ,
                 char      tics     ,
                 double  * x        ,
                 int     * nx       ,
                 double  * y        ,
                 int     * ny       ,
                 char   ** val      ,
                 int       sub_int  ,
                 char    * format   ,
                 int       fontsize ,
                 int       textcolor,
                 int       ticscolor,
                 char      flag     ,
                 int       seg_flag  ) ;

void Xplot2d ( double    x[]      ,
               double    y[]      ,
               integer * n1       ,
               integer * n2       ,
               integer   style[]  ,
               char      strflag[],
               char      legend[] ,
               double    brect[]  ,
               integer   aaint[]   ) ;

void Xgrayplot ( double    x[]      ,
                 double    y[]      ,
                 double    z[]      ,
                 integer * n1       ,
                 integer * n2       ,
                 char      strflag[],
                 double    brect[]  ,
                 integer   aaint[]   ) ;

void Xmatplot ( double    z[]      ,
                integer * n1       ,
                integer * n2       ,
                char      strflag[],
                double    brect[]  ,
                integer   aaint[]   ) ;

void Xmatplot1 ( double    z[],
                 integer * n1 ,
                 integer * n2 ,
                 double    xrect[] ) ;

void Xplot3d ( char    * fname ,
               integer * isfac ,
               integer * izcol ,
               double    x[]   ,
               double    y[]   ,
               double    z[]   ,
               integer * zcol  ,
               integer * m     ,
               integer * n     ,
               double  * theta ,
               double  * alpha ,
               char    * legend,
               integer * iflag ,
               double  * ebox   ) ;

void Xnumb ( char    * fname,
             integer   n    ,
             integer   flag ,
             double  * x    ,
             double  * y    ,
             double  * angle,
             double  * box   ) ;

void Xfec ( double    x[]        ,
            double    y[]        ,
            double    noeud[]    ,
            double  * fun        ,
            integer * n          ,
            integer * m          ,
            char      strflag[]  ,
            char      legend[]   ,
            double    brect[]    ,
            integer   aaint[]    ,
            double    Zminmax[]  ,
            integer   Colminmax[],
            integer   ColOut[]   ,
            int       WithMesh    ) ;

#endif /* _X_CALL_1_H_ */