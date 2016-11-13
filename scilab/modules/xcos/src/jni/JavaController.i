/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
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

/*
 * swig -java -c++ -directors -package org.scilab.modules.xcos -outdir src/java/org/scilab/modules/xcos/ src/jni/JavaController.i
 * or make swig
 */

%module(director="1") JavaController;

%{
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>

#include "utilities.hxx"
#include "View.hxx"
#include "Controller.hxx"

#include "scicos.h"
%}

%include <enums.swg>
%include <std_common.i>
%include <typemaps.i>
%include <std_string.i>

/*
 * Inline std_vector.i and add insert and remove methods
 */
namespace std {

    template<class T> class vector {
      public:
        typedef size_t size_type;
        typedef T value_type;
        typedef const value_type& const_reference;
        vector();
        vector(size_type n);
        %rename(ensureCapacity) reserve;
        void reserve(size_type n);
        void resize(size_type n);
        int size() const;
        %rename(isEmpty) empty;
        bool empty() const;
        void clear();
        %rename(add) push_back;
        void push_back(const value_type& x);
        %extend {
            bool contains(const T& o) {
               return std::find(self->begin(), self->end(), o) != self->end();
            }
            int indexOf(const T& o) {
               auto it = std::find(self->begin(), self->end(), o);
               if (it != self->end())
                   return std::distance(self->begin(), it);
               else
                   return -1;
            }
            const_reference get(int i) throw (std::out_of_range) {
                int size = int(self->size());
                if (i>=0 && i<size)
                    return (*self)[i];
                else
                    throw std::out_of_range("vector index out of range");
            }
            void set(int i, const value_type& val) throw (std::out_of_range) {
                int size = int(self->size());
                if (i>=0 && i<size)
                    (*self)[i] = val;
                else
                    throw std::out_of_range("vector index out of range");
            }
            void add(int i, const value_type& val) throw (std::out_of_range) {
                int size = int(self->size());
                if (i>=0 && i<=size)
                    self->insert(self->begin() + i, val);
                else
                    throw std::out_of_range("vector index out of range");
            }
            bool remove(const value_type& val) {
                auto it = std::find(self->begin(), self->end(), val);
                if (it == self->end())
                    return false;
                else
                    self->erase(it);
                return true;
            }
            void* asByteBuffer(int i, int capacity) {
                (void) capacity;
                void* buffer = nullptr;
                int size = int(self->size()) ;
                if (i>=0 && i<size) {
                    buffer = self->data() + i;
                } else if (i==0 && size == 0) {
                    buffer = self->data();
                } else {
                    throw std::out_of_range("vector index out of range");
                }
                return buffer;
            }
        }
    };

    // bool specialization
    template<> class vector<bool> {
      public:
        typedef size_t size_type;
        typedef bool value_type;
        typedef bool const_reference;
        vector();
        vector(size_type n);
        %rename(ensureCapacity) reserve;
        void reserve(size_type n);
        void resize(size_type n);
        int size() const;
        %rename(isEmpty) empty;
        bool empty() const;
        void clear();
        %rename(add) push_back;
        void push_back(const value_type& x);
        %extend {
            bool get(int i) throw (std::out_of_range) {
                int size = int(self->size());
                if (i>=0 && i<size)
                    return (*self)[i];
                else
                    throw std::out_of_range("vector index out of range");
            }
            void set(int i, const value_type& val) throw (std::out_of_range) {
                int size = int(self->size());
                if (i>=0 && i<size)
                    (*self)[i] = val;
                else
                    throw std::out_of_range("vector index out of range");
            }
            void add(int i, const value_type& val) throw (std::out_of_range) {
                int size = int(self->size());
                if (i>=0 && i<=size)
                    self->insert(self->begin() + i, val);
                else
                    throw std::out_of_range("vector index out of range");
            }
            bool remove(const value_type& val) {
                auto it = std::find(self->begin(), self->end(), val);
                if (it == self->end())
                    return false;
                else
                    self->erase(it);
                return true;
            }
        }

    };
}

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
%rename(PortKind) portKind;

%include "../scicos/includes/utilities.hxx";

// define scicos symbol visibility
#define SCICOS_IMPEXP

/*
 * Custom typemap definition
 */

// std::string &
%typemap(jni)           std::string &           "jobjectArray"
%typemap(jtype)         std::string &           "String[]"
%typemap(jstype)        std::string &           "String[]"
%typemap(javain)        std::string &           "$javainput"

%typemap(in) std::string & ($*1_ltype temp) {
  if (!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "array null");
    return $null;
  }
  if (JCALL1(GetArrayLength, jenv, $input) == 0) {
    SWIG_JavaThrowException(jenv, SWIG_JavaIndexOutOfBoundsException, "Array must contain at least 1 element");
    return $null;
  }
  $1 = &temp;
}

%typemap(argout) std::string &OUTPUT {
  jstring jnewstring = JCALL1(NewStringUTF, jenv, $1->c_str());
  JCALL3(SetObjectArrayElement, jenv, $input, 0, jnewstring);
}

// const std::string &
%typemap(jni)           const std::string &     "jstring"
%typemap(jtype)         const std::string &     "String"
%typemap(jstype)        const std::string &     "String"
%typemap(javain)        const std::string &     "$javainput"

%typemap(in,noblock=1) const std::string & {
  Swig::JavaString javaString(jenv, $input);
  std::string $1_str(javaString.c_str());
  $1 = &$1_str;
}

%typemap(argout,noblock=1) const std::string & { }

// apply the typemaps to manage outputs arguments
%apply double &OUTPUT { double &v };
%apply int &OUTPUT { int &v };
%apply bool &OUTPUT { bool &v };
%apply long long &OUTPUT { long long &v }; // ScicosID
%apply std::string &OUTPUT { std::string &v };

/*
 * Custom typemap to retrieve a view of the memory as a ByteBuffer
 */
%typemap(jni)     void* "jobject"
%typemap(jtype)   void* "java.nio.ByteBuffer"
%typemap(jstype)  void* "java.nio.ByteBuffer"

%typemap(out) void* {
    if (arg3 <= 0) {
        arg3 = arg1->size() - arg2;
    } else if (arg1->size() < arg3) {
        throw std::out_of_range("vector index out of range");
    }
    $result = JCALL2(NewDirectByteBuffer, jenv, $1, arg3 * sizeof(decltype(arg1->back())));
}
%typemap(javaout) void* {
    java.nio.ByteBuffer buffer = $jnicall;
    buffer.order(java.nio.ByteOrder.nativeOrder());
    return buffer;
  }

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

/*
 * Template instanciation
 */
%template(VectorOfInt)      std::vector<int>;
%template(VectorOfBool)     std::vector<bool>;
%template(VectorOfDouble)   std::vector<double>;
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
import java.util.Map;
import java.util.TreeMap;
%}

%pragma(java) modulebase="Controller"

%pragma(java) modulecode=%{
  // will contain all registered JavaViews to prevent garbage-collection
  private static Map<String, View> references = new TreeMap<String, View>();

  private static long add_reference(String name, View v) {
    references.put(name, v);
    return View.getCPtr(v);
  }

  private static View remove_reference(View v) {
    references.values().remove(v);
    return v;
  }

  public static View lookup_view(String name) {
    return references.get(name);
  }
%}

/* Quick 'n dirty but works fine */
%typemap(javain) org_scilab_modules_scicos::View* "add_reference(name, $javainput)"
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
