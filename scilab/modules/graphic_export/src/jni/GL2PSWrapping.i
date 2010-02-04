/* Swig file: GL2PSWrapping.i */
/**
 * Windows: swig -java -package org.scilab.modules.graphic_export.jni -outdir ../java/org/scilab/modules/graphic_export/jni/ GL2PSWrapping.i 
 * Other: Use the option --enable-build-swig to the configure
*/
%module GL2PSWrapping


%{
#include "GL2PSWrapping.hxx"
%}
%include "arrays_java.i";

/* JavaDoc for GL2PSWrappingCallJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
  * Call from the JoGL display thread to figure drawing code using JNI 
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007
  */
public class%}

/* Constructor for GL2PSWrappingCallJNI class */
%pragma(java) jniclasscode="
  /**
   * Constructor. Should not be called
   */
  protected GL2PSWrappingJNI() {
	throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scigraphic_export");
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library scigraphic_export does not exist or cannot be found.");
		e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for GL2PSWrappingCall class */
%pragma(java) moduleclassmodifiers="
 /** 
  * Call from the JoGL dislay thread to figure drawing code using JNI 
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007 
  */
public class";

/* Constructor for GL2PSWrappingCall class */
%pragma(java) modulecode="
 /**
  * Constructor. Should not be called.
  */
 protected GL2PSWrapping() {
	throw new UnsupportedOperationException();
 }";
 
/* JavaDoc */
%javamethodmodifiers sci_gl2psBeginPage(const char *title, const char *producer, 
												   int viewport[4], int format, int sort, 
												   int options, int colormode, 
												   int colorsize, float colormap_r[],
												   float colormap_g[], float colormap_b[], float colormap_a[], 
												   int nr, int ng, int nb, int buffersize, 
												   const char *filename) "
 /**
  * Begin page
  * @param title of the page
  * @param producer of the page
  * @param viewport of the page
  * @param format of the page
  * @param sort index of the figure to redraw
  * @param options index of the figure to redraw
  * @param colormode index of the figure to redraw
  * @param colorsize index of the figure to redraw
  * @param colormap_r index of the figure to redraw
  * @param colormap_g index of the figure to redraw
  * @param colormap_b index of the figure to redraw
  * @param colormap_a index of the figure to redraw
  * @param nr index of the figure to redraw
  * @param ng index of the figure to redraw
  * @param nb index of the figure to redraw
  * @param buffersize index of the figure to redraw
  * @param filename index of the figure to redraw
  * @return GL2PSWrappingJNI.sci_gl2psBeginPage
  */
public";
int sci_gl2psBeginPage(const char *title, const char *producer, 
                                  int viewport[4], int format, int sort,
                                  int options, int colormode,
                                  int colorsize, float colormap_r[],
                                  float colormap_g[], float colormap_b[],
                                  float colormap_a[],								  
                                  int nr, int ng, int nb, int buffersize,
                                  const char *filename);


/* JavaDoc */
%javamethodmodifiers sci_gl2psEndPage(void) "
 /**
  * End of the page
  * @return GL2PSWrappingJNI.sci_gl2psEndPage
  */
public";
int sci_gl2psEndPage(void);

/* JavaDoc */
%javamethodmodifiers sci_gl2psSetOptions(int options) "
 /**
  * Option of the page
  * @param options of the page
  * @return GL2PSWrappingJNI.sci_gl2psSetOptions
  */
public";
int sci_gl2psSetOptions(int options);

/* JavaDoc */
%javamethodmodifiers sci_gl2psBeginViewport(int viewport[4]) "
 /**
  * Beginning of the viewport
  * @param viewport of the figure
  * @return GL2PSWrappingJNI.sci_gl2psBeginViewport
  */
public";
int sci_gl2psBeginViewport(int viewport[4]);

/* JavaDoc */
%javamethodmodifiers sci_gl2psEndViewport(void) "
 /**
  * End of the viewport
  * @return GL2PSWrappingJNI.sci_gl2psEndViewport
  */
public";
int sci_gl2psEndViewport(void);

/* JavaDoc */
%javamethodmodifiers sci_gl2psText(const char *str, const char *fontname, 
                             short fontsize) "
 /**
  * Text
  * @param str text to display
  * @param fontname of the text
  * @param fontsize of the text
  * @return GL2PSWrappingJNI.sci_gl2psText
  */
public";
int sci_gl2psText(const char *str, const char *fontname, 
                             short fontsize);

/* JavaDoc */
%javamethodmodifiers sci_gl2psTextOpt(const char *str, const char *fontname, 
                                short fontsize, int align, float angle) "
 /**
  * TextOPT
  * @param str text to display
  * @param fontname of the text
  * @param fontsize of the text
  * @param align of the text
  * @param angle of the text
  * @return GL2PSWrappingJNI.sci_gl2psTextOpt
  */
public";
int sci_gl2psTextOpt(const char *str, const char *fontname, 
                                short fontsize, int align, float angle);

/* JavaDoc */
%javamethodmodifiers sci_gl2psSpecial(int format, const char *str) "
 /**
  * Special
  * @param format of special
  * @param str of special
  * @return GL2PSWrappingJNI.sci_gl2psSpecial
  */
public";
int sci_gl2psSpecial(int format, const char *str);					 


/* JavaDoc */
%javamethodmodifiers sci_gl2psEnable(int mode) "
 /**
  * Enable
  * @param mode enable mode
  * @return GL2PSWrappingJNI.sci_gl2psEnable
  */
public";
int sci_gl2psEnable(int mode);

/* JavaDoc */
%javamethodmodifiers sci_gl2psDisable(int mode) "
 /**
  * Disable
  * @param mode disable mode
  * @return GL2PSWrappingJNI.sci_gl2psDisable
  */
public";
int sci_gl2psDisable(int mode);

/* JavaDoc */
%javamethodmodifiers sci_gl2psPointSize(float value) "
 /**
  * Size of the point
  * @param value value given for the size
  * @return GL2PSWrappingJNI.sci_gl2psPointSize
  */
public";
int sci_gl2psPointSize(float value);

/* JavaDoc */
%javamethodmodifiers sci_gl2psLineWidth(float value) "
 /**
  * Width of the line
  * @param value value value given for the width
  * @return GL2PSWrappingJNI.sci_gl2psLineWidth
  */
public";
int sci_gl2psLineWidth(float value);

/* JavaDoc */
%javamethodmodifiers sci_gl2psBlendFunc(int sfactor, int dfactor) "
 /**
  * Blend Function
  * @param sfactor sfactor
  * @param dfactor dfactor
  * @return GL2PSWrappingJNI.sci_gl2psBlendFunc
  */
public";
int sci_gl2psBlendFunc(int sfactor, int dfactor);

/* JavaDoc */
%javamethodmodifiers sci_gl2psDrawImageMap(int width, int height,
                                     float position[3],
                                     const char *imagemap) "
 /**
  * Draw an image map (undocumented function)
  * @param width of the image map
  * @param height of the image map
  * @param position of the image map
  * @param imagemap imagemap
  * @return GL2PSWrappingJNI.sci_gl2psDrawImageMap
  */
public";
int sci_gl2psDrawImageMap(int width, int height,
                                     float position[3],
                                     const char *imagemap);

/* JavaDoc */
%javamethodmodifiers sci_gl2psGetFileExtension(int format) "
 /**
  * Get the extension of the file (undocumented function)
  * @param format file format
  * @return GL2PSWrappingJNI.sci_gl2psGetFileExtension
  */
public";
char * sci_gl2psGetFileExtension(int format);

/* JavaDoc */
%javamethodmodifiers sci_gl2psGetFormatDescription(int format) "
 /**
  * Get format description (undocumented function)
  * @param format file format description
  * @return GL2PSWrappingJNI.sci_gl2psGetFormatDescription
  */
public";
char * sci_gl2psGetFormatDescription(int format);


%typemap(jni) void* buffer "jobject"
%typemap(jtype) void* buffer "java.nio.Buffer"
%typemap(jstype) void* buffer "java.nio.Buffer"
%typemap(javain) void* buffer "$javainput"
%typemap(in) void* buffer {
  $1 = (*jenv)->GetDirectBufferAddress(jenv, $input);
  if ($1 == NULL) {
    SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
  }
}

/* JavaDoc */
%javamethodmodifiers sci_gl2psDrawPixels(int w, int h, int format, int type, void* buffer) "
 /**
  *Draw a pixel buffer
  * @param w width of image
  * @param h height of image
  * @param format specifies the format of the pixel data. GL_RGB and GL_RGBA are the only values accepted at the moment.
  * @param type specifies the data type for pixels
  * @param buffer is a direct java.nio.Buffer containing pixmap
  * @return GL2PSWrappingJNI.sci_gl2psGetFormatDescription
  */
public";
int sci_gl2psDrawPixels(int w, int h, int format, int type, void* buffer);