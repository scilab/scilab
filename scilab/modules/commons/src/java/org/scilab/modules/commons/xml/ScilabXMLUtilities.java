/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.StringTokenizer;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.xml.sax.SAXException;

import org.scilab.modules.localization.Messages;

/**
 * Class to provide different tools to manage xml.
 *
 * @author Calixte DENIZET
 */
public class ScilabXMLUtilities {

    private static final String ENCODING = "UTF-8";
    private static final String ERROR_WRITE = Messages.gettext("Could not save file: ");
    private static final String ERROR_READ = Messages.gettext("Could not load file: ");
    private static final Integer INDENTATION = new Integer(4);

    private static Transformer transformer;

    /**
     * Read a xml file and parse it
     * @param fileToRead the file to read
     * @return the corresponding document
     */
    public static Document readDocument(String fileToRead) {
        Document doc = null;
        try {
            DocumentBuilderFactory factory = ScilabDocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = factory.newDocumentBuilder();
            doc = docBuilder.parse(new File(fileToRead));
        } catch (ParserConfigurationException e) {
            System.err.println(ERROR_READ + fileToRead);
        } catch (SAXException e) {
            System.err.println(ERROR_READ + fileToRead);
        } catch (IOException e) {
            System.err.println(ERROR_READ + fileToRead);
        }

        return doc;
    }

    /**
     * Write a Document in a file. The empty lines are removed and the xml code is indented
     * @param doc the document to write
     * @param fileName the file name
     */
    public static void writeDocument(Document doc, String fileName) {
        if (doc != null && fileName != null) {
            if (transformer == null) {
                initTransformer();
            }

            if (transformer != null) {
                removeEmptyLines(doc.getDocumentElement());

                FileOutputStream fos = null;
                OutputStreamWriter osw = null;

                try {
                    /* FileOutputStream & OutputStreamWriter are needed to be sure that
                       the indentation will be correct (known Java bug) */

                    fos = new FileOutputStream(fileName, false);
                    osw = new OutputStreamWriter(fos, ENCODING);
                    transformer.transform(new DOMSource(doc), new StreamResult(osw));
                } catch (FileNotFoundException e) {
                    System.err.println(ERROR_WRITE + fileName);
                    System.err.println(e);
                } catch (UnsupportedEncodingException e) {
                    System.err.println(ERROR_WRITE + fileName);
                    System.err.println(e);
                } catch (TransformerException e) {
                    System.err.println(ERROR_WRITE + fileName);
                    System.err.println(e);
                } finally {
                    if (osw != null) {
                        try {
                            osw.close();
                        } catch (IOException ex) {
                            System.err.println(ex);
                        }
                    }
                }
            }
        }
    }

    /**
     * Remove empty lines which are a descendant of node
     * @param node the initial node
     */
    public static void removeEmptyLines(Node node) {
        Set<Node> nodesToRemove = new HashSet<Node>();
        collectEmptyLines(node, nodesToRemove);
        for (Node n : nodesToRemove) {
            n.getParentNode().removeChild(n);
        }
    }

    /**
     * Collect the empty lines to remove
     * @param node the parent node
     * @param nodesToRemove the set containing the nodes to remove
     */
    private static void collectEmptyLines(Node node, Set<Node> nodesToRemove) {
        if (node != null) {
            NodeList list = node.getChildNodes();
            int length = getNodeListLength(list);
            for (int i = 0; i < length; i++) {
                Node n = list.item(i);
                if (n != null) {
                    if (n.getNodeType() == Node.TEXT_NODE) {
                        nodesToRemove.add(n);
                    } else {
                        collectEmptyLines(n, nodesToRemove);
                    }
                }
            }
        }
    }

    /**
     * Replace or create a named node with parent element
     * @param doc the document
     * @param parent the parent element
     * @param nodeName the node name
     * @param attr an array containing attribute name followed by its value: "attr1", 1, "attr2", true, ...
     * @return the created element
     */
    public static Element replaceNamedNode(Document doc, Element parent, String nodeName, Object[] attr) {
        NodeList children = parent.getElementsByTagName(nodeName);
        Element elem;
        if (getNodeListLength(children) > 0) {
            elem = (Element) children.item(0);
        } else {
            elem = doc.createElement(nodeName);
            parent.appendChild(elem);
        }

        for (int i = 0; i < attr.length; i += 2) {
            elem.setAttribute(attr[i].toString(), attr[i + 1].toString());
        }

        return elem;
    }

    /**
     * Replace or create a named node with parent element
     * @param doc the document
     * @param parent the name of the parent element
     * @param nodeName the node name
     * @param attr an array containing attribute name followed by its value: "attr1", 1, "attr2", true, ...
     * @return the created element
     */
    public static Element replaceNamedNode(Document doc, String parent, String nodeName, Object[] attr) {
        NodeList parents = doc.getDocumentElement().getElementsByTagName(parent);
        if (getNodeListLength(parents) > 0) {
            return replaceNamedNode(doc, (Element) parents.item(0), nodeName, attr);
        }

        return null;
    }

