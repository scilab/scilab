/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by DrawableObject 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_OBJECT_JAVA_MAPPER_HXX_
#define _DRAWABLE_OBJECT_JAVA_MAPPER_HXX_

namespace sciGraphics
{

class DrawableObjectJavaMapper
{
public:
  
  DrawableObjectJavaMapper(void) {}

  virtual ~DrawableObjectJavaMapper(void) {}

  virtual void display(void) = 0;

  virtual void initializeDrawing(int figureIndex) = 0;

  virtual void endDrawing(void) = 0;

  virtual void show(int figureIndex) = 0;

  virtual void destroy(int parentFigureIndex) = 0;

  virtual void translate(const double translation[3]) { };

  virtual void endTranslate(void) { };

};

}

#endif /* _DRAWABLE_OBJECT_JAVA_MAPPER_HXX_ */
