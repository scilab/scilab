/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.xml;

import java.awt.Color;
import java.io.File;
import java.io.FilenameFilter;
import java.io.FileFilter;
import java.io.IOException;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.KeyStroke;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
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

import org.scilab.modules.commons.ScilabCommons;

/**
 * Class to retrieve object from the xml configuration file
 *
 * @author Calixte DENIZET
 *
 */
public class XConfiguration {

    // User configuration file
    private static final String USER_CONFIG_FILE = ScilabCommons.getSCIHOME() + "/XConfiguration.xml";
    private static final String SCI = System.getenv("SCI");
    private static final String SCILAB_CONFIG_FILE = SCI + "/modules/preferences/etc/XConfiguration.xml";

    private static final String ERROR_READ = "Could not load file: ";

    private static final XPathFactory xpathFactory = XPathFactory.newInstance();
    private static final Map<Class<?>, StringParser> conv = new HashMap<Class<?>, StringParser>();

    /**
     * Get the document in SCIHOME corresponding to the configuration file.
     * @return the configuration document.
     */
    public static Document getXConfigurationDocument() {
        File xml = new File(USER_CONFIG_FILE);
        if (!xml.exists()) {
            return createDocument();
        }

        DocumentBuilder docBuilder = null;

        try {
            DocumentBuilderFactory factory = ScilabDocumentBuilderFactory.newInstance();
            docBuilder = factory.newDocumentBuilder();
            return docBuilder.parse(xml);
        } catch (ParserConfigurationException pce) {
            System.err.println(ERROR_READ + USER_CONFIG_FILE);
        } catch (SAXException se) {
            System.err.println(ERROR_READ + USER_CONFIG_FILE);
        } catch (IOException ioe) {
            System.err.println(ERROR_READ + USER_CONFIG_FILE);
        }
        return null;
    }

    /**
     * Create a document in using the XConfiguration-*.xml found in SCI/modules/MODULE_NAME/etc/
     * @return the built document
     */
    public static Document createDocument() {
        DocumentBuilder docBuilder;
        DocumentBuilderFactory factory;
        Document mainDoc;

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

        Element root = mainDoc.getDocumentElement();

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
     * Get a the list of the etc dirs in modules dirs
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

        Method[] meths = type.getMethods();
        Map<String, Method> mapMethods = new HashMap<String, Method>();
        for (Method m : meths) {
            m.setAccessible(true);
            String name = m.getName();
            if (name.startsWith("set") && m.getParameterTypes().length == 1 && m.getReturnType().equals(Void.TYPE)) {
                mapMethods.put(m.getName(), m);
            }
        }

        Map<String, String> names = new HashMap<String, String>();

        T[] values = (T[]) Array.newInstance(type, nodes.getLength());
        for (int i = 0; i < values.length; i++) {
            NamedNodeMap map = nodes.item(i).getAttributes();
            try {
                Constructor<T> constructor = type.getConstructor(new Class[]{});
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
                Node n =  map.item(j);
                String name = n.getNodeName();
                String methName = names.get(name);
                if (methName == null) {
                    StringBuilder buffer = new StringBuilder("set");
                    String[] parts = name.split("-");
                    for (String part : parts) {
                        if (part != null && part.length() > 0) {
                            buffer.append(part.substring(0,1).toUpperCase());
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
                        Object[] params = new Object[]{parser.parse(value)};
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
            public Object parse(String str) {
                try {
                    return Integer.parseInt(str);
                } catch (NumberFormatException e) {
                    try {
                        return (int) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return null;
                    }
                }
            }
        });
        conv.put(char.class, new StringParser() {
            public Object parse(String str) {
                if (str.length() > 0) {
                    return str.charAt(0);
                } else {
                    return null;
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
                        return null;
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
                        return null;
                    }
                }
            }
        });
        conv.put(double.class, new StringParser() {
            public Object parse(String str) {
                try {
                    return Double.parseDouble(str);
                } catch (NumberFormatException ee) {
                    return null;
                }
            }
        });
        conv.put(float.class, new StringParser() {
            public Object parse(String str) {
                try {
                    return Float.parseFloat(str);
                } catch (NumberFormatException ee) {
                    return null;
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
                        return null;
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
                    return null;
                }
            }
        });
        conv.put(KeyStroke.class, new StringParser() {
            public Object parse(String str) {
                return KeyStroke.getKeyStroke(str);
            }
        });
    }
}
