/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
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

package org.scilab.modules.gui.bridge.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CLOSEREQUESTFCN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.HierarchyBoundsListener;
import java.awt.event.HierarchyEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.lang.reflect.InvocationTargetException;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JLayeredPane;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import org.flexdock.docking.DockingConstants;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.props.PropertyChangeListenerFactory;
import org.flexdock.view.Titlebar;
import org.flexdock.view.View;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.helpbrowser.SwingScilabHelpBrowser;
import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabScroll;
import org.scilab.modules.gui.bridge.tree.SwingScilabTree;
import org.scilab.modules.gui.bridge.uidisplaytree.SwingScilabUiDisplayTree;
import org.scilab.modules.gui.bridge.uitable.SwingScilabUiTable;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.editor.EditorEventListener;
import org.scilab.modules.gui.events.GlobalEventWatcher;
import org.scilab.modules.gui.events.ScilabEventListener;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.uidisplaytree.UiDisplayTree;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.SciClosingAction;
import org.scilab.modules.gui.utils.SciHelpOnComponentAction;
import org.scilab.modules.gui.utils.SciUndockingAction;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.Widget;

/**
 * Swing implementation for Scilab tabs in GUIs This implementation uses
 * FlexDock package
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 * @author Jean-Baptiste SILVY
 */

public class SwingScilabDockablePanel extends View implements SimpleTab, FocusListener, KeyListener, SwingScilabPanel {

    private static final Image SCILAB_ICON = new ImageIcon(FindIconHelper.findIcon("scilab", "256x256")).getImage();

    private static final long serialVersionUID = 1L;

    private static final String UNDOCK = "undock";
    private static final String HELP = "help";
    protected boolean hasLayout = false;

    private Integer id;

    private boolean eventEnabled = false;
    ComponentListener componentListener;

    static {
        PropertyChangeListenerFactory.addFactory(new BarUpdater.UpdateBarFactory());
    }

    private String parentWindowId;
    private MenuBar menuBar;
    private ToolBar toolBar;
    private TextBox infoBar;
    private String helpID;
    private boolean paintEnable = true;

    /** Contains the canvas and widgets */
    private JLayeredPane uiContentPane;
    private JLayeredPane layeredPane;

    /** Scroll the axes */
    private SwingScilabScrollPane scrolling;

    private Image icon;

    private Action closeAction;

    /** The listener for event handling */
    private ScilabEventListener eventHandler;
    private EditorEventListener editorEventHandler = null;

    /** A reference to the canvas used for event handling management */
    private SwingScilabCanvas contentCanvas = null;

    private Dimension deltaSize = null;

