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

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.List;
import java.util.Set;

import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.scilab.modules.xcos.palette.model.PaletteBlock;

/**
 * Index the help pages of all blocks.
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
public final class PaletteIndexer {

    private PaletteSearchManager mgr;
    private List<String> roots;

    /**
     * Default constructor 
     * @param psm PaletteSearchManager
     */
    public PaletteIndexer(PaletteSearchManager psm) {
        mgr = psm;
        // javaHelp directories
        roots = new ArrayList<String>();
        roots.add("/home/cardinot/scilab/scilab/modules/helptools/javaHelp"); // TODO fix path
    }

    /**
     * @param ht Hashtable
     */
    public void createIndex(Hashtable<String, List<PaletteBlock>> ht) {
        try {
            mgr.getIndexWriter().deleteAll();
            Set<String> treePaths = ht.keySet();
            for (String treePath : treePaths) {
                List<PaletteBlock> blocks = ht.get(treePath);
                for (PaletteBlock block : blocks) {
                    indexBlock(treePath, block.getName());
                }
            }
            mgr.getIndexWriter().commit();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * @param treePath tree path
     * @param blockName block name
     * @throws IOException If there is a low-level I/O error
     */
    private void indexBlock(String treePath, String blockName) throws IOException {
        Document doc = new Document();
        doc.add(new StringField("blockName", blockName, Field.Store.YES));
        doc.add(new StringField("treePath", treePath, Field.Store.YES));

        List<File> helpPages = findHelpPages(blockName);
        if (helpPages.isEmpty()) {
            doc.add(new TextField("helpPage", blockName, Field.Store.YES));
        } else {
            for (File helpPage : helpPages) {
                InputStream stream = new FileInputStream(helpPage);
                doc.add(new StringField("filePath", helpPage.getAbsolutePath(), Field.Store.YES));
                doc.add(new TextField("helpPage", new BufferedReader(new InputStreamReader(stream, StandardCharsets.UTF_8))));
            }
        }

        mgr.getIndexWriter().addDocument(doc);
    }

    /**
     * @param blockName block name
     * @return File[]
     */
    private List<File> findHelpPages(final String blockName) {
        List<String> subdirs = new ArrayList<String>();
        for (String root : roots) {
            File r = new File(root);
            if (!r.exists()) {
                continue;
            }

            String[] ss = r.list(new FilenameFilter() {
                public boolean accept(File current, String name) {
                    return new File(current, name).isDirectory();
                }
            });
            for (int i = 0; i < ss.length; ++i) {
                ss[i] = root + File.separator + ss[i];
            }
            subdirs.addAll(Arrays.asList(ss));
        }

        List<File> helpPages = new ArrayList<File>();
        for (String dir : subdirs) {
            File file = new File(dir + File.separator + blockName + ".html");
            if (file.exists()) {
                helpPages.add(file);
            }
        }
        return helpPages;
    }
}
