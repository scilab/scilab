/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

%module SurfaceData
 

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scigraphic_objects");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}



%{
extern char * createObject3dData(char *obj, char *newObj, int type);
extern double pickSurface(char * uid, double x, double y,  double z, double dx, double dy, double dz, double mx, double my, double mz, double mw);
%}

char * createObject3dData(char *obj, char *newObj, int type);
double pickSurface(char * uid, double x, double y,  double z, double dx, double dy, double dz, double mx, double my, double mz, double mw);



