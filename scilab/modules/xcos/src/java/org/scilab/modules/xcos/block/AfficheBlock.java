/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xcos.block;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.Serializable;
import java.util.Collections;
import java.util.Formatter;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.Timer;

import org.scilab.modules.graph.utils.Font;
import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.AbstractElement;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraphView;

/**
 * Implement the AFFICH_m block
 */
public final class AfficheBlock extends BasicBlock {

    /**
     * Default refresh rate used on the simulation to update block.
     */
    private static final int DEFAULT_TIMER_RATE = 200;

    private static final Logger LOG = Logger.getLogger(AfficheBlock.class.getName());
    private static final String EXPRS = "exprs";
    private static final int PRECISION_INDEX = 4;
    private static final String NEW_LINE = System.getProperty("line.separator");
    private static final String SPACE = "  ";
    private static final long serialVersionUID = 6874403612919831380L;

    /**
     * Map any id to an affiche block instance.
     *
     * This property is linked to the affich2.cpp native implementation
     */
    private static final Map<Integer, AfficheBlock> INSTANCES = Collections.synchronizedMap(new HashMap<Integer, AfficheBlock>());

    /**
     * Update the value of the associated block
     */
    @SuppressWarnings(value = { "serial" })
    private static class UpdateValueListener implements ActionListener, Serializable {
        private AfficheBlock block;
        private String[][] data;

        /**
         * Default constructor
         *
         * @param block
         *            the current block
         */
        public UpdateValueListener(AfficheBlock block) {
            this.block = block;
        }

        /**
         * @param data
         *            the data to set
         */
        public synchronized void setData(String[][] data) {
            this.data = data;
        }

        /**
         * Calculate an print the new String value.
         *
         * @param e
         *            the current event
         * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
         */
        @Override
        public synchronized void actionPerformed(ActionEvent e) {
            XcosDiagram graph = block.getParentDiagram();
            if (graph == null) {
                block.setParentDiagram(Xcos.findParent(block));
                graph = block.getParentDiagram();
                LOG.severe("Parent diagram was null");
            }

            /*
             * Construct a String representation of the values.
             */
            final StringBuilder blockResult = new StringBuilder();
            final int iRows = data.length;
            final int iCols = data[0].length;

            for (int i = 0; i < iRows; i++) {
                for (int j = 0; j < iCols; j++) {
                    if (iCols != 0) {
                        blockResult.append(SPACE);
                    }

                    blockResult.append(data[i][j]);
                }
                blockResult.append(NEW_LINE);
            }

            /*
             * Update and refresh the values
             */
            String value = blockResult.toString();

            block.setValue(value);

            final mxCellState state = graph.getView().getState(block);
            state.setLabel(value);
            graph.getAsComponent().redraw(state);

            if (LOG.isLoggable(Level.FINEST)) {
                LOG.finest(blockResult.toString());
            }
        }
    }

    /**
     * Update the style according to the values of the expression
    @SuppressWarnings(value = { "serial" })
     */
    private static final class UpdateStyle implements PropertyChangeListener, Serializable {
        /**
         *
         */
        private static final String OPENING_BRACKET = "[";
        private static transient UpdateStyle instance;

        /**
         * Default constructor.
         */
        private UpdateStyle() {
        }

        /**
         * @return the instance
         */
        public static UpdateStyle getInstance() {
            if (instance == null) {
                instance = new UpdateStyle();
            }
            return instance;
        }

        /**
         * Update the style of the block according to the expression value
         *
         * @param evt
         *            the evnt
         * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
         */
        @Override
        public void propertyChange(PropertyChangeEvent evt) {
            // Pre-condition
            final String property = evt.getPropertyName();
            if (evt.getSource() == null || !property.equals(EXPRS)) {
                LOG.severe("Unable to update the style");
            }

            final AfficheBlock src = (AfficheBlock) evt.getSource();
            final String[][] data = ((ScilabString) evt.getNewValue()).getData();

            updateValue(src, data);
            updateStyle(src, data);

            /*
             * Refresh
             */
            if (src.getParentDiagram() != null) {
                final XcosDiagram parent = src.getParentDiagram();
                final mxRectangle rect = parent.getPreferredSizeForCell(src);
                rect.setX(src.getGeometry().getX());
                rect.setY(src.getGeometry().getY());
                parent.resizeCell(src, rect);

                final mxGraphView view = parent.getView();
                final mxCellState parentState = view.getState(src.getParent());

                view.validateBounds(parentState, src);
                parent.repaint(view.validatePoints(parentState, src));
            }
        }

