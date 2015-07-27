/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette;

import java.io.File;
import java.io.IOException;
import java.util.Hashtable;
import java.util.List;

import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.tree.TreeModel;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.RAMDirectory;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.palette.view.PaletteSearchView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Manager class for the palettes search.
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
public final class PaletteSearchManager {

    private Boolean indexIsOutdated = true;

    private Analyzer analyzer;
    private IndexWriter writer;
    private PaletteIndexer paletteIndexer;
    private PaletteSearcher paletteSearcher;
    private PaletteSearchView view;
    private Hashtable<String, List<PaletteBlock>> ht;

    /** Default constructor */
    public PaletteSearchManager() {
        try {
            Directory ramDir = new RAMDirectory();
            analyzer = new StandardAnalyzer();
            IndexWriterConfig config = new IndexWriterConfig(analyzer);
            config.setOpenMode(OpenMode.CREATE);
            writer = new IndexWriter(ramDir, config);
        } catch (IOException e) {
            e.printStackTrace();
        }

        view = new PaletteSearchView();
        paletteIndexer = new PaletteIndexer(this);
        paletteSearcher = new PaletteSearcher(this);
        ht = new Hashtable<String, List<PaletteBlock>>();
    }

    /**
     * Look for a block into the Index.
     * @param query Query
     */
    public void search(String query) {
        PaletteManagerView.get().getTree().clearSelection();
        view.initComponents();

        final String queryLabel = "'" + query + "' - ";
        view.setText(queryLabel + XcosMessages.SEARCHING);

        JSplitPane splitPanel = (JSplitPane) PaletteManagerView.get().getPanel();
        JScrollPane scrollPane = (JScrollPane) splitPanel.getRightComponent();
        scrollPane.setViewportView(view);

        if (indexIsOutdated) {
            refreshIndex();
            indexIsOutdated = false;
        }

        List<String> blockPaths = paletteSearcher.search(query);
        for (String blockPath : blockPaths) {
            PaletteBlock block = getBlock(blockPath);
            if (block != null) {
                view.addBlock(block);
            }
        }
        view.setText(queryLabel + blockPaths.size() + " " + XcosMessages.MATCHES);
    }

    /**
     * Refreshes the whole Index
     */
    private void refreshIndex() {
        TreeModel model = PaletteManagerView.get().getTree().getModel();
        if (model != null) {
            loadHashTable(model, model.getRoot(), "");
            paletteIndexer.createIndex(ht);
        }
    }

    /**
     * Recursive function that loads a hashtable containing all available blocks.
     * @param model TreeModel
     * @param obj Object
     * @param treePath String
     */
    private void loadHashTable(TreeModel model, Object obj, String treePath) {
        int  cc = model.getChildCount(obj);
        for (int i = 0; i < cc; ++i) {
            PaletteNode node = (PaletteNode) model.getChild(obj, i);
            if (node instanceof PreLoaded) {
                ht.put(treePath + File.separator + node.getName(),
                       ((PreLoaded) node).getBlock());
            } else {
                loadHashTable(model,
                              node,
                              treePath + File.separator + node.toString());
            }
        }
    }

    /**
     * When the Index is outdated, the PaletteSearchManager
     * will refresh it before the next search().
     * @param b True if it is outdated. 
     */
    public void setIndexIsOutdated(Boolean b) {
        indexIsOutdated = b;
    }

    /**
     * Get analyzer
     * @return Analyzer
     */
    public Analyzer getAnalyzer() {
        return analyzer;
    }

    /**
     * Get IndexWriter
     * @return IndexWriter
     */
    public IndexWriter getIndexWriter() {
        return writer;
    }

    /**
     * Get block
     * @param blockPath the block path
     * @return PaletteBlock
     */
    public PaletteBlock getBlock(String blockPath) {
        int lastSeparator = blockPath.lastIndexOf(File.separator);
        String treePath = blockPath.substring(0, lastSeparator);
        String blockName = blockPath.substring(lastSeparator + 1);
        List<PaletteBlock> blocks = ht.get(treePath);
        if (blocks == null) {
            return null;
        }

        for (PaletteBlock block : blocks) {
            if (blockName.equals(block.getName())) {
                return block;
            }
        }
        return null;
    }
}
