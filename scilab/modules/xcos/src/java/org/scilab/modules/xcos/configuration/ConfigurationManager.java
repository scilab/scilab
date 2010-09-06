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

package org.scilab.modules.xcos.configuration;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.io.File;
import java.net.MalformedURLException;
import java.util.Arrays;
import java.util.GregorianCalendar;
import java.util.List;

import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.datatype.DatatypeConfigurationException;
import javax.xml.datatype.DatatypeFactory;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.configuration.model.DocumentType;
import org.scilab.modules.xcos.configuration.model.ObjectFactory;
import org.scilab.modules.xcos.configuration.model.SettingType;
import org.scilab.modules.xcos.configuration.utils.ConfigurationConstants;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.xml.sax.SAXException;

/**
 * Entry point to manage the configuration
 */
public final class ConfigurationManager {
	private static final String UNABLE_TO_VALIDATE_CONFIG = "Unable to validate the configuration file.\n";
	private static final String MODEL_CLASS_PACKAGE = "org.scilab.modules.xcos.configuration.model";
	private static final String SCHEMA_FILENAME = "/XcosConfiguration.xsd";
	private static final String INSTANCE_FILENAME = "/xcos.xml";

	private static ConfigurationManager instance;
	private static Marshaller marshaller;
	private static Unmarshaller unmarshaller;

	private final SettingType settings;
	private final PropertyChangeSupport changeSupport;

	/**
	 * Default constructor
	 */
	private ConfigurationManager() {
		settings = loadConfig();
		changeSupport = new PropertyChangeSupport(this);
	}

	/**
	 * The only manager instance
	 * 
	 * @return the instance
	 */
	public static ConfigurationManager getInstance() {
		if (instance == null) {
			instance = new ConfigurationManager();
		}
		return instance;
	}

	/**
	 * @return the settings
	 */
	public SettingType getSettings() {
		return settings;
	}
	
