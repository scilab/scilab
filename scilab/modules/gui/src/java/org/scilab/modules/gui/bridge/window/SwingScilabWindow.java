/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU (Mac OS X port)
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.window;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.docking.defaults.DefaultDockingPort;
import org.flexdock.docking.defaults.DefaultDockingStrategy;
import org.flexdock.docking.drag.effects.EffectsManager;
import org.flexdock.docking.drag.preview.GhostPreview;
import org.flexdock.docking.event.hierarchy.DockingPortTracker;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.SciDockingListener;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.SimpleWindow;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.lang.reflect.InvocationTargetException;
import java.rmi.server.UID;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

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

    public static Map<String, SwingScilabWindow> allScilabWindows = Collections.synchronizedMap(new HashMap<String, SwingScilabWindow>());

    static {
        DefaultDockingStrategy.setDefaultResizeWeight(0.5);
        DefaultDockingStrategy.keepConstantPercentage(true);
    }

    private DefaultDockingPort sciDockingPort;
    private SciDockingListener sciDockingListener;
    private SimpleMenuBar menuBar;
    private SimpleToolBar toolBar;
    private SimpleTextBox infoBar;
    private String uuid;
    private int elementId; // the id of the Window which contains this SimpleWindow
    private String windowUID;
    private final boolean MAC_OS_X = (System.getProperty("os.name").toLowerCase().startsWith("mac os x"));
    private Dimension lastDimension;
    private Point lastPosition;

    /**
     * Constructor
     */
    public SwingScilabWindow() {
        super();
        this.uuid = UUID.randomUUID().toString();

        setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);

        // By default ctrl+w close the window
        ActionListener listener = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    processWindowEvent(new WindowEvent(SwingScilabWindow.this, WindowEvent.WINDOW_CLOSING));
                }
            };
        getRootPane().registerKeyboardAction(listener, ScilabKeyStroke.getKeyStroke("OSSCKEY W"), JComponent.WHEN_IN_FOCUSED_WINDOW);

        // TODO : Only for testing : Must be removed
        this.setDims(new Size(DEFAULTWIDTH, DEFAULTHEIGHT));
        this.setTitle("Scilab");
        setIconImage(new ImageIcon(ScilabSwingUtilities.findIcon("scilab", "256x256")).getImage());

        /* defining the Layout */
        super.setLayout(new java.awt.BorderLayout());

        /* Create automatically a docking port associated to the window */
        sciDockingPort = new DefaultDockingPort();

        EffectsManager.setPreview(new GhostPreview());

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
        //this.setFocusable(false);

        // let the OS choose the window position if not specified by user.
        setLocationByPlatform(true);

        addWindowListener(new WindowAdapter() {
                @Override
                public void windowClosing(WindowEvent e) {
                    ClosingOperationsManager.startClosingOperation(SwingScilabWindow.this);
                }
            });

        addComponentListener(new ComponentAdapter() {
                @Override
                public void componentResized(ComponentEvent e) {
                    if (getExtendedState() == NORMAL) {
                        lastDimension = getSize();
                    }
                }

                @Override
                public void componentMoved(ComponentEvent e) {
                    if (getExtendedState() == NORMAL) {
                        lastPosition = getLocation();
                    }
                }
            });

        if (MAC_OS_X) {
            registerForMacOSXEvents();
        }

        windowUID = new UID().toString();

        sciDockingListener.setAssociatedWindowId(windowUID);

        allScilabWindows.put(windowUID, this);
    }

    /**
     * Get the last dimension of the window before MAXIMIZED or MINIMIZED
     * @return the last dimension
     */
    public Dimension getLastDimension() {
        if (lastDimension == null) {
            return getSize();
        }

        return lastDimension;
    }

    /**
     * Get the last position of the window before MAXIMIZED or MINIMIZED
     * @return the last position
     */
    public Point getLastPosition() {
        if (lastPosition == null) {
            return getLocation();
        }

        return lastPosition;
    }

    /**
     * This method registers some methods against the specific Mac OS X API
     * (in order to set the "special" mac os x menus)
     */
    private void registerForMacOSXEvents() {
        try {
            // Generate and register the OSXAdapter, passing it a hash of all the methods we wish to
            // use as delegates for various com.apple.eawt.ApplicationListener methods
            OSXAdapter.setAboutHandler(this, getClass().getDeclaredMethod("macosxAbout", (Class[]) null));
            OSXAdapter.setQuitHandler(this, getClass().getDeclaredMethod("macosxQuit", (Class[]) null));
            OSXAdapter.setPreferencesHandler(this, getClass().getDeclaredMethod("macosxPreferences", (Class[]) null));
            OSXAdapter.setDockIcon(new ImageIcon(ScilabSwingUtilities.findIcon("puffin", "256x256")));
        } catch (java.lang.NoSuchMethodException e) {
            System.err.println("OSXAdapter could not find the method: " + e.getLocalizedMessage());
        }
    }

    /**
     * This method is called by the OSXAdapter class when the specific Mac
     * OS X "About" menu is called. It is the only case where this method
     * should be used
     */
    public void macosxAbout() {
        InterpreterManagement.requestScilabExec("about();");
    }

    /**
     * This method is called by the OSXAdapter class when the specific Mac
     * OS X "Quit Scilab" menu is called. It is the only case where this method
     * should be used
     */
    public boolean macosxQuit() {
        InterpreterManagement.requestScilabExec("exit();");
        return false;
    }

    /**
     * This method is called by the OSXAdapter class when the specific Mac
     * OS X "Preferences" menu is called. It is the only case where this method
     * should be used
     */
    public void macosxPreferences() {
        InterpreterManagement.requestScilabExec("preferences();");
    }

    /**
     * @return the UUID associated with this window
     */
    public String getUUID() {
        return uuid;
    }

    /**
     * @param uuid the UUID associated with this window
     */
    public void setUUID(String uuid) {
        this.uuid = uuid;
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
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    @Override
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Private method to raise to the front the window
     */
    private void raiseToFront() {
        // force visibility
        setVisible(true);

        // deiconify the window if needed
        setState(NORMAL);

        // put it in front of others
        toFront();
    }

    /**
     * Deiconify the window and put it in front of other window
     */
    @Override
    public void raise() {
        // blocking call. So graphic synchronization must be desactivated here.
        if (!SwingUtilities.isEventDispatchThread()) {
            /* javasci bug: See bug 9544 why we are doing this check */
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                        @Override
                        public void run() {
                            raiseToFront();
                        }
                    });
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
        } else {
            raiseToFront();
        }
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab window
     * @return the dimensions of the window
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    @Override
    public Size getDims() {
        return new Size(getSize().width, getSize().height);
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab window
     * @param newWindowSize the dimensions to set to the window
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    @Override
    public void setDims(Size newWindowSize) {
        //if (!SwingUtilities.isEventDispatchThread()) {
        if (getDims().getWidth() != newWindowSize.getWidth() || getDims().getHeight() != newWindowSize.getHeight()) {
            Dimension finalDim = new Dimension(newWindowSize.getWidth(), newWindowSize.getHeight());

            setSize(finalDim);
            // validate so the new values are taken into account immediately
            validate();
        }
        //}
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab window
     * @return the position of the window
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    @Override
    public Position getPosition() {
        return new Position(this.getX(), this.getY());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab window
     * @param newWindowPosition the position to set to the window
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    @Override
    public void setPosition(Position newWindowPosition) {
        //if (!SwingUtilities.isEventDispatchThread()) {
        if (getPosition().getX() != newWindowPosition.getX() || getPosition().getY() != newWindowPosition.getY()) {
            this.setLocation(newWindowPosition.getX(), newWindowPosition.getY());
        }
        //}
    }

    /**
     * Gets the title of a swing Scilab window
     * @return the title of the window
     * @see javax.swing.JFrame#getTitle()
     */
    @Override
    public String getTitle() {
        return super.getTitle();
    }

    /**
     * Sets the title of a swing Scilab window
     * @param newWindowTitle the title to set to the window
     * @see java.awt.Frame#setTitle(java.lang.String)
     */
    @Override
    public void setTitle(String newWindowTitle) {
        // set only if required
        if (newWindowTitle != null && !newWindowTitle.equals(getTitle())) {
            super.setTitle(newWindowTitle);
        }
    }

    /**
     * {@inheritedDoc}
     */
    @Override
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
        return sciDockingPort;
    }

    /**
     * Add a Scilab tab to a Scilab window
     * @param newTab the Scilab tab to add to the Scilab window
     * @see org.scilab.modules.gui.window.Window#addTab(org.scilab.modules.gui.tab.Tab)
     */
    @Override
    public void addTab(SimpleTab newTab) {
        SwingScilabTab tab = (SwingScilabTab) newTab;
        tab.setParentWindowId(this.windowUID);
        DockingManager.dock(tab, this.getDockingPort());
        ActiveDockableTracker.requestDockableActivation(tab);
    }

    /**
     * Remove a Scilab tab from a Scilab window
     * @param tabs the Scilab tabs to remove from the Scilab window
     * @see org.scilab.modules.gui.window.Window#removeTab(org.scilab.modules.gui.tab.Tab)
     */
    public void removeTabs(SwingScilabTab[] tabs) {
        for (SwingScilabTab tab : tabs) {
            DockingManager.unregisterDockable((Dockable) tab);
            tab.close();
            DockingManager.close(tab);
        }

        if (getDockingPort() == null || getDockingPort().getDockables().isEmpty()) {
            // remove xxxBars
            if (toolBar != null) {
                ((SwingScilabToolBar) toolBar).close();
            }
            if (menuBar != null) {
                ((SwingScilabMenuBar) menuBar).close();
            }

            // clean all
            this.removeAll();
            close();
        } else {
            /* Make sur a Tab is active */
            Set<SwingScilabTab> docks = sciDockingPort.getDockables();
            Iterator<SwingScilabTab> it = docks.iterator();
            ActiveDockableTracker.requestDockableActivation(it.next());
        }
    }

    /**
     * Remove a Scilab tab from a Scilab window
     * @param tab the Scilab tab to remove from the Scilab window
     * @see org.scilab.modules.gui.window.Window#removeTab(org.scilab.modules.gui.tab.Tab)
     */
    @Override
    public void removeTab(SimpleTab tab) {
        removeTabs(new SwingScilabTab[] {(SwingScilabTab) tab});
    }

    /**
     * Sets a Scilab MenuBar to a Scilab window
     * @param newMenuBar the Scilab MenuBar to add to the Scilab window
     * @see org.scilab.modules.gui.window.Window#addMenuBar(org.scilab.modules.gui.menubar.MenuBar)
     */
    @Override
    public void addMenuBar(MenuBar newMenuBar) {
        if (newMenuBar == null) {
            if (this.menuBar != null) {
                this.menuBar = null;
                super.setJMenuBar(null);
            }
            // else nothing to do both are null
        } else {
            if (this.menuBar != newMenuBar.getAsSimpleMenuBar()) {
                this.menuBar = newMenuBar.getAsSimpleMenuBar();
                super.setJMenuBar((SwingScilabMenuBar) newMenuBar.getAsSimpleMenuBar());
            }
            //  else nothing to do element alredy set
        }
    }

    /**
     * Sets a Scilab ToolBar to a Scilab window
     * @param newToolBar the Scilab ToolBar to set to the Scilab window
     * @see org.scilab.modules.gui.window.Window#addToolBar(org.scilab.modules.gui.toolbar.ToolBar)
     */
    @Override
    public void addToolBar(ToolBar newToolBar) {
        if (newToolBar == null) {
            if (this.toolBar != null) {
                // Remove old InfoBar if already set
                super.remove((SwingScilabToolBar) this.toolBar);
                this.toolBar = null;
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
            }
            //  else nothing to do element alredy set
        }
    }

    /**
     * Sets a Scilab InfoBar to a Scilab window
     * @param newInfoBar the Scilab InfoBar to set to the Scilab window
     * @see org.scilab.modules.gui.window.Window#addInfoBar(org.scilab.modules.gui.textbox.TextBox)
     */
    @Override
    public void addInfoBar(TextBox newInfoBar) {
        if (newInfoBar == null) {
            if (this.infoBar != null) {
                // Remove old InfoBar if already set
                super.remove((SwingScilabTextBox) this.infoBar);
                this.infoBar = null;
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
            }
            //  else nothing to do element alredy set
        }
    }

    /**
     * Get the element id for this window
     * @return id the id of the corresponding window object
     */
    @Override
    public int getElementId() {
        return elementId;
    }

    /**
     * Set the element id for this window
     * @param id the id of the corresponding window object
     */
    @Override
    public void setElementId(int id) {
        this.elementId = id;
        //sciDockingListener.setAssociatedWindowId(id);
    }

    /**
     * Close the window
     * @see org.scilab.modules.gui.window.SimpleWindow#close()
     */
    @Override
    public void close() {
        try {
            dispose();
            // disable docking port
            ActiveDockableTracker.getTracker(this).setActive(null);
            if (sciDockingPort != null) {
                sciDockingPort.removeDockingListener(sciDockingListener);
                sciDockingPort = null;
                sciDockingListener = null;
            }
            DockingPortTracker.remove(this);
        } catch (IllegalStateException e) {
            enableInputMethods(false);
        }
        allScilabWindows.remove(windowUID);
    }

    /**
     * @return number of objects (tabs) docked in this window
     */
    @Override
    public int getNbDockedObjects() {
        return sciDockingPort.getDockables().size();
    }

    /**
     * Update the dimension of the window and its component.
     * Only useful when the window is not yet visible
     */
    @Override
    public void updateDimensions() {
        pack();
    }

    /**
     * DeIconify Window
     */
    @Override
    public void windowDeiconified() {
        super.setState(Frame.NORMAL);
    }

    /**
     * Iconify Window
     */
    @Override
    public void windowIconified() {
        super.setState(Frame.ICONIFIED);
    }

    /**
     * Maximized Window
     */
    @Override
    public void windowMaximized() {
        super.setExtendedState(Frame.MAXIMIZED_BOTH);
    }

    /**
     * Window is in the "normal" state.
     */
    @Override
    public void windowNormal() {
        super.setState(Frame.NORMAL);
    }

    /**
     * Get the window UID
     * @return the UID
     */
    public String getId() {
        return windowUID;
    }
}
