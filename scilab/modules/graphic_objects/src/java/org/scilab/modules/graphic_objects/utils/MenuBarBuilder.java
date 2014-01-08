/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.utils;

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
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.uimenu.Uimenu;
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
    public static Object buildMenuBar(Class<?>[] resultClass, String fileToLoad) throws SAXException, IOException, ParserConfigurationException {

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
            Class<?> clazz = ClassLoader.getSystemClassLoader().loadClass("org.scilab.modules.gui.SwingView");
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
        @SuppressWarnings("unused")
        public void addMenus(Integer parentId) {
            GraphicController controller = GraphicController.getController();
            GraphicObject parent = controller.getObjectFromId(parentId);
            // delete old menus
            for (Integer childId : (Integer []) parent.getChildren()) {
                GraphicObject child = controller.getObjectFromId(childId);
                if (child.getType() == __GO_UIMENU__) {
                    controller.removeRelationShipAndDelete(childId);
                }
            }

            NodeList menus = dom.getElementsByTagName(MENU);

            for (int i = 0; i < menus.getLength(); i++) {
                // Create the menu
                Integer menuId = null;
                Node node = menus.item(i);
                NamedNodeMap map = node.getAttributes();
                if (isParentValid) {
                    menuId = controller.askObject(Type.UIMENU);
                } else {
                    menuId = controller.askObject(Type.UIMENUMODEL);
                }

                Uimenu menu = (Uimenu)controller.getObjectFromId(menuId);
                // The menu is not visible in Scilab view by default
                menu.setHidden(true);

                // Set the label
                String menuLabel = Messages.gettext(map.getNamedItem(LABEL).getNodeValue());
                menu.setLabel(menuLabel);

                // Set the mnemonic if given
                if (map.getNamedItem(MNEMONIC) != null) {
                    menu.setMnemonic(map.getNamedItem(MNEMONIC).getNodeValue());
                }

                // Set the icon if given
                if (map.getNamedItem(ICON) != null) {
                    menu.setIcon(node.getNodeValue());
                }

                // Set the enable status if given
                if (map.getNamedItem(ENABLED) != null) {
                    menu.setEnable(map.getNamedItem(ENABLED).getNodeValue().equals(TRUE));
                }

                controller.objectCreated(menuId);
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
            GraphicController controller = GraphicController.getController();
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
                        menuId = controller.askObject(Type.UIMENU);
                    } else {
                        menuId = controller.askObject(Type.UIMENUMODEL);
                    }

                    Uimenu menu = (Uimenu)controller.getObjectFromId(menuId);
                    // The menu is not visible in Scilab view by default
                    menu.setHidden(true);
                    menu.setSeparator(separator);

                    // First we have to read its attributes
                    NamedNodeMap attributes = submenu.getAttributes();

                    for (int i = 0; i < attributes.getLength(); i++) {
                        Node item = attributes.item(i);
                        if (item.getNodeName() == LABEL) {
                            menu.setLabel(Messages.gettext(item.getNodeValue()));
                        } else if (item.getNodeName() == MNEMONIC) {
                            menu.setMnemonic(item.getNodeValue());
                        } else if (item.getNodeName() == ENABLED) {
                            menu.setEnable(item.getNodeValue().equals(TRUE));
                        } else if (item.getNodeName() == ICON) {
                            menu.setIcon(item.getNodeValue());
                        } else if (item.getNodeName() == ACCELERATOR) {
                            menu.setAccelerator(item.getNodeValue());
                        } else if (item.getNodeName() == MACOSX) {
                            macosx = item.getNodeValue().equals(TRUE);
                            if (!macosx && OS.get() == OS.MAC) {
                                GraphicModel.getModel().deleteObject(menuId);
                                return;
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
                                Node item = cbAttributes.item(j);
                                if (item.getNodeName() == INSTRUCTION) {
                                    command = item.getNodeValue();
                                } else if (item.getNodeName() == TYPE) {
                                    commandType = Integer.parseInt(item.getNodeValue());
                                }
                            }

                            if (command != null && commandType != CallBack.UNTYPED) {
                                menu.setCallback(command);
                                menu.setCallbacktype(commandType);
                            }
                        } else if (callback.getNodeName() == SUBMENU) {
                            addSubMenuItem(menuId, callback);
                        }
                        // Read next child
                        callback = callback.getNextSibling();
                    }

                    controller.objectCreated(menuId);
                    // Set the menu parent
                    controller.setGraphicObjectRelationship(parentMenuId, menuId);
                }
                // Read next child
                submenu = submenu.getNextSibling();
            }
        }

        /**
         * Add submenu for menu
         * @param parentMenuItemId object with this id will become a menu with subMenuItems
         * @param node to get attributs of the menu
         */
        public void addSubMenuItem(Integer parentMenuItemId, Node node) {
            GraphicController controller = GraphicController.getController();

            NamedNodeMap attributes = node.getAttributes();

            // Create the menu
            Integer subMenuItemId = null;
            if (isParentValid) {
                subMenuItemId = GraphicController.getController().askObject(Type.UIMENU);
            } else {
                subMenuItemId = GraphicController.getController().askObject(Type.UIMENUMODEL);
            }

            Uimenu menu = (Uimenu)controller.getObjectFromId(subMenuItemId);
            // The menu is not visible in Scilab view by default
            menu.setHidden(true);

            for (int i = 0; i < attributes.getLength(); i++) {
                Node item = attributes.item(i);
                if (item.getNodeName() == LABEL) {
                    menu.setLabel(Messages.gettext(item.getNodeValue()));
                } else if (item.getNodeName() == MNEMONIC) {
                    menu.setMnemonic(item.getNodeValue());
                } else if (item.getNodeName() == ICON) {
                    menu.setIcon(item.getNodeValue());
                } else if (item.getNodeName() == ENABLED) {
                    menu.setEnable(item.getNodeValue().equals(TRUE));
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
                        Node item = cbAttributes.item(j);
                        if (item.getNodeName() == INSTRUCTION) {
                            command = item.getNodeValue();
                        } else if (item.getNodeName() == TYPE) {
                            commandType = Integer.parseInt(item.getNodeValue());
                        }
                    }
                    if (command != null && commandType != CallBack.UNTYPED) {
                        menu.setCallback(command);
                        menu.setCallbacktype(commandType);
                    }
                } else if (callback.getNodeName() == SUBMENU) {
                    addSubMenuItem(subMenuItemId, callback);
                }
                // Read next child
                callback = callback.getNextSibling();
            }

            controller.objectCreated(subMenuItemId);
            // Set the menu parent
            GraphicController.getController().setGraphicObjectRelationship(parentMenuItemId, subMenuItemId);
        }
    }
}
