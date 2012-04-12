/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2012 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

%module DataLoader

%include "arrays_java.i"

%define NIO_BUFFER_TYPEMAP(CTYPE, LABEL, BUFFERTYPE)
  %typemap(jni) CTYPE* LABEL "jobject"
  %typemap(jtype) CTYPE* LABEL "BUFFERTYPE"
  %typemap(jstype) CTYPE* LABEL "BUFFERTYPE"
  %typemap(javain,	  pre="    assert $javainput.isDirect() : \"Buffer must be allocated direct.\";") CTYPE* LABEL "$javainput"
  %typemap(javaout) CTYPE* LABEL {
      return $jnicall;
  }
  %typemap(in) CTYPE* LABEL {
    $1 = (*jenv)->GetDirectBufferAddress(jenv, $input);
    if ($1 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  %typemap(memberin) CTYPE* LABEL {
    if ($input) {
      $1 = $input;
    } else {
      $1 = 0;
    }
  }
%enddef

/*
NIO_BUFFER_TYPEMAP(void, BUFF, java.nio.Buffer);
NIO_BUFFER_TYPEMAP(char, BUFF, java.nio.ByteBuffer);
NIO_BUFFER_TYPEMAP(char, CBUFF, java.nio.CharBuffer);
NIO_BUFFER_TYPEMAP(short, BUFF, java.nio.ShortBuffer);
NIO_BUFFER_TYPEMAP(unsigned short, BUFF, java.nio.IntBuffer);
NIO_BUFFER_TYPEMAP(unsigned int, BUFF, java.nio.LongBuffer);
NIO_BUFFER_TYPEMAP(long, BUFF, java.nio.IntBuffer);
NIO_BUFFER_TYPEMAP(unsigned long, BUFF, java.nio.LongBuffer);
NIO_BUFFER_TYPEMAP(long long, BUFF, java.nio.LongBuffer);
NIO_BUFFER_TYPEMAP(double, BUFF, java.nio.DoubleBuffer);
*/
NIO_BUFFER_TYPEMAP(int, BUFF, java.nio.IntBuffer);
NIO_BUFFER_TYPEMAP(unsigned char, BUFF, java.nio.ByteBuffer);
NIO_BUFFER_TYPEMAP(float, BUFF, java.nio.FloatBuffer);
#undef NIO_BUFFER_TYPEMAP

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

%apply int* BUFF {int* buffer};
%apply unsigned char* BUFF {unsigned char* buffer};
%apply float* BUFF {float* buffer};

%include "../../includes/DataLoader.hxx"

%inline
%{
  #include "DataLoader.hxx"
%}
