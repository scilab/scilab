/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.configuration;

import java.awt.GraphicsEnvironment;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Arrays;
import java.util.GregorianCalendar;
import java.util.Iterator;
import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.stream.Stream;

import javax.swing.JFileChooser;
import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.datatype.DatatypeConfigurationException;
import javax.xml.datatype.DatatypeFactory;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.configuration.model.DocumentType;
import org.scilab.modules.xcos.configuration.model.ObjectFactory;
import org.scilab.modules.xcos.configuration.model.SettingType;
import org.scilab.modules.xcos.configuration.utils.ConfigurationConstants;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.preferences.XcosOptions;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.xml.sax.SAXException;
import org.scilab.modules.gui.filechooser.FileChooser;

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
                f = new File(ScilabConstants.SCIHOME.getAbsoluteFile() + INSTANCE_FILENAME);

                if (!f.exists()) {
                    File base = new File(ScilabConstants.SCI.getAbsoluteFile() + XcosConstants.XCOS_ETC + INSTANCE_FILENAME);
                    FileUtils.forceCopy(base, f);
                }

                final JAXBElement<SettingType> config = unmarshaller.unmarshal(new StreamSource(f), SettingType.class);
                return config.getValue();
            } catch (JAXBException e) {
                if (GraphicsEnvironment.isHeadless()) {
                    Logger.getLogger(ConfigurationManager.class.getName()).warning("user configuration file is not valid.\n" + "Switching to the default one." + e);
                } else {
                    ScilabModalDialog.show(null, XcosMessages.ERR_CONFIG_INVALID, XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
                }

                try {
                    f = new File(ScilabConstants.SCI.getAbsoluteFile() + XcosConstants.XCOS_ETC + INSTANCE_FILENAME);
                    final JAXBElement<SettingType> config = unmarshaller.unmarshal(new StreamSource(f), SettingType.class);
                    return config.getValue();
                } catch (JAXBException ex) {
                    Logger.getLogger(ConfigurationManager.class.getName()).severe("base configuration file corrupted.\n" + ex);
                }
            }

        } catch (JAXBException e) {
            e.printStackTrace();
        }

        // in case of error, create an empty configuration
        return new org.scilab.modules.xcos.configuration.model.ObjectFactory().createSettingType();
    }

    /**
     * Initialize the shared unmarshaller instance
     *
     * @throws JAXBException
     *             when an unsupported error has occurred
     */
    private void initUnmarshaller() throws JAXBException {
        final String schemaPath = ScilabConstants.SCI.getAbsolutePath() + XcosConstants.XCOS_ETC + SCHEMA_FILENAME;

        JAXBContext jaxbContext = JAXBContext.newInstance(MODEL_CLASS_PACKAGE);
        unmarshaller = jaxbContext.createUnmarshaller();

        try {
            Schema schema;
            schema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(new File(schemaPath));
            unmarshaller.setSchema(schema);
        } catch (SAXException e) {
            Logger.getLogger(ConfigurationManager.class.getName()).severe(UNABLE_TO_VALIDATE_CONFIG + e);
        }
    }

    /**
     * Save settings on the configuration file.
     */
    public void saveConfig() {
        try {
            if (marshaller == null) {
                initMarshaller();
            }

            File f;
            try {
                f = new File(ScilabConstants.SCIHOME.getAbsoluteFile() + INSTANCE_FILENAME);
                marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
                marshaller.marshal(new ObjectFactory().createSettings(getSettings()), f);
            } catch (JAXBException e) {
                Logger.getLogger(ConfigurationManager.class.getName()).warning("Unable to save user configuration file.\n" + e);
            }

        } catch (JAXBException e) {
            e.printStackTrace();
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

        JAXBContext jaxbContext = JAXBContext.newInstance(MODEL_CLASS_PACKAGE);
        marshaller = jaxbContext.createMarshaller();

        try {
            Schema schema;
            schema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(new File(schemaPath));
            marshaller.setSchema(schema);
        } catch (SAXException e) {
            Logger.getLogger(ConfigurationManager.class.getName()).warning(UNABLE_TO_VALIDATE_CONFIG + e);
        }
    }

    /**
     * Update the configuration by adding a file.
     *
     * This method doesn't perform the save.
     *
     * @param string
     *            the file path to add
     */
    public void addToRecentFiles(File string) {
        List<DocumentType> files = getSettings().getRecent();

        /*
         * Update recently opened file list so as it match the Xcos preferences
         */
        final int numberOfRecentlyOpen = XcosOptions.getPreferences().getNumberOfRecentlyOpen();
        final int diffRecentlyOpen = files.size() - numberOfRecentlyOpen;
        for (int i = 0; i < diffRecentlyOpen; i++) {
            files.remove(files.size() - 1);
        }

        /*
         * Create the url
         */
        String url;
        try {
            url = string.toURI().toURL().toExternalForm();
        } catch (MalformedURLException e1) {
            Logger.getLogger(ConfigurationManager.class.getName()).severe(e1.toString());
            return;
        }

        /*
         * Create the date
         */
        DatatypeFactory factory;
        try {
            factory = DatatypeFactory.newInstance();
        } catch (DatatypeConfigurationException e) {
            Logger.getLogger(ConfigurationManager.class.getName()).severe(e.toString());
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
        int search = Arrays.binarySearch(perNameSortedFiles, element, ConfigurationConstants.FILENAME_COMPARATOR);

        if (search >= 0) {
            // Element found, remove the old element
            oldElement = perNameSortedFiles[search];
            files.remove(oldElement);
        } else {
            // Element not found, remove the last element if
            // there is no more place.
            if ((files.size() == numberOfRecentlyOpen) && (files.size() > 0)) {
                oldElement = files.remove(files.size() - 1);
            }
        }

        files.add(0, element);

        /*
         * Fire the associated event
         */
        firePropertyChange(ConfigurationConstants.RECENT_FILES_CHANGED, oldElement, element);
    }

    /**
     * Add the graph to the recent tab file saved.
     *
     * @param graph
     *            the graph configuration to store
     */
    public void addToRecentTabs(XcosDiagram graph) {
        final DocumentType doc = allocate(graph);

        /*
         * Store the file name
         */
        if (graph.getSavedFile() != null) {
            try {
                doc.setUrl(graph.getSavedFile().toURI().toURL().toExternalForm());
            } catch (MalformedURLException e) {
            }
        }

        /*
         * Store the internal path if applicable
         */
        savePath(graph, doc);

        /*
         * Store uuids
         */
        doc.setUuid(graph.getGraphTab());
        doc.setViewport(graph.getViewPortTab());
    }

    /**
     * @return a Stream of all existing Document
     */
    public Stream<DocumentType> streamTab() {
        return getSettings().getTab().stream();
    }

    private DocumentType allocate(XcosDiagram graph) {
        final List<DocumentType> tabs = getSettings().getTab();
        DocumentType doc = null;

        for (DocumentType d : tabs) {
            if (d.getUuid().equals(graph.getGraphTab())) {
                doc = d;
                break;
            }
        }

        if (doc == null) {
            doc = new ObjectFactory().createDocumentType();
            tabs.add(doc);
        }

        return doc;
    }

    /**
     * Remove an uuid from the last opened tab list
     *
     * @param uuid
     *            the uuid (xcos or viewport)
     */
    public void removeFromRecentTabs(final String uuid) {
        final List<DocumentType> tabs = getSettings().getTab();

        boolean isTab = false;
        boolean isViewport = false;

        DocumentType doc = null;
        for (DocumentType d : tabs) {
            isTab = uuid.equals(d.getUuid());
            isViewport = uuid.equals(d.getViewport());

            if (isTab || isViewport) {
                doc = d;
                break;
            }
        }

        /*
         * Removing it
         */
        if (isTab) {
            doc.setUuid(null);
        } else if (isViewport) {
            doc.setViewport(null);
        }

        /*
         * Clear the doc if empty
         */
        if (doc != null && doc.getUuid() == null) {
            tabs.remove(doc);
        }
    }

    /**
     * Load the diagram according to the doc
     *
     * @param doc
     *            the document to load
     * @return the loaded diagram or null on error
     */
    public XcosDiagram loadDiagram(JavaController controller, DocumentType doc) {

        File f = getFile(doc);
        if (f != null && !f.exists()) {
            f = null;
        }

        // check if not already loaded by another restore
        final Optional<ScicosObjectOwner> root = Xcos.getInstance().openedDiagram(controller, f);
        // layer is the path if present or the root
        final Optional<ScicosObjectOwner> layer = decodePath(doc, controller, root);

        if (root.isPresent()) {
            // the diagram is already being loaded by the caller as a tab
            Optional<XcosDiagram> loaded = Xcos.getInstance().openedDiagrams(root.get()).stream()
                .filter(d -> Objects.equals(d.getGraphTab(), doc.getUuid()))
                .findFirst();
            if (loaded.isPresent())
                return loaded.get();

            // the diagram is already being loaded by the caller as layer
            loaded = Xcos.getInstance().openedDiagrams(root.get()).stream()
                .filter(d -> d.getUID() == layer.map(c -> c.getUID()).orElse((long) 0))
                .findFirst();
            if (loaded.isPresent())
                return loaded.get();   
        }

        if (root.isPresent() && root == layer) {
            // the layer is the root, create a DIAGRAM
            return createDiagram(controller, root.get(), root.get(), "", doc.getUuid());
        }

        if (root.isPresent() && layer.isPresent()) {
            // diagram is loaded but not the layer
            String[] blockUID = {""};
            controller.getObjectProperty(layer.get().getUID(), layer.get().getKind(), ObjectProperties.UID, blockUID);

            return createDiagram(controller, root.get(), layer.get(), blockUID[0], doc.getUuid());
        }

        if (root.isPresent()) {
            // diagram is not supposed to be loaded from there
            return null;
        }

        // load the file into a rootGraph
        ScicosObjectOwner rootOwner = new ScicosObjectOwner(controller, controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM);
        XcosDiagram rootGraph = XcosDiagram.create(controller, rootOwner.getUID(), rootOwner.getKind(), "");
        rootGraph.setSavedFile(f);

        try {
            if (f != null) {
                final String filename = f.getCanonicalPath();
                final XcosFileType filetype = XcosFileType.findFileType(f);

                filetype.load(filename, rootGraph);
            }
        } catch (Exception e) {
            // the only way to spot something on window restoration is to print the stacktrace
            e.printStackTrace();
            Logger.getLogger(ConfigurationManager.class.getName()).log(Level.SEVERE, null, e);
        }

        Optional<ScicosObjectOwner> loadedLayer = decodePath(doc, controller, Optional.of(rootOwner));

        if (loadedLayer.isPresent() && loadedLayer.get() == rootOwner) {
            // the layer is the root, return the DIAGRAM
            rootGraph.installListeners();
            rootGraph.setModified(false);
    
            Xcos.getInstance().addDiagram(rootOwner, rootGraph);
            rootGraph.setGraphTab(doc.getUuid());
            return rootGraph;
        }
        
        if (loadedLayer.isPresent()) {
            // diagram is loaded but not the layer
            String[] blockUID = {""};
            controller.getObjectProperty(loadedLayer.get().getUID(), loadedLayer.get().getKind(), ObjectProperties.UID, blockUID);

            return createDiagram(controller, rootOwner, loadedLayer.get(), blockUID[0], doc.getUuid());
        }

        // failed to restore a file
        return null;
    }

    /**
     * Create a new XcosDiagram and prepare for rendering
     */
    private XcosDiagram createDiagram(JavaController controller, ScicosObjectOwner root, ScicosObjectOwner layer, String uid, String tabUUID)
    {
        XcosDiagram graph = XcosDiagram.create(controller, layer, uid);
        XcosCellFactory.insertChildren(controller, graph);
        graph.installListeners();
        graph.setModified(false);

        Xcos.getInstance().addDiagram(root, graph);
        graph.setGraphTab(tabUUID);

        return graph;
    }

    /**
     * Get the file for the document
     *
     * @param doc
     *            the doc
     * @return the associated file
     */
    public File getFile(DocumentType doc) {
        final URL url;
        final File f;
        try {
            url = URI.create(doc.getUrl()).toURL();
            f = new File(url.toURI());
        } catch (Exception e) {
            return null;
        }
        return f;
    }

    private Optional<ScicosObjectOwner> decodePath(final DocumentType doc, final JavaController controller, final Optional<ScicosObjectOwner> root) {
        final String path = doc.getPath();
        if (path == null || path.isEmpty()) {
            return root;
        }

        final String uid = doc.getPath();
        String[] blockUID = {""};
        
        // TODO is this algorithm fast enough ?
        VectorOfScicosID blocks = controller.getAll(Kind.BLOCK);
        final int len = blocks.size();
        for (int i = 0 ; i < len ; i++) {
            controller.getObjectProperty(blocks.get(i), Kind.BLOCK, ObjectProperties.UID, blockUID);
            if (uid.equals(blockUID[0])) {
                // return the layer, the root has already been stored in Xcos#diagrams
                return Optional.of(new ScicosObjectOwner(controller, blocks.get(i), Kind.BLOCK));
            }
        }

        return Optional.empty();
    }

    private void savePath(XcosDiagram graph, final DocumentType doc) {
        if (graph.getKind() != Kind.BLOCK) {
            return;
        }

        JavaController controller = new JavaController();
        String[] blockUID = {""};

        controller.getObjectProperty(graph.getUID(), Kind.BLOCK, ObjectProperties.UID, blockUID);
        doc.setPath(blockUID[0]);
    }

    /**
     * Configure the file chooser to use the Xcos current directory (path of the
     * last saved directory).
     *
     * @param fc
     *            any file chooser
     */
    public static void configureCurrentDirectory(Object fc) {
        final ConfigurationManager manager = ConfigurationManager.getInstance();
        final Iterator<DocumentType> recentFiles = manager.getSettings().getRecent().iterator();

        File lastFile = null;
        if (recentFiles.hasNext()) {
            try {
                lastFile = new File(URI.create(recentFiles.next().getUrl()));
            } catch (Exception e) {
            }
        }
        if (lastFile != null) {
            if (fc instanceof JFileChooser) {
                ((JFileChooser) fc).setCurrentDirectory(lastFile);
            } else if (fc instanceof FileChooser) {
                ((FileChooser) fc).setInitialDirectory(lastFile.getPath());                
            }
        }
    }

    /*
     * Change support methods
     */

    /**
     * Add a PropertyChangeListener for a specific property. The listener will
     * be invoked only when a call on firePropertyChange names that specific
     * property. The same listener object may be added more than once. For each
     * property, the listener will be invoked the number of times it was added
     * for that property. If <code>propertyName</code> or <code>listener</code>
     * is null, no exception is thrown and no action is taken.
     *
     * @param propertyName
     *            The name of the property to listen on.
     * @param listener
     *            The PropertyChangeListener to be added
     */
    public void addPropertyChangeListener(String propertyName, PropertyChangeListener listener) {
        changeSupport.addPropertyChangeListener(propertyName, listener);
    }

    /**
     * Report a bound property update to any registered listeners. No event is
     * fired if old and new are equal and non-null.
     *
     * <p>
     * This is merely a convenience wrapper around the more general
     * firePropertyChange method that takes {@code PropertyChangeEvent} value.
     *
     * @param propertyName
     *            The programmatic name of the property that was changed.
     * @param oldValue
     *            The old value of the property.
     * @param newValue
     *            The new value of the property.
     */
    public void firePropertyChange(String propertyName, Object oldValue, Object newValue) {
        changeSupport.firePropertyChange(propertyName, oldValue, newValue);
    }
}
