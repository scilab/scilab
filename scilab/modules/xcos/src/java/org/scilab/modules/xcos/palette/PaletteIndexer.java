/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 * Copyright (C) 2017 - Scilab Enterprises - Clement DAVID
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
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.lucene.document.Document;

import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;

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
        // javaHelp directories
        roots = new ArrayList<>();

        // for local builds
        roots.add(new File(ScilabConstants.SCI.getAbsolutePath() + "/modules/helptools/javaHelp"));

        // for binary version
        roots.add(new File(ScilabConstants.SCI.getAbsolutePath() + "/modules/helptools/jar"));
    }

    public void createIndex(Map<String, PaletteBlock> blockNameToPalette) {
        try {
            mgr.getIndexWriter().deleteAll();

            // insert all block names
            for (String blk : blockNameToPalette.keySet()) {
                index(blk);
            }

            // insert all help pages
            for (File r : roots) {
                if (!r.exists()) {
                    continue;
                }

                Files.walkFileTree(r.toPath(), new SimpleFileVisitor<Path>() {
                    @Override
                    public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                        String fname = file.getFileName().toString();
                        int dot = fname.lastIndexOf('.');
                        if (dot <= 0) {
                            return FileVisitResult.CONTINUE;
                        }

                        String basename = fname.substring(0, dot);
                        if (fname.endsWith(".jar")) {
                            try (JarFile jf = new JarFile(file.toFile())) {
                                index(file.toAbsolutePath().toUri().toString(), basename, jf, blockNameToPalette);
                            }
                        } else if (fname.endsWith(".html")) {
                            // this is a regular file
                            if (blockNameToPalette.containsKey(basename)) {
                                index(basename, file.toUri().toURL());
                            }
                        }

                        return FileVisitResult.CONTINUE;
                    }
                });
            }

            mgr.getIndexWriter().commit();
        } catch (IOException ex) {
            Logger.getLogger(PaletteIndexer.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    private void index(String fileURI, String basename, JarFile jf, Map<String, PaletteBlock> blockNameToTreePath) {
        class Entry {

            Entry(JarEntry jarEntry) {
                je = jarEntry;
            }

            JarEntry je;
            String blk;
            URL url;

            boolean block() {
                int slash = je.getName().lastIndexOf('/') + 1;
                int dot = je.getName().lastIndexOf('.');
                if (slash < 0 || dot < 0) {
                    return false;
                }

                blk = je.getName().substring(slash, dot);
                return true;
            }

            boolean url() {
                try {
                    // we have to generate a url in the form before indexing
                    // jar:file:/modules/helptools/jar/scilab_en_US_help.jar!/scilab_en_US_help/blockName.html
                    url = new URL("jar:" + fileURI + "!/" + basename + '/' + blk + ".html");
                } catch (MalformedURLException ex) {
                    Logger.getLogger(PaletteIndexer.class.getName()).log(Level.SEVERE, null, ex);
                }
                return true;
            }
        }

        jf.stream()
        .map(je -> new Entry(je))
        .filter(e -> e.block())
        .filter(e -> blockNameToTreePath.containsKey(e.blk))
        .filter(e -> e.url())
        .forEach(e -> index(e.blk, e.url));
    }

    private void index(String basename, URL url) {
        try {
            Document doc = new Document();

            // add the block name
            Field refname = new TextField("refname", basename, Field.Store.YES);
            refname.setBoost(100f);
            doc.add(refname);

            // add the refpurpose
            try (BufferedReader r = new BufferedReader(new InputStreamReader(url.openStream()))) {
                Optional<String> found = r.lines().filter(l -> l.contains("refpurpose")).findFirst();

                Field refpurpose;
                if (found.isPresent()) {
                    refpurpose = new TextField("refpurpose", found.get(), Field.Store.YES);
                } else {
                    refpurpose = new TextField("refpurpose", "", Field.Store.YES);
                }

                refpurpose.setBoost(10f);
                doc.add(refpurpose);
            }

            // add the html content
            try (BufferedReader r = new BufferedReader(new InputStreamReader(url.openStream()))) {
                doc.add(new TextField("content", r));
            }

            mgr.getIndexWriter().addDocument(doc);
        } catch (IOException e) {
            Logger.getLogger(PaletteIndexer.class.getName()).log(Level.SEVERE, null, e);
        }
    }

    private void index(String block) {
        try {
            Document doc = new Document();
            doc.add(new StringField("refname", block, Field.Store.YES));
            doc.add(new StringField("refpurpose", block, Field.Store.YES));
            doc.add(new TextField("content", block, Field.Store.YES));

            mgr.getIndexWriter().addDocument(doc);
        } catch (IOException e) {
            Logger.getLogger(PaletteIndexer.class.getName()).log(Level.SEVERE, null, e);
        }
    }
}
