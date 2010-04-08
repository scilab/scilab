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

import javax.xml.bind.annotation.XmlRegistry;

/**
 * This object contains factory methods for each Java content interface and Java
 * element interface generated in the
 * org.scilab.modules.xcos.configuration.model package.
 * <p>
 * An ObjectFactory allows you to programatically construct new instances of the
 * Java representation for XML content. The Java representation of XML content
 * can consist of schema derived interfaces and classes representing the binding
 * of schema type definitions, element declarations and model groups. Factory
 * methods for each of these are provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

	/**
	 * Create a new ObjectFactory that can be used to create new instances of
	 * schema derived classes for package:
	 * org.scilab.modules.xcos.configuration.model
	 * 
	 */
	public ObjectFactory() {
	}

	/**
	 * Create an instance of {@link RecentFilesType }
	 * 
	 * @return a new instance
	 */
	public RecentFilesType createRecentFilesType() {
		return new RecentFilesType();
	}

	/**
	 * Create an instance of {@link DocumentType }
	 * 
	 * @return a new instance
	 */
	public DocumentType createDocumentType() {
		return new DocumentType();
	}

	/**
	 * Create an instance of {@link WindowsManagementType }
	 * 
	 * @return a new instance
	 */
	public WindowsManagementType createWindowsManagementType() {
		return new WindowsManagementType();
	}

	/**
	 * Create an instance of {@link PositionType }
	 * 
	 * @return a new instance
	 */
	public PositionType createPositionType() {
		return new PositionType();
	}

	/**
	 * Create an instance of {@link SettingType }
	 * 
	 * @return a new instance
	 */
	public SettingType createSettingType() {
		return new SettingType();
	}
}
