/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

package org.scilab.modules.xcos.graph;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.swing.ScrollPaneConstants;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.palette.view.PaletteComponent;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosConstants;

import com.mxgraph.model.mxGeometry;
import java.rmi.server.UID;
import org.scilab.modules.xcos.JavaController;

/**
 * @author Antoine ELIAS
 *
 */
public class PaletteDiagram extends XcosDiagram {

    private static final int BLOCK_MAX_WIDTH = (int) (XcosConstants.PALETTE_BLOCK_WIDTH * 0.8); // 80%
    private static final int BLOCK_MAX_HEIGHT = (int) (XcosConstants.PALETTE_BLOCK_HEIGHT * 0.8); // 80%

    private String name;
    private String fileName;
    private double windowWidth;

    /**
     * Constructor
     */
    public PaletteDiagram(long uid) {
        super(new JavaController(), uid, Kind.DIAGRAM, new UID().toString());
        setComponent(new PaletteComponent(this));

        setTitle(PaletteDiagram.class.getName());

        setCellsLocked(true);
        setGridVisible(false);
        setCellsDeletable(false);
        setCellsEditable(false);
        this.getAsComponent().setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);

        getUndoManager().setEventsEnabled(false);
    }

    /**
     * @param diagramFileName
     *            palette file
     * @return status
     */
    public boolean openDiagramAsPal(String diagramFileName) {
        File theFile = new File(diagramFileName);

        if (theFile.exists()) {
            try {
                final XcosFileType filetype = XcosFileType.findFileType(theFile);
                filetype.load(diagramFileName, this);
            } catch (Exception e) {
                return false;
            }
            setName(theFile.getName());
            setFileName(theFile.getAbsolutePath());
            getRubberBand().setEnabled(false);

            /* change some diagram parameters */
            /* delete all links */
            List<Object> tobeRemoved = new ArrayList<Object>();
            for (int i = 0; i < getModel().getChildCount(getDefaultParent()); i++) {
                Object obj = getModel().getChildAt(getDefaultParent(), i);
                if (obj instanceof BasicLink || obj instanceof SplitBlock || obj instanceof TextBlock) {
                    tobeRemoved.add(obj);
                }
            }
            for (Object object : tobeRemoved) {
                getModel().remove(object);
            }

            return true;
        }
        return false;
    }

    /**
     * @param newWidth
     *            update diagram width
     */
    public void updateDiagram(double newWidth) {

        if (newWidth == windowWidth) {
            return;
        }

        int oldRowItem = (int) (newWidth / (XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN));
        int maxRowItem = (int) (windowWidth / (XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN));

        // only compute for signifiant changes
        if (oldRowItem == maxRowItem) {
            return;
        }

        windowWidth = newWidth;
        int blockCount = 0;

        getModel().beginUpdate();
        for (int i = 0; i < getModel().getChildCount(getDefaultParent()); i++) {
            Object obj = getModel().getChildAt(getDefaultParent(), i);
            if (obj instanceof BasicBlock) {
                BasicBlock block = (BasicBlock) obj;
                block.setGeometry(getNewBlockPosition(block.getGeometry(), blockCount));
                BlockPositioning.updateBlockView(this, block);
                blockCount++;
            }
        }
        getModel().endUpdate();
        refresh();
        setModified(false);
    }

    /**
     * @param geom
     *            current block geometry
     * @param blockCount
     *            block index
     * @return new geometry
     */
    private mxGeometry getNewBlockPosition(mxGeometry geom, int blockCount) {

        int maxRowItem = (int) (windowWidth / (XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN));
        if (maxRowItem <= 0) {
            maxRowItem = 1;
        }
        int row = blockCount % maxRowItem;
        int col = blockCount / maxRowItem;
        double x = geom.getX();
        double y = geom.getY();
        double w = geom.getWidth();
        double h = geom.getHeight();

        if (geom.getWidth() > BLOCK_MAX_WIDTH || geom.getHeight() > BLOCK_MAX_HEIGHT) {
            // update block size to fill "block area"
            double ratio = Math.min(BLOCK_MAX_HEIGHT / h, BLOCK_MAX_WIDTH / w);
            w *= ratio;
            h *= ratio;
        }

        x = row * (XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN);
        x += (XcosConstants.PALETTE_BLOCK_WIDTH - w) / 2;
        y = col * (XcosConstants.PALETTE_BLOCK_HEIGHT + XcosConstants.PALETTE_VMARGIN);
        y += (XcosConstants.PALETTE_BLOCK_HEIGHT - h) / 2;

        return new mxGeometry(x, y, w, h);
    }

    /**
     * @return name
     */
    public String getName() {
        return name;
    }

    /**
     * @param name
     *            palette name
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Always return false as we cannot draw links on the palette diagram.
     *
     * @param cell
     *            the cell we are workling on
     * @return always false
     * @see org.scilab.modules.xcos.graph.XcosDiagram#isCellConnectable(java.lang.Object)
     */
    @Override
    public boolean isCellConnectable(Object cell) {
        return false;
    }

    /**
     * @param fileName
     *            palette filename
     */
    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    /**
     * @return palette filename
     */
    public String getFileName() {
        return fileName;
    }

}
