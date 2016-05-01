/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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

package org.scilab.modules.graphic_objects.utils;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_HIDDEN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ACCELERATOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LABEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MNEMONIC__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SEPARATOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.Collection;
import java.util.TreeSet;

import javax.xml.parsers.ParserConfigurationException;

import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.localization.Messages;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


/**
 * Create a menuBar from an XML file
 * @author Vincent COUVERT
 */
public final class MenuBarBuilder {

    private static final String MAINMENUBARXMLFILE = ScilabConstants.SCI + "/modules/gui/etc/main_menubar.xml";
    private static final String GRAPHICSMENUBARXMLFILE = ScilabConstants.SCI + "/modules/gui/etc/graphics_menubar.xml";

    private static final String FILE_NOT_FOUND = "Could not find file: ";

    private static final String CANNOT_CREATE_MENUBAR = "Cannot create MenuBar.\n"
            + "Check if file *_menubar.xml is available and valid.";

    private static boolean isParentValid = true;;

    /**
     * Default constructor
     */
    private MenuBarBuilder() {
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
    public static Object buildMenuBar(Class[] resultClass, String fileToLoad) throws SAXException, IOException, ParserConfigurationException {

        InvocationHandler invocationHandler = new MenuBarConfigurationHandler(fileToLoad);

        return Proxy.newProxyInstance(invocationHandler.getClass().getClassLoader(), resultClass, invocationHandler);
    }

    /**
     * Create console menubar from data in a XML file
     * @param consoleId the console
     */
    public static void buildConsoleMenuBar(Integer consoleId) {
        buildMenuBar(MAINMENUBARXMLFILE, consoleId);
    }

    /**
     * Create graphic figure menubar from data in a XML file
     * @param figureId the figure
     */
    public static void buildFigureMenuBar(Integer figureId) {
        boolean isheadless = false;

        try {
            Class clazz = ClassLoader.getSystemClassLoader().loadClass("org.scilab.modules.gui.SwingView");
            Method meth = clazz.getMethod("isHeadless");
            isheadless = (Boolean) meth.invoke(null);
        } catch (Exception e) {
            System.err.println(e);
        }

        if (!isheadless) {
            MenuBarBuilder.isParentValid = false;
            buildMenuBar(GRAPHICSMENUBARXMLFILE, figureId);
        }
    }

    /**
     * Create children used in the menubar from data in a XML file
     * @param fileToLoad XML file to load
     * @param parentId the menubar parent
     */
    public static void buildMenuBar(String fileToLoad, Integer parentId) {

        try {
            MenuBarConfiguration menuBarConfig =
                (MenuBarConfiguration) buildMenuBar(new Class[] {MenuBarConfiguration.class}, fileToLoad);
            menuBarConfig.addMenus(parentId);
        } catch (IllegalArgumentException e) {
            System.err.println(CANNOT_CREATE_MENUBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (SAXException e) {
            System.err.println(CANNOT_CREATE_MENUBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (IOException e) {
            System.err.println(CANNOT_CREATE_MENUBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (ParserConfigurationException e) {
            System.err.println(CANNOT_CREATE_MENUBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        }
    }

    /**
     * Class used to read the XMl file
     */
    private static class MenuBarConfigurationHandler implements InvocationHandler {
        protected static final String LABEL = "label";
        protected static final String MENU = "menu";
        protected static final String MNEMONIC = "mnemonic";
        protected static final String SUBMENU = "submenu";
        protected static final String SEPARATOR = "separator";
        protected static final String ENABLED = "enabled";
        protected static final String ACCELERATOR = "accelerator";
        protected static final String CALLBACK = "callback";
        protected static final String TYPE = "type";
        protected static final String INSTRUCTION = "instruction";
        protected static final String TRUE = "true";
        protected static final String ICON = "icon";
        protected static final String MACOSX = "macosx";

        private Document dom;
        private Collection<String> internalMethodNames;


        /**
         * Constructor
         * @param xmlFile XML file to load
         * @throws SAXException can be thrown when an error occurs while reading the file
         * @throws IOException can be thrown when an error occurs while accessing the file
         * @throws ParserConfigurationException can be thrown when an error occurs while parsing the file
         */
        public MenuBarConfigurationHandler(String xmlFile) throws SAXException, IOException, ParserConfigurationException {

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
         * Add menus to a menubar using Scilab MVC
         * @param parentId the tab ID to which the menus will be added to
         * @see org.scilab.modules.MenuBarConfiguration.utils.MenuBarConfiguration#addMenus(org.scilab.modules.gui.menubar.MenuBar)
         */
        public void addMenus(Integer parentId) {

            // delete old menus
            for (Integer childId : (Integer []) GraphicController.getController().getProperty(parentId, GraphicObjectProperties.__GO_CHILDREN__)) {
                if (GraphicController.getController().getProperty(childId, GraphicObjectProperties.__GO_TYPE__).equals(__GO_UIMENU__)) {
                    GraphicController.getController().removeRelationShipAndDelete(childId);
                }
            }

            NodeList menus = dom.getElementsByTagName(MENU);

            for (int i = 0; i < menus.getLength(); i++) {
                // Create the menu
                Integer menuId = null;
                if (isParentValid) {
                    menuId = GraphicController.getController().askObject(Type.UIMENU);
                } else {
                    menuId = GraphicController.getController().askObject(Type.UIMENUMODEL);
                }

                // The menu is not visible in Scilab view by default
                GraphicController.getController().setProperty(menuId, __GO_HIDDEN__, true);

                // Set the label
                String menuLabel = Messages.gettext(menus.item(i).getAttributes().getNamedItem(LABEL).getNodeValue());
                GraphicController.getController().setProperty(menuId, __GO_UI_LABEL__, menuLabel);

                // Set the mnemonic if given
                if (menus.item(i).getAttributes().getNamedItem(MNEMONIC) != null) {
                    String mnemonicString = menus.item(i).getAttributes().getNamedItem(MNEMONIC).getNodeValue();
                    GraphicController.getController().setProperty(menuId, __GO_UI_MNEMONIC__, mnemonicString);
                }

                // Set the icon if given
                if (menus.item(i).getAttributes().getNamedItem(ICON) != null) {
                    String iconName = menus.item(i).getNodeValue();
                    GraphicController.getController().setProperty(menuId, __GO_UI_ICON__, iconName);
                }

                // Set the enable status if given
                if (menus.item(i).getAttributes().getNamedItem(ENABLED) != null) {
                    boolean enabled = menus.item(i).getAttributes().getNamedItem(ENABLED).getNodeValue().equals(TRUE);
                    GraphicController.getController().setProperty(menuId, __GO_UI_ENABLE__, enabled);
                }
                // Set the menu parent
                GraphicController.getController().setGraphicObjectRelationship(parentId, menuId);
                addSubMenus(menuId, i);
            }
        }

        /**
         * Read submenus data in the XML file and create them using Scilab MVC
         * @param parentMenuId the parent menu UID for submenus
         * @param index the index of the parent in menu list
         */
        public void addSubMenus(Integer parentMenuId, int index) {
            Node submenu = dom.getElementsByTagName(MENU).item(index).getFirstChild();

            boolean separator = false;
            boolean macosx = true;

            while (submenu != null) {
                if (submenu.getNodeName() == SEPARATOR) {
                    // Add a separator
                    separator = true;
                } else if (submenu.getNodeName() == SUBMENU) {
                    // Create the menu
                    Integer menuId = null;
                    if (isParentValid) {
                        menuId = GraphicController.getController().askObject(Type.UIMENU);
                    } else {
                        menuId = GraphicController.getController().askObject(Type.UIMENUMODEL);
                    }

                    // The menu is not visible in Scilab view by default
                    GraphicController.getController().setProperty(menuId, __GO_HIDDEN__, true);

                    // Set the menu parent
                    GraphicController.getController().setGraphicObjectRelationship(parentMenuId, menuId);

                    // First we have to read its attributes
                    NamedNodeMap attributes = submenu.getAttributes();

                    for (int i = 0; i < attributes.getLength(); i++) {
                        if (attributes.item(i).getNodeName() == LABEL) {
                            // Set the label
                            String menuLabel = Messages.gettext(attributes.item(i).getNodeValue());
                            GraphicController.getController().setProperty(menuId, __GO_UI_LABEL__, menuLabel);
                        } else if (attributes.item(i).getNodeName() == MNEMONIC) {
                            // Set the mnemonic
                            String mnemonicString = attributes.item(i).getNodeValue();
                            GraphicController.getController().setProperty(menuId, __GO_UI_MNEMONIC__, mnemonicString);
                        } else if (attributes.item(i).getNodeName() == ENABLED) {
                            // Set the enable status
                            boolean enabled = attributes.item(i).getNodeValue().equals(TRUE);
                            GraphicController.getController().setProperty(menuId, __GO_UI_ENABLE__, enabled);
                        } else if (attributes.item(i).getNodeName() == ICON) {
                            // Set the icon
                            String iconName = attributes.item(i).getNodeValue();
                            GraphicController.getController().setProperty(menuId, __GO_UI_ICON__, iconName);
                        } else if (attributes.item(i).getNodeName() == ACCELERATOR) {
                            // Set the accelerator
                            String acceleratorString = attributes.item(i).getNodeValue();
                            GraphicController.getController().setProperty(menuId, __GO_UI_ACCELERATOR__, acceleratorString);
                        } else if (attributes.item(i).getNodeName() == MACOSX) {
                            macosx = attributes.item(i).getNodeValue().equals(TRUE);
                            if (!macosx && OS.get() == OS.MAC) {
                                GraphicController.getController().removeRelationShipAndDelete(menuId);
                                separator = false;
                            }
                        }
                    }

                    // Then we get its callback (if exists)
                    Node callback = submenu.getFirstChild();
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
                                GraphicController.getController().setProperty(menuId, __GO_CALLBACK__, command);
                                GraphicController.getController().setProperty(menuId, __GO_CALLBACKTYPE__, commandType);
                            }
                        } else if (callback.getNodeName() == SUBMENU) {
                            addSubMenuItem(menuId, callback);
                        }
                        // Read next child
                        callback = callback.getNextSibling();
                    }
                    // Manage separators
                    if (separator) {
                        GraphicController.getController().setProperty(menuId, __GO_UI_SEPARATOR__, true);
                        separator = false;
                    }
                }
                // Read next child
                submenu = submenu.getNextSibling();
            }
        }

        /**
         * Add submenu for menu
         * @param parentMenuItemId object with this id will become a menu with subMenuItems
         * @param node to get attributes of the menu
         */
        public void addSubMenuItem(Integer parentMenuItemId, Node node) {

            NamedNodeMap attributes = node.getAttributes();

            // Create the menu
            Integer subMenuItemId = null;
            if (isParentValid) {
                subMenuItemId = GraphicController.getController().askObject(Type.UIMENU);
            } else {
                subMenuItemId = GraphicController.getController().askObject(Type.UIMENUMODEL);
            }

            // The menu is not visible in Scilab view by default
            GraphicController.getController().setProperty(subMenuItemId, __GO_HIDDEN__, true);

            // Set the menu parent
            GraphicController.getController().setGraphicObjectRelationship(parentMenuItemId, subMenuItemId);

            for (int i = 0; i < attributes.getLength(); i++) {
                if (attributes.item(i).getNodeName() == LABEL) {
                    // Set the label
                    String menuLabel = Messages.gettext(attributes.item(i).getNodeValue());
                    GraphicController.getController().setProperty(subMenuItemId, __GO_UI_LABEL__, menuLabel);
                } else if (attributes.item(i).getNodeName() == MNEMONIC) {
                    // Set the mnemonic
                    String mnemonicString = attributes.item(i).getNodeValue();
                    GraphicController.getController().setProperty(subMenuItemId, __GO_UI_MNEMONIC__, mnemonicString);
                } else if (attributes.item(i).getNodeName() == ICON) {
                    String iconName = attributes.item(i).getNodeValue();
                    GraphicController.getController().setProperty(subMenuItemId, __GO_UI_ICON__, iconName);
                } else if (attributes.item(i).getNodeName() == ENABLED) {
                    // Set the enable status
                    boolean enabled = attributes.item(i).getNodeValue().equals(TRUE);
                    GraphicController.getController().setProperty(subMenuItemId, __GO_UI_ENABLE__, enabled);
                }
            }

            // Then we get its callback (if exists)
            Node callback = node.getFirstChild();
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
                        GraphicController.getController().setProperty(subMenuItemId, __GO_CALLBACK__, command);
                        GraphicController.getController().setProperty(subMenuItemId, __GO_CALLBACKTYPE__, commandType);
                    }
                } else if (callback.getNodeName() == SUBMENU) {
                    addSubMenuItem(subMenuItemId, callback);
                }
                // Read next child
                callback = callback.getNextSibling();
            }
        }
    }
}
