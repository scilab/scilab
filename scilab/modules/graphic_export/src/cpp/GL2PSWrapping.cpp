/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "GL2PSWrapping.hxx"
#include "JoGLInterface.h"

#include "gl2ps/gl2ps.h"

extern "C" {
#include "charEncoding.h"
#include "MALLOC.h"
}

FILE * curExportFile = NULL;

int sci_gl2psBeginPage(const char *title, const char *producer, 
                                  int viewport[4], int format, int sort,
                                  int options, int colormode,
                                  int colorsize, float colormap_r[],float colormap_g[],
								  float colormap_b[],float colormap_a[],
                                  int nr, int ng, int nb, int buffersize,
								  const char *filename){

	GL2PSrgba * colorMap = new GL2PSrgba[colorsize];
	for (int i = 0; i < colorsize; i++) {
		colorMap[i][0] = colormap_r[i];
		colorMap[i][1] = colormap_g[i];
		colorMap[i][2] = colormap_b[i];
		colorMap[i][3] = colormap_a[i];
	}

	wcfopen(curExportFile , (char*)filename, "wb");

	int returnStatus = gl2psBeginPage(title, producer, viewport, format, sort,
									  options, colormode, colorsize, colorMap, nr, ng, nb, buffersize, curExportFile, filename);

	delete[] colorMap;

	return returnStatus;
}

int sci_gl2psEndPage(void){
	int returnStatus = gl2psEndPage();
	fclose(curExportFile);
	curExportFile = NULL;
	return returnStatus;
}

int sci_gl2psSetOptions(int options){
	return gl2psSetOptions(options);
};

int sci_gl2psBeginViewport(int viewport[4]){
	return gl2psBeginViewport(viewport);
};

int sci_gl2psEndViewport(void){
	return gl2psEndViewport();
}

int sci_gl2psText(const char *str, const char *fontname, short fontsize){
	return gl2psText(str, fontname, fontsize);
};

int sci_gl2psTextOpt(const char *str, const char *fontname, 
                                short fontsize, int align, float angle){
	return gl2psTextOpt(str, fontname, fontsize, align, angle);
};

int sci_gl2psSpecial(int format, const char *str){
	return gl2psSpecial(format, str);
};

int sci_gl2psEnable(int mode){
	return gl2psEnable(mode);
};

int sci_gl2psDisable(int mode){
	return gl2psDisable(mode);
};

int sci_gl2psPointSize(float value){
	return gl2psPointSize(value);
};

int sci_gl2psLineWidth(float value){
	return gl2psLineWidth(value);
};

int sci_gl2psBlendFunc(int sfactor, int dfactor){
	return gl2psBlendFunc(sfactor, dfactor);
};

/* undocumented */
int sci_gl2psDrawImageMap(int width, int height,
                                     float position[3],
                                     const char *imagemap){
	return gl2psDrawImageMap(width, height, position, (const unsigned char *)imagemap);
};

const char * sci_gl2psGetFileExtension(int format){
	return gl2psGetFileExtension(format);
};

const char * sci_gl2psGetFormatDescription(int format){
	return gl2psGetFormatDescription(format);
};

/* Added by Calixte */
int sci_gl2psDrawPixels(int w, int h, int format, int type, const void * buffer){
  if (type == joglGL_UNSIGNED_BYTE())
    {
      type = joglGL_FLOAT();
      char pix = (format == joglGL_RGBA()) ? 4 : 3;
      float *arrf = (float*)MALLOC(w * h * sizeof(float) * pix);
      int i;
      for (i = 0; i < w * h * pix; i++) 
	arrf[i] = (float)(((unsigned char*)buffer)[i]) / 255.0f;
	
      int ret = gl2psDrawPixels(w, h, 0, 0, format, type, arrf);
      free(arrf);
      return ret;
    }
  return gl2psDrawPixels(w, h, 0, 0, format, type, buffer);
};
/* End */
