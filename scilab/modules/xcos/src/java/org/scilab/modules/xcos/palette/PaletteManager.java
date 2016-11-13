/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.palette;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.io.File;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;
import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
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
    /**
     * Model class package
     */
    public static final String MODEL_CLASS_PACKAGE = "org.scilab.modules.xcos.palette.model";

    private static final String UNABLE_TO_VALIDATE_CONFIG = "Unable to validate the configuration file.\n";
    private static final String SCHEMA_FILENAME = "/PaletteConfiguration.xsd";
    private static final String INSTANCE_FILENAME = "/palettes.xml";

    private static final Logger LOG = Logger.getLogger(PaletteManager.class.getName());

    private static PaletteManager instance;
    private static Marshaller marshaller;
    private static Unmarshaller unmarshaller;

    private Category root;
    private final PropertyChangeSupport pcs;

    /** Default constructor */
    private PaletteManager() {
        pcs = new PropertyChangeSupport(this);
    }

    /**
     * @param root
     *            the root to set
     */
    public void setRoot(final Category root) {
        this.root = root;
    }

    /**
     * @return the root
     */
    public Category getRoot() {
        return root;
    }

    /**
     * Reports a boolean bound property update to listeners that have been
     * registered to track updates of all properties or a property with the
     * specified name.
     * <p>
     * No event is fired if old and new values are equal.
     * <p>
     * This is merely a convenience wrapper around the more general
     * {@link #firePropertyChange(String, Object, Object)} method.
     *
     * @param propertyName
     *            the programmatic name of the property that was changed
     * @param oldValue
     *            the old value of the property
     * @param newValue
     *            the new value of the property
     */
    public void firePropertyChange(String propertyName, boolean oldValue, boolean newValue) {
        pcs.firePropertyChange(propertyName, oldValue, newValue);
    }

    /**
     * Add a PropertyChangeListener to the listener list.
     *
     * @param listener
     *            the listener
     */
    public void addPropertyChangeListener(final PropertyChangeListener listener) {
        pcs.addPropertyChangeListener(listener);
    }

    /**
     * Add a PropertyChangeListener from the listener list.
     *
     * @param listener
     *            the listener
     */
    public void removePropertyChangeListener(final PropertyChangeListener listener) {
        pcs.removePropertyChangeListener(listener);
    }

    /** @return the default instance */
    public static synchronized PaletteManager getInstance() {
        if (instance == null) {
            instance = new PaletteManager();
            instance.loadConfig();
        }
        return instance;
    }

    /** @return true if the palette window is visible, false otherwise */
    public static boolean isVisible() {
        return ScilabTabFactory.getInstance().getFromCache(PaletteManagerView.DEFAULT_TAB_UUID) != null;
    }

    /**
     * Set visible or hide the palette.
     *
     * @param new true to set visible, false to hide.
     */
    public static void setVisible(final boolean newValue) {
        final PaletteManagerView tab = PaletteManagerView.get();
        final boolean oldValue = isVisible();

        if (newValue && tab == null) {
            final boolean view = WindowsConfigurationManager.restoreUUID(PaletteManagerView.DEFAULT_TAB_UUID);
            if (!view) {
                PaletteManagerView.restore(null, true);
            }
        } else if (oldValue) {
            ClosingOperationsManager.startClosingOperation((SwingScilabDockablePanel) PaletteManagerView.get());
        }

        getInstance().firePropertyChange("visible", oldValue, newValue);
    }

    /**
     * Load the palette configuration file on {@link #root}.
     */
    private void loadConfig() {
        try {
            if (unmarshaller == null) {
                initUnmarshaller();
            }

            File f;
            try {
                f = new File(ScilabConstants.SCIHOME.getAbsoluteFile() + INSTANCE_FILENAME);

                if (!f.exists()) {
                    final File base = new File(ScilabConstants.SCI.getAbsoluteFile() + XcosConstants.XCOS_ETC + INSTANCE_FILENAME);
                    FileUtils.forceCopy(base, f);
                }

                setRoot((Category) unmarshaller.unmarshal(f));
            } catch (final JAXBException e) {
                LOG.warning("user palette configuration file is not valid.\n" + "Switching to the default one." + e);

                ScilabModalDialog.show(PaletteManagerView.get(), XcosMessages.ERR_CONFIG_PALETTE_INVALID, XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);

                try {
                    f = new File(ScilabConstants.SCI.getAbsoluteFile() + XcosConstants.XCOS_ETC + INSTANCE_FILENAME);
                    setRoot((Category) unmarshaller.unmarshal(f));
                } catch (final JAXBException ex) {
                    LOG.severe("base palette configuration file corrupted.\n" + e);
                    return;
                }
            }

        } catch (final JAXBException e) {
            e.printStackTrace();
            return;
        }
    }

    /**
     * Initialize the shared unmarshaller instance
     *
     * @throws JAXBException
     *             when an unsupported error has occurred
     */
    private void initUnmarshaller() throws JAXBException {
        final String schemaPath = ScilabConstants.SCI.getAbsolutePath() + XcosConstants.XCOS_ETC + SCHEMA_FILENAME;

        final JAXBContext jaxbContext = JAXBContext.newInstance(MODEL_CLASS_PACKAGE);
        unmarshaller = jaxbContext.createUnmarshaller();

        try {
            Schema schema;
            schema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(new File(schemaPath));
            unmarshaller.setSchema(schema);
        } catch (final SAXException e) {
            LOG.severe(UNABLE_TO_VALIDATE_CONFIG + e);
        }
    }

    /**
     * Save the configuration file.
     */
    public void saveConfig() {
        try {
            if (marshaller == null) {
                initMarshaller();
            }

            File f;
            try {
                f = new File(ScilabConstants.SCIHOME.getAbsoluteFile() + INSTANCE_FILENAME);
                marshaller.marshal(getRoot(), f);
            } catch (final JAXBException e) {
                LOG.warning("Unable to save user palette configuration file.\n" + e);
            }

        } catch (final JAXBException e) {
            LOG.severe(e.toString());
            return;
        }
    }

    /**
     * Initialize the shared marshaller instance
     *
     * @throws JAXBException
     *             when an unsupported error has occurred
     */
    private void initMarshaller() throws JAXBException {
        final String schemaPath = ScilabConstants.SCI.getAbsolutePath() + XcosConstants.XCOS_ETC + SCHEMA_FILENAME;

        final JAXBContext jaxbContext = JAXBContext.newInstance(MODEL_CLASS_PACKAGE);
        marshaller = jaxbContext.createMarshaller();

        try {
            Schema schema;
            schema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(new File(schemaPath));
            marshaller.setSchema(schema);
        } catch (final SAXException e) {
            LOG.warning(UNABLE_TO_VALIDATE_CONFIG + e);
        }

        marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
    }

    /**
     * For debugging purpose
     *
     * @param args
     *            Non used
     */
    public static void main(final String[] args) {
        InterpreterManagement.requestScilabExec("");
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                setVisible(true);
            }
        });
    }
}
