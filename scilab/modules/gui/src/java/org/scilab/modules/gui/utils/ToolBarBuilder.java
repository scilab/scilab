/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.utils;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.Collection;
import java.util.TreeSet;

import javax.swing.AbstractButton;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JToggleButton;
import javax.xml.parsers.ParserConfigurationException;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.localization.Messages;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;

/**
 * Create a toolBar from an XML file
 * @author Vincent COUVERT
 */
public final class ToolBarBuilder {

    private static final String FILE_NOT_FOUND = "Could not find file: ";

    private static final String CANNOT_CREATE_TOOLBAR = "Cannot create ToolBar.\n"
            + "Check if file *_toolbar.xml is available and valid.";

    private static int figureIndex;

    /**
     * Default constructor
     */
    private ToolBarBuilder() {
        throw new UnsupportedOperationException();
    }

    /**
     * Create a proxy to access a XML file
     * @param resultClass class for the return value
     * @param fileToLoad XML file to load
     * @return a proxy used to read the XML file
     * @throws SAXException can be thrown when an error occurs while reading the file
     * @throws IOException can be thrown when an error occurs while accessing the file
     * @throws ParserConfigurationException can be thrown when an error occurs while parsing the file
     */
    public static Object buildToolBar(Class[] resultClass, String fileToLoad) throws SAXException, IOException, ParserConfigurationException {

        InvocationHandler invocationHandler = new ToolBarConfigurationHandler(fileToLoad);

        return Proxy.newProxyInstance(invocationHandler.getClass().getClassLoader(), resultClass, invocationHandler);
    }

    /**
     * Create a Scilab toolbar from data in a XML file
     * @param fileToLoad XML file to load
     * @return the toolbar created
     */
    public static ToolBar buildToolBar(String fileToLoad) {
        return buildToolBar(fileToLoad, 0);
    }

