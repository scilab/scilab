/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.bridge.helpbrowser;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.JarURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import javax.help.DefaultHelpHistoryModel;
import javax.help.JHelpContentViewer;
import javax.help.HelpSet;
import javax.help.plaf.basic.BasicContentViewerUI;
import javax.swing.JComponent;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import javax.swing.event.HyperlinkEvent;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultEditorKit;
import javax.swing.text.Document;
import javax.swing.text.Element;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.localization.Messages;

/**
 * This class inherits from BasicContentViewerUI from Javahelp.
 * Through this class, we are adding some features on the javahelp browser
 * We are adding a popup menu on the right click of the mouse
 * In this menu, we are providing:
 *  - Execute in Scilab
 *  - Edit in the text editor
 *  - Copy
 *  - Select all
 *  - History
 *
 * @author Sylvestre LEDRU
 * @author Calixte DENIZET
 */
public class SwingScilabHelpBrowserViewer extends BasicContentViewerUI {

    private static final String SCILAB_PROTO = "scilab://";
    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final long serialVersionUID = -2593697956426596790L;

    /* This field is a copy of BasicContentViewerUI which is privated.
     * Therefor, I am changing the permission here to make it available
     * to the methods of this object
     */
    private javax.swing.JEditorPane accessibleHtml;

    private JHelpContentViewer x;
    private List<HelpSet> helpSets;

    public SwingScilabHelpBrowserViewer(JHelpContentViewer x) {
        super(x);
        this.x = x;
    }

    public static javax.swing.plaf.ComponentUI createUI(JComponent x) {
        return new SwingScilabHelpBrowserViewer((JHelpContentViewer) x);
    }

