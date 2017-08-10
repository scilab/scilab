/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011-2017 - Scilab Enterprises - Clement DAVID
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
import com.mxgraph.util.mxConstants;
import com.mxgraph.view.mxCellState;
import java.awt.GraphicsEnvironment;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serializable;
import java.util.Formatter;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map.Entry;
import java.util.TreeMap;

import javax.swing.Timer;
import org.scilab.modules.graph.utils.Font;

import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.XcosGraphModel;

/**
 * Implement the AFFICH_m block
 */
public final class AfficheBlock extends BasicBlock {
    /**
     * Default refresh rate used on the simulation to update block.
     */
    private static final int DEFAULT_TIMER_RATE = 200;

    private static final String NEW_LINE = System.getProperty("line.separator");
    private static final String SPACE = "  ";

    private static final TreeMap<String, String[][]> values = new TreeMap<>();
    private static final UpdateValueListener updateAction = new UpdateValueListener();
    private static final Timer printTimer = new Timer(DEFAULT_TIMER_RATE, updateAction);

    /**
     * Update the value of the associated block
     */
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
            List<XcosDiagram> diags = Xcos.getInstance().openedDiagrams();

            synchronized (values) {
                for (Iterator<Entry<String, String[][]>> it = values.entrySet().iterator(); it.hasNext();) {
                    final Entry<String, String[][]> entry = it.next();

                    update(diags, entry.getKey(), entry.getValue());

                    it.remove();
                }
            }
        }

        /**
         * Update and refresh the values
         */
        private void update(List<XcosDiagram> diags, final String uid, final String[][] data) {
            /* Look for the cell uid, keeping its diag Diagram available */
            class DiagramBlockPair {
                final XcosDiagram diag;
                final Object block;

                DiagramBlockPair(final XcosDiagram p, final Object b) {
                    diag = p;
                    block = b;
                }
            }
            DiagramBlockPair found = diags.stream()
                                     .map(d -> new DiagramBlockPair(d, ((XcosGraphModel)d.getModel()).getCell(uid)))
                                     .filter(p -> p.block != null)
                                     .findAny().orElse(null);
            if (found == null) {
                return;
            }

            /* render and refresh */
            final String value = getText(data);
            found.diag.getModel().setValue(found.block, value);

            final mxCellState state = found.diag.getView().getState(found.block);
            if (state != null) {
                state.setLabel(value);
            }

            found.diag.getAsComponent().redraw(state);
        }
    }

    public AfficheBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);
    }

    @Override
    protected void updateStyle(JavaController controller, XcosDiagram parent, BasicBlock modifiedBlock) {
        final StyleMap styleMap = new StyleMap(modifiedBlock.getStyle());

        VectorOfInt ipar = new VectorOfInt();
        controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.IPAR, ipar);

        // font
        final int parsedFontInt = ipar.get(0);
        styleMap.put(mxConstants.STYLE_FONTFAMILY, Font.getFont(parsedFontInt).getName());

        // fontsize
        final int parsedFontSizeInt = ipar.get(1);
        styleMap.put(mxConstants.STYLE_FONTSIZE, Integer.toString(Font.getSize(parsedFontSizeInt)));

        // colr
        final int parsedFontColorInt = ipar.get(2);
        String color = "#" + Integer.toHexString(Font.getColor(parsedFontColorInt).getRGB());
        styleMap.put(mxConstants.STYLE_FONTCOLOR, color);

        parent.getModel().setStyle(this, styleMap.toString());
    }

    @Override
    protected void updateValue(JavaController controller, XcosDiagram parent, BasicBlock modifiedBlock) {

        VectorOfInt ipar = new VectorOfInt();
        controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.IPAR, ipar);

        VectorOfDouble dstate = new VectorOfDouble();
        controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.DSTATE, dstate);

        // nt
        final int width = ipar.get(3);

        // nd
        final int rational = ipar.get(4);

        // in(1,1)
        final int rows = ipar.get(5);
        final int cols = (dstate.size() - 6) / rows;

        final String format = "%" + Integer.toString(width) + "." + Integer.toString(rational) + "f";
        final StringBuilder sb = new StringBuilder();
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                sb.append(new Formatter(Locale.US).format(format, 0.0).toString());
                sb.append(SPACE);
            }
            sb.append(NEW_LINE);
        }

        parent.getModel().setValue(this, sb.toString());
    }

    /**
     * Construct a String representation of the values.
     */
    private static String getText(final String[][] data) {
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
