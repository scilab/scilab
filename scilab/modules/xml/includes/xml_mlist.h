/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

int createXMLObjectAtPos(int type, int pos, int id);
int createXMLObjectAtPosInList(int * list, int stackPos, int type, int pos, int id);
int isXMLDoc(int * mlist);
int isXMLElem(int * mlist);
int isXMLAttr(int * mlist);
int isXMLNs(int * mlist);
int isXMLList(int * mlist);
int isXMLObject(int * mlist);
int isXMLNotHandled(int * mlist);
int getXMLObjectId(int * mlist);

#endif /* __XML_MLIST_H__ */
