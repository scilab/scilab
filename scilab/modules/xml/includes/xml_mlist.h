/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __XML_MLIST_H__
#define __XML_MLIST_H__

#define XMLDOCUMENT 1
#define XMLELEMENT 2
#define XMLATTRIBUTE 3
#define XMLNAMESPACE 4
#define XMLLIST 5
#define XMLNOTHANDLED 6
#define XMLSET 7
#define XMLVALID 8

#include "dynlib_xml_scilab.h"

/**
 * Creates an XML object at a given position in the stack
 * @param type the object type
 * @param pos the position in the stack
 * @param id the object id
 * @return 1 if all is ok else 0
 */
XML_SCILAB_IMPEXP int createXMLObjectAtPos(int type, int pos, int id, void *pvApiCtx);

/**
 * Creates an XML object at a given position in a list
 * @param list the list addr
 * @param stackpos the position of the list in the stack
 * @param type the object type
 * @param pos the position in the list
 * @param id the object id
 * @return 1 if all is ok else 0
 */
XML_SCILAB_IMPEXP int createXMLObjectAtPosInList(int *list, int stackPos, int type, int pos, int id, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLDoc(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLElem(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLAttr(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLNs(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLList(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLObject(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLObjects(const int * types, int nb, int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLNotHandled(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLSet(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
XML_SCILAB_IMPEXP int isXMLValid(int *mlist, void *pvApiCtx);

/**
 * Get the XMLObject field _id
 * @param mlist the mlist address
 * @return the object id.
 */
XML_SCILAB_IMPEXP int getXMLObjectId(int *mlist, void *pvApiCtx);

#endif /* __XML_MLIST_H__ */