	/**
	 * Load the configuration file and return the root object.
	 * 
	 * @return the configuration instance
	 */
	public SettingType loadConfig() {
		try {
			if (unmarshaller == null) {
				initUnmarshaller();
			}
			
			File f;
			try {
				f = new File(ScilabConstants.SCIHOME.getAbsoluteFile()
						+ INSTANCE_FILENAME);
				
				if (!f.exists()) {
					File base = new File(ScilabConstants.SCI.getAbsoluteFile()
							+ XcosConstants.XCOS_ETC + INSTANCE_FILENAME);
					FileUtils.forceCopy(base, f);
				}
				
				return (SettingType) unmarshaller.unmarshal(f);
			} catch (JAXBException e) {
				LogFactory.getLog(ConfigurationManager.class).warn(
						"user configuration file is not valid.\n"
								+ "Switching to the default one."
								+ e);

				ScilabModalDialog.show(null,
						XcosMessages.ERR_CONFIG_INVALID,
						XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
				
				try {
					f = new File(ScilabConstants.SCI.getAbsoluteFile()
							+ XcosConstants.XCOS_ETC + INSTANCE_FILENAME);
					return (SettingType) unmarshaller.unmarshal(f);
				} catch (JAXBException ex) {
					LogFactory.getLog(ConfigurationManager.class).error(
							"base configuration file corrupted.\n"
							+ ex);
					return null;
				}
			}

		} catch (JAXBException e) {
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * Initialize the shared unmarshaller instance
	 * @throws JAXBException when an unsupported error has occured
	 */
	private void initUnmarshaller() throws JAXBException {
		final String schemaPath = ScilabConstants.SCI.getAbsolutePath()
		+ XcosConstants.XCOS_ETC + SCHEMA_FILENAME;
		
		JAXBContext jaxbContext = JAXBContext
				.newInstance(MODEL_CLASS_PACKAGE);
		unmarshaller = jaxbContext.createUnmarshaller();

		try {
			Schema schema;
			schema = SchemaFactory.newInstance(
					XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(
					new File(schemaPath));
			unmarshaller.setSchema(schema);
		} catch (SAXException e) {
			LogFactory.getLog(ConfigurationManager.class).error(
					UNABLE_TO_VALIDATE_CONFIG
							+ e);
		}
	}

	/**
	 * Save {@link #settings} on the configuration file.
	 */
	public void saveConfig() {
		try {
			if (marshaller == null) {
				initMarshaller();
			}

			File f;
			try {
				f = new File(ScilabConstants.SCIHOME.getAbsoluteFile()
						+ INSTANCE_FILENAME);
				marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
				marshaller.marshal(getSettings(), f);
			} catch (JAXBException e) {
				LogFactory.getLog(ConfigurationManager.class).warn(
						"Unable to save user configuration file.\n"
						+ e);
			}

		} catch (JAXBException e) {
			e.printStackTrace();
			return;
		}
	}

	/**
	 * Initialize the shared marshaller instance
	 * @throws JAXBException when an unsupported error has occured
	 */
	private void initMarshaller() throws JAXBException {
		final String schemaPath = ScilabConstants.SCI.getAbsolutePath()
		+ XcosConstants.XCOS_ETC + SCHEMA_FILENAME;
		
		JAXBContext jaxbContext = JAXBContext
				.newInstance(MODEL_CLASS_PACKAGE);
		marshaller = jaxbContext.createMarshaller();

		try {
			Schema schema;
			schema = SchemaFactory.newInstance(
					XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(
					new File(schemaPath));
			marshaller.setSchema(schema);
		} catch (SAXException e) {
			LogFactory.getLog(ConfigurationManager.class).warn(
					UNABLE_TO_VALIDATE_CONFIG
							+ e);
		}
	}
	
	/**
	 * Update the configuration by adding a file.
	 * 
	 * This method doesn't perform the save.
	 * 
	 * @param string the file path to add
	 */
	public void addToRecentFiles(File string) {
		List<DocumentType> files = getSettings().getRecentFiles().getDocument();
		
		/*
		 * Create the url
		 */
		String url;
		try {
			url = string.toURI().toURL().toExternalForm();
		} catch (MalformedURLException e1) {
			LogFactory.getLog(ConfigurationManager.class).error(e1);
			return;
		}
		
		/*
		 * Create the date
		 */
		DatatypeFactory factory;
		try {
			factory = DatatypeFactory.newInstance();
		} catch (DatatypeConfigurationException e) {
			LogFactory.getLog(OpenAction.class).error(e);
			return;
		} 
		
		/*
		 * Initialize the new element
		 */
		DocumentType element = (new ObjectFactory()).createDocumentType();
		element.setUrl(url);
		element.setDate(factory.newXMLGregorianCalendar(new GregorianCalendar()));
		
		/*
		 * Create an arrays sorted by name.
		 */
		DocumentType[] perNameSortedFiles = files.toArray(new DocumentType[files.size()]);
		Arrays.sort(perNameSortedFiles, ConfigurationConstants.FILENAME_COMPARATOR);		
		
		/*
		 * Insert the element 
		 */
		DocumentType oldElement = null;
		int search = Arrays.binarySearch(perNameSortedFiles, element,
				ConfigurationConstants.FILENAME_COMPARATOR);
		
		if (search >= 0) {
			// Element found, remove the old element
			oldElement = perNameSortedFiles[search];
			files.remove(oldElement);
		} else {
			// Element not found, remove the last element if
			// there is no more place.
			if (files.size() == ConfigurationConstants.MAX_RECENT_FILES) {
				oldElement = files.remove(ConfigurationConstants.MAX_RECENT_FILES - 1);
			}
		}
		
		files.add(0, element);
		
		/*
		 * Fire the associated event
		 */
		firePropertyChange(ConfigurationConstants.RECENT_FILES_CHANGED,
				oldElement, element);
	}
	
	/*
	 * Change support methods
	 */
	
    /**
     * Add a PropertyChangeListener for a specific property.  The listener
     * will be invoked only when a call on firePropertyChange names that
     * specific property.
     * The same listener object may be added more than once.  For each
     * property,  the listener will be invoked the number of times it was added
     * for that property.
     * If <code>propertyName</code> or <code>listener</code> is null, no
     * exception is thrown and no action is taken.
     *
     * @param propertyName  The name of the property to listen on.
     * @param listener  The PropertyChangeListener to be added
     */
	public void addPropertyChangeListener(String propertyName, PropertyChangeListener listener) {
		changeSupport.addPropertyChangeListener(propertyName, listener);
	}
	
    /**
     * Report a bound property update to any registered listeners.
     * No event is fired if old and new are equal and non-null.
     *
     * <p>
     * This is merely a convenience wrapper around the more general
     * firePropertyChange method that takes {@code
     * PropertyChangeEvent} value.
     *
     * @param propertyName  The programmatic name of the property
     *          that was changed.
     * @param oldValue  The old value of the property.
     * @param newValue  The new value of the property.
     */
    public void firePropertyChange(String propertyName,
                                        Object oldValue, Object newValue) {
    	changeSupport.firePropertyChange(propertyName, oldValue, newValue);
    }
}