        /**
         * Update the value according to the data (matrix size and precision)
         *
         * @param src
         *            the src block
         * @param data
         *            the "exprs" data
         */
        private void updateValue(AfficheBlock src, String[][] data) {
            /*
             * Getting the first parameter for the matrix size
             */
            final int[] size;
            int[] index = new int[] { 0, 0 };
            final String data00 = data[index[0]][index[1]];
            if (data00.startsWith(OPENING_BRACKET)) {
                final String[] strSize = data00.split("[\\[\\],]");
                try {
                    size = new int[] { Integer.parseInt(strSize[1]), Integer.parseInt(strSize[2]) };
                } catch (NumberFormatException e) {
                    LOG.severe(e.toString());
                    return;
                }
            } else {
                size = new int[] { 1, 1 };
            }

            index = new int[] { PRECISION_INDEX, 0 };
            final String width = data[index[0]][index[1]];

            AbstractElement.incrementIndexes(index, true);
            final String rational = data[index[0]][index[1]];

            final String format = "%" + width + "." + rational + "f";
            final StringBuilder value = new StringBuilder();
            for (int i = 0; i < size[0]; i++) {
                for (int j = 0; j < size[1]; j++) {
                    value.append(new Formatter(Locale.US).format(format, Double.valueOf(0.0)).toString());
                    value.append(SPACE);
                }
                value.append(NEW_LINE);
            }

            src.setValue(value.toString());
        }

        /**
         * Update the style of the block according to the data.
         *
         * @param src
         *            the block
         * @param data
         *            the "exprs" data
         */
        private void updateStyle(final AfficheBlock src, final String[][] data) {
            /*
             * Getting the first index to handle Affich_f and Affich_m
             */
            int[] index = new int[] { 0, 0 };
            final String data00 = data[index[0]][index[1]];
            if (data00.startsWith(OPENING_BRACKET)) {
                AbstractElement.incrementIndexes(index, true);
            }

            /*
             * Apply style
             */
            final StyleMap style = new StyleMap(src.getStyle());

            try {
                final int parsedFontInt = Integer.parseInt(data[index[0]][index[1]]);
                style.put(mxConstants.STYLE_FONTFAMILY, Font.getFont(parsedFontInt).getName());

                AbstractElement.incrementIndexes(index, true);
                final int parsedFontSizeInt = Integer.parseInt(data[index[0]][index[1]]);
                style.put(mxConstants.STYLE_FONTSIZE, Integer.toString(Font.getSize(parsedFontSizeInt)));

                AbstractElement.incrementIndexes(index, true);
                final int parsedFontColorInt = Integer.parseInt(data[index[0]][index[1]]);
                String color = "#" + Integer.toHexString(Font.getColor(parsedFontColorInt).getRGB());
                style.put(mxConstants.STYLE_FONTCOLOR, color);
            } catch (NumberFormatException e) {
                LOG.severe(e.toString());
                return;
            }

            src.setStyle(style.toString());
        }
    }

    private final Timer printTimer;
    private final UpdateValueListener updateAction;

    /** Default constructor */
    public AfficheBlock() {
        super();

        updateAction = new UpdateValueListener(this);
        printTimer = new Timer(DEFAULT_TIMER_RATE, updateAction);
        printTimer.setRepeats(false);

        getParametersPCS().addPropertyChangeListener(EXPRS, UpdateStyle.getInstance());
    }

    /**
     * Set the default values
     *
     * @see org.scilab.modules.xcos.block.BasicBlock#setDefaultValues()
     */
    @Override
    protected void setDefaultValues() {
        super.setDefaultValues();

        setValue("0.0");
    }

    /**
     * Assign a value to an AfficheBlock instance
     *
     * @param uid
     *            the block id
     * @param value
     *            the value to set.
     */
    @ScilabExported(module = "scicos_blocks", filename = "Blocks.giws.xml")
    public static void setValue(final int uid, final String[][] value) {
        if (value.length == 0 || value[0].length == 0) {
            throw new IllegalArgumentException("value is not a non-empty String matrix (String[][])");
        }

        final AfficheBlock block = INSTANCES.get(uid);

        block.updateAction.setData(value);
        if (!block.printTimer.isRunning()) {
            block.printTimer.start();
        }
    }

    /**
     * @return the instance UID.
     */
    @Deprecated
    public synchronized int getHashCode() {
        return hashCode();
    }

    /**
     * @return The scilab formated object parameters
     */
    @Override
    public ScilabType getObjectsParameters() {
        int id = hashCode();

        /*
         * As hashCode() may return an already existing id, we need to change it
         * in this case.
         *
         * see
         * http://java.sun.com/javase/6/docs/api/java/lang/Object.html#hashCode
         * ()
         */
        while (INSTANCES.containsKey(id) && INSTANCES.get(id) != this) {
            id++;
        }

        INSTANCES.put(id, this);

        ScilabList list = new ScilabList();
        list.add(new ScilabDouble(id));
        return list;
    }

    /**
     * Remove the instance from the INSTANCES map.
     *
     * @throws Throwable
     *             when unable to do so.
     * @see java.lang.Object#finalize()
     */
    @Override
    // CSOFF: IllegalThrows
    protected void finalize() throws Throwable {
        INSTANCES.remove(hashCode());
        super.finalize();
    }

    // CSON: IllegalThrows

    /**
     * @return a clone of the block
     * @throws CloneNotSupportedException
     *             on error
     * @see com.mxgraph.model.mxCell#clone()
     */
    @Override
    public Object clone() throws CloneNotSupportedException {
        AfficheBlock clone = (AfficheBlock) super.clone();

        // reassociate the update action data
        clone.updateAction.block = clone;

        return clone;
    }

}
