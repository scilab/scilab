/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.commons.xml;

import java.awt.Color;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FilenameFilter;
import java.io.FileFilter;
import java.io.IOException;
import java.lang.annotation.Annotation;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.HashSet;
import java.util.Set;

import javax.swing.KeyStroke;
import javax.swing.event.EventListenerList;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.xml.sax.SAXException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.ScilabGeneralPrefs;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.localization.Messages;

/**
 * Class to retrieve object from the xml configuration file
 *
 * @author Calixte DENIZET
 *
 */
public class XConfiguration {

    // User configuration file
    private static final String SCI = System.getenv("SCI");
    private static final String SCILAB_CONFIG_FILE = SCI + "/modules/preferences/etc/XConfiguration.xml";

    private static final String ERROR_READ = Messages.gettext("Could not load file: ");
    private static final String ERROR_WRITE = Messages.gettext("Could not write the file: ");
    private static final String SEVERE_ERROR = Messages.gettext("A severe error occurred: cannot load the preferences file.");
    private static final String PARSING_ERROR = Messages.gettext("An error occurred when loading the preferences file, try to reload the default one.");

    private static final XPathFactory xpathFactory = ScilabXPathFactory.newInstance();
    private static final Map < Class<?>, StringParser > conv = new HashMap < Class<?>, StringParser > ();

    private static final EventListenerList listenerList = new EventListenerList();
    private static final Set<String> modifiedPaths = new HashSet<String>();

    private static Document doc;
    private static boolean hasBeenRead;
    private static boolean mustSave = true;;
    private static String USER_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/XConfiguration.xml";

