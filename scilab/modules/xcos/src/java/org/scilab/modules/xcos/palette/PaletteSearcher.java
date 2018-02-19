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

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.flexible.core.QueryNodeException;
import org.apache.lucene.queryparser.flexible.standard.StandardQueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * Handles a near real time IndexReader from the IndexWriter.
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
public final class PaletteSearcher {

    private PaletteSearchManager mgr;

    /**
     * Default constructor
     * @param psm PaletteSearchManager
     */
    public PaletteSearcher(PaletteSearchManager psm) {
        mgr = psm;
    }

    /**
     * @param str Query
     * @return paths to the found blocks
     */
    public List<Document> search(String str) {
        List<Document> found = new ArrayList<>();
        try (IndexReader reader = DirectoryReader.open(mgr.getDirectory())) {
            IndexSearcher searcher = new IndexSearcher(reader);

            StandardQueryParser queryParserHelper = new StandardQueryParser();
            queryParserHelper.setAllowLeadingWildcard(true);
            queryParserHelper.setLowercaseExpandedTerms(true);
            queryParserHelper.setAnalyzer(mgr.getAnalyzer());
            queryParserHelper.setMultiFields(new String[] {"refname", "refpurpose", "content"});

            Query query = queryParserHelper.parse(str, null);
            TopDocs results  = searcher.search(query, XcosConstants.MAX_HITS);
            ScoreDoc[] hits = results.scoreDocs;

            if (hits.length == 0) {
                query = queryParserHelper.parse("*" + str + "*", null);
                results  = searcher.search(query, XcosConstants.MAX_HITS);
                hits = results.scoreDocs;
            }

            for (int i = 0; i < hits.length; i++) {
                Document doc = searcher.doc(hits[i].doc);
                found.add(doc);
            }
        } catch (IOException | QueryNodeException e) {
            Logger.getLogger(PaletteSearcher.class.getName()).log(Level.SEVERE, null, e);
        }
        return found;
    }
}
