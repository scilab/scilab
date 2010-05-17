/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette;

import java.io.File;

import javax.swing.SwingUtilities;
import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.xml.sax.SAXException;

/**
 * Main class for the palette management.
 * 
 * All the palette are described in the configuration file.
 */
public final class PaletteManager {
	private static final String UNABLE_TO_VALIDATE_CONFIG = "Unable to validate the configuration file.\n";
	private static final String MODEL_CLASS_PACKAGE = "org.scilab.modules.xcos.palette.model";
	private static final String SCHEMA_FILENAME = "/PaletteConfiguration.xsd";
	private static final String INSTANCE_FILENAME = "/palettes.xml";

	private static PaletteManager instance;
	private static Marshaller marshaller;
	private static Unmarshaller unmarshaller;
	
	private PaletteManagerView view;
	private Category root;

	/** Default constructor */
	private PaletteManager() {
	}

	/**
	 * @param view
	 *            the view to set
	 */
	public void setView(PaletteManagerView view) {
		this.view = view;
	}

	/** @return the view */
	public PaletteManagerView getView() {
		return view;
	}

	/**
	 * @param root
	 *            the root to set
	 */
	public void setRoot(Category root) {
		this.root = root;
	}

	/**
	 * @return the root
	 */
	public Category getRoot() {
		return root;
	}

	/** @return the default instance */
	public static PaletteManager getInstance() {
		if (instance == null) {
			instance = new PaletteManager();
			instance.loadConfig();
			instance.setView(new PaletteManagerView(instance));
			instance.getView().getTree().revalidate();
			instance.getView().getPanel().performStartUpLayout();
		}
		return instance;
	}

	/** @return true if the palette window is visible, false otherwise */
	public static boolean isVisible() {
		return getInstance().getView().isVisible();
	}

	/**
	 * Set visible or hide the palette.
	 * 
	 * @param status
	 *            true to set visible, false to hide.
	 */
	public static void setVisible(boolean status) {
		getInstance().getView().setVisible(status);
	}

	/**
	 * Load the palette configuration file on {@link #root}.
	 */
	public void loadConfig() {
		try {
			if (unmarshaller == null) {
				initUnmarshaller();
			}
			
			File f;
			try {
				f = new File(XcosConstants.SCIHOME.getAbsoluteFile()
						+ INSTANCE_FILENAME);
				
				if (!f.exists()) {
					File base = new File(XcosConstants.SCI.getAbsoluteFile()
							+ XcosConstants.XCOS_ETC + INSTANCE_FILENAME);
					FileUtils.forceCopy(base, f);
				}
				
				setRoot((Category) unmarshaller.unmarshal(f));
			} catch (JAXBException e) {
				LogFactory.getLog(PaletteManager.class).warn(
						"user palette configuration file is not valid.\n"
								+ "Switching to the default one."
								+ e);

				ScilabModalDialog.show(getView(),
						XcosMessages.ERR_CONFIG_PALETTE_INVALID,
						XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
				
				try {
					f = new File(XcosConstants.SCI.getAbsoluteFile()
							+ XcosConstants.XCOS_ETC + INSTANCE_FILENAME);
					setRoot((Category) unmarshaller.unmarshal(f));
				} catch (JAXBException ex) {
					LogFactory.getLog(PaletteManager.class).error(
							"base palette configuration file corrupted.\n"
							+ e);
					return;
				}
			}

		} catch (JAXBException e) {
			e.printStackTrace();
			return;
		}
	}

	/**
	 * Initialize the shared unmarshaller instance
	 * @throws JAXBException when an unsupported error has occured
	 */
	private void initUnmarshaller() throws JAXBException {
		final String schemaPath = XcosConstants.SCI.getAbsolutePath()
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
			LogFactory.getLog(PaletteManager.class).error(
					UNABLE_TO_VALIDATE_CONFIG
							+ e);
		}
	}

	/**
	 * Save {@link #root} on the configuration file.
	 */
	public void saveConfig() {
		try {
			if (marshaller == null) {
				initMarshaller();
			}

			File f;
			try {
				f = new File(XcosConstants.SCIHOME.getAbsoluteFile()
						+ INSTANCE_FILENAME);
				marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
				marshaller.marshal(getRoot(), f);
			} catch (JAXBException e) {
				LogFactory.getLog(PaletteManager.class).warn(
						"Unable to save user palette configuration file.\n"
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
		final String schemaPath = XcosConstants.SCI.getAbsolutePath()
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
			LogFactory.getLog(PaletteManager.class).warn(
					UNABLE_TO_VALIDATE_CONFIG
							+ e);
		}
	}

	/**
	 * For debugging purpose
	 * 
	 * @param args
	 *            Non used
	 */
	public static void main(String[] args) {
		InterpreterManagement.requestScilabExec("");
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				setVisible(true);
			}
		});
	}
}
