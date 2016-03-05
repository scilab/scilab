/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.text.DateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.Iterator;
import java.util.List;

import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.graph.view.ScilabGraphView;

import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxGraphModel.mxChildChange;
import com.mxgraph.model.mxGraphModel.mxCollapseChange;
import com.mxgraph.model.mxGraphModel.mxGeometryChange;
import com.mxgraph.model.mxGraphModel.mxStyleChange;
import com.mxgraph.model.mxGraphModel.mxTerminalChange;
import com.mxgraph.model.mxGraphModel.mxValueChange;
import com.mxgraph.model.mxGraphModel.mxVisibleChange;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.handler.mxRubberband;
import com.mxgraph.swing.util.mxGraphActions;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUndoManager;
import com.mxgraph.util.mxUndoableEdit;
import com.mxgraph.util.mxUndoableEdit.mxUndoableChange;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxGraphView;
import com.mxgraph.view.mxStylesheet;

/**
 * Represent the base diagram of Xcos.
 *
 * It performs generic operations like undo/redo management, action clean-up,
 * modification state management, SwingScilabTab association, etc...
 */
public class ScilabGraph extends mxGraph {
    /**
     * The default component of a scilab graph
     */
    private ScilabComponent component;

    private final mxUndoManager undoManager = new mxUndoManager();

    private String title = null;
    private File savedFile;
    private boolean modified;
    private boolean opened;
    private boolean readOnly;

    private transient mxRubberband rubberBand;

    private transient String graphTab;
    private transient String viewPortTab;

    /**
     * Manage the modification state on change
     */
    private final mxIEventListener changeTracker = new mxIEventListener() {
        @Override
        public void invoke(Object source, mxEventObject evt) {
            setModified(true);
        }
    };

    /**
     * Manage the undo/redo on change
     */
    private final mxIEventListener undoHandler = new mxIEventListener() {
        @Override
        public void invoke(Object source, mxEventObject evt) {
            undoManager.undoableEditHappened((mxUndoableEdit) evt.getProperty(ScilabGraphConstants.EVENT_CHANGE_EDIT));
        }
    };

    /**
     * Remove the undo handler from the component
     */
    public void removeUndoHandler() {
        getModel().removeListener(undoHandler, mxEvent.UNDO);
    }

    /**
     * Register the undo handler on the right component
     */
    public void registerUndoHandler() {
        // Undo / Redo capabilities
        getModel().addListener(mxEvent.UNDO, undoHandler);
    }

    /**
     * Update the selection on undo/redo
     */
    private final mxIEventListener selectionHandler = new mxIEventListener() {
        @Override
        public void invoke(Object source, mxEventObject evt) {
            List<mxUndoableChange> changes = ((mxUndoableEdit) evt.getProperty(ScilabGraphConstants.EVENT_CHANGE_EDIT)).getChanges();
            getSelectionModel().setCells(getSelectionCellsForChanges(changes));
        }
    };

    /**
     * Update the component when the graph is locked
     */
    private final PropertyChangeListener cellLockBackgroundUpdater = new PropertyChangeListener() {
        @Override
        public void propertyChange(PropertyChangeEvent evt) {
            if (evt.getPropertyName().equals("cellsLocked")) {
                getAsComponent().getGraphControl().repaint();
            }
        }
    };

    /**
     * Default constructor:
     * <UL>
     *   <LI> disable unused actions
     *   <LI> install listeners
     *   <LI> Replace JGraphX components by specialized components if needed.
     */
    public ScilabGraph() {
        this(null, null);
    }

    /**
     * Constructor using a specific model
     * @param model the model to use
     */
    public ScilabGraph(mxIGraphModel model, mxStylesheet stylesheet) {
        super(model, stylesheet);

        // Disabling the default connected action and event listeners.
        mxGraphActions.getSelectNextAction().setEnabled(false);
        mxGraphActions.getSelectPreviousAction().setEnabled(false);
        mxGraphActions.getSelectChildAction().setEnabled(false);
        mxGraphActions.getSelectParentAction().setEnabled(false);

        registerUndoHandler();

        // Keeps the selection in sync with the command history
        undoManager.addListener(mxEvent.UNDO, selectionHandler);
        undoManager.addListener(mxEvent.REDO, selectionHandler);

        setComponent(new ScilabComponent(this));

        // graph locked change support
        changeSupport.addPropertyChangeListener(cellLockBackgroundUpdater);

        // Modified property change
        getModel().addListener(mxEvent.CHANGE, changeTracker);
    }



    /**
     * @return The previously saved file or null.
     */
    public File getSavedFile() {
        return savedFile;
    }

    /**
     * @param savedFile
     *            The new saved file
     */
    public void setSavedFile(final File savedFile) {
        this.savedFile = savedFile;

        // register the saved dir as the image base path (for relative images
        // location).
        if (savedFile != null && savedFile.getParentFile() != null) {
            getAsComponent().getCanvas().setImageBasePath(savedFile.getParentFile().toURI().toASCIIString());
        }
    }

