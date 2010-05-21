/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __GUI_GW_HXX__
#define __GUI_GW_HXX__

#ifdef _MSC_VER
	#if GUI_GW_EXPORTS
		#define EXTERN_GUI_GW __declspec (dllexport)
	#else
		#define EXTERN_GUI_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_GUI_GW 
#endif

class GuiModule
{
private :
	GuiModule(){};
	~GuiModule(){};
public :
	EXTERN_GUI_GW static bool Load();
};

#endif /* !__GUI_GW_HXX__ */
