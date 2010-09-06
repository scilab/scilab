/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.modelica.model;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElements;
import javax.xml.bind.annotation.XmlType;


/**
 * Partial tree of a structured content. Each node of a this type must not be 
 * a leaf.
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Struct", propOrder = {
    "name",
    "subnodes"
})
public final class Struct {

    @XmlElement(required = true)
    private String name;
    @XmlElement(required = true)
    private Struct.Subnodes subnodes;

    /**
	 * Default constructor.
	 */
	public Struct() { }
    
    /**
     * Gets the value of the name property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getName() {
        return name;
    }

    /**
     * Sets the value of the name property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setName(String value) {
        name = value;
    }

    /**
     * Gets the value of the subnodes property.
     * 
     * @return
     *     possible object is
     *     {@link Struct.Subnodes }
     *     
     */
    public Struct.Subnodes getSubnodes() {
        return subnodes;
    }

    /**
     * Sets the value of the subnodes property.
     * 
     * @param value
     *     allowed object is
     *     {@link Struct.Subnodes }
     *     
     */
    public void setSubnodes(Struct.Subnodes value) {
        subnodes = value;
    }


    /**
     * Wrapper around the descendant or a {@link Terminal}.  
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "", propOrder = {
        "terminalOrStruct"
    })
    public static class Subnodes {

        @XmlElements({
            @XmlElement(name = "struct", type = Struct.class),
            @XmlElement(name = "terminal", type = Terminal.class)
        })
        private List<Object> terminalOrStruct;

        /**
		 * Default constructor.
		 */
		public Subnodes() { }
        
        /**
         * Gets the value of the terminalOrStruct property.
         * 
         * <p>
         * This accessor method returns a reference to the live list,
         * not a snapshot. Therefore any modification you make to the
         * returned list will be present inside the JAXB object.
         * This is why there is not a <CODE>set</CODE> method for the terminalOrStruct property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * <pre>
         *    getTerminalOrStruct().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link Struct }
         * {@link Terminal }
         * 
         * @return the value of the terminalOrStruct property.
         */
        public List<Object> getTerminalOrStruct() {
            if (terminalOrStruct == null) {
                terminalOrStruct = new ArrayList<Object>();
            }
            return terminalOrStruct;
        }

    }
    
    /*
     * Overriden methods
     */
    
    /**
     * @return the name
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
    	return getName();
    }

}
