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
 
#ifndef _GL2PS_WRAPPING_H_
#define _GL2PS_WRAPPING_H_

#ifdef __cplusplus
extern "C" {
#endif



int sci_gl2psBeginPage(const char *title, const char *producer, 
                                  int viewport[4], int format, int sort,
                                  int options, int colormode,
                                  int colorsize, float colormap_r[],float colormap_g[],
								  float colormap_b[],float colormap_a[], 
                                  int nr, int ng, int nb, int buffersize,
                                  const char *filename);


int sci_gl2psEndPage(void);

int sci_gl2psSetOptions(int options);

int sci_gl2psBeginViewport(int viewport[4]);

int sci_gl2psEndViewport(void);

int sci_gl2psText(const char *str, const char *fontname, 
                             short fontsize);

int sci_gl2psTextOpt(const char *str, const char *fontname, 
                                short fontsize, int align, float angle);

int sci_gl2psSpecial(int format, const char *str);

int sci_gl2psEnable(int mode);

int sci_gl2psDisable(int mode);

int sci_gl2psPointSize(float value);

int sci_gl2psLineWidth(float value);

int sci_gl2psBlendFunc(int sfactor, int dfactor);

/* undocumented */
int sci_gl2psDrawImageMap(int width, int height,
                                     float position[3],
                                     const char *imagemap);


const char * sci_gl2psGetFileExtension(int format);

const char * sci_gl2psGetFormatDescription(int format);

/* Added by Calixte */
int sci_gl2psDrawPixels(int w, int h, int format, int type, const void * buffer);
/* End */

#ifdef __cplusplus
}
#endif

#endif /* _GL2PS_WRAPPING_H_ */