    /**
     * Update the browser links
     */
    public void hyperlinkUpdate(HyperlinkEvent event) {
        if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
            if (event.getDescription().startsWith("http://")) {
                WebBrowser.openUrl(event.getURL(), event.getDescription());
            } else if (event.getDescription().startsWith(SCILAB_PROTO)) {
                if (helpSets == null) {
                    initHelpSets(x.getModel().getHelpSet());
                }
                URL url = resolvScilabLink(event);
                if (url != null) {
                    super.hyperlinkUpdate(new HyperlinkEvent(event.getSource(), event.getEventType(), url, ""));
                }
            } else if (event.getDescription().startsWith("file://")) {
                String url = event.getDescription();
                url = url.replaceFirst("SCI", SCI);
                WebBrowser.openUrl(url);
            } else {
                super.hyperlinkUpdate(event);
            }
        }
    }

    private void initHelpSets(HelpSet hs) {
        helpSets = new ArrayList();
        helpSets.add(hs);
        for (Enumeration<HelpSet> e = hs.getHelpSets(); e.hasMoreElements();) {
            helpSets.add(e.nextElement());
        }
    }

    /**
     * Try to find an id
     * @param id the id to find
     * @return the URL corresponding to the id
     */
    public URL getURLFromID(String id) {
        URL url = null;
        try {
            for (HelpSet hs : helpSets) {
                javax.help.Map map = hs.getLocalMap();
                if (map.isValidID(id, hs)) {
                    url = map.getURLFromID(javax.help.Map.ID.create(id, hs));
                    if (url != null) {
                        return url;
                    }
                }
            }
            url = new URL(helpSets.get(0).getHelpSetURL().toString().replace("jhelpset.hs", "ScilabErrorPage.html"));
        } catch (MalformedURLException ex) { }

        return url;
    }

    /**
     * Try to find an id in a toolbox
     * @param tbxName the toolbox's name
     * @param id the id to find
     * @return the URL corresponding to the id
     */
    public URL getURLFromID(String tbxName, String id) {
        if (tbxName == null) {
            return getURLFromID(id);
        }
        URL url = null;
        try {
            for (HelpSet hs : helpSets) {
                if (hs.getHelpSetURL().toString().replaceAll("\\\\", "/").indexOf("/" + tbxName + "/") !=  -1) {
                    javax.help.Map map = hs.getLocalMap();
                    if (map.isValidID(id, hs)) {
                        url = map.getURLFromID(javax.help.Map.ID.create(id, hs));
                        if (url != null) {
                            return url;
                        }
                    }
                }
            }
            url = new URL(helpSets.get(0).getHelpSetURL().toString().replace("jhelpset.hs", "ScilabErrorPage.html"));
        } catch (MalformedURLException ex) { }

        return url;
    }

    /**
     * Try to transform an address such as scilab://scilab.help/bvode into a conform URL
     * pointing to the corresponding file in using jar: protocol.
     * E.g. scilab://scilab.help/bvode will be transform into
     * jar:file:SCI/modules/helptools/jar/scilab_fr_FR_help.jar!/scilab_fr_FR_help/bvode.html
     * (where SCI has the good value)
     * @param address the address to convert
     * @return the correct address in using jar:// protocol
     **/
    public URL resolvScilabLink(HyperlinkEvent event) {
        int pos = SCILAB_PROTO.length();
        String addr = event.getDescription();
        addr = addr.trim().replaceAll("\\\\", "/");
        addr = addr.substring(pos);

        pos = addr.indexOf("/");
        String location;
        String path = "";
        if (pos != -1) {
            location = addr.substring(0, pos);
            if (pos != addr.length()) {
                path = addr.substring(pos + 1);
            }
        } else {
            return getURLFromID(addr);
        }

        String[] splitLoc = location.split("\\.");
        String mainLocation = null;
        String subLocation = null;

        if (splitLoc.length >= 1) {
            mainLocation = splitLoc[0];
        }
        if (splitLoc.length >= 2) {
            subLocation = splitLoc[1];
        }

        if (subLocation.equals("help")) {
            if (mainLocation.equals("scilab")) {
                return getURLFromID(path);
            } else {
                return getURLFromID(mainLocation, path);
            }
        } else if (subLocation.equals("exec")) {
            if (!mainLocation.equals("scilab")) {
                exec(getToolboxPath() + "/" + path);
            } else {
                exec(SCI + "/modules/" + path);
            }
        } else if (subLocation.equals("demos")) {
            if (!mainLocation.equals("scilab")) {
                exec(getToolboxPath() + "/demos/" + path + ".sce");
            } else {
                exec(SCI + "/modules/" + path + ".sce");
            }
        } else if (subLocation.equals("execexample")) {
            execExample(event.getSourceElement().getParentElement().getParentElement().getParentElement().getElement(0).getElement(0));
        } else if (subLocation.equals("editexample")) {
            editExample(event.getSourceElement().getParentElement().getParentElement().getParentElement().getElement(0).getElement(0));
        }

        return null;
    }

    /**
     * @return the path of the toolbox
     */
    public String getToolboxPath() {
        try {
            URL url = ((JarURLConnection) x.getCurrentURL().openConnection()).getJarFileURL();
            return new File(url.toURI()).getParentFile().getParent();
        } catch (Exception e) { }

        return "";
    }

    /**
     * Execute the code in example
     * @param pre the preformatted Element containing Scilab's code
     */
    public static void execExample(Element pre) {
        String code = getCode(pre);
        ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(code, true /* display */, true /* store in history */);
    }

    /**
     * Edit the code in example
     * @param pre the preformatted Element containing Scilab's code
     */
    public static void editExample(Element pre) {
        edit(getCode(pre));
    }

    /**
     * Edit the code
     * @param code the code to edit
     */
    private static void edit(String code) {
        try {
            /* Dynamic load of the SciNotes class.
             * This is done to avoid a cyclic dependency on gui <=> scinotes
             */
            Class scinotesClass = Class.forName("org.scilab.modules.scinotes.SciNotes");
            Class[] arguments = new Class[] {String.class};
            Method method = scinotesClass.getMethod("scinotesWithText", arguments);
            method.invoke(scinotesClass, new Object[]{code});

        } catch (ClassNotFoundException e) {
            System.err.println("Could not find SciNotes class");
            e.printStackTrace();
        } catch (SecurityException e) {
            System.err.println("Security error: Could not access to SciNotes class");
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            System.err.println("Could not access to scinotesWithText method from object SciNotes");
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            System.err.println("Wrong argument used with scinotesWithText method from object SciNotes");
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            System.err.println("Illegal access with scinotesWithText method from object SciNotes");
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            System.err.println("Error of invocation with scinotesWithText method from object SciNotes");
            e.printStackTrace();
        }
    }

    /**
     * @param pre the preformatted Element containing Scilab's code
     * @return the code
     */
    private static String getCode(Element pre) {
        int size = pre.getElementCount();
        Document doc = pre.getDocument();
        StringBuilder buffer = new StringBuilder();
        for (int i = 0; i < size; i++) {
            Element line = pre.getElement(i);
            int ssize = line.getElementCount();
            for (int j = 0; j < ssize; j++) {
                Element content = line.getElement(j);
                if (content.isLeaf()) {
                    try {
                        buffer.append(doc.getText(content.getStartOffset(), content.getEndOffset() - content.getStartOffset()));
                    } catch (BadLocationException e) { }
                }
            }
        }

        return buffer.toString().trim();
    }

    /**
     * Execute a file given by its path
     * @param the file path
     */
    public void exec(String path) {
        String cmd = "exec('" + path + "', -1)";
        try {
            ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(cmd, true, false);
        } catch (NoClassDefFoundError e) {
            ScilabModalDialog.show((Tab) SwingUtilities.getAncestorOfClass(Tab.class, x), Messages.gettext("Could not find the console nor the InterpreterManagement."));
        }
    }

    /**
     * Create the UI interface
     * @see javax.help.plaf.basic.BasicContentViewerUI#installUI(javax.swing.JComponent)
     * @param c The component
     */
    public void installUI(JComponent c) {
        super.installUI(c);
        this.retrievePrivateFieldFromBasicContentViewerUI();
        this.createPopupMenu(c);
    }

    /**
     * Retrieve the field "html" from BasicContentViewerUI and change
     * permission (it is private by default)
     */
    private void retrievePrivateFieldFromBasicContentViewerUI() {
        Field privateField = null;
        try {
            privateField = BasicContentViewerUI.class.getDeclaredField("html");
            privateField.setAccessible(true);
        } catch (SecurityException e) {
            System.err.println("Security error: Could not change the accessibility on the html component of the help browser.");
            System.err.println("Please submit a bug report: http://bugzilla.scilab.org");
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            System.err.println("Could not find the field of the html component of the help browser.");
            System.err.println("Please submit a bug report: http://bugzilla.scilab.org");
            e.printStackTrace();
        }

        try {
            this.accessibleHtml = (javax.swing.JEditorPane) privateField.get(this);
        } catch (IllegalArgumentException e) {
            System.err.println("Illegal argument in the retrieval of the html component of Javahelp");
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            System.err.println("Illegal access in the retrieval of the html component of Javahelp");
            e.printStackTrace();
        }
    }

    /**
     * Create the popup menu on the help
     * @param c The graphic component
     */
    private void createPopupMenu(JComponent c) {
        final JPopupMenu popup = new JPopupMenu();

        JMenuItem menuItem = null;

        /* Execute into Scilab */
        ActionListener actionListenerExecuteIntoScilab = new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                String selection = accessibleHtml.getSelectedText();
                if (selection == null) {
                    ScilabHelpBrowser.getHelpBrowser().getInfoBar().setText(Messages.gettext("No text selected"));
                } else {
                    ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(selection, true /* display */, true /* store in history */);
                }
            }
        };
        menuItem = new JMenuItem(Messages.gettext("Execute into Scilab"));
        menuItem.addActionListener(actionListenerExecuteIntoScilab);
        if (!ScilabConsole.isExistingConsole()) { /* Only available in STD mode */
            menuItem.setEnabled(false);
        }
        popup.add(menuItem);


        /* Edit in the Scilab Text Editor */
        ActionListener actionListenerLoadIntoTextEditor = new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                String selection = accessibleHtml.getSelectedText();
                if (selection == null) {
                    ScilabHelpBrowser.getHelpBrowser().getInfoBar().setText(Messages.gettext("No text selected"));
                } else {
                    edit(selection);
                }
            }
        };

        menuItem = new JMenuItem(Messages.gettext("Edit in the Scilab Text Editor"));
        try {
            Class scinotesClass = Class.forName("org.scilab.modules.scinotes.SciNotes");
        } catch (ClassNotFoundException e) {
            /* SciNotes not available */
            menuItem.setEnabled(false);
        }
        menuItem.addActionListener(actionListenerLoadIntoTextEditor);
        popup.add(menuItem);
        popup.addSeparator();

        /* Back in the history*/
        ActionListener actionListenerBackHistory = new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                DefaultHelpHistoryModel history = SwingScilabHelpBrowser.getHelpHistory();
                /* Not at the first position */
                if (history.getIndex() > 0) {
                    SwingScilabHelpBrowser.getHelpHistory().goBack();
                }
            }
        };

        menuItem = new JMenuItem(Messages.gettext("Back"));
        menuItem.addActionListener(actionListenerBackHistory);
        popup.add(menuItem);

        /* Forward in the history*/
        ActionListener actionListenerForwardHistory = new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                DefaultHelpHistoryModel history = SwingScilabHelpBrowser.getHelpHistory();
                /* Not at the last position */
                if (history.getHistory().size() != (history.getIndex() + 1)) {
                    SwingScilabHelpBrowser.getHelpHistory().goForward();
                }
            }
        };

        menuItem = new JMenuItem(Messages.gettext("Forward"));
        menuItem.addActionListener(actionListenerForwardHistory);
        popup.add(menuItem);
        popup.addSeparator();

        /* Copy */
        menuItem = new JMenuItem(new DefaultEditorKit.CopyAction());
        menuItem.setText(Messages.gettext("Copy"));
        popup.add(menuItem);
        popup.addSeparator();


        /* Select all */
        ActionListener actionListenerSelectAll = new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                accessibleHtml.selectAll();
            }
        };
        menuItem = new JMenuItem(Messages.gettext("Select All"));
        menuItem.addActionListener(actionListenerSelectAll);
        popup.add(menuItem);

        /* Edit in the Scilab Text Editor */
        final JMenuItem helpMenuItem = new JMenuItem("Help on the selected text");

        ActionListener actionListenerHelpOnKeyword= new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                String selection = accessibleHtml.getSelectedText();
                if (selection == null) {
                    ScilabHelpBrowser.getHelpBrowser().getInfoBar().setText(Messages.gettext("No text selected"));
                } else {
                    ScilabHelpBrowser.getHelpBrowser().searchKeywork(selection);
                }
            }
        };
        PropertyChangeListener listenerTextItem = new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent arg0) {
                String keyword = accessibleHtml.getSelectedText();
                if (keyword == null) {
                    helpMenuItem.setText(Messages.gettext("Help about a selected text"));
                } else {
                    int nbOfDisplayedOnlyXChar=10;
                    if (keyword.length() > nbOfDisplayedOnlyXChar) {
                        keyword = keyword.substring(0, nbOfDisplayedOnlyXChar) + "...";
                    }
                    helpMenuItem.setText(Messages.gettext("Help about '") +keyword+"'");
                }
            }
        };
        helpMenuItem.addPropertyChangeListener(listenerTextItem);
        helpMenuItem.addActionListener(actionListenerHelpOnKeyword);
        popup.add(helpMenuItem);


        /* Creates the Popupmenu on the component */
        accessibleHtml.setComponentPopupMenu(popup);
    }
}