    /**
     * Constructor
     * @param name the name of the tab
     * @param uuid an uuid to identify the tab
     */
    public SwingScilabDockablePanel(String name, String uuid) {
        super(uuid, name, name);
        //This button is "overloaded" when we add a callback
        //this.addAction(DockingConstants.CLOSE_ACTION);
        // Removed because make JOGL crash when "Unpin"
        //this.addAction(DockingConstants.PIN_ACTION);
        this.addAction(DockingConstants.ACTIVE_WINDOW);

        // no need for an axes
        contentPane = null;
        uiContentPane = null;
        scrolling = null;

        this.setVisible(true);

        getTitlebar().addFocusListener(this);
        addFocusListener(this);
        setCallback(null);

        getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("shift F6"), ACTION_TOGGLE_PREVIOUS);
    }

    /**
     * Create a graphic tab used to display a figure with 3D graphics and/or
     * UIcontrols
     * @param name name of the tab
     * @param figureId id of the displayed figure
     */
    public SwingScilabDockablePanel(String name, int figureId) {
        super(name, name, name);

        // This button is "overloaded" when we add a callback
        //this.addAction(DockingConstants.CLOSE_ACTION);
        // Removed because make JOGL crash when "Unpin"
        //this.addAction(DockingConstants.PIN_ACTION);
        this.addAction(DockingConstants.ACTIVE_WINDOW);

        // create the panel in which all the uiobjects will lie.
        //contentPane = new SwingScilabAxes(figureId);

        // add it inside a JSCrollPane
        //scrolling = new SwingScilabScrollPane(contentPane);
        //scrolling.setBackground(1, 0, 0);
        // put in in the back of the tab
        //setContentPane(scrolling.getAsContainer());

        this.setVisible(true);

        getTitlebar().addFocusListener(this);
        addFocusListener(this);
        setCallback(null);

        getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("shift F6"), ACTION_TOGGLE_PREVIOUS);
    }

    /**
     * Constructor
     * @param name the name of the tab (used to identify it)
     */
    public SwingScilabDockablePanel(String name) {
        this(name, name);
    }

    /**
     * Set the associated help page
     * @param helpID the xml id of the associated help page
     */
    public void setAssociatedXMLIDForHelp(String helpID) {
        this.helpID = helpID;
    }

    /**
     * Get the associated help page
     * @return the xml id of the associated help page
     */
    public String getAssociatedXMLIDForHelp() {
        return helpID;
    }

    public static void removeActions(SwingScilabDockablePanel tab) {
        tab.setActionBlocked(DockingConstants.CLOSE_ACTION, true);
        tab.setActionBlocked(UNDOCK, true);
        if (tab.getTitlebar() != null) {
            tab.getTitlebar().revalidate();
        }
    }

    public static void addActions(SwingScilabDockablePanel tab) {
        tab.setActionBlocked(DockingConstants.CLOSE_ACTION, false);
        tab.setActionBlocked(UNDOCK, false);
        tab.getTitlebar().revalidate();
    }

    public SwingScilabDockablePanel(String figureTitle, int figureId, final Figure figure) {
        this(figureTitle, figureId);
        /* OpenGL context */
        //SwingScilabCanvas canvas = new SwingScilabCanvas(figureId, figure);
        //contentCanvas = canvas;

        editorEventHandler = new EditorEventListener(figure.getIdentifier());

        layeredPane = new JLayeredPane();
        layeredPane.setLayout(null);
        layeredPane.setBorder(null);
        layeredPane.setOpaque(true);

        uiContentPane = new JLayeredPane();
        uiContentPane.setOpaque(false);
        uiContentPane.setLayout(null);
        uiContentPane.setBorder(null);

        layeredPane.add(uiContentPane, JLayeredPane.DEFAULT_LAYER + 1, 0);
        scrolling = new SwingScilabScrollPane(layeredPane, uiContentPane, figure);

        setContentPane(scrolling.getAsContainer());

        uiContentPane.setVisible(true);

        /* Manage figure_position property */
        addHierarchyBoundsListener(new HierarchyBoundsListener() {
            public void ancestorResized(HierarchyEvent arg0) {
            }

            public void ancestorMoved(HierarchyEvent e) {
                if (e.getChanged() instanceof SwingScilabWindow) {
                    Position parentPosition = SwingScilabWindow.allScilabWindows.get(parentWindowId).getPosition();
                    Integer[] newPosition = new Integer[] { parentPosition.getX(), parentPosition.getY() };
                    GraphicController.getController().setProperty(id, __GO_POSITION__, newPosition);
                }
            }
        });

        /* Manage figure_size property */
        componentListener = new ComponentListener() {
            public void componentShown(ComponentEvent arg0) {
            }

            public void componentResized(ComponentEvent arg0) {

                /* Update the figure_size property */
                Size parentSize = SwingScilabWindow.allScilabWindows.get(parentWindowId).getDims();
                Integer[] newSize = new Integer[] { parentSize.getWidth(), parentSize.getHeight() };

                GraphicController.getController().setProperty(id, __GO_SIZE__, newSize);

                Boolean autoResize = (Boolean) GraphicController.getController().getProperty(id, __GO_AUTORESIZE__);

                if (autoResize != null && autoResize) {
                    /* Update the axes_size property */
                    Integer[] newAxesSize = new Integer[] { getContentPane().getWidth(), getContentPane().getHeight() };
                    GraphicController.getController().setProperty(id, __GO_AXES_SIZE__, newAxesSize);
                }

                String resizeFcn = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_RESIZEFCN__);
                if (resizeFcn != null && !resizeFcn.equals("")) {
                    String resizeCommand = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;" + "gcbo = getcallbackobject(" + id + ");" + resizeFcn
                                           + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;";
                    InterpreterManagement.requestScilabExec(resizeCommand);
                } else if (hasLayout == false) {
                    for (Component comp : getWidgetPane().getComponents()) {
                        if (comp instanceof Widget) {
                            Widget widget = (Widget) comp;
                            SwingViewObject obj = (SwingViewObject) comp;
                            SwingViewWidget.update(widget, __GO_POSITION__, GraphicController.getController().getProperty(obj.getId(), __GO_POSITION__));
                        }
                    }
                }
            }

            public void componentMoved(ComponentEvent arg0) {
            }

            public void componentHidden(ComponentEvent arg0) {
            }
        };

        addComponentListener(componentListener);
        /* Manage closerequestfcn */
        ClosingOperationsManager.registerClosingOperation(SwingScilabDockablePanel.this, new ClosingOperationsManager.ClosingOperation() {

            public int canClose() {
                String closeRequestFcn = (String) GraphicController.getController().getProperty(getId(), __GO_CLOSEREQUESTFCN__);
                if (!closeRequestFcn.equals("")) {
                    String closeCommand = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
                                          + "gcbo = getcallbackobject(" + getId() + ");"
                                          + closeRequestFcn + ";fire_closing_finished();"
                                          + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;";
                    InterpreterManagement.putCommandInScilabQueue(closeCommand);
                    return -1;
                } else {
                    closeAction.actionPerformed(null);
                    return 1;
                }
            }

            public void destroy() {
            }

            public String askForClosing(final List<SwingScilabDockablePanel> list) {
                return null;
            }

            @Override
            public void updateDependencies(List<SwingScilabDockablePanel> list, ListIterator<SwingScilabDockablePanel> it) {

            }
        });
        ClosingOperationsManager.addDependencyWithRoot(this);
    }

    /**
     * @param e the FocusEvent
     */
    @Override
    public void focusGained(FocusEvent e) {
        //ActiveDockableTracker.requestDockableActivation(this);
        if (contentPane != null) {
            contentPane.requestFocus();
        } else if (getContentPane() != null) {
            getContentPane().requestFocus();
        } else {
            SwingScilabDockablePanel.this.requestFocusInWindow();
        }
    }

    /**
     * Call when the tab restoration is ended.
     */
    public void endedRestoration() {
    }

    /**
     * @return the window icon associated with this tab
     */
    public Image getWindowIcon() {
        if (icon == null) {
            return SCILAB_ICON;
        } else {
            return icon;
        }
    }

    /**
     * @param icon the window icon associated with this tab
     */
    public void setWindowIcon(Image icon) {
        this.icon = icon;
    }

    /**
     * @param iconName window icon associated with this tab
     */
    public void setWindowIcon(String iconName) {
        setWindowIcon(new ImageIcon(FindIconHelper.findIcon(iconName, "256x256")).getImage());
    }

    /**
     * @param e the FocusEvent
     */
    @Override
    public void focusLost(FocusEvent e) {
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void dockingComplete(DockingEvent evt) {
        super.dockingComplete(evt);

        DockingPort port = evt.getNewDockingPort();
        SwingScilabWindow win = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (Component) port);
        Iterator iter = port.getDockables().iterator();

        if (port.getDockables().size() > 1) {
            while (iter.hasNext()) {
                Object d = iter.next();
                if (d instanceof SwingScilabDockablePanel) {
                    SwingScilabDockablePanel view = (SwingScilabDockablePanel) d;
                    addActions(view);
                }
            }
        } else {
            removeActions(this);
        }

        if (win != null) {
            setParentWindowId(win.getId());
        } else {
            // Should not occur
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    if (getParentWindow() != null) {
                        setParentWindowId(getParentWindow().getId());
                    } else {
                        System.err.println("No window for tab:" + SwingScilabDockablePanel.this.getClass().getName() + " after docking complete");
                    }
                }
            });
        }
    }

    /**
     * Sets the Name of a swing Scilab tab
     * @param newTabName the Name of the tab
     * @see org.scilab.modules.gui.tab.ScilabTab#setName(String)
     */
    @Override
    public void setName(String newTabName) {
        setTitle(newTabName, true);
        getTitlePane().repaint();
        SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this).setName(newTabName);
    }

    /**
     * @return the UUID of the parent window
     */
    public String getParentWindowUUID() {
        return ((SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this)).getUUID();
    }

    /**
     * @return the UUID of the parent window
     */
    public SwingScilabWindow getParentWindow() {
        return (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this);
    }

    /**
     * Gets the title of a swing Scilab tab
     * @return the title of the tab
     * @see org.scilab.modules.gui.tab.ScilabTab#getName()
     */
    @Override
    public String getName() {
        return this.getTitle();
    }

    /**
     * Paint immediately this component
     */
    public void paintImmediately() {
        // paint all
        paintImmediately(0, 0, getWidth(), getHeight());
    }

    /**
     * Draws a swing Scilab tab
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    @Override
    public void draw() {
        if (SwingUtilities.isEventDispatchThread()) {
            setVisible(true);
            paintImmediately();
        } else {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    @Override
                    public void run() {
                        setVisible(true);
                        paintImmediately();
                    }
                });
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
        }

    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab tab
     * @return the dimensions of the tab
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    @Override
    public Size getDims() {
        return new Size(this.getSize().width, this.getSize().height);
    }

    /**
     * Get the size for the axes
     * @return size of the axes in pixels
     */
    @Override
    public Size getAxesSize() {
        return new Size(contentPane.getWidth(), contentPane.getHeight());
    }

    /**
     * @param newSize new size to set for the axes
     */
    @Override
    public void setAxesSize(Size newSize) {
        contentPane.setSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
        uiContentPane.setSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
     * @return the position of the tab
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    @Override
    public Position getPosition() {
        return new Position(this.getX(), this.getY());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab tab
     * @param newSize the dimensions we want to set to the tab
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    @Override
    public void setDims(Size newSize) {
        this.setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
     * @param newPosition the position we want to set to the tab
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    @Override
    public void setPosition(Position newPosition) {
        this.setLocation(newPosition.getX(), newPosition.getY());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    @Override
    public int addMember(Canvas member) {
        return 0;
    }

    /**
     * We want to be able to remove directly a Canvas from a Tab.
     * @param member canvas to remove
     */
    @Override
    public void removeMember(Canvas member) {
    }

    /**
     * Add a SwingViewObject (from SwingView.java) to container and returns its
     * index
     * @param member the member to add
     */
    public void addMember(SwingViewObject member) {
        if (member instanceof SwingScilabAxes) {
            if (contentCanvas == null) {
                contentCanvas = new SwingScilabCanvas((Figure) GraphicController.getController().getObjectFromId(((SwingScilabAxes) member).getFigureId()));
                contentCanvas.addEventHandlerKeyListener(editorEventHandler);
                contentCanvas.addEventHandlerMouseListener(editorEventHandler);
                contentCanvas.addEventHandlerMouseMotionListener(editorEventHandler);
                if (eventEnabled) {
                    editorEventHandler.setEnable(false);
                    enableEventHandler();
                }

                layeredPane.add(contentCanvas, JLayeredPane.FRAME_CONTENT_LAYER);

                scrolling.setCanvas(contentCanvas);

                contentCanvas.addKeyListener(this);
            }
            return;
        }

        SwingScilabCommonPanel.addMember(this, member);
    }

    /**
     * Remove a SwingViewObject (from SwingView.java)
     * @param member the member to remove
     */
    public void removeMember(SwingViewObject member) {
        SwingScilabCommonPanel.removeMember(this, member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    @Override
    public int addMember(Console member) {
        return this.addMember((SwingScilabConsole) member.getAsSimpleConsole());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabConsole member) {
        // replace the current content pane
        this.setContentPane(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    @Override
    public int addMember(HelpBrowser member) {
        return this.addMember((SwingScilabHelpBrowser) member.getAsSimpleHelpBrowser());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabHelpBrowser member) {
        // replace the current content pane
        this.setContentPane(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(UiDisplayTree member) {
        return this.addMember((SwingScilabUiDisplayTree) member.getAsSimpleUiDisplayTree());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabUiDisplayTree member) {
        return 0;
    }

    /**
     * Remove a Tree from its container
     * @param member the Tree to remove
     */
    public void removeMember(UiDisplayTree member) {
        this.removeMember((SwingScilabUiDisplayTree) member.getAsSimpleUiDisplayTree());
    }

    /**
     * Remove a Tree from its container
     * @param member the Tree to remove
     */
    private void removeMember(SwingScilabUiDisplayTree member) {
    }

    /**
     * Add a Tree member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    @Override
    public int addMember(Tree member) {
        return this.addMember((SwingScilabTree) member.getAsSimpleTree());
    }

    /**
     * Add a Tree member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(SwingScilabTree member) {
        return 0;
    }

    /**
     * Add a Tree member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addTree(SwingScilabTree member) {
        this.setContentPane(member.getAsComponent());
        return this.getComponentZOrder(member.getAsComponent());
    }

    /**
     * Remove a PopupMenu from its container
     * @param member the PopupMenu to remove
     */
    @Override
    public void removeMember(Tree member) {
        this.removeMember((SwingScilabTree) member.getAsSimpleTree());
    }

    /**
     * Remove a PopupMenu from its container
     * @param member the PopupMenu to remove
     */
    private void removeMember(SwingScilabTree member) {
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Dockable member) {
        // TODO Auto-generated method stub
        return 0;
    }

    /**
     * Get the current status of the Tab in its parent
     * @return true is the tab is the tab currently "on top" in its parent
     */
    @Override
    public boolean isCurrentTab() {
        // TODO should not always return TRUE
        return true;
    }

    /**
     * Get the parent window id for this tab
     * @return the id of the parent window
     */
    @Override
    public String getParentWindowId() {
        return this.parentWindowId;
    }

    /**
     * Get the canvas
     * @return the canvas
     */
    public SwingScilabCanvas getContentCanvas() {
        return contentCanvas;
    }

    /**
     * Set the parent window id for this tab
     * @param id the id of the parent window
     */
    @Override
    public void setParentWindowId(String id) {
        this.parentWindowId = id;
    }

    /**
     * Setter for MenuBar
     * @param newMenuBar : the MenuBar to set.
     * @see org.scilab.modules.gui.tab.SimpleTab#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
     */
    @Override
    public void setMenuBar(MenuBar newMenuBar) {
        if (this.menuBar != newMenuBar) {
            if (this.menuBar != null) {
                ((SwingScilabMenuBar) this.menuBar.getAsSimpleMenuBar()).close();
            }
            this.menuBar = newMenuBar;
        }
    }

    /**
     * Getter for MenuBar
     * @return MenuBar : the MenuBar associated to the Tab.
     * @see org.scilab.modules.gui.tab.SimpleTab#getMenuBar()
     */
    @Override
    public MenuBar getMenuBar() {
        return this.menuBar;
    }

    /**
     * Setter for ToolBar
     * @param newToolBar : the ToolBar to set.
     * @see org.scilab.modules.gui.tab.SimpleTab#setToolBar(org.scilab.modules.gui.toolbar.ToolBar)
     */
    @Override
    public void setToolBar(ToolBar newToolBar) {
        this.toolBar = newToolBar;
    }

    /**
     * Getter for ToolBar
     * @return ToolBar : the ToolBar associated to the Tab.
     * @see org.scilab.modules.gui.tab.SimpleTab#getToolBar()
     */
    @Override
    public ToolBar getToolBar() {
        return this.toolBar;
    }

    /**
     * Setter for InfoBar
     * @param newInfoBar the InfoBar to set.
     */
    @Override
    public void setInfoBar(TextBox newInfoBar) {
        this.infoBar = newInfoBar;
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the Tab.
     */
    @Override
    public TextBox getInfoBar() {
        return this.infoBar;
    }

    /**
     * Set the callback of the tab
     * @param callback the callback to set.
     */
    @Override
    public void setCallback(CommonCallBack callback) {

        if (closeAction != null) {
            this.getTitlebar().removeAction(closeAction);
        }

        if (callback != null) {
            closeAction = new SciClosingAction(this, callback);
        } else {
            closeAction = new SciClosingAction(this);
        }

        closeAction.putValue(Action.NAME, DockingConstants.CLOSE_ACTION);
        ((Titlebar) getTitlePane()).removeAction(DockingConstants.CLOSE_ACTION);
        addAction(closeAction);

        /* Undock button */
        SciUndockingAction undockAction = new SciUndockingAction(this);
        undockAction.putValue(Action.NAME, UNDOCK);
        ((Titlebar) getTitlePane()).removeAction(UNDOCK);
        addAction(undockAction);

        /* Help button */
        SciHelpOnComponentAction helpAction = new SciHelpOnComponentAction(this);
        helpAction.putValue(Action.NAME, HELP);
        ((Titlebar) getTitlePane()).removeAction(HELP);
        addAction(helpAction);
    }

    /**
     * Set this tab as the current tab of its parent Window
     */
    @Override
    public void setCurrent() {
        ActiveDockableTracker.requestDockableActivation(this);
    }

    /**
     * Set the background color of the tab.
     * @param red red channel of the color
     * @param green green channel
     * @param blue blue channel
     */
    @Override
    public void setBackground(double red, double green, double blue) {
        Color newColor = new Color((float) red, (float) green, (float) blue);
        setBackground(newColor);
    }

    /**
     * Get the part of the axes which is currently viewed
     * @return [x,y,w,h] array
     */
    @Override
    public int[] getViewingRegion() {
        return scrolling.getViewingRegion();
    }

    /**
     * Specify a new viewport for the axes For SwingScilabCanvas viewport can
     * not be modified since it match the parent tab size
     * @param posX X coordinate of upper left point of the viewport within the
     * axes
     * @param posY Y coordinate of upper left point of the viewport within the
     * axes
     * @param width width of the viewport
     * @param height height of the viewport
     */
    @Override
    public void setViewingRegion(int posX, int posY, int width, int height) {
    }

    /**
     * Set the event handler of the Canvas
     * @param funName the name of the Scilab function to call
     */
    public void setEventHandler(String funName) {
        disableEventHandler();
        eventHandler = new ScilabEventListener(funName, getId(), false);
        if (eventEnabled) {
            editorEventHandler.setEnable(false);
            enableEventHandler();
        }
    }

    /**
     * Set the status of the event handler of the Canvas
     * @param status is true to set the event handler active
     */
    public void setEventHandlerEnabled(boolean status) {
        if (status && eventEnabled) {
            return;
        }

        if (status) {
            editorEventHandler.setEnable(false);
            enableEventHandler();
            eventEnabled = true;
        } else {
            editorEventHandler.setEnable(true);
            disableEventHandler();
            eventEnabled = false;
        }
    }

    /**
     * Specify whether the canvas should fit the parent tab size (and
     * consequently the scrollpane size) or not
     * @param onOrOff true to enable autoresize mode
     */
    @Override
    public void setAutoResizeMode(boolean onOrOff) {
    }

    /**
     * @return whether the resize mode is on or off
     */
    @Override
    public boolean getAutoResizeMode() {
        return true;
    }

    /**
     * Close the tab and disable it.
     */
    public void close() {
        if (getTitlePane() != null) {
            ((Titlebar) getTitlePane()).removeAction(DockingConstants.CLOSE_ACTION);
            ((Titlebar) getTitlePane()).removeAction(UNDOCK);
            ((Titlebar) getTitlePane()).removeAction(HELP);
        }

        if (closeAction != null) {
            getTitlebar().removeAction(closeAction);
            if (closeAction instanceof SciClosingAction) {
                ((SciClosingAction) closeAction).clean();
            }
            closeAction = null;
        }

        setMenuBar(null);
        setToolBar(null);
        setInfoBar(null);
        setTitlebar(null);
        removeAll();
        //setActive(false);

        scrolling = null;
        contentPane = null;
        DockingManager.unregisterDockable((Component) this);

        // without this children canvas are not released.
        Container dummyContainer = new Container();
        this.setContentPane(dummyContainer);
        if (editorEventHandler != null) {
            editorEventHandler.onExit();
        }
    }

    public void disablePaint() {
        paintEnable = false;
    }

    public void setFigureBackground(Color color) {
        if (layeredPane != null) {
            layeredPane.setBackground(color);
        }
    }

    /**
     * Redefine paint children to be sure that AWT components are well painted.
     * @param g a Graphics
     */
    @Override
    public void paintChildren(Graphics g) {
        if (paintEnable) {
            Component[] children = getComponents();
            for (int i = 0; i < children.length; i++) {
                // AWT children don't draw themselves automatically
                // so force their draw
                if (!children[i].isLightweight()) {
                    children[i].paint(g);
                }
            }
            super.paintChildren(g);
        }
    }

    /**
     * Update the tab after a modification of its properties
     * @param property the property name
     * @param value the property value
     * @see org.scilab.modules.gui.SwingViewObject#update(java.lang.String,
     * java.lang.Object)
     */
    public void update(int property, Object value) {
        SwingScilabCommonPanel.update(this, property, value);
    }

    /**
     * Get the tab UID
     * @return the UID
     * @see org.scilab.modules.gui.SwingViewObject#getId()
     */
    public Integer getId() {
        return id;
    }

    /**
     * Set the tab UID
     * @param id the UID
     * @see org.scilab.modules.gui.SwingViewObject#setId(java.lang.String)
     */
    public void setId(Integer id) {
        this.id = id;
    }

    /**
     * Turn on event handling.
     */
    private void enableEventHandler() {
        if (contentCanvas != null) {
            contentCanvas.addEventHandlerKeyListener(eventHandler);
            contentCanvas.addEventHandlerMouseListener(eventHandler);
            contentCanvas.addEventHandlerMouseMotionListener(eventHandler);
        }
    }

    /**
     * Turn off event handling.
     */
    private void disableEventHandler() {
        if (eventHandler != null && contentCanvas != null) {
            contentCanvas.removeEventHandlerKeyListener(eventHandler);
            contentCanvas.removeEventHandlerMouseListener(eventHandler);
            contentCanvas.removeEventHandlerMouseMotionListener(eventHandler);
        }
    }

    public void keyPressed(KeyEvent e) {

    }

    public void keyReleased(KeyEvent e) {
    }

    public void keyTyped(KeyEvent e) {
        if (ScilabConstants.isGUI() && (eventHandler == null || !eventEnabled) && !GlobalEventWatcher.isActivated() && !editorEventHandler.isDatatipEnable()
                && Character.isLetterOrDigit(e.getKeyChar())) {
            SwingScilabConsole console = (SwingScilabConsole) ScilabConsole.getConsole().getAsSimpleConsole();
            JTextPane input = (JTextPane) console.getConfiguration().getInputCommandView();
            input.requestFocus();
            SwingScilabWindow win = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, console);
            win.toFront();
            input.dispatchEvent(new KeyEvent(input, KeyEvent.KEY_TYPED, System.currentTimeMillis(), e.getModifiers(), e.getKeyCode(), e.getKeyChar(), e.getKeyLocation()));
        }
    }

    public JLayeredPane getWidgetPane() {
        return uiContentPane;
    }

    public void setHasLayout(boolean hasLayout) {
        this.hasLayout = hasLayout;
    }

    public void applyDeltaSize() {
        if (deltaSize != null) {
            if (deltaSize.getWidth() != 0 || deltaSize.getHeight() != 0) {
                //update view and update model ( from componentResize of Window )
                SwingScilabWindow figure = SwingScilabWindow.allScilabWindows.get(getParentWindowId());
                Dimension oldAxesSize = getContentPane().getSize();
                figure.setDims(new Size((int)(oldAxesSize.getWidth() + deltaSize.getWidth()), (int)(oldAxesSize.getHeight() + deltaSize.getHeight())));
            }

            deltaSize = null;
        }
    }

    public void storeSizeDelta() {
        Dimension axesSize = getContentPane().getSize();
        SwingScilabWindow figure = SwingScilabWindow.allScilabWindows.get(getParentWindowId());
        Size figureSize = figure.getDims();
        deltaSize = new Dimension((int)(figureSize.getWidth() - axesSize.getWidth()), (int)(figureSize.getHeight() - axesSize.getHeight()));
    }

    public void disableResizeEvent() {
        removeComponentListener(componentListener);
    }

    public void enableResizeEvent() {
        addComponentListener(componentListener);
    }
}
