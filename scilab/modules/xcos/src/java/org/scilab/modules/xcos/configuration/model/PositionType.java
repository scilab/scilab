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
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;

/**
 * 
 * Contains any position (x, y, w, h) on the screen.
 * 
 * 
 * <p>
 * Java class for PositionType complex type.
 * 
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 * 
 * <pre>
 * &lt;complexType name="PositionType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="x" type="{http://www.w3.org/2001/XMLSchema}int" />
 *       &lt;attribute name="y" type="{http://www.w3.org/2001/XMLSchema}int" />
 *       &lt;attribute name="width" type="{http://www.w3.org/2001/XMLSchema}int" />
 *       &lt;attribute name="height" type="{http://www.w3.org/2001/XMLSchema}int" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "PositionType")
public class PositionType {

	@XmlAttribute
	private Integer x;
	@XmlAttribute
	private Integer y;
	@XmlAttribute
	private Integer width;
	@XmlAttribute
	private Integer height;

	/**
	 * Default constructor
	 */
	protected PositionType() {
	}

	/**
	 * Gets the value of the x property.
	 * 
	 * @return possible object is {@link Integer }
	 * 
	 */
	public Integer getX() {
		return x;
	}

	/**
	 * Sets the value of the x property.
	 * 
	 * @param value
	 *            allowed object is {@link Integer }
	 * 
	 */
	public void setX(Integer value) {
		this.x = value;
	}

	/**
	 * Gets the value of the y property.
	 * 
	 * @return possible object is {@link Integer }
	 * 
	 */
	public Integer getY() {
		return y;
	}

	/**
	 * Sets the value of the y property.
	 * 
	 * @param value
	 *            allowed object is {@link Integer }
	 * 
	 */
	public void setY(Integer value) {
		this.y = value;
	}

	/**
	 * Gets the value of the width property.
	 * 
	 * @return possible object is {@link Integer }
	 * 
	 */
	public Integer getWidth() {
		return width;
	}

	/**
	 * Sets the value of the width property.
	 * 
	 * @param value
	 *            allowed object is {@link Integer }
	 * 
	 */
	public void setWidth(Integer value) {
		this.width = value;
	}

	/**
	 * Gets the value of the height property.
	 * 
	 * @return possible object is {@link Integer }
	 * 
	 */
	public Integer getHeight() {
		return height;
	}

	/**
	 * Sets the value of the height property.
	 * 
	 * @param value
	 *            allowed object is {@link Integer }
	 * 
	 */
	public void setHeight(Integer value) {
		this.height = value;
	}

}