    static {
        if (ScilabConstants.SCIHOME != null && ScilabConstants.SCIHOME.canRead() && ScilabConstants.SCIHOME.canWrite()) {
            USER_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/XConfiguration.xml";
        } else {
            USER_CONFIG_FILE = SCILAB_CONFIG_FILE;
            mustSave = false;
        }

        addXConfigurationListener(ScilabGeneralPrefs.getInstance());

        try {
            Class histoprefs = ClassLoader.getSystemClassLoader().loadClass("org.scilab.modules.history_manager.HistoryPrefs");
            Method getinstance = histoprefs.getDeclaredMethod("getInstance");
            addXConfigurationListener((XConfigurationListener)getinstance.invoke(null));
        } catch (ClassNotFoundException e) {
            // Nothing displayed (always occurs in MN mode)
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    /**
     * Get the document in SCIHOME corresponding to the configuration file.
     * @return the configuration document.
     */
    public static Document getXConfigurationDocument() {
        if (doc == null) {
            boolean error = false;
            File xml = new File(USER_CONFIG_FILE);
            if (!xml.exists() && mustSave) {
                ScilabXMLUtilities.writeDocument(createDocument(), USER_CONFIG_FILE);
            }

            DocumentBuilder docBuilder = null;

            if (mustSave) {
                try {
                    DocumentBuilderFactory factory = ScilabDocumentBuilderFactory.newInstance();
                    docBuilder = factory.newDocumentBuilder();
                    doc = docBuilder.parse(xml);
                    float version = getDocumentVersion(doc);
                    float defaultVersion = getDocumentVersion(getDefaultDocument());
                    if (defaultVersion != version) {
                        xml.delete();
                        doc = null;
                        return getXConfigurationDocument();
                    } else {
                        return doc;
                    }
                } catch (ParserConfigurationException pce) {
                    error = true;
                } catch (SAXException se) {
                    error = true;
                } catch (IOException ioe) {
                    error = true;
                }

                if (error) {
                    if (hasBeenRead) {
                        System.err.println(SEVERE_ERROR);
                        doc = null;
                        xml.delete();
                        return docBuilder.newDocument();
                    }

                    hasBeenRead = true;
                    doc = null;
                    xml.delete();
                    System.err.println(PARSING_ERROR);
                    return getXConfigurationDocument();
                }

                return docBuilder.newDocument();
            } else {
                doc = createDocument();
            }
        }

        return doc;
    }

    /**
     * Save the modifications
     */
    public static void writeDocument(String filename, Node written) {
        if (mustSave) {
            Transformer transformer = null;
            try {
                transformer = ScilabTransformerFactory.newInstance().newTransformer();
            } catch (TransformerConfigurationException e1) {
                System.err.println(ERROR_WRITE + filename);
                return;
            } catch (TransformerFactoryConfigurationError e1) {
                System.err.println(ERROR_WRITE + filename);
                return;
            }
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");

            StreamResult result = new StreamResult(new File(filename));
            DOMSource source = new DOMSource(written);
            try {
                transformer.transform(source, result);
            } catch (TransformerException e) {
                System.err.println(ERROR_WRITE + filename);
                return;
            }

            // Invalidate the current document
            if (filename.equals(USER_CONFIG_FILE)) {
                doc = null;
            }
        }
    }

    /**
     * Save the modifications
     */
    public static String dumpNode(Node written) {
        Transformer transformer = null;
        try {
            transformer = ScilabTransformerFactory.newInstance().newTransformer();
        } catch (TransformerConfigurationException e1) {
            System.err.println("Cannot dump xml");
            return "";
        } catch (TransformerFactoryConfigurationError e1) {
            System.err.println("Cannot dump xml");
            return "";
        }
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");

        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        StreamResult result = new StreamResult(new BufferedOutputStream(stream));
        DOMSource source = new DOMSource(written);
        String str = "";
        try {
            transformer.transform(source, result);
            str = stream.toString();
        } catch (TransformerException e) {
            System.err.println("Cannot dump xml");
            return str;
        } finally {
            try {
                stream.close();
            } catch (Exception e) { }
        }

        return str;
    }

    /**
     * Get the document version
     * @param doc the document
     * @return the version
     */
    private static float getDocumentVersion(Document doc) {
        if (doc != null) {
            Element root = doc.getDocumentElement();
            String version = root.getAttribute("version");

            try {
                return Float.parseFloat(version);
            } catch (NumberFormatException e) { }
        }

        return 0.0f;
    }

    /**
     * Get the default document
     * @return the document
     */
    private static Document getDefaultDocument() {
        DocumentBuilder docBuilder;
        DocumentBuilderFactory factory;
        Document mainDoc = null;

        try {
            factory = ScilabDocumentBuilderFactory.newInstance();
            docBuilder = factory.newDocumentBuilder();
            mainDoc = docBuilder.parse(SCILAB_CONFIG_FILE);
        } catch (ParserConfigurationException pce) {
            System.err.println("Cannot create a XML DocumentBuilder:\n" + pce);
            return null;
        } catch (SAXException se) {
            System.err.println("Weird... Cannot parse basic file:\n" + se);
            return null;
        } catch (IOException ioe) {
            System.err.println("Weird... Cannot parse basic file:\n" + ioe);
            return null;
        }

        return mainDoc;
    }

    /**
     * Create a document in using the XConfiguration-*.xml found in SCI/modules/MODULE_NAME/etc/
     * @return the built document
     */
    public static Document createDocument() {
        DocumentBuilder docBuilder;
        DocumentBuilderFactory factory;
        Document mainDoc = getDefaultDocument();
        if (mainDoc == null) {
            return null;
        }

        Element root = mainDoc.getDocumentElement();

        factory = ScilabDocumentBuilderFactory.newInstance();

        try {
            docBuilder = factory.newDocumentBuilder();
        } catch (ParserConfigurationException pce) {
            System.err.println("Cannot create a XML DocumentBuilder:\n" + pce);
            return null;
        }

        List<File> etcs = getEtcDir();
        for (File etc : etcs) {
            File[] xmls = etc.listFiles(new FilenameFilter() {
                public boolean accept(File dir, String name) {
                    return name.endsWith(".xml") && name.startsWith("XConfiguration-");
                }
            });
            for (File xml : xmls) {
                try {
                    Document doc = docBuilder.parse(xml);
                    if (xml.getName().equals("XConfiguration-general.xml")) {
                        try {
                            XPath xp = xpathFactory.newXPath();
                            NodeList nodes = (NodeList) xp.compile("//shortcuts/body/actions/action-folder/action[contains(@key, 'OSSCKEY')]").evaluate(doc, XPathConstants.NODESET);
                            for (int i = 0; i < nodes.getLength(); i++) {
                                Element e = (Element) nodes.item(i);
                                e.setAttribute("key", e.getAttribute("key").replace("OSSCKEY", ScilabKeyStroke.getOSMetaKey()));
                            }
                        } catch (XPathExpressionException e) {
                            System.err.println(e);
                        }
                    }
                    Node node = mainDoc.importNode(doc.getDocumentElement(), true);
                    NodeList list = root.getElementsByTagName(node.getNodeName());
                    if (list.getLength() != 0) {
                        root.replaceChild(node, list.item(0));
                    }
                } catch (SAXException se) {
                    System.err.println(ERROR_READ + xml.getName());
                } catch (IOException ioe) {
                    System.err.println(ERROR_READ + xml.getName());
                }
            }
        }

        return mainDoc;
    }

    /**
     * Get the list of the etc dirs in modules dirs
     * @return the lit of etc dirs
     */
    public static List<File> getEtcDir() {
        List<File> list = new ArrayList<File>();
        File modulesDir = new File(SCI + "/modules/");
        File[] modules = modulesDir.listFiles(new FileFilter() {
            public boolean accept(File f) {
                return f.isDirectory();
            }
        });

        for (File module : modules) {
            File etc = new File(module, "/etc/");
            if (etc.exists() && etc.isDirectory()) {
                list.add(etc);
            }
        }

        return list;
    }

    public static void addModifiedPath(String path) {
        if (path != null && !path.isEmpty()) {
            modifiedPaths.add(path);
        }
    }

    public static void invalidate() {
        modifiedPaths.clear();
        doc = null;
    }

    public static void addXConfigurationListener(XConfigurationListener listener) {
        listenerList.add(XConfigurationListener.class, listener);
    }

    public static void removeXConfigurationListener(XConfigurationListener listener) {
        listenerList.remove(XConfigurationListener.class, listener);
    }

    public static XConfigurationListener[] getXConfigurationListeners() {
        return listenerList.getListeners(XConfigurationListener.class);
    }

    public static void fireXConfigurationEvent() {
        if (!modifiedPaths.isEmpty()) {
            XConfigurationEvent event = null;
            Object[] listeners = listenerList.getListenerList();
            for (int i = listeners.length - 2; i >= 0; i -= 2) {
                if (listeners[i] == XConfigurationListener.class) {
                    if (event == null) {
                        event = new XConfigurationEvent(modifiedPaths);
                    }
                    ((XConfigurationListener) listeners[i + 1]).configurationChanged(event);
                }
            }

            modifiedPaths.clear();
        }
    }

    /**
     * Register a StringParser for a given Class
     * @param type the class type
     * @param parser the StringParser
     */
    public static void registerStringParser(Class<?> type, StringParser parser) {
        conv.put(type, parser);
    }

    /**
     * Get a StringParser for a given Class
     * @param type the class type
     * @return the corresponding parser
     */
    public static StringParser getStringParser(Class<?> type) {
        return conv.get(type);
    }

    public static void set(final Document doc, final String path, String value) {
        XPath xp = xpathFactory.newXPath();
        NodeList nodes;
        try {
            nodes = (NodeList) xp.compile(path).evaluate(doc, XPathConstants.NODESET);
        } catch (XPathExpressionException e) {
            System.err.println(e);
            return;
        }

        for (int i = 0; i < nodes.getLength() ; i++) {
            Node n = nodes.item(i);
            if (n != null && n.getNodeType() == Node.ATTRIBUTE_NODE) {
                n.setNodeValue(value);
            }
        }

        writeDocument(USER_CONFIG_FILE, doc);
    }

    /**
     * Save the current file
     */
    public static void save() {
        if (doc != null) {
            writeDocument(USER_CONFIG_FILE, doc);
        }
    }

    /**
     * Get all the nodes with the given path.
     * All the get nodes are serialized into an object (generic paramater) which must have
     * a constructor without argument and with methods named set&lt;Attribute Name&gt; with
     * one argument and no returned value.
     * For example a node &lt;foo aaa="1" bbb="true" ccc-ddd="#001122"/&gt; could be retrieved with something like
     * XConfiguration.get(MyObject.class, doc, "//path/to/node") where MyObject should be something like
     * <code>
     * public class MyObject {
     *
     *    public MyObject() {
     *       // ...
     *    }
     *
     *    public void setAaa(int a) {
     *       // ...
     *    }
     *
     *    public void setBbb(boolean b) {
     *       // ...
     *    }
     *
     *    public void setCccDdd(Color c) {
     *       // ...
     *    }
     * }
     * </code>
     * If an attribute must not be retrieved, just remove the setter.
     *
     * It is possible to use the annotation @XConfAttribute to make easier the retrievement.
     * For example
     * <code>
     * @XConfAttribute
     * public class MyObject {
     *
     *    public MyObject() {
     *       // ...
     *    }
     *
     *    @XConfAttribute(attributes={"aaa", "bbb", "ccc-ddd"})
     *    // the contents of aaa will be converted into int and passed as first argument
     *    // the contents of bbb will be converted into boolean and passed as second argument
     *    // the contents of ccc-ddd will be converted into Color and passed as third argument
     *    public void set(int n, boolean b, Color c) {
     *       // ...
     *    }
     *  }
     * </code>
     *
     * @param type the Class type to retrieve
     * @param doc the document to explore
     * @param path the xpath query to retrieve the corresponding nodeset.
     * @return an array of instance of class type.
     */
    public static final <T> T[] get(final Class<T> type, final Document doc, final String path) {
        XPath xp = xpathFactory.newXPath();
        NodeList nodes;
        try {
            nodes = (NodeList) xp.compile(path).evaluate(doc, XPathConstants.NODESET);
        } catch (XPathExpressionException e) {
            System.err.println(e);
            return (T[]) Array.newInstance(type, 0);
        }

        if (type.getAnnotation(XConfAttribute.class) != null) {
            T[] arr = get(type, nodes);
            if (arr != null) {
                return arr;
            }
        }

        Method[] meths = type.getDeclaredMethods();
        Map<String, Method> mapMethods = new HashMap<String, Method>();
        for (Method m : meths) {
            String name = m.getName();
            if (name.startsWith("set") && m.getParameterTypes().length == 1 && m.getReturnType().equals(Void.TYPE)) {
                mapMethods.put(m.getName(), m);
                m.setAccessible(true);
            }
        }

        Map<String, String> names = new HashMap<String, String>();

        T[] values = (T[]) Array.newInstance(type, nodes.getLength());
        for (int i = 0; i < values.length; i++) {
            NamedNodeMap map = nodes.item(i).getAttributes();
            try {
                Constructor<T> constructor = type.getDeclaredConstructor(new Class[] {});
                constructor.setAccessible(true);
                values[i] = constructor.newInstance();
            } catch (InstantiationException e) {
                System.err.println(e);
                break;
            } catch (IllegalAccessException e) {
                System.err.println(e);
                break;
            } catch (NoSuchMethodException e) {
                System.err.println(e);
                break;
            } catch (InvocationTargetException e) {
                System.err.println(e.getTargetException());
            }

            for (int j = 0; j < map.getLength(); j++) {
                Node n = map.item(j);
                String name = n.getNodeName();
                String methName = names.get(name);
                if (methName == null) {
                    StringBuilder buffer = new StringBuilder("set");
                    String[] parts = name.split("-");
                    for (String part : parts) {
                        if (part != null && part.length() > 0) {
                            buffer.append(part.substring(0, 1).toUpperCase());
                            buffer.append(part.substring(1).toLowerCase());
                        }
                    }
                    methName = buffer.toString();
                    names.put(name, methName);
                }
                String value = n.getNodeValue();
                Method method = mapMethods.get(methName);
                if (method != null) {
                    Class[] paramsClass = method.getParameterTypes();
                    StringParser parser = conv.get(paramsClass[0]);
                    if (parser != null) {
                        Object[] params = new Object[] {parser.parse(value)};
                        try {
                            method.invoke(values[i], params);
                        } catch (IllegalAccessException e) {
                            System.err.println(e);
                        } catch (IllegalArgumentException e) {
                            System.err.println(e);
                        } catch (InvocationTargetException e) {
                            System.err.println(e.getTargetException());
                        }
                    }
                }
            }
        }

        return values;
    }

    /**
     * Get a Map with where the key is the converted value (according to keyType) of the attribute named key
     * and the value is given in the same way.
     * @param doc the document to read
     * @param key the attribute name where its value will be converted and used as a key in the map
     * @param keyType the Class of the key
     * @param value the attribute name where its value will be converted and used as a value in the map
     * @param valueType the Class of the value
     * @return the corresponding map.
     */
    public static final <T, U> Map<T, U> get(final Document doc, final String key, final Class<T> keyType, final String value, final Class<U> valueType, final String path) {
        XPath xp = xpathFactory.newXPath();
        Map<T, U> map = new HashMap<T, U>();
        NodeList nodes;
        try {
            nodes = (NodeList) xp.compile(path).evaluate(doc, XPathConstants.NODESET);
        } catch (XPathExpressionException e) {
            System.err.println(e);
            return map;
        }

        int len = nodes.getLength();
        for (int i = 0; i < len; i++) {
            NamedNodeMap nmap = nodes.item(i).getAttributes();
            Node k = nmap.getNamedItem(key);
            Node v = nmap.getNamedItem(value);
            if (k == null || v == null) {
                return map;
            }

            String kk = k.getNodeValue();
            String vv = v.getNodeValue();

            StringParser convK = conv.get(keyType);
            StringParser convV = conv.get(valueType);
            if (convK == null || convV == null) {
                return map;
            }

            map.put((T) convK.parse(kk), (U) convV.parse(vv));
        }

        return map;
    }

    /**
     * Getter for annoted class (with @XConfAttribute)
     * @param type the class type
     * @param nodes the nodes to read
     * @return an array of instances of type, if the class is annoted with @XConfAttribute(isStatic=true),
     * the returned array is empty.
     */
    private static final <T> T[] get(final Class<T> type, NodeList nodes) {
        Method[] meths = type.getDeclaredMethods();
        List<String[]> attrs = new ArrayList<String[]>();
        List<Method> methods = new ArrayList<Method>();
        Map<String[], Method> mapMethods = new HashMap<String[], Method>();
        for (Method m : meths) {
            String name = m.getName();
            Annotation ann = m.getAnnotation(XConfAttribute.class);
            if (ann != null) {
                String[] attributes = ((XConfAttribute) ann).attributes();
                if (attributes.length == m.getParameterTypes().length) {
                    m.setAccessible(true);
                    attrs.add(attributes);
                    methods.add(m);
                } else {
                    return null;
                }
            }
        }

        Annotation ann = type.getAnnotation(XConfAttribute.class);
        boolean mustInstantiate = !((XConfAttribute) ann).isStatic();

        T[] values = null;
        int len = nodes.getLength();
        if (mustInstantiate) {
            values = (T[]) Array.newInstance(type, len);
        }

        for (int i = 0; i < len; i++) {
            NamedNodeMap map = nodes.item(i).getAttributes();
            String nodeName = nodes.item(i).getNodeName();

            if (mustInstantiate) {
                try {
                    Constructor<T> constructor = type.getDeclaredConstructor(new Class[] {});
                    constructor.setAccessible(true);
                    values[i] = constructor.newInstance();
                } catch (InstantiationException e) {
                    System.err.println(e);
                    break;
                } catch (IllegalAccessException e) {
                    System.err.println(e);
                    break;
                } catch (NoSuchMethodException e) {
                    System.err.println(e);
                    break;
                } catch (InvocationTargetException e) {
                    System.err.println(e.getTargetException());
                }
            }

            for (int j = 0; j < methods.size(); j++) {
                Method method = methods.get(j);
                ann = method.getAnnotation(XConfAttribute.class);
                String tag = ((XConfAttribute) ann).tag();
                if (tag.isEmpty() || tag.equals(nodeName)) {
                    String[] attributes = attrs.get(j);
                    Object[] params = new Object[attributes.length];
                    Class[] paramsClass = method.getParameterTypes();
                    for (int k = 0; k < attributes.length; k++) {
                        String p = "";
                        Node node = null;
                        if (map != null) {
                            node = map.getNamedItem(attributes[k]);
                        }
                        if (node != null) {
                            p = node.getNodeValue();
                        }

                        StringParser parser = conv.get(paramsClass[k]);
                        if (parser != null) {
                            params[k] = parser.parse(p);
                        }
                    }

                    try {
                        if (mustInstantiate) {
                            method.invoke(values[i], params);
                        } else {
                            method.invoke(null, params);
                        }
                    } catch (IllegalAccessException e) {
                        System.err.println(e);
                    } catch (IllegalArgumentException e) {
                        System.err.println(e);
                    } catch (InvocationTargetException e) {
                        System.err.println(e.getTargetException());
                    }
                }
            }
        }

        if (mustInstantiate) {
            return values;
        } else {
            return (T[]) Array.newInstance(type, 0);
        }
    }

    /**
     * Interface to implement to parse an attribute String into a Java object
     */
    public static interface StringParser {

        /**
         * Parse a string
         * @param str the string to parse
         * @return the corresponding Object
         */
        public Object parse(String str);
    }

    static {
        conv.put(int.class, new StringParser() {
            public Integer parse(String str) {
                try {
                    return Integer.parseInt(str);
                } catch (NumberFormatException e) {
                    try {
                        return (int) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Integer(0);
                    }
                }
            }
        });
        conv.put(char.class, new StringParser() {
            public Object parse(String str) {
                if (str.length() > 0) {
                    return str.charAt(0);
                } else {
                    return new Character((char) 0);
                }
            }
        });
        conv.put(byte.class, new StringParser() {
            public Object parse(String str) {
                try {
                    return Byte.parseByte(str);
                } catch (NumberFormatException e) {
                    try {
                        return (byte) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Byte((byte) 0);
                    }
                }
            }
        });
        conv.put(short.class, new StringParser() {
            public Object parse(String str) {
                try {
                    return Short.parseShort(str);
                } catch (NumberFormatException e) {
                    try {
                        return (short) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Short((short) 0);
                    }
                }
            }
        });
        conv.put(double.class, new StringParser() {
            public Object parse(String str) {
                try {
                    return Double.parseDouble(str);
                } catch (NumberFormatException ee) {
                    return new Double((double) 0);
                }
            }
        });
        conv.put(float.class, new StringParser() {
            public Object parse(String str) {
                try {
                    return Float.parseFloat(str);
                } catch (NumberFormatException ee) {
                    return new Float((float) 0);
                }
            }
        });
        conv.put(boolean.class, new StringParser() {
            public Object parse(String str) {
                return Boolean.parseBoolean(str);
            }
        });
        conv.put(long.class, new StringParser() {
            public Object parse(String str) {
                try {
                    return Long.parseLong(str);
                } catch (NumberFormatException e) {
                    try {
                        return (long) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Long((long) 0);
                    }
                }
            }
        });
        conv.put(String.class, new StringParser() {
            public Object parse(String str) {
                return str;
            }
        });
        conv.put(Color.class, new StringParser() {
            public Object parse(String str) {
                try {
                    return Color.decode(str);
                } catch (NumberFormatException e) {
                    return Color.BLACK;
                }
            }
        });
        conv.put(KeyStroke.class, new StringParser() {
            public Object parse(String str) {
                String[] toks = str.split(" +");
                StringBuilder buffer = new StringBuilder();
                for (int i = 0; i < toks.length - 1; i++) {
                    buffer.append(toks[i].toLowerCase());
                    buffer.append(" ");
                }
                buffer.append(toks[toks.length - 1].toUpperCase());
                return KeyStroke.getKeyStroke(buffer.toString());
            }
        });
    }

    @Retention(RetentionPolicy.RUNTIME)
    public @interface XConfAttribute {

        /**
         * Map method arguments with attributes name
         * For example,
         * <code>
         * @XConfAttribute(tag="mytag", attributes={"a", "b"})
         * void foo(String one, int tow) { ... }
         * </code>
         * The value of attribute "a" is converted into a String and passed as "one" argument,...
         */
    public String[] attributes() default {
            ""
        };

    public String tag() default "";

        /**
         * Used to avoid object instanciation so the differents annotated methods must be static.
         */
    public boolean isStatic() default false;
    }
}
