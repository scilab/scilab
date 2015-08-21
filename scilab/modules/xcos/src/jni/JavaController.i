/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*
 * swig -java -c++ -directors -package org.scilab.modules.xcos -outdir src/java/org/scilab/modules/xcos/ src/jni/JavaController.i
 * or make swig
 */

%module(director="1") JavaController;

%{
#include <vector>
#include <string>

#include "utilities.hxx"
#include "View.hxx"
#include "Controller.hxx"
%}

%include <enums.swg>
%include <typemaps.i>
%include <std_vector.i>
%include <std_string.i>

/*
 * Map as simple Java enum, see "25.10.1 Simpler Java enums"
 */
%typemap(javain) enum SWIGTYPE "$javainput.ordinal()"
%typemap(javaout) enum SWIGTYPE {
    return $javaclassname.class.getEnumConstants()[$jnicall];
  }
%typemap(javadirectorin) enum SWIGTYPE "$javaclassname.class.getEnumConstants()[$jniinput]"
%typemap(javadirectorout) enum SWIGTYPE "($javacall).ordinal()"
%typemap(javabody) enum SWIGTYPE ""
%javaconst(1);

// Rename the enums
%rename(Kind) kind_t;
%rename(ObjectProperties) object_properties_t;
%rename(UpdateStatus) update_status_t;

%include "../scicos/includes/utilities.hxx";

// define scicos symbol visibility
#define SCICOS_IMPEXP

/*
 * Custom typemap definition
 */

%typemap(jni) std::string &OUTPUT "jobjectArray"
%typemap(jtype) std::string &OUTPUT "String[]"
%typemap(jstype) std::string &OUTPUT "String[]"
%typemap(javain) std::string &OUTPUT "$javainput"

%typemap(in) std::string &OUTPUT($*1_ltype temp) {
  if (!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "array null");
    return $null;
  }
  if (JCALL1(GetArrayLength, jenv, $input) == 0) {
    SWIG_JavaThrowException(jenv, SWIG_JavaIndexOutOfBoundsException, "Array must contain at least 1 element");
    return $null;
  }
  $1 = &temp; 
  *$1 = "";
}

%typemap(argout) std::string &OUTPUT {
  jstring jnewstring = NULL;
  if ($1) {
     jnewstring = JCALL1(NewStringUTF, jenv, $1->c_str());
  }
  JCALL3(SetObjectArrayElement, jenv, $input, 0, jnewstring); 
}

%apply double &OUTPUT { double &v };
%apply int &OUTPUT { int &v };
%apply bool &OUTPUT { bool &v };
%apply long long &OUTPUT { long long &v }; // ScicosID
%apply std::string &OUTPUT { std::string &v };

/*
 * Generate the View interface
 */
%feature("director", assumeoverride=0) org_scilab_modules_scicos::View;
%include "../scicos/includes/View.hxx";


/*
 * Generate a Controller class
 */
// Ignore not applicable methods
%ignore org_scilab_modules_scicos::Controller::getObject;
%ignore org_scilab_modules_scicos::Controller::unregister_view;
%ignore org_scilab_modules_scicos::Controller::register_view;
%include "../scicos/includes/Controller.hxx";

// Instanciate templates mapped to Java
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty<int>;
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty<bool>;
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty<double>;
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty<std::string>;
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty<ScicosID>;
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty< std::vector<int> >;
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty< std::vector<double> >;
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty< std::vector<std::string> >;
%template(getObjectProperty) org_scilab_modules_scicos::Controller::getObjectProperty< std::vector<ScicosID> >;

%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty<int>;
%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty<bool>;
%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty<double>;
%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty<std::string>;
%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty<ScicosID>;
%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty< std::vector<int> >;
%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty< std::vector<double> >;
%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty< std::vector<std::string> >;
%template(setObjectProperty) org_scilab_modules_scicos::Controller::setObjectProperty< std::vector<ScicosID> >;

/*
 * Template instanciation
 */

%template(VectorOfDouble)   std::vector<double>;
%template(VectorOfInt)      std::vector<int>;
%template(VectorOfBool)     std::vector<bool>;
%template(VectorOfString)   std::vector<std::string>;
%template(VectorOfScicosID) std::vector<ScicosID>;

/*
 * Fill the main module by needed methods
 */
%{
static void register_view(const std::string& name, org_scilab_modules_scicos::View* view) {
  org_scilab_modules_scicos::Controller::register_view(name, view);
};
static void unregister_view(org_scilab_modules_scicos::View* view) {
  org_scilab_modules_scicos::Controller::unregister_view(view);
};
%}

%pragma(java) moduleimports=%{
import java.util.ArrayList;
%}

%pragma(java) modulebase="Controller"

%pragma(java) modulecode=%{
  // will contains all registered JavaViews to prevent garbage-collection 
  private static ArrayList<View> references = new ArrayList<View>();
  
  private static long add_reference(View v) {
    references.add(v);
    return View.getCPtr(v);
  }

  private static View remove_reference(View v) {
    references.remove(v);
    return v;
  }
%}

%typemap(javain) org_scilab_modules_scicos::View* "add_reference($javainput)"
void register_view(const std::string& name, org_scilab_modules_scicos::View* view);
%typemap(javaout) void "{
    JavaControllerJNI.unregister_view(View.getCPtr(view), view);
    remove_reference(view);
  }"
void unregister_view(org_scilab_modules_scicos::View* view);

/*
 * Static load of library
 */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scixcos");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        System.exit(-1);
    } catch (UnsatisfiedLinkError e)    {
           System.err.println("The native library scicommons does not exist or cannot be found.");
        if (System.getenv("CONTINUE_ON_JNI_ERROR") == null) {
           System.err.println(e.getLocalizedMessage());
           System.err.println("Current java.library.path is : "+System.getProperty("java.library.path"));
           System.exit(-1);
        }else{
           System.err.println("Continuing anyway because of CONTINUE_ON_JNI_ERROR");
        }
    }
  }
%}
