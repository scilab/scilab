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

#ifndef __FILEIO_GW_HXX__
#define __FILEIO_GW_HXX__

#include "funcmanager.hxx"
#include "context.hxx"

#ifdef _MSC_VER
	#if FILEIO_GW_EXPORTS
		#define EXTERN_FILEIO_GW __declspec (dllexport)
	#else
		#define EXTERN_FILEIO_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_FILEIO_GW 
#endif

class FileioModule
{
private :
    FileioModule() {};
    ~FileioModule() {};

public :
	EXTERN_FILEIO_GW static bool Load();
};

CPP_GATEWAY_PROTOTYPE(sci_mopen);
CPP_GATEWAY_PROTOTYPE(sci_mclose);
CPP_GATEWAY_PROTOTYPE(sci_mput);
CPP_GATEWAY_PROTOTYPE(sci_mget);

#endif /* !__FILEIO_GW_HXX__ */
