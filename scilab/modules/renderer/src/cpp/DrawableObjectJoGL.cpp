/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Objects which can be drawn using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>

#include "DrawableObjectJoGL.h"
#include "DrawableObject.h"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableObjectJoGL::DrawableObjectJoGL( DrawableObject * drawer )
{
  m_pDrawer = drawer ;
  m_pJavaMapper = NULL;
}
/*---------------------------------------------------------------------------------*/
DrawableObjectJoGL::~DrawableObjectJoGL( void )
{
  destroy() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::initializeDrawing( void )
{
  getJavaMapper()->initializeDrawing(sciGetNum(sciGetParentFigure(getDrawer()->getDrawedObject())) );
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endDrawing( void )
{
	try
	{
		getJavaMapper()->endDrawing();
	}
	catch (const std::exception & e)
	{
		sciprint(const_cast<char*>(_("%s: No more memory.\n")),"DrawableObjectJoGL::endDrawing");
	}
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::translate(const double translation[3])
{
  getJavaMapper()->translate(translation);
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endTranslate(void)
{
  getJavaMapper()->endTranslate();
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::show( void )
{
  getJavaMapper()->show(sciGetNum(sciGetParentFigure(getDrawer()->getDrawedObject())));
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::setJavaMapper( DrawableObjectJavaMapper * javaMapper )
{
  if (m_pJavaMapper != NULL)
  {
    destroy();
  }
  m_pJavaMapper = javaMapper;
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::destroy( void )
{
  if (m_pJavaMapper != NULL)
  {
    // bug on intel compiler
    // it is not possible here to retrieve m_pDrawer using the function getDrawer()
    // it may return invalid pointers.
    DrawableObject * drawer = m_pDrawer;
    sciPointObj * pObj = drawer->getDrawedObject();
    getJavaMapper()->destroy(sciGetNum(sciGetParentFigure(pObj)));
    delete getJavaMapper();
    m_pJavaMapper = NULL;
  }
}
/*---------------------------------------------------------------------------------*/
}
