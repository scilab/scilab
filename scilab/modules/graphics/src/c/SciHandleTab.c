/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - Digiteo - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SciHandleTab.h"
#include "HandleManagement.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
/**
 * Singleton for the Scilab hashtable
 */
static SciHandleTab * scilabHandleTab = NULL;
/*--------------------------------------------------------------------------*/
/**
 * Hash function used by the SciHashTable
 */
static unsigned int sciHashTableHash(void * hash)
{
	/* normally its a long, here we use an int */
	long * key = (long *) hash;
	return (unsigned int) *key;
}
/*--------------------------------------------------------------------------*/
/**
 * Hash table equality
 */
static int sciHashTableEqualsKeys(void * k1, void * k2)
{
	return (*((long *) k1) == *((long *) k2));
}
/*--------------------------------------------------------------------------*/
SciHandleTab * createHandleTab(void)
{
	unsigned int minsize = 16;
	return create_hashtable(minsize, sciHashTableHash, sciHashTableEqualsKeys);
}
/*--------------------------------------------------------------------------*/
void destroyHandleTab(SciHandleTab * tab)
{
	hashtable_destroy(tab, 0) ;
}
/*--------------------------------------------------------------------------*/
BOOL addHandleObjectMapping(SciHandleTab * tab, long handle, sciPointObj * pObj)
{
	/* allocate a new key because the hashtable claims ownership */
  /* and will free it when destroyed */
	long * newKey = MALLOC(sizeof(long));
	if (newKey == NULL) {return FALSE;}
	*newKey = handle;

	return hashtable_insert(tab, newKey, pObj);
}
/*--------------------------------------------------------------------------*/
sciPointObj * removeHandleObjectMapping(SciHandleTab * tab, long handle)
{
	return (sciPointObj *) hashtable_remove(tab, &handle);
}
/*--------------------------------------------------------------------------*/
sciPointObj * removeObjectMapping(SciHandleTab * tab, sciPointObj * pObj)
{
	return removeHandleObjectMapping(tab, sciGetHandle(pObj));
}
/*--------------------------------------------------------------------------*/
sciPointObj * getObjectFromHandle(SciHandleTab * tab, long handle)
{
	return (sciPointObj *) hashtable_search(tab, &handle);
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
SciHandleTab * getScilabHandleTab(void)
{
	if (scilabHandleTab == NULL)
	{
		scilabHandleTab = createHandleTab();
	}
	return scilabHandleTab;
}
/*--------------------------------------------------------------------------*/
void destroyScilabHandleTab(void)
{
	if (scilabHandleTab != NULL)
	{
		destroyHandleTab(scilabHandleTab);
		scilabHandleTab = NULL;
	}
}
/*--------------------------------------------------------------------------*/
BOOL sciAddHandleObjectMapping(long handle, sciPointObj * pObj)
{
	return addHandleObjectMapping(scilabHandleTab, handle, pObj);
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciRemoveHandleObjectMapping(long handle)
{
	return removeHandleObjectMapping(scilabHandleTab, handle);
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciRemoveObjectMapping(sciPointObj * pObj)
{
	return removeObjectMapping(scilabHandleTab, pObj);
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciGetObjectFromHandle(long handle)
{
	return getObjectFromHandle(scilabHandleTab, handle);
}
/*--------------------------------------------------------------------------*/
