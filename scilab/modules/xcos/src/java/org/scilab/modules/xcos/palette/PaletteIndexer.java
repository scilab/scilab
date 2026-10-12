/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 * Copyright (C) 2017 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * https://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.xcos.palette;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.lucene.document.Document;

import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.xcos.palette.model.PaletteBlock;

/**
 * Index the help pages of all blocks.
 *
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
public final class PaletteIndexer {

    private PaletteSearchManager mgr;
    private List<File> roots;

    /**
     * Default constructor
     *
     * @param psm PaletteSearchManager
     */
    public PaletteIndexer(PaletteSearchManager psm) {
        mgr = psm;
        // XML help pages of the blocks: current language first, en_US for the missing ones
        roots = new ArrayList<>();
        String help = ScilabConstants.SCI.getAbsolutePath() + "/modules/xcos/help/";
        String lang = ScilabCommons.getlanguage();
        if (lang != null && !lang.isEmpty() && !lang.equals("en_US")) {
            roots.add(new File(help + lang));
        }
        roots.add(new File(help + "en_US"));
    }

    public void createIndex(Map<String, PaletteBlock> blockNameToPalette) {
        try {
            mgr.getIndexWriter().deleteAll();
            
            HashSet<String> blocks = new HashSet<>(blockNameToPalette.keySet());

            // insert all help pages
            for (File r : roots) {
                if (!r.exists()) {
                    continue;
                }

                Files.walkFileTree(r.toPath(), new SimpleFileVisitor<Path>() {
                    @Override
                    public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                        String fname = file.getFileName().toString();
                        if (!fname.endsWith(".xml")) {
                            return FileVisitResult.CONTINUE;
                        }

                        String basename = fname.substring(0, fname.length() - 4);
                        if (blocks.remove(basename)) {
                            index(basename, file.toUri().toURL());
                        }

                        return FileVisitResult.CONTINUE;
                    }
                });
            }
            
            // insert all missing block names
            for (String blk : blocks) {
                index(blk);
            }

            mgr.getIndexWriter().commit();
        } catch (IOException ex) {
            Logger.getLogger(PaletteIndexer.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    private void index(String basename, URL url) {
        try {
            Document doc = new Document();

            // add the block name
            Field refname = new StringField("refname", basename, Field.Store.YES);
            doc.add(refname);

            // add the refpurpose
            try (BufferedReader r = new BufferedReader(new InputStreamReader(url.openStream(), "UTF-8"))) {
                Optional<String> found = r.lines().filter(l -> l.contains("refpurpose")).findFirst();

                Field refpurpose;
                if (found.isPresent()) {
                    refpurpose = new TextField("refpurpose", found.get(), Field.Store.NO);
                } else {
                    refpurpose = new TextField("refpurpose", "", Field.Store.NO);
                }

                doc.add(refpurpose);
            }

            // add the html content
            try (BufferedReader r = new BufferedReader(new InputStreamReader(url.openStream(), "UTF-8"))) {
                doc.add(new TextField("content", r));
                
                mgr.getIndexWriter().addDocument(doc);
            }
        } catch (IOException e) {
            Logger.getLogger(PaletteIndexer.class.getName()).log(Level.SEVERE, null, e);
        }
    }

    private void index(String block) {
        try {
            Document doc = new Document();
            doc.add(new StringField("refname", block, Field.Store.YES));
            doc.add(new TextField("refpurpose", block, Field.Store.NO));
            doc.add(new TextField("content", block, Field.Store.NO));

            mgr.getIndexWriter().addDocument(doc);
        } catch (IOException e) {
            Logger.getLogger(PaletteIndexer.class.getName()).log(Level.SEVERE, null, e);
        }
    }
}
