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

package org.scilab.modules.helptools.external;

import org.xml.sax.Attributes;
import org.xml.sax.Locator;

import org.scilab.modules.helptools.DocbookTagConverter;

public abstract class ExternalXMLHandler {

    private DocbookTagConverter converter;

    public abstract StringBuilder startExternalXML(String localName, Attributes attributes, Locator locator);

    public abstract String endExternalXML(String localName);

    public abstract String getURI();

    public DocbookTagConverter getConverter() {
        return converter;
    }

    public void setConverter(DocbookTagConverter converter) {
        this.converter = converter;
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
}