/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "funcmanager.hxx"
#include "foo.hxx"
#include "cosinus.hxx"
#include <sstream>

#include "libxml/xpath.h"
#include "libxml/xmlreader.h"

using namespace std;

#define basenamemodulesfile "etc/modules.xml"

FuncManager::FuncManager(void)
{
	/*parse all modules directories*/
	
}

FuncManager::~FuncManager(void)
{
	map <string, ScilabFunction*>::iterator itMap;
	//clear containt of m_mapFuncList map
	for (itMap = m_mapFuncList.begin( ); itMap != m_mapFuncList.end( ); itMap++ )
		delete m_mapFuncList[itMap->first];

}

bool FuncManager::ReadModulefile()
{
	bool bOK							= false;
/*
	char *ModulesFilename = NULL;
	char *SciPath					= NULL;

	SciPath=getSCIpath();
	if (SciPath==NULL)
	{
		sciprint(_("The SCI environment variable is not set.\n"));
		return FALSE;
	}

	ModulesFilename=(char*)MALLOC((strlen(SciPath)+strlen("/")+strlen(basenamemodulesfile)+1)*sizeof(char));
	sprintf(ModulesFilename,"%s/%s",SciPath,basenamemodulesfile);
	FREE(SciPath);
	SciPath=NULL;
	
	if (FileExist(ModulesFilename))
	{
		AppendModules(ModulesFilename);
		FREE(ModulesFilename);
		ModulesFilename=NULL;
	}
	else
	{
		sciprint(_("Cannot load the module declaration file: %s.\n"),ModulesFilename);
		FREE(ModulesFilename);
		ModulesFilename=NULL;
		return FALSE;
	}
*/
	return bOK;
}