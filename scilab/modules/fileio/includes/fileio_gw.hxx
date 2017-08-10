/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __FILEIO_GW_HXX__
#define __FILEIO_GW_HXX__

#include "cpp_gateway_prototype.hxx"

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
    EXTERN_FILEIO_GW static int Load();
    EXTERN_FILEIO_GW static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_mopen);
CPP_GATEWAY_PROTOTYPE(sci_mclose);
CPP_GATEWAY_PROTOTYPE(sci_mput);
CPP_GATEWAY_PROTOTYPE(sci_mget);
CPP_GATEWAY_PROTOTYPE(sci_mputl);
CPP_GATEWAY_PROTOTYPE(sci_mgetl);
CPP_GATEWAY_PROTOTYPE(sci_pathconvert);
CPP_GATEWAY_PROTOTYPE(sci_isdir);
CPP_GATEWAY_PROTOTYPE(sci_filesep);
CPP_GATEWAY_PROTOTYPE(sci_fileparts);
CPP_GATEWAY_PROTOTYPE(sci_findfiles);
CPP_GATEWAY_PROTOTYPE(sci_basename);
CPP_GATEWAY_PROTOTYPE(sci_isfile);
CPP_GATEWAY_PROTOTYPE(sci_fileinfo);
CPP_GATEWAY_PROTOTYPE(sci_deletefile);
CPP_GATEWAY_PROTOTYPE(sci_getshortpathname);
CPP_GATEWAY_PROTOTYPE(sci_getlongpathname);
CPP_GATEWAY_PROTOTYPE(sci_fileext);
CPP_GATEWAY_PROTOTYPE(sci_createdir);
CPP_GATEWAY_PROTOTYPE(sci_fullpath);
CPP_GATEWAY_PROTOTYPE(sci_removedir);
CPP_GATEWAY_PROTOTYPE(sci_meof);
CPP_GATEWAY_PROTOTYPE(sci_mputstr);
CPP_GATEWAY_PROTOTYPE(sci_mgetstr);
CPP_GATEWAY_PROTOTYPE(sci_mseek);
CPP_GATEWAY_PROTOTYPE(sci_mtell);
CPP_GATEWAY_PROTOTYPE(sci_newest);
CPP_GATEWAY_PROTOTYPE(sci_merror);
CPP_GATEWAY_PROTOTYPE(sci_mclearerr);
CPP_GATEWAY_PROTOTYPE(sci_get_absolute_file_path);
CPP_GATEWAY_PROTOTYPE(sci_getrelativefilename);
CPP_GATEWAY_PROTOTYPE(sci_pathsep);
CPP_GATEWAY_PROTOTYPE(sci_mfprintf);
CPP_GATEWAY_PROTOTYPE(sci_mscanf);
CPP_GATEWAY_PROTOTYPE(sci_mfscanf);
CPP_GATEWAY_PROTOTYPE(sci_msscanf);
CPP_GATEWAY_PROTOTYPE(sci_getdrives);

#endif /* !__FILEIO_GW_HXX__ */
