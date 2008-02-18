/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Djalel Abdemouche
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sciCall.h                                                        */
/* desc : Graphic subroutines interface                                   */
/*------------------------------------------------------------------------*/

#include "machine.h"
#include "ObjectStructure.h"

#ifndef _SCI_CALL_H_
#define _SCI_CALL_H_

void Objrect ( double * x         ,
               double * y         ,
               double * width     ,
               double * height    ,
               int    * foreground,
               int    * background,
               BOOL     isfilled  ,
               BOOL     isline    ,
               int      n         ,
               long   * hdl       ,
               BOOL     flagstring ) ;

void Objarc( double * angle1    ,
             double * angle2    ,
             double * x         ,
             double * y         ,
             double * width     ,
             double * height    ,
             int    * foreground,
             int    * background,
             BOOL     isfilled  ,
             BOOL     isline    ,
             long   * hdl        ) ;

void Objpoly ( double  * x     ,
               double  * y     ,
               integer   n     ,
               integer   closed,
               int       mark  ,
               long    * hdl    ) ;

void Objfpoly ( double  * x    ,
                double  * y    ,
                integer   n    ,
                integer * style,
                long    * hdl  ,
                integer   shading ) ;

void Objsegs ( integer * style,
               integer   flag ,
               integer   n1   ,
               double  * x    ,
               double  * y    ,
               double    arsize ) ;

void Objstring( char            ** fname      ,
                int                nbRow      ,
                int                nbCol      ,
                double             x          ,
                double             y          ,
                double           * angle      ,
                double             box[4]     ,
                BOOL               autoSize   ,
                double             userSize[2],
                long             * hdl        ,
                BOOL               centerPos  ,
                int              * foreground ,
                int              * background ,
                BOOL               isboxed    ,
                BOOL               isline     ,
                BOOL               isfilled   ,
                sciTextAlignment   alignment   ) ;

void Objtitle( char * str,
               int    n  ,
               long * hdl ) ;

void Objplot2d ( int       ptype     ,
                 char      logflags[],
                 double    x[]       ,
                 double    y[]       ,
                 integer * n1        ,
                 integer * n2        ,
                 integer   style[]   ,
                 char      strflag[] ,
                 char      legend[]  ,
                 double    brect[]   ,
                 integer   aaint[]   ,
                 BOOL      flagNax    ) ;

void Objgrayplot ( double    x[]      ,
                   double    y[]      ,
                   double    z[]      ,
                   integer * n1       ,
                   integer * n2       ,
                   char      strflag[],
                   double    brect[]  ,
                   integer   aaint[]  ,
                   BOOL      flagNax   ) ;

void Objmatplot (double    z[]      ,
                 integer * n1       ,
                 integer * n2       ,
                 char      strflag[],
                 double    brect[]  ,
                 integer   aaint[]  ,
                 BOOL      flagNax   ) ;

void Objmatplot1 ( double    z[],
                   integer * n1 ,
                   integer * n2 ,
                   double    xrect[] ) ;

void Objplot3d ( char    * fname ,
                 integer * isfac ,
                 integer * izcol ,
                 double    x[]   ,
                 double    y[]   ,
                 double    z[]   ,
                 double  * zcol  ,
                 integer * m     ,
                 integer * n     ,
                 double  * theta ,
                 double  * alpha ,
                 char    * legend,
                 integer * iflag ,
                 double  * ebox  ,
                 integer * m1    , /*Adding F.Leray 12.03.04 and 19.03.04*/
                 integer * n1    ,
                 integer * m2    ,
                 integer * n2    ,
                 integer * m3    ,
                 integer * n3    ,
                 integer * m3n   ,
                 integer * n3n    ) ;

void Objdrawaxis ( char     dir    ,
                   char     tics   ,
                   double * x      ,
                   int    * nx     ,
                   double * y      ,
                   int    * ny     ,
                   char   * val[]  ,
                   int      subint ,
                   char   * format ,
                   int      font   ,
                   int      textcol,
                   int      ticscol,
                   char     flag   ,
                   int      seg    ,
                   int      nb_tics_labels ) ;

void Objnumb( char          * fname    ,
              unsigned long   fname_len,
              integer         n        ,
              integer         flag     ,
              double          x        ,
              double          y        ,
              double        * angle    ,
              double        * box       ) ;

void Objfec ( double    x[]        ,
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
              BOOL      WithMesh   ,
              BOOL      flagNax     ) ;

#endif /* _SCI_CALL_H_ */
