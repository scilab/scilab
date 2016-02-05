/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.helptools;

import java.util.Map;

public class JavaHelpDocbookElement extends DocbookElement {

    private StringBuilder mapIdContents;
    private DocbookElement parent;

    /**
     * {@inheritDoc}
     */
    protected JavaHelpDocbookElement(String name, String uri, Map<String, String> attributes) {
        super(name, uri, attributes);
        mapIdContents = new StringBuilder();
    }

    /**
     * {@inheritDoc}
     */
    public DocbookElement getNewInstance(String name, String uri, Map<String, String> attributes) {
        return new JavaHelpDocbookElement(name, uri, attributes);
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
