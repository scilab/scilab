/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
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

package org.scilab.modules.gui.bridge.tree;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Vector;

import javax.swing.Icon;
import javax.swing.JComponent;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.UIManager;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;

import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.tree.SimpleTree;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.localization.Messages;

/**
 * Class define the Scilab Mutable tree node
 * @author Sylvestre Koumar
 *
 */
public class SwingScilabTree extends DefaultMutableTreeNode implements SimpleTree {

    private static final long serialVersionUID = 1L;

    private Icon icon;
    private CommonCallBack callback;
    private JScrollPane scrollPane = new JScrollPane();

    /**
     * Default constructor
     * @param tree structure to display
     */
    public SwingScilabTree(Tree tree) {
        super(tree.getLabel());
        this.setIcon(tree.getIcon());
        this.setCallback(tree.getCallback());

        Vector<Tree> children = tree.getChildren();

        for (int i = 0; i < children.size(); ++i)	{
            SwingScilabTree swingScilabTree = new SwingScilabTree(children.get(i));
            this.add(swingScilabTree);
        }
    }

    /**
     * Getter for icon
     * @return an icon
     */
    public Icon getIcon() {
        return icon;
    }

    /**
     * Setter for icon
     * @param icon an Icon
     */
    public void setIcon(Icon icon) {
        this.icon = icon;
    }

    /**
     * Getter for callback
     * @return a callback
     */
    public CommonCallBack getCallback() {
        return callback;
    }

    /**
     * Setter for callback
     * @param callback a callback
     */
    public void setCallback(CommonCallBack callback) {
        this.callback = callback;
    }

    /**
     *  This is a private class to embed the JTree inside its Mouse Listener
     *  in order we can figure out on which leaff the user clicked on.
     */
    private class ClickListener extends MouseAdapter {
        private JTree myJtree;

        /**
         * Constructor
         * @param jtree a Jtree
         */
        public ClickListener(JTree jtree) {
            myJtree = jtree;
        }

        /**
         * Mouse clicked actions
         * @param arg0 event
         */
        public void mouseClicked(MouseEvent arg0) {
            TreePath path = myJtree.getPathForLocation(arg0.getX(), arg0.getY());
            /* We only want double click to do something */
            if (path != null && arg0.getClickCount() >= 2) {
                ((SwingScilabTree) path.getLastPathComponent()).getCallback().actionPerformed(null);
            }

        }
    }

    /**
     * Get as Component
     * @return a component
     */
    public JComponent getAsComponent() {
        // Tree Model
        DefaultTreeModel model = new DefaultTreeModel(this);
        // Renderer
        ScilabTreeCellRenderer renderer = new ScilabTreeCellRenderer();
        // Swing tree component
        JTree jtree = new JTree();
        // Show Root Handles
        jtree.setShowsRootHandles(true);
        // Set model to the JTree
        jtree.setModel(model);
        // Set renderer
        jtree.setCellRenderer(renderer);

        jtree.addMouseListener(new ClickListener(jtree));
        jtree.setVisible(true);


        scrollPane.getViewport().add(jtree);
        return scrollPane;
    }

    public JTree getJTree() {
        // Tree Model
        DefaultTreeModel model = new DefaultTreeModel(this);
        // Renderer
        ScilabTreeCellRenderer renderer = new ScilabTreeCellRenderer();
        // Swing tree component
        JTree jtree = new JTree();
        // Show Root Handles
        jtree.setShowsRootHandles(true);
        // Set model to the JTree
        jtree.setModel(model);
        // Set renderer
        jtree.setCellRenderer(renderer);

        jtree.addMouseListener(new ClickListener(jtree));
        jtree.setVisible(true);

        return jtree;
    }

    /**
     * Display the tree
     * @param tree a tree
     */
    public static void  showTree(Tree tree) {

        // Scilab tree
        SwingScilabTree swingScilabTree = new SwingScilabTree(tree);

        SwingScilabWindow window = SwingScilabWindow.createWindow(true);
        final SwingScilabDockablePanel tab = new SwingScilabDockablePanel(Messages.gettext("Tree Overview"));
        tab.setCallback(new CommonCallBack("", CallBack.UNTYPED) {
            private static final long serialVersionUID = 8418506008885202932L;

            public void callBack() {
                tab.close();
            }
        });
        tab.addTree(swingScilabTree);
        window.addTab(tab);
        tab.setVisible(true);
        window.setVisible(true);
    }

    public void destroy() {
        // TODO Auto-generated method stub

    }

    public Color getBackground() {
        // TODO Auto-generated method stub
        return null;
    }

    public Font getFont() {
        // TODO Auto-generated method stub
        return null;
    }

    public Color getForeground() {
        // TODO Auto-generated method stub
        return null;
    }

    public String getText() {
        // TODO Auto-generated method stub
        return null;
    }

    public boolean isEnabled() {
        // TODO Auto-generated method stub
        return false;
    }

    public void requestFocus() {
        // TODO Auto-generated method stub

    }

    public void setBackground(Color color) {
        // TODO Auto-generated method stub

    }

    public void setEnabled(boolean status) {
        // TODO Auto-generated method stub

    }

    public void setFont(Font font) {
        // TODO Auto-generated method stub

    }

    public void setForeground(Color color) {
        // TODO Auto-generated method stub

    }

    public void setHorizontalAlignment(String alignment) {
        // TODO Auto-generated method stub

    }

    public void setRelief(String reliefType) {
        // TODO Auto-generated method stub

    }

    public void setText(String text) {
        // TODO Auto-generated method stub

    }

    public void setEmptyText() {
        setText(null);
    }

    public void setVerticalAlignment(String alignment) {
        // TODO Auto-generated method stub

    }

    public void addInfoBar(TextBox infoBarToAdd) {
        // TODO Auto-generated method stub

    }

    public void addMenuBar(MenuBar menuBarToAdd) {
        // TODO Auto-generated method stub

    }

    public void addToolBar(ToolBar toolBarToAdd) {
        // TODO Auto-generated method stub

    }

    public void draw() {
        // TODO Auto-generated method stub

    }

    /**
     * Get size
     * @return the size
     */
    public Size getDims() {
        return new Size(scrollPane.getSize().width, scrollPane.getSize().height);
    }

    public TextBox getInfoBar() {
        // TODO Auto-generated method stub
        return null;
    }

    public MenuBar getMenuBar() {
        // TODO Auto-generated method stub
        return null;
    }

    /**
     * Get position
     * @return the position
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(scrollPane.getLocation(), scrollPane.getSize(), scrollPane.getParent());
    }

    public ToolBar getToolBar() {
        // TODO Auto-generated method stub
        return null;
    }

    public boolean isVisible() {
        // TODO Auto-generated method stub
        return false;
    }

    /**
     * Set size
     * @param newSize a new size
     */
    public void setDims(Size newSize) {
        scrollPane.setSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
    }

    /**
     * Set position
     * @param newPosition a new position
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, this.getDims(), scrollPane.getParent());
        scrollPane.setLocation(javaPosition.getX(), javaPosition.getY());
    }

    public void setVisible(boolean newVisibleState) {
    }

    public void setToolTipText(String tooltipText) {
    }

    public void resetBackground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Tree.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Tree.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
