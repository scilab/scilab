/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU (Mac OS X port)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.window;

import java.awt.Dimension;
import java.awt.Frame;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.lang.reflect.InvocationTargetException;
import java.util.Iterator;
import java.util.Set;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingConstants;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.docking.defaults.DefaultDockingPort;
import org.flexdock.view.View;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.SciDockingListener;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.SimpleWindow;
import org.scilab.modules.renderer.utils.RenderingCapabilities;

/**
 * Swing implementation for Scilab windows in GUIs
 * This implementation uses FlexDock package
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 * @author Marouane BEN JELLOUL
 * @author Sylvestre LEDRU (Mac OS X port)

 */
public class SwingScilabWindow extends JFrame implements SimpleWindow {

        private static final long serialVersionUID = -5661926417765805660L;

        private static final int DEFAULTWIDTH = 500;
        private static final int DEFAULTHEIGHT = 500;

        private DefaultDockingPort sciDockingPort;
        private SciDockingListener sciDockingListener;
        private SimpleMenuBar menuBar;
        private SimpleToolBar toolBar;
        private SimpleTextBox infoBar;

        private int elementId; // the id of the Window which contains this SimpleWindow
        boolean MAC_OS_X = (System.getProperty("os.name").toLowerCase().startsWith("mac os x"));

        /**
         * Constructor
         */
        public SwingScilabWindow() {
                super();

                setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);

