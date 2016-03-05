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

#ifndef __XMLREMOVABLE_HXX__
#define __XMLREMOVABLE_HXX__

namespace org_modules_xml
{
/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Pure virtual class for XMLElement and XMLList
 */
class XMLRemovable
{

public:

    /**
     * Remove this object or this list of objects from the document
     */
    virtual void remove() const = 0;
};
}

#endif
