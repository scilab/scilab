/* GuiManagement.i */
/*  swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ GuiManagement.i */
%module GuiManagement
%{
#include "../c/GuiManagement.h"
%}

/* JavaDoc for GuiManagementJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * All Scilab gui management used in Java console 
   * @author Vincent COUVERT
   * @copyright INRIA 2007
   */
public class%}

/* Constructor for GuiManagementJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected GuiManagementJNI() {
	throw new UnsupportedOperationException();
  }";

/* JavaDoc for GuiManagement class */
%pragma(java) moduleclassmodifiers="
 /** 
   * All Scilab gui management used in Java console 
   * @author Vincent COUVERT
   * @copyright INRIA 2007 
   */
public class";

/* Constructor for GuiManagement class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected GuiManagement() {
	throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers setScilabLines(int nbRows, int nbCols) "
 /**
   * Set Scilab parameters used to display data according the size of the console
   * @param nbRows number of rows that can be used to display data
   * @param nbCols number of columns that can be used to display data
   */
";
void setScilabLines(int nbRows, int nbCols);