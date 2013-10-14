/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// swig -java -package org.scilab.modules.gui.uiwidget -outdir ../java/org/scilab/modules/gui/uiwidget/ UserData.i

%module UserData

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scigui");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}

%inline %{
    extern void removeUIWidgetUserData(const int uid);
%}
