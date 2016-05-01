/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