                // TODO : Only for testing : Must be removed
                this.setDims(new Size(DEFAULTWIDTH, DEFAULTHEIGHT));
                this.setTitle("Scilab");
                setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());

                /* defining the Layout */
                super.setLayout(new java.awt.BorderLayout());

                /* Create automatically a docking port associated to the window */
                sciDockingPort = new DefaultDockingPort();

                //EffectsManager.setPreview(new GhostPreview());

                /* The docking port is the center of the Layout of the Window */
                super.add(sciDockingPort, java.awt.BorderLayout.CENTER);

                /* there is no menuBar, no toolBar and no infoBar at creation */
                this.menuBar = null;
                this.toolBar = null;
                this.infoBar = null;

                sciDockingListener = new SciDockingListener();
                sciDockingPort.addDockingListener(sciDockingListener);
                /*
                 * Prevent the background RootPane to catch Focus.
                 * Causes trouble with Scicos use xclick & co.
                 */
                this.setFocusable(false);

                // let the OS choose the window position if not specified by user.
                setLocationByPlatform(true);

                addWindowListener(new WindowAdapter() {
                        public void windowClosing(WindowEvent e) {
                                Object[] dockArray = sciDockingPort.getDockables().toArray();
                                for (int i = 0; i < dockArray.length; i++) {
                                        ((View) dockArray[i]).getActionButton(DockingConstants.CLOSE_ACTION).getAction().actionPerformed(null);
                                }

                                //if dock stay open, do not close main window
                                if(dockArray.length == 0){
                                        removeWindowListener(this);
                                }
                        }
                });

                if (MAC_OS_X) {
                        registerForMacOSXEvents();
                }
        }

    /**
     * This method registers some methods against the specific Mac OS X API
     * (in order to set the "special" mac os x menus)
     */
    private void registerForMacOSXEvents() {
            try {
                        // Generate and register the OSXAdapter, passing it a hash of all the methods we wish to
                        // use as delegates for various com.apple.eawt.ApplicationListener methods
                        OSXAdapter.setAboutHandler(this, getClass().getDeclaredMethod("OSXabout", (Class[])null));
                        OSXAdapter.setQuitHandler(this, getClass().getDeclaredMethod("OSXquit", (Class[])null));
                        OSXAdapter.setDockIcon(System.getenv("SCI") + "/icons/puffin.png");
            } catch (java.lang.NoSuchMethodException e) {
                        System.err.println("OSXAdapter could not find the method: "+e.getLocalizedMessage());
            }
    }

    /**
     * This method is called by the OSXAdapter class when the specific Mac
     * OS X about menu is called. It is the only case where this method
     * should be used
     */
    public void OSXabout() {
        InterpreterManagement.requestScilabExec("about()");
    }

    public void OSXquit() {
        InterpreterManagement.requestScilabExec("exit()");
    }



        /**
         * Creates a swing Scilab window
         * @return the created window
         */
        public static SimpleWindow createWindow() {
                return new SwingScilabWindow();
        }

        /**
         * Draws a swing Scilab window
         * @see org.scilab.modules.gui.UIElement#draw()
         */
        public void draw() {
                this.setVisible(true);
                this.doLayout();
        }

        /**
         * Deiconify the window and put it in front of other window
         */
        public void raise() {
                // blocking call. So graphic synchronization must be desactivated here.
                try {
                        SwingUtilities.invokeAndWait(new Runnable() {
                                public void run() {
                                        // force visibility
                                        setVisible(true);

                                        // deiconify the window if needed
                                        setState(NORMAL);

                                        // put it in front of others
                                        toFront();
                                }
                        });
                } catch (InterruptedException e) {
                        e.printStackTrace();
                } catch (InvocationTargetException e) {
                        e.printStackTrace();
                }

        }

        /**
         * Gets the dimensions (width and height) of a swing Scilab window
         * @return the dimensions of the window
         * @see org.scilab.modules.gui.UIElement#getDims()
         */
        public Size getDims() {
                return new Size(getSize().width, getSize().height);
        }

        /**
         * Sets the dimensions (width and height) of a swing Scilab window
         * @param newWindowSize the dimensions to set to the window
         * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
         */
        public void setDims(Size newWindowSize) {

                // get the greatest size we can use
                int[] maxSize = RenderingCapabilities.getMaxWindowSize();

                // make suze size is not greater than the max size
                Dimension finalDim = new Dimension(Math.min(newWindowSize.getWidth(), maxSize[0]),
                                                                                   Math.min(newWindowSize.getHeight(), maxSize[1]));

                setSize(finalDim);
                // validate so the new values are taken into account immediately
                validate();
        }

        /**
         * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab window
         * @return the position of the window
         * @see org.scilab.modules.gui.UIElement#getPosition()
         */
        public Position getPosition() {
                return new Position(this.getX(), this.getY());
        }

        /**
         * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab window
         * @param newWindowPosition the position to set to the window
         * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
         */
        public void setPosition(Position newWindowPosition) {
                this.setLocation(newWindowPosition.getX(), newWindowPosition.getY());
        }

        /**
         * Gets the title of a swing Scilab window
         * @return the title of the window
         * @see java.awt.Frame#getTitle(java.lang.String)
         */
        public String getTitle() {
                return super.getTitle();
        }

        /**
         * Sets the title of a swing Scilab window
         * @param newWindowTitle the title to set to the window
         * @see java.awt.Frame#setTitle(java.lang.String)
         */
        public void setTitle(String newWindowTitle) {
                // set only if required
                if (newWindowTitle != null && !newWindowTitle.equals(getTitle())) {
                        super.setTitle(newWindowTitle);
                }
        }

        /**
         * {@inheritedDoc}
         */
        public void setName(String name) {
            super.setName(name);
            setTitle(name);
        }

        /**
         * Gets the docking port associated to the window (created by default at window creation)
         * @return the docking port associated to the window
         */
        public DockingPort getDockingPort() {
                //return (DockingPort) centerFrame.getContentPane();
                return (DockingPort) sciDockingPort;
        }

        /**
         * Add a Scilab tab to a Scilab window
         * @param newTab the Scilab tab to add to the Scilab window
         * @see org.scilab.modules.gui.window.Window#addTab(org.scilab.modules.gui.tab.Tab)
         */
        public void addTab(Tab newTab) {
                final SwingScilabTab tabImpl = ((SwingScilabTab) newTab.getAsSimpleTab());

                tabImpl.setParentWindowId(this.elementId);
                DockingManager.dock(tabImpl, this.getDockingPort());
                ActiveDockableTracker.requestDockableActivation(tabImpl);
        }

        /**
         * Remove a Scilab tab from a Scilab window
         * @param tab the Scilab tab to remove from the Scilab window
         * @see org.scilab.modules.gui.window.Window#removeTab(org.scilab.modules.gui.tab.Tab)
         */
        public void removeTab(Tab tab) {
                DockingManager.close(((SwingScilabTab) tab.getAsSimpleTab()));
                DockingManager.unregisterDockable((Dockable) ((SwingScilabTab) tab.getAsSimpleTab()));
                ((SwingScilabTab) tab.getAsSimpleTab()).close();
                if (getDockingPort().getDockables().isEmpty()) {
                        // remove xxxBars
                        if (toolBar != null) {
                                ((SwingScilabToolBar) toolBar).close();
                        }
                        if (menuBar != null) {
                                UIElementMapper.removeMapping(menuBar.getElementId());
                        }
                        UIElementMapper.removeMapping(this.elementId);

                        // clean all
                        this.removeAll();
                        this.dispose();

                        // disable docking port
                        ActiveDockableTracker.getTracker(this).setActive(null);
                        sciDockingPort.removeDockingListener(sciDockingListener);
                        sciDockingPort = null;
                        sciDockingListener = null;
                } else {
                        /* Make sur a Tab is active */
                        Set<SwingScilabTab> docks = sciDockingPort.getDockables();
                        Iterator<SwingScilabTab> it = docks.iterator();
                        ActiveDockableTracker.requestDockableActivation((SwingScilabTab) it.next());
                }
        }



        /**
         * Sets a Scilab MenuBar to a Scilab window
         * @param newMenuBar the Scilab MenuBar to add to the Scilab window
         * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
         */
        public void addMenuBar(MenuBar newMenuBar) {

                if (newMenuBar == null) {
                        if (this.menuBar != null) {
                                this.menuBar = null;
                                super.setJMenuBar(null);
                                this.repaint();
                        }
                        // else nothing to do both are null
                } else {
                        if (this.menuBar != newMenuBar.getAsSimpleMenuBar()) {
                                this.menuBar = newMenuBar.getAsSimpleMenuBar();
                                super.setJMenuBar((SwingScilabMenuBar) newMenuBar.getAsSimpleMenuBar());
                                this.repaint();
                        }
                        //  else nothing to do element alredy set
                }
        }

        /**
         * Sets a Scilab ToolBar to a Scilab window
         * @param newToolBar the Scilab ToolBar to set to the Scilab window
         * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.toolbar.ToolBar)
         */
        public void addToolBar(ToolBar newToolBar) {

                if (newToolBar == null) {
                        if (this.toolBar != null) {
                                // Remove old InfoBar if already set
                                super.remove((SwingScilabToolBar) this.toolBar);
                                this.toolBar = null;
                                this.repaint();
                        }
                        // else nothing to do both are null
                } else {
                        if (this.toolBar != newToolBar.getAsSimpleToolBar()) {
                                if (this.toolBar != null) {
                                        // Remove old InfoBar if already set
                                        super.remove((SwingScilabToolBar) this.toolBar);
                                }
                                this.toolBar = newToolBar.getAsSimpleToolBar();
                                super.add((SwingScilabToolBar) this.toolBar, java.awt.BorderLayout.PAGE_START);
                                this.repaint();
                        }
                        //  else nothing to do element alredy set
                }
        }

        /**
         * Sets a Scilab InfoBar to a Scilab window
         * @param newInfoBar the Scilab InfoBar to set to the Scilab window
         * @see org.scilab.modules.gui.window.Window#setInfoBar(org.scilab.modules.gui.textbox.TextBox)
         */
        public void addInfoBar(TextBox newInfoBar) {

                if (newInfoBar == null) {
                        if (this.infoBar != null) {
                                // Remove old InfoBar if already set
                                super.remove((SwingScilabTextBox) this.infoBar);
                                this.infoBar = null;
                                this.repaint();
                        }
                        // else nothing to do both are null
                } else {
                        if (this.infoBar != newInfoBar.getAsSimpleTextBox()) {
                                if (this.infoBar != null) {
                                        // Remove old InfoBar if already set
                                        super.remove((SwingScilabTextBox) this.infoBar);
                                }
                                this.infoBar = newInfoBar.getAsSimpleTextBox();
                                super.add((SwingScilabTextBox) this.infoBar, java.awt.BorderLayout.PAGE_END);
                                this.repaint();
                        }
                        //  else nothing to do element alredy set
                }
        }

        /**
         * Get the element id for this window
         * @return id the id of the corresponding window object
         */
        public int getElementId() {
                return elementId;
        }

        /**
         * Set the element id for this window
         * @param id the id of the corresponding window object
         */
        public void setElementId(int id) {
                this.elementId = id;
                sciDockingListener.setAssociatedWindowId(id);
        }

        /**
         * Close the window
         * @see org.scilab.modules.gui.window.SimpleWindow#close()
         */
        public void close() {
                dispose();
        }

        /**
         * @return number of objects (tabs) docked in this window
         */
        public int getNbDockedObjects() {
                return sciDockingListener.getNbDockedObjects();
        }

        /**
         * Update the dimension of the window and its component.
         * Only useful when the window is not yet visible
         */
        public void updateDimensions() {
                pack();
        }

        /**
        * DeIconify Window
        */
        public void windowDeiconified() {
                super.setState(Frame.NORMAL);
        }

        /**
        * Iconify Window
        */
        public void windowIconified() {
                super.setState(Frame.ICONIFIED);
        }

        /**
        * Maximized Window
        */
        public void windowMaximized() {
                super.setExtendedState(Frame.MAXIMIZED_BOTH);
        }

        /**
         * Window is in the "normal" state.
         */
        public void windowNormal() {
                super.setState(Frame.NORMAL);
        }
}
