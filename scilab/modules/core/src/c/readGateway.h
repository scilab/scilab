/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __READGATEWAY_H__
#define __READGATEWAY_H__

struct gateway_struct
{
	char **primitivesList;
	int *gatewayIdList;
	int *primiviteIdList;
	int dimLists;
};

/**
* Read gateway (xml file) associated to modulename
* and return gateway info 
* @param[in] module name
* @return gateway_struct structure
*/
struct gateway_struct *readGateway(char *modulename);

#endif /* __READGATEWAY_H__ */
/*--------------------------------------------------------------------------*/