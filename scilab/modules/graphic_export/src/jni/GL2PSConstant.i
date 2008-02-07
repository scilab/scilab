/* Swig file: GL2PSConstant.i */
/*  swig -java -package org.scilab.modules.graphic_export.jni -outdir ../java/org/scilab/modules/graphic_export/jni/ GL2PSConstant.i */
%module GL2PSConstant


%{
#include "GL2PSConstant.hxx"
%}

/* JavaDoc for GL2PSConstantCallJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
  * Call from the JoGL display thread to figure drawing code using JNI 
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007
  */
public class%}

/* Constructor for GL2PSConstantCallJNI class */
%pragma(java) jniclasscode="
  /**
   * Constructor. Should not be called
   */
  protected GL2PSConstantJNI() {
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

/* JavaDoc for GL2PSConstantCall class */
%pragma(java) moduleclassmodifiers="
 /** 
  * Call from the JoGL display thread to figure drawing code using JNI 
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007 
  */
public class";

/* Constructor for GL2PSConstantCall class */
%pragma(java) modulecode="
 /**
  * Constructor. Should not be called.
  */
 protected GL2PSConstant() {
	throw new UnsupportedOperationException();
 }";


/* JavaDoc */
%javamethodmodifiers get_GL2PS_MAJOR_VERSION()"
 /**
  * get_GL2PS_MAJOR_VERSION
  * @return get_GL2PS_MAJOR_VERSION
  */
public";
int get_GL2PS_MAJOR_VERSION();


/* JavaDoc */
%javamethodmodifiers get_GL2PS_MINOR_VERSION() "
 /**
  * get_GL2PS_MINOR_VERSION
  * @return get_GL2PS_MINOR_VERSION
  */
public";
int get_GL2PS_MINOR_VERSION();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_PATCH_VERSION() "
 /**
  * get_GL2PS_PATCH_VERSION
  * @return get_GL2PS_PATCH_VERSION
  */
public";
int get_GL2PS_PATCH_VERSION();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_EXTRA_VERSION() "
 /**
  * get_GL2PS_EXTRA_VERSION
  * @return get_GL2PS_EXTRA_VERSION
  */
public";
char * get_GL2PS_EXTRA_VERSION();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_PS() "
 /**
  * get_GL2PS_PS
  * @return get_GL2PS_PS
  */
public";
int get_GL2PS_PS();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_EPS() "
 /**
  * get_GL2PS_EPS
  * @return get_GL2PS_EPS
  */
public";
int get_GL2PS_EPS();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEX() "
 /**
  * get_GL2PS_TEX
  * @return get_GL2PS_TEX
  */
public";
int get_GL2PS_TEX();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_PDF() "
 /**
  * get_GL2PS_PDF
  * @return get_GL2PS_PDF
  */
public";
int get_GL2PS_PDF();
							 

/* JavaDoc */
%javamethodmodifiers get_GL2PS_SVG() "
 /**
  * get_GL2PS_SVG
  * @return get_GL2PS_SVG
  */
public";
int get_GL2PS_SVG();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_PGF() "
 /**
  * get_GL2PS_PGF
  * @return get_GL2PS_PGF
  */
public";
int get_GL2PS_PGF();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_NO_SORT() "
 /**
  * get_GL2PS_NO_SORT
  * @return get_GL2PS_NO_SORT
  */
public";
int get_GL2PS_NO_SORT();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_SIMPLE_SORT() "
 /**
  * get_GL2PS_SIMPLE_SORT
  * @return get_GL2PS_SIMPLE_SORT
  */
public";
int get_GL2PS_SIMPLE_SORT();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_BSP_SORT() "
 /**
  * get_GL2PS_BSP_SORT
  * @return get_GL2PS_BSP_SORT
  */
public";
int get_GL2PS_BSP_SORT();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_SUCCESS() "
 /**
  * get_GL2PS_SUCCESS
  * @return get_GL2PS_SUCCESS
  */
public";
int get_GL2PS_SUCCESS();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_INFO() "
 /**
  * get_GL2PS_INFO
  * @return get_GL2PS_INFO
  */
public";
int get_GL2PS_INFO();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_WARNING() "
 /**
  * get_GL2PS_WARNING
  * @return get_GL2PS_WARNING
  */
public";
int get_GL2PS_WARNING();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_ERROR() "
 /**
  * get_GL2PS_ERROR 
  * @return get_GL2PS_ERROR
  */
public";
int get_GL2PS_ERROR();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_NO_FEEDBACK() "
 /**
  * get_GL2PS_NO_FEEDBACK 
  * @return get_GL2PS_NO_FEEDBACK
  */
public";
int get_GL2PS_NO_FEEDBACK();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_OVERFLOW() "
 /**
  * get_GL2PS_OVERFLOW 
  * @return get_GL2PS_OVERFLOW
  */
public";
int get_GL2PS_OVERFLOW();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_UNINITIALIZED() "
 /**
  * get_GL2PS_UNINITIALIZED 
  * @return get_GL2PS_UNINITIALIZED
  */
public";
int get_GL2PS_UNINITIALIZED();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_NONE() "
 /**
  * get_GL2PS_NONE 
  * @return get_GL2PS_NONE
  */
public";
int get_GL2PS_NONE();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_DRAW_BACKGROUND() "
 /**
  * get_GL2PS_DRAW_BACKGROUND 
  * @return get_GL2PS_DRAW_BACKGROUND
  */
public";
int get_GL2PS_DRAW_BACKGROUND();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_SIMPLE_LINE_OFFSET() "
 /**
  * get_GL2PS_SIMPLE_LINE_OFFSET 
  * @return get_GL2PS_SIMPLE_LINE_OFFSET
  */
public";
int get_GL2PS_SIMPLE_LINE_OFFSET();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_SILENT() "
 /**
  * get_GL2PS_SILENT 
  * @return get_GL2PS_SILENT
  */
public";
int get_GL2PS_SILENT();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_BEST_ROOT() "
 /**
  * get_GL2PS_BEST_ROOT 
  * @return get_GL2PS_BEST_ROOT
  */
public";
int get_GL2PS_BEST_ROOT();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_OCCLUSION_CULL() "
 /**
  * get_GL2PS_OCCLUSION_CULL 
  * @return get_GL2PS_OCCLUSION_CULL
  */
public";
int get_GL2PS_OCCLUSION_CULL();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_NO_TEXT() "
 /**
  * get_GL2PS_NO_TEXT 
  * @return get_GL2PS_NO_TEXT
  */
public";
int get_GL2PS_NO_TEXT();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_LANDSCAPE()"
 /**
  * get_GL2PS_LANDSCAPE 
  * @return get_GL2PS_LANDSCAPE
  */
public";
int get_GL2PS_LANDSCAPE();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_NO_PS3_SHADING"
 /**
  * get_GL2PS_NO_PS3_SHADING 
  * @return get_GL2PS_NO_PS3_SHADING
  */
public";
int get_GL2PS_NO_PS3_SHADING();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_NO_PIXMAP()"
 /**
  * get_GL2PS_NO_PIXMAP 
  * @return get_GL2PS_NO_PIXMAP
  */
public";
int get_GL2PS_NO_PIXMAP();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_USE_CURRENT_VIEWPORT()"
 /**
  * get_GL2PS_USE_CURRENT_VIEWPORT 
  * @return get_GL2PS_USE_CURRENT_VIEWPORT
  */
public";
int get_GL2PS_USE_CURRENT_VIEWPORT();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_COMPRESS()"
 /**
  * get_GL2PS_COMPRESS 
  * @return get_GL2PS_COMPRESS
  */
public";
int get_GL2PS_COMPRESS();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_NO_BLENDING()"
 /**
  * get_GL2PS_NO_BLENDING 
  * @return get_GL2PS_NO_BLENDING
  */
public";
int get_GL2PS_NO_BLENDING();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TIGHT_BOUNDING_BOX()"
 /**
  * get_GL2PS_TIGHT_BOUNDING_BOX 
  * @return get_GL2PS_TIGHT_BOUNDING_BOX
  */
public";
int get_GL2PS_TIGHT_BOUNDING_BOX();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_POLYGON_OFFSET_FILL()"
 /**
  * get_GL2PS_POLYGON_OFFSET_FILL 
  * @return get_GL2PS_POLYGON_OFFSET_FILL
  */
public";
int get_GL2PS_POLYGON_OFFSET_FILL();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_POLYGON_BOUNDARY()"
 /**
  * get_GL2PS_POLYGON_BOUNDARY 
  * @return get_GL2PS_POLYGON_BOUNDARY
  */
public";
int get_GL2PS_POLYGON_BOUNDARY();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_LINE_STIPPLE()"
 /**
  * get_GL2PS_LINE_STIPPLE 
  * @return get_GL2PS_LINE_STIPPLE
  */
public";
int get_GL2PS_LINE_STIPPLE();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_BLEND()"
 /**
  * get_GL2PS_BLEND 
  * @return get_GL2PS_BLEND
  */
public";
int get_GL2PS_BLEND();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_C()"
 /**
  * get_GL2PS_TEXT_C 
  * @return get_GL2PS_TEXT_C
  */
public";
int get_GL2PS_TEXT_C();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_CL()"
 /**
  * get_GL2PS_TEXT_CL 
  * @return get_GL2PS_TEXT_CL
  */
public";
int get_GL2PS_TEXT_CL();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_CR()"
 /**
  * get_GL2PS_TEXT_CR 
  * @return get_GL2PS_TEXT_CR
  */
public";
int get_GL2PS_TEXT_CR();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_B()"
 /**
  * get_GL2PS_TEXT_B 
  * @return get_GL2PS_TEXT_B
  */
public";
int get_GL2PS_TEXT_B();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_BL()"
 /**
  * get_GL2PS_TEXT_BL 
  * @return get_GL2PS_TEXT_BL
  */
public";
int get_GL2PS_TEXT_BL();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_BR()"
 /**
  * get_GL2PS_TEXT_BR 
  * @return get_GL2PS_TEXT_BR
  */
public";
int get_GL2PS_TEXT_BR();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_T()"
 /**
  * get_GL2PS_TEXT_T 
  * @return get_GL2PS_TEXT_T
  */
public";
int get_GL2PS_TEXT_T();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_TL()"
 /**
  * get_GL2PS_TEXT_TL 
  * @return get_GL2PS_TEXT_TL
  */
public";
int get_GL2PS_TEXT_TL();

/* JavaDoc */
%javamethodmodifiers get_GL2PS_TEXT_TR"
 /**
  * get_GL2PS_TEXT_TR 
  * @return get_GL2PS_TEXT_TR
  */
public";
int get_GL2PS_TEXT_TR(); 
 
 
 
