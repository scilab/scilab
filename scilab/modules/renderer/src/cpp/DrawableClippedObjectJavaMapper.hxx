/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by DrawableClippedObject
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_CLIPPED_OBJECT_JAVA_MAPPER_HXX_
#define _DRAWABLE_CLIPPED_OBJECT_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"

namespace sciGraphics
{

class DrawableClippedObjectJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  DrawableClippedObjectJavaMapper( void ) {}

  virtual ~DrawableClippedObjectJavaMapper( void ) {}

  virtual void clipX(double xMin, double xMax) = 0;
  virtual void clipY(double yMin, double yMax) = 0;
  virtual void clipZ(double zMin, double zMax) = 0;

  virtual void unClip(void) = 0;

};

}

#endif /* _DRAWABLE_CLIPPED_OBJECT_JAVA_MAPPER_HXX_ */