    /**
     * Create a Scilab toolbar from data in a XML file
     * @param fileToLoad XML file to load
     * @param figureIndex the index of the figure in Scilab (for graphics figures only)
     * @return the toolbar created
     */
    public static ToolBar buildToolBar(String fileToLoad, int figureIndex) {

        ToolBarBuilder.figureIndex = figureIndex;

        ToolBar toolbar = ScilabToolBar.createToolBar();

        try {
            ToolBarConfiguration toolBarConfig =
                (ToolBarConfiguration) buildToolBar(new Class[] {ToolBarConfiguration.class}, fileToLoad);
            toolBarConfig.addPushButtons(toolbar);
        } catch (IllegalArgumentException e) {
            System.err.println(CANNOT_CREATE_TOOLBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (SAXException e) {
            System.err.println(CANNOT_CREATE_TOOLBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (IOException e) {
            System.err.println(CANNOT_CREATE_TOOLBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (ParserConfigurationException e) {
            System.err.println(CANNOT_CREATE_TOOLBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        }

        return toolbar;
    }

    /**
     * Class used to read the XMl file
     */
    private static class ToolBarConfigurationHandler implements InvocationHandler {
        protected static final String BUTTON = "button";
        protected static final String ICON = "icon";
        protected static final String SEPARATOR = "separator";
        protected static final String ENABLED = "enabled";
        protected static final String CALLBACK = "callback";
        protected static final String TYPE = "type";
        protected static final String INSTRUCTION = "instruction";
        protected static final String TRUE = "true";
        protected static final String TOOLTIPTEXT = "tooltiptext";
        protected static final String TOGGLE = "toggle";

        private final Document dom;
        private final Collection<String> internalMethodNames;


        /**
         * Constructor
         * @param xmlFile XML file to load
         * @throws SAXException can be thrown when an error occurs while reading the file
         * @throws IOException can be thrown when an error occurs while accessing the file
         * @throws ParserConfigurationException can be thrown when an error occurs while parsing the file
         */
        public ToolBarConfigurationHandler(String xmlFile) throws SAXException, IOException, ParserConfigurationException {

            if (!new File(xmlFile).exists()) {
                throw new java.io.IOException();
            }

            // Build dictionary for internal method
            internalMethodNames = new TreeSet<String>();
            Method[] internalMethodes = this.getClass().getMethods();
            for (Method method : internalMethodes) {
                internalMethodNames.add(method.getName());
            }

            // Build xml document for request
            dom = ScilabDocumentBuilderFactory.newInstance().newDocumentBuilder().parse(new File(xmlFile));
        }

        /**
         * Invoke a proxy to get data
         * @param proxy the proxy to call
         * @param method the method to call
         * @param args the arguments for the method
         * @return the object read
         * @throws IllegalAccessException thrown when the method called is inaccessible
         * @throws InvocationTargetException thorwn when the method called threw an exception
         * @throws NoSuchMethodException thrown when invoking a non-existing method
         * @see java.lang.reflect.InvocationHandler#invoke(java.lang.Object, java.lang.reflect.Method, java.lang.Object[])
         */
        public Object invoke(Object proxy, Method method, Object[] args)
        throws IllegalAccessException, InvocationTargetException, NoSuchMethodException {

            if (internalMethodNames.contains(method.getName())) {
                return getClass().getMethod(method.getName(), method.getParameterTypes()).invoke(this, args);
            } else {
                return null;
            }
        }

        /**
         * Add tools to a toolbar
         * @param tb the toolbar which the buttons will be added to
         * @see org.scilab.modules.gui.utils.ToolBarConfiguration#addButtons(org.scilab.modules.gui.toolbar.ToolBar)
         */
        public void addPushButtons(ToolBar tb) {
            // First Child of this XML file is a menubar
            Node menubar = dom.getFirstChild();
            // Children of the menubar are buttons (or separators)
            Node button = menubar.getFirstChild();

            NamedNodeMap buttonAttributes = null;
            AbstractButton pushButton = null;

            try {
                // Get all children
                while (button != null) {
                    if (button.getNodeName().equals(BUTTON)) {
                        // Read all its attributes
                        buttonAttributes = button.getAttributes();

                        boolean toggle = false;
                        for (int i = 0; i < buttonAttributes.getLength(); i++) {
                            if (buttonAttributes.item(i).getNodeName().equals(TOGGLE)) {
                                toggle = buttonAttributes.item(i).getNodeValue().equals(TRUE);
                                break;
                            }
                        }

                        if (toggle) {
                            pushButton = new JToggleButton();
                        } else {
                            pushButton = new JButton();
                        }

                        ScilabLAF.setDefaultProperties(pushButton);

                        for (int i = 0; i < buttonAttributes.getLength(); i++) {
                            if (buttonAttributes.item(i).getNodeName().equals(ICON)) {
                                // Icon file
                                pushButton.setIcon(new ImageIcon(FindIconHelper.findIcon(buttonAttributes.item(i).getNodeValue())));
                            } else if (buttonAttributes.item(i).getNodeName().equals(ENABLED)) {
                                // Enable are disable the button
                                pushButton.setEnabled(buttonAttributes.item(i).getNodeValue().equals(TRUE));
                            } else if (buttonAttributes.item(i).getNodeName().equals(TOOLTIPTEXT)) {
                                // Add a ToolTip on the button
                                pushButton.setToolTipText(Messages.gettext(buttonAttributes.item(i).getNodeValue()));
                            }
                        }
                        // Add the button to the toolbar
                        ((SwingScilabToolBar) tb.getAsSimpleToolBar()).add(pushButton);

                        // Then we get its callback (if exists)
                        Node callback = button.getFirstChild();
                        while (callback != null) {
                            if (callback.getNodeName() == CALLBACK) {
                                NamedNodeMap cbAttributes = callback.getAttributes();
                                String command = null;
                                int commandType = CallBack.UNTYPED;
                                for (int j = 0; j < cbAttributes.getLength(); j++) {
                                    if (cbAttributes.item(j).getNodeName() == INSTRUCTION) {
                                        command = cbAttributes.item(j).getNodeValue();
                                    } else if (cbAttributes.item(j).getNodeName() == TYPE) {
                                        commandType = Integer.parseInt(cbAttributes.item(j).getNodeValue());
                                    }
                                }
                                if (command != null && commandType != CallBack.UNTYPED) {
                                    pushButton.addActionListener(CommonCallBack.createCallback(replaceFigureID(command), commandType));
                                }
                            }
                            // Read next child
                            callback = callback.getNextSibling();
                        }

                    } else if (button.getNodeName().equals(SEPARATOR)) {
                        // The child is a separator
                        tb.addSeparator();
                    }
                    // Get next child
                    button = button.getNextSibling();
                }
            } catch (Exception e) {
                System.err.println(e);
                e.printStackTrace();
            }
        }


        /**
         * Replace pattern [SCILAB_FIGURE_ID] by the figure index
         * @param initialString string read in XML file
         * @return callback string
         */
        private String replaceFigureID(String initialString) {
            return initialString.replaceAll("\\[SCILAB_FIGURE_ID\\]", Integer.toString(ToolBarBuilder.figureIndex));
        }

    }
}
