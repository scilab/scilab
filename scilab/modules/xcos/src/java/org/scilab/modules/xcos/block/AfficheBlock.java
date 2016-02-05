/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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
package org.scilab.modules.xcos.block;

import com.mxgraph.model.mxGeometry;
import java.awt.GraphicsEnvironment;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.Serializable;
import java.util.Formatter;
import java.util.Iterator;
import java.util.Locale;
import java.util.Map.Entry;
import java.util.TreeMap;
import java.util.logging.Logger;

import javax.swing.Timer;

import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

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

    private static final TreeMap<String, String[][]> values = new TreeMap<String, String[][]>();
    private static final UpdateValueListener updateAction = new UpdateValueListener();
    private static final Timer printTimer = new Timer(DEFAULT_TIMER_RATE, updateAction);

    /**
     * Update the value of the associated block
     */
    @SuppressWarnings(value = { "serial" })
    private static class UpdateValueListener implements ActionListener, Serializable {
        /**
         * Default constructor
         */
        public UpdateValueListener() {
        }

        /**
         * Calculate an print the new String value.
         *
         * @param e
         *            the current event
         * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
         */
        @Override
        public void actionPerformed(ActionEvent e) {
            synchronized (values) {
                for (Iterator<Entry<String, String[][]>> it = values.entrySet().iterator(); it.hasNext();) {
                    final Entry<String, String[][]> entry = it.next();

                    update(entry.getKey(), entry.getValue());

                    it.remove();
                }
            }
        }

        /**
         * Update and refresh the values
         */
        private void update(String uid, String[][] data) {
            // FIXME re-implement this sheet
            //            final Object cell = Xcos.getInstance().lookupForCell(new String[] { uid });
            //            if (cell != null) {
            //                final XcosDiagram diag = Xcos.findParent(cell);
            //                final String value = getText(data);
            //
            //                diag.getModel().setValue(cell, value);
            //
            //                final mxCellState state = diag.getView().getState(cell);
            //                if (state != null) {
            //                    state.setLabel(value);
            //                }
            //
            //                diag.getAsComponent().redraw(state);
            //            }
        }

        /**
         * Construct a String representation of the values.
         */
        private final String getText(final String[][] data) {
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

            return blockResult.toString();
        }
    }

    /**
     * Update the style according to the values of the expression
     *
     * @SuppressWarnings(value = { "serial" })
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
            // FIXME
            //            if (src.getParentDiagram() != null) {
            //                final XcosDiagram parent = src.getParentDiagram();
            //                final mxRectangle rect = parent.getPreferredSizeForCell(src);
            //
            //                parent.getModel().setGeometry(src, new mxGeometry(src.getGeometry().getX(), src.getGeometry().getY(), rect.getWidth(), rect.getHeight()));
            //            }
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

            //            AbstractElement.incrementIndexes(index, true);
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
            //            if (data00.startsWith(OPENING_BRACKET)) {
            //                AbstractElement.incrementIndexes(index, true);
            //            }

            /*
             * Apply style
             */
            final StyleMap style = new StyleMap(src.getStyle());

            //            try {
            //                final int parsedFontInt = Integer.parseInt(data[index[0]][index[1]]);
            //                style.put(mxConstants.STYLE_FONTFAMILY, Font.getFont(parsedFontInt).getName());
            //
            //                AbstractElement.incrementIndexes(index, true);
            //                final int parsedFontSizeInt = Integer.parseInt(data[index[0]][index[1]]);
            //                style.put(mxConstants.STYLE_FONTSIZE, Integer.toString(Font.getSize(parsedFontSizeInt)));
            //
            //                AbstractElement.incrementIndexes(index, true);
            //                final int parsedFontColorInt = Integer.parseInt(data[index[0]][index[1]]);
            //                String color = "#" + Integer.toHexString(Font.getColor(parsedFontColorInt).getRGB());
            //                style.put(mxConstants.STYLE_FONTCOLOR, color);
            //            } catch (NumberFormatException e) {
            //                LOG.severe(e.toString());
            //                return;
            //            }

            src.setStyle(style.toString());
        }
    }

    public AfficheBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);
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
    public static void setValue(final String uid, final String[][] value) {
        if (value.length == 0 || value[0].length == 0) {
            throw new IllegalArgumentException("value is not a non-empty String matrix (String[][])");
        }

        // update nothing in case of a headless env.
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        synchronized (values) {
            values.put(uid, value);

            if (!printTimer.isRunning()) {
                printTimer.start();
            }
        }
    }
}
