/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.awt.Dimension;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.swing.ScrollPaneConstants;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.palette.view.PaletteComponent;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosConstants.PaletteBlockSize;

import com.mxgraph.model.mxGeometry;

/**
 * @author Antoine ELIAS
 *
 */
public class PaletteDiagram extends XcosDiagram {

    private String name;
    private String fileName;
    private double windowWidth;
    private PaletteBlockSize palBlockSize;

    /**
     * Constructor
     */
    public PaletteDiagram() {
        super();
        setPaletteBlockSize(PaletteBlockSize.NORMAL);
        setComponent(new PaletteComponent(this));
        installStylesheet();

        setCellsLocked(true);
        setGridVisible(false);
        setCellsDeletable(false);
        setCellsEditable(false);
        this.getAsComponent().setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);

        getUndoManager().setEventsEnabled(false);
    }

    /**
     * @param diagramFileName Palette file
     * @param palBlockSize PaletteBlockSize
     * @return status
     */
    public boolean openDiagramAsPal(PaletteBlockSize palBlockSize, String diagramFileName) {
        File theFile = new File(diagramFileName);

        if (theFile.exists()) {
            try {
                final XcosFileType filetype = XcosFileType.findFileType(theFile);
                filetype.load(diagramFileName, this);
            } catch (Exception e) {
                return false;
            }
            setPaletteBlockSize(palBlockSize);
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

        int blockWidth = palBlockSize.getBlockDimension().width + XcosConstants.PALETTE_HMARGIN;
        int oldRowItem = (int) (newWidth / blockWidth);
        int maxRowItem = (int) (windowWidth / blockWidth);

        // only compute for significant changes
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
                BlockPositioning.updateBlockView(block);
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

        Dimension blockD = palBlockSize.getBlockDimension();
        int blockWidthAndMargin = blockD.width + XcosConstants.PALETTE_HMARGIN;
        int blockHeightAndMargin = blockD.height + XcosConstants.PALETTE_VMARGIN;

        int maxRowItem = (int) (windowWidth / blockWidthAndMargin);
        if (maxRowItem <= 0) {
            maxRowItem = 1;
        }
        int row = blockCount % maxRowItem;
        int col = blockCount / maxRowItem;
        double x = geom.getX();
        double y = geom.getY();
        double w = geom.getWidth();
        double h = geom.getHeight();

        double ratio = Math.min(palBlockSize.getMaxIconWidth() / w,
                                palBlockSize.getMaxIconHeight() / h);
        w *= ratio;
        h *= ratio;

        x = row * blockWidthAndMargin;
        x += (blockD.width - w) / 2;
        y = col * blockHeightAndMargin;
        y += (blockD.height - h) / 2;

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
     * @return PaletteBlockSize
     */
    public PaletteBlockSize getPaletteBlockSize() {
        return this.palBlockSize;
    }

    /**
     * @param pbs PaletteBlockSize
     */
    public void setPaletteBlockSize(PaletteBlockSize pbs) {
        this.palBlockSize = pbs;
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