    /**
     * Replace or create a named node with parent element
     * @param doc the document
     * @param parent the parent element
     * @param nodeName the node name
     * @param map a map containing {attributes -&gt; value}, the method value.toString() will be used.
     * @return the created element
     */
    public static Element replaceNamedNode(Document doc, Element parent, String nodeName, Map<String, Object> map) {
        NodeList children = parent.getElementsByTagName(nodeName);
        Element elem;
        if (getNodeListLength(children) > 0) {
            elem = (Element) children.item(0);
        } else {
            elem = doc.createElement(nodeName);
            parent.appendChild(elem);
        }

        for (String name : map.keySet()) {
            elem.setAttribute(name, map.get(name).toString());
        }

        return elem;
    }

    /**
     * Replace or create a named node with parent element
     * @param doc the document
     * @param parent the name of the parent element
     * @param nodeName the node name
     * @param map a map containing {attributes -&gt; value}, the method value.toString() will be used.
     * @return the created element
     */
    public static Element replaceNamedNode(Document doc, String parent, String nodeName, Map<String, Object> map) {
        NodeList parents = doc.getDocumentElement().getElementsByTagName(parent);
        if (getNodeListLength(parents) > 0) {
            return replaceNamedNode(doc, (Element) parents.item(0), nodeName, map);
        }

        return null;
    }

    /**
     * Create a new node with parent element
     * @param doc the document
     * @param parent the parent element
     * @param nodeName the node name
     * @param attr an array containing attribute name followed by its value: "attr1", 1, "attr2", true, ...
     * @return the created element
     */
    public static Element createNode(Document doc, Element parent, String nodeName, Object[] attr) {
        Element elem = doc.createElement(nodeName);
        for (int i = 0; i < attr.length; i += 2) {
            elem.setAttribute(attr[i].toString(), attr[i + 1].toString());
        }
        parent.appendChild(elem);

        return elem;
    }

    /**
     * Create a new node with parent element
     * @param doc the document
     * @param parent the parent element
     * @param nodeName the node name
     * @param map a map containing {attributes -&gt; value}, the method value.toString() will be used.
     * @return the created element
     */
    public static Element createNode(Document doc, Element parent, String nodeName, Map<String, Object> map) {
        Element elem = doc.createElement(nodeName);
        for (String name : map.keySet()) {
            elem.setAttribute(name, map.get(name).toString());
        }
        parent.appendChild(elem);

        return elem;
    }

    /**
     * Read the attributes of elem.
     * Map must be like this: "attr1" -&gt; integer.class, "attr2" -&gt; boolean.class, ...
     * Map will be filled with the value (as Object) of the different attributes
     * @param elem the element to analyze
     * @param map the map containing the attributes type.
     */
    public static void readNodeAttributes(Element elem, Map<String, Object> map) {
        NamedNodeMap attrMap = elem.getAttributes();
        for (int i = 0; i < attrMap.getLength(); i++) {
            Attr attr = (Attr) attrMap.item(i);
            String key = attr.getName();
            if (map.containsKey(key)) {
                map.put(key, convert(attr.getValue(), (Class) map.get(key)));
            }
        }
    }

    /**
     * Read the attributes of elem.
     * Map must be like this: Object[]{"attr1", integer.class, "attr2", boolean.class, ...}
     * Map will be filled with the value (as Object) of the different attributes
     * @param elem the element to analyze
     * @param map the map containing the attributes type.
     */
    public static void readNodeAttributes(Element elem, Object[] map) {
        NamedNodeMap attrMap = elem.getAttributes();
        for (int i = 0; i < attrMap.getLength(); i++) {
            Attr attr = (Attr) attrMap.item(i);
            String key = attr.getName();
            for (int j = 0; j < map.length; j += 2) {
                if (map[j].equals(key)) {
                    map[j + 1] = convert(attr.getValue(), (Class) map[j + 1]);
                }
            }
        }
    }

    /**
     * Read the attributes of first element named nodeName in the document.
     * Map must be like this: "attr1" -&gt; integer.class, "attr2" -&gt; boolean.class, ...
     * Map will be filled with the value (as Object) of the different attributes
     * @param doc the document
     * @param nodeName the node name
     * @param map the map containing the attributes type.
     * @return the corresponding element or null if it doesn't exist
     */
    public static Element readNodeAttributes(Document doc, String nodeName, Map<String, Object> map) {
        NodeList list = doc.getDocumentElement().getElementsByTagName(nodeName);
        if (getNodeListLength(list) > 0) {
            Node n = list.item(0);
            if (n instanceof Element) {
                readNodeAttributes((Element) n, map);
                return (Element) n;
            }
        }

        return null;
    }

    /**
     * Read the attributes of first element named nodeName in the document.
     * Map must be like this: Object[]{"attr1", integer.class, "attr2", boolean.class, ...}
     * Map will be filled with the value (as Object) of the different attributes
     * @param doc the document
     * @param nodeName the node name
     * @param map the map containing the attributes type.
     * @return the corresponding element or null if it doesn't exist
     */
    public static Element readNodeAttributes(Document doc, String nodeName, Object[] map) {
        NodeList list = doc.getDocumentElement().getElementsByTagName(nodeName);
        if (getNodeListLength(list) > 0) {
            Node n = list.item(0);
            if (n instanceof Element) {
                readNodeAttributes((Element) n, map);
                return (Element) n;
            }
        }

        return null;
    }

