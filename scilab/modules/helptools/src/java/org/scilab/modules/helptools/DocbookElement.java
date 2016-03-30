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

public class DocbookElement {

    private Map<String, String> attributes;
    private String name;
    private StringBuilder contents;
    private String uri;

    /**
     * Default constructor
     * @param name the tag name
     * @param attributes the attributes and its values
     */
    protected DocbookElement(String name, String uri, Map<String, String> attributes) {
        this.name = name;
        this.attributes = attributes;
        this.uri = uri;
        contents = new StringBuilder();
    }

    /**
     * @return a new DocbookElement
     */
    public DocbookElement getNewInstance(String name, String uri, Map<String, String> attributes) {
        return new DocbookElement(name, uri, attributes);
    }

    /**
     * @return the tag name
     */
    public String getName() {
        return name;
    }

    /**
     * @return the tag uri
     */
    public String getURI() {
        return uri;
    }

    /**
     * @return the attributes
     */
    public Map<String, String> getAttributes() {
        return attributes;
    }

    /**
     * @return the buffer used to add contents of the tag
     */
    public StringBuilder getStringBuilder() {
        return contents;
    }

    /**
     * @param the buffer to use
     */
    public void setStringBuilder(StringBuilder buf) {
        contents = buf;
    }

    /**
     * This method can be used to store other things...
     * and it should be override since it does nothing...
     * I created it to handle the case where the converter has two String (or more)
     * to generate with the same data.
     * @param obj the Object to append.
     */
    public void append(Object obj) { }

    /**
     * This method can be used to get what it has been stored with append
     * @return an object
     */
    public Object get() {
        return null;
    }

    /**
     * Add a parent to retrieve its contents
     * @param elem the parent
     */
    public void setParent(DocbookElement elem) { }

    /**
     * @return the parent element
     */
    public DocbookElement getParent() {
        return null;
    }

    @Override
    public String toString() {
        return "<" + name + ">:" + attributes;
    }
}
