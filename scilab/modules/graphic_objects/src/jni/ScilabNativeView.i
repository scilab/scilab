/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/**
 * Windows: swig -java -package org.scilab.modules.graphic_objects -outdir ../java/org/scilab/modules/graphic_objects/ ScilabNativeView.i
 * Other: Use the option --enable-build-swig to the configure
*/

%module ScilabNativeView

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

%inline %{
    extern void ScilabNativeView__createObject(int id);
    extern void ScilabNativeView__deleteObject(int id);
    extern void ScilabNativeView__updateObject(int id, int iProperty);
    extern void ScilabNativeView__setCurrentFigure(int id);
    extern void ScilabNativeView__setCurrentSubWin(int id);
    extern void ScilabNativeView__setCurrentObject(int id);
    extern int ScilabNativeView__getValidDefaultFigureId();
    extern int ScilabNativeView__getFigureFromIndex(int figNum);
    %}