    /**
     * Retrieve the list of the elements which have an attribute equal to the given value.
     * @param root the root element
     * @param attribute the attribute name
     * @param value the value
     * @return the list
     */
    public static List<Element> getElementsWithAttributeEquals(Element root, String attribute, String value) {
        List<Element> list = new ArrayList<Element>();
        getElementsWithAttributeEquals(root, attribute, value, list);

        return list;
    }

    /**
     * Retrieve the list of the elements which have an attribute equal to the given value (recursive function).
     * @param root the root element
     * @param attribute the attribute name
     * @param value the value
     * @param list the list to fill
     */
    private static final void getElementsWithAttributeEquals(Element root, String attribute, String value, List<Element> list) {
        if (root.getAttribute(attribute).equals(value)) {
            list.add(root);
        }
        if (root.hasChildNodes()) {
            NodeList nodes = root.getChildNodes();
            int length = nodes.getLength();
            for (int i = 0; i < length; i++) {
                Node node = nodes.item(i);
                if (node instanceof Element) {
                    Element elem = (Element) nodes.item(i);
                    getElementsWithAttributeEquals(elem, attribute, value, list);
                }
            }
        }
    }

    /**
     * Convert a value (as String) into an object according to its class type giving in clazz
     * @param value the value to convert
     * @param clazz the class type of the value
     * @return the converted object
     */
    private static final Object convert(String value, Class clazz) {
        if (clazz == int.class) {
            return Integer.parseInt(value);
        } else if (clazz == float.class) {
            return Float.parseFloat(value);
        } else if (clazz == boolean.class) {
            return Boolean.parseBoolean(value);
        } else if (clazz == double.class) {
            return Double.parseDouble(value);
        } else if (clazz == short.class) {
            return Short.parseShort(value);
        } else if (clazz == long.class) {
            return Long.parseLong(value);
        } else if (clazz == byte.class) {
            return Byte.parseByte(value);
        } else if (clazz == char.class) {
            return value.length() == 0 ? '\0' : value.charAt(0);
        } else if (clazz == String[].class) {
            return getArray(value, ";");
        }

        return value;
    }

    /**
     * Get an array from a string containing strings separated with a delimitor
     * @param value the string to parse
     * @param delimitor the delimitor
     * @return an array of String
     */
    private static final String[] getArray(String value, String delimitor) {
        StringTokenizer tokens = new StringTokenizer(value, delimitor);
        String[] arr = new String[tokens.countTokens()];
        for (int i = 0; i < arr.length; i++) {
            arr[i] = tokens.nextToken();
        }

        return arr;
    }

    /**
     * Init the transformer
     */
    private static final void initTransformer() {
        try {
            TransformerFactory factory = ScilabTransformerFactory.newInstance();
            factory.setAttribute("indent-number", INDENTATION);
            transformer = factory.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "no");
            transformer.setOutputProperty(OutputKeys.ENCODING, ENCODING);
        } catch (TransformerConfigurationException e) {
            System.err.println("Problem to init the Transformer to write xml files");
            System.err.println(e);
        } catch (TransformerFactoryConfigurationError e) {
            System.err.println("Problem to init the Transformer to write xml files");
            System.err.println(e);
        }
    }

    /**
     * @param list a node list
     * @return the length
     */
    private static int getNodeListLength(NodeList list) {
        int length = 0;
        try {
            length = list.getLength();
        } catch (NullPointerException e) {
            /* Avoid Java bug */
        }
        return length;
    }

    /**
     * Replace classical XML reserved chars by their XML equivalent entities
     * @param str the String to parse and transform
     * @return a String with the XML entities.
     */
    public static String getXMLString(String str) {
        if (str != null && !str.isEmpty()) {
            StringBuilder buf = new StringBuilder();
            char[] chars = str.toCharArray();
            int last = 0;
            for (int i = 0; i < chars.length; i++) {
                switch (chars[i]) {
                    case '\'' :
                        buf.append(chars, last, i - last).append("&apos;");
                        last = i + 1;
                        break;
                    case '\"' :
                        buf.append(chars, last, i - last).append("&quot;");
                        last = i + 1;
                        break;
                    case '<' :
                        buf.append(chars, last, i - last).append("&lt;");
                        last = i + 1;
                        break;
                    case '>' :
                        buf.append(chars, last, i - last).append("&gt;");
                        last = i + 1;
                        break;
                    case '&' :
                        buf.append(chars, last, i - last).append("&amp;");
                        last = i + 1;
                        break;
                    default :
                        break;
                }
            }

            if (last == 0) {
                return str;
            }

            if (last < chars.length) {
                buf.append(chars, last, chars.length - last);
            }

            return buf.toString();
        }

        return str;
    }
}