    /**
     * @return true, if the graph has been modified ; false otherwise.
     */
    public boolean isModified() {
        return modified;
    }

    /**
     * Modify the state of the diagram.
     *
     * @param modified
     *            The new modified state.
     * @category UseEvent
     */
    public void setModified(boolean modified) {
        boolean oldValue = this.modified;
        this.modified = modified;

        if (getAsComponent() != null) {
            getAsComponent().firePropertyChange("modified", oldValue, modified);
        }
    }

    /**
     * @param title
     *            The new title of the tab
     */
    public void setTitle(String title) {
        this.title = title;
    }

    /**
     * @return The current Tab title
     */
    public String getTitle() {
        if (title == null) {
            final Date d = Calendar.getInstance().getTime();
            final String time = DateFormat.getTimeInstance().format(d);
            title = String.format(ScilabGraphMessages.UNTITLED, time);
        }
        return title;
    }

    /**
     * Get the graph tab uuid
     *
     * @return
     */
    public String getGraphTab() {
        return graphTab;
    }

    /**
     * Set the graph tab uuid
     *
     * @param uuid
     *            the diagram tab
     */
    public void setGraphTab(String uuid) {
        this.graphTab = uuid;
    }

    /**
     * Get the view port tab uuid
     *
     * @return the view port tab
     */
    public String getViewPortTab() {
        return viewPortTab;
    }

    /**
     * Set the view port tab uuid
     *
     * @param uuid
     *            the view port tab
     */
    public void setViewPortTab(String uuid) {
        this.viewPortTab = uuid;
    }

    /**
     * @return The component associated with the current graph.
     */
    public mxGraphComponent getAsComponent() {
        return component;
    }

    /**
     * @param component
     *            The graphical component associated with this graph
     */
    public void setComponent(ScilabComponent component) {
        this.component = component;

        if (component != null) {
            // Adds rubberband selection
            rubberBand = new mxRubberband(component);
        } else {
            rubberBand = null;
        }
    }

    /**
     * The instance can be not visible but used (when using SuperBlock). The
     * openned flag is true in this case and also when the Window/Tab is
     * visible.
     *
     * @param opened
     *            Openned state
     */
    public void setOpened(boolean opened) {
        this.opened = opened;
    }

    /**
     * @return Openned state
     */
    public boolean isOpened() {
        return opened;
    }

    /**
     * A read-only state will disable all actions in the graph.
     *
     * @param readOnly
     *            Read-only state
     */
    public void setReadOnly(boolean readOnly) {
        this.readOnly = readOnly;

        setCellsLocked(readOnly);
    }

    /**
     * @return True if actions are not allowed, false otherwise.
     */
    public boolean isReadonly() {
        return readOnly;
    }

    /**
     * @return The associated RubberBand
     * @see com.mxgraph.swing.handler.mxRubberband
     */
    public mxRubberband getRubberBand() {
        return rubberBand;
    }

    /**
     * @return The undo manager associated with this graph
     */
    public final mxUndoManager getUndoManager() {
        return undoManager;
    }

    /**
     * @return the newly allocated graph
     * @see com.mxgraph.view.mxGraph#createGraphView()
     */
    @Override
    protected mxGraphView createGraphView() {
        return new ScilabGraphView(this);
    }

    /*
     * Utils
     */
    /**
     * Returns the cells to be selected for the given list of changes.
     *
     * @param changes
     *            the changes
     * @param model
     *            the model to work on
     * @return the cells
     */
    public static Object[] getSelectionCellsForChanges(final List<mxUndoableChange> changes, final mxGraphModel model) {
        List<Object> cells = new ArrayList<Object>();
        Iterator<mxUndoableChange> it = changes.iterator();

        while (it.hasNext()) {
            Object change = it.next();

            if (change instanceof mxChildChange) {
                cells.add(((mxChildChange) change).getChild());
            } else if (change instanceof mxTerminalChange) {
                cells.add(((mxTerminalChange) change).getCell());
            } else if (change instanceof mxValueChange) {
                cells.add(((mxValueChange) change).getCell());
            } else if (change instanceof mxStyleChange) {
                cells.add(((mxStyleChange) change).getCell());
            } else if (change instanceof mxGeometryChange) {
                cells.add(((mxGeometryChange) change).getCell());
            } else if (change instanceof mxCollapseChange) {
                cells.add(((mxCollapseChange) change).getCell());
            } else if (change instanceof mxVisibleChange) {
                mxVisibleChange vc = (mxVisibleChange) change;

                if (vc.isVisible()) {
                    cells.add(((mxVisibleChange) change).getCell());
                }
            }
        }

        return mxGraphModel.getTopmostCells(model, cells.toArray());
    }
}
