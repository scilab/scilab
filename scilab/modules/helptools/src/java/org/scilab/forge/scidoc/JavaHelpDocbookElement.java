/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.forge.scidoc;

import java.util.Map;

public class JavaHelpDocbookElement extends DocbookElement {
	
    private StringBuilder mapIdContents;
    private DocbookElement parent;
	
    /**
     * {@inheritDoc}
     */
    protected JavaHelpDocbookElement(String name, Map attributes) {
	super(name, attributes);
	mapIdContents = new StringBuilder();
    }

    /**
     * {@inheritDoc}
     */
    public DocbookElement getNewInstance(String name, Map attributes) {
	return new JavaHelpDocbookElement(name, attributes);
    }

    /**
     * {@inheritDoc}
     */
    public void append(Object obj) {
	if (obj instanceof String) {
	    mapIdContents.append((String) obj);
	} else if (obj instanceof StringBuilder) {
	    mapIdContents.append((StringBuilder) obj);
	}
    }
    
    /**
     * {@inheritDoc}
     */
    public Object get() {
	return mapIdContents;
    }

    /**
     * {@inheritDoc}
     */
    public void setParent(DocbookElement elem) {
	parent = elem;
    }

    /**
     * {@inheritDoc}
     */
    public DocbookElement getParent() {
	return parent;
    }
}
