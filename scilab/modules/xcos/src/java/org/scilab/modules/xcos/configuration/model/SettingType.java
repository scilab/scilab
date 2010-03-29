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
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;

/**
 * 
 * Global configuration.
 * 
 * 
 * <p>
 * Java class for SettingType complex type.
 * 
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 * 
 * <pre>
 * &lt;complexType name="SettingType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="windows" type="{http://www.scilab.org/2010/XcosConfiguration}WindowsManagementType"/>
 *         &lt;element name="recentFiles" type="{http://www.scilab.org/2010/XcosConfiguration}RecentFilesType"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlRootElement(name = "settings")
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "SettingType", propOrder = { "windows", "recentFiles" })
public class SettingType {

	@XmlElement(required = true)
	private WindowsManagementType windows;
	@XmlElement(required = true)
	private RecentFilesType recentFiles;

	/**
	 * Default constructor
	 */
	protected SettingType() {
	}

	/**
	 * Gets the value of the windows property.
	 * 
	 * @return possible object is {@link WindowsManagementType }
	 * 
	 */
	public WindowsManagementType getWindows() {
		return windows;
	}

	/**
	 * Sets the value of the windows property.
	 * 
	 * @param value
	 *            allowed object is {@link WindowsManagementType }
	 * 
	 */
	public void setWindows(WindowsManagementType value) {
		this.windows = value;
	}

	/**
	 * Gets the value of the recentFiles property.
	 * 
	 * @return possible object is {@link RecentFilesType }
	 * 
	 */
	public RecentFilesType getRecentFiles() {
		return recentFiles;
	}

	/**
	 * Sets the value of the recentFiles property.
	 * 
	 * @param value
	 *            allowed object is {@link RecentFilesType }
	 * 
	 */
	public void setRecentFiles(RecentFilesType value) {
		this.recentFiles = value;
	}

}
