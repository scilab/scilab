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

package org.scilab.modules.helptools.external;

import org.xml.sax.Attributes;
import org.xml.sax.Locator;
import org.scilab.modules.helptools.DocbookTagConverter;

public abstract class ExternalXMLHandler {

    private DocbookTagConverter converter;
    protected int compt = 1;

    public abstract StringBuilder startExternalXML(String localName, Attributes attributes, Locator locator);

    public abstract String endExternalXML(String localName);

    public abstract String getURI();

    public final DocbookTagConverter getConverter() {
        return converter;
    }

    public final void setConverter(DocbookTagConverter converter) {
        this.converter = converter;
    }

    public final void resetCompt() {
        compt = 1;
    }

    public String getScilabURI() {
        return "http://www.scilab.org";
    }

    public void recreateTag(StringBuilder buf, String localName, Attributes attrs) {
        if (attrs != null) {
            buf.append("<");
            buf.append(localName);
            int len = attrs.getLength();
            for (int i = 0; i < len; i++) {
                String at = attrs.getLocalName(i);
                if (at != null && at.length() > 0) {
                    buf.append(" ");
                    buf.append(at);
                    buf.append("=\'");
                    buf.append(attrs.getValue(i));
                    buf.append("\'");
                }
            }
        } else {
            buf.append("</");
            buf.append(localName);
        }

        buf.append(">");
    }

    protected static final Boolean getLocalized(final String URI, final Attributes attributes) {
        String v = URI == null ? attributes.getValue("localized") : attributes.getValue(URI, "localized");
        if (v == null || v.isEmpty()) {
            return Boolean.FALSE;
        } else if ("true".equalsIgnoreCase(v)) {
            return Boolean.TRUE;
        } else if ("false".equalsIgnoreCase(v)) {
            return null;
        } else {
            return Boolean.FALSE;
        }
    }
}