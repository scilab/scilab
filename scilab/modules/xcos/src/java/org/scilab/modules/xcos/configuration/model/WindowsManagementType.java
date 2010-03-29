/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.configuration.model;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * 
 * Window position and size.
 * 
 * 
 * <p>
 * Java class for WindowsManagementType complex type.
 * 
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 * 
 * <pre>
 * &lt;complexType name="WindowsManagementType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="palette" type="{http://www.scilab.org/2010/XcosConfiguration}PositionType"/>
 *         &lt;element name="diagram" type="{http://www.scilab.org/2010/XcosConfiguration}PositionType"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "WindowsManagementType", propOrder = { "palette", "diagram" })
public class WindowsManagementType {

	@XmlElement(required = true)
	private PositionType palette;
	@XmlElement(required = true)
	private PositionType diagram;

	/**
	 * Default constructor
	 */
	protected WindowsManagementType() {
	};

	/**
	 * Gets the value of the palette property.
	 * 
	 * @return possible object is {@link PositionType }
	 * 
	 */
	public PositionType getPalette() {
		return palette;
	}

	/**
	 * Sets the value of the palette property.
	 * 
	 * @param value
	 *            allowed object is {@link PositionType }
	 * 
	 */
	public void setPalette(PositionType value) {
		this.palette = value;
	}

	/**
	 * Gets the value of the diagram property.
	 * 
	 * @return possible object is {@link PositionType }
	 * 
	 */
	public PositionType getDiagram() {
		return diagram;
	}

	/**
	 * Sets the value of the diagram property.
	 * 
	 * @param value
	 *            allowed object is {@link PositionType }
	 * 
	 */
	public void setDiagram(PositionType value) {
		this.diagram = value;
	}

}
