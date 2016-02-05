/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.helptools.source;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.scilab.modules.helptools.scilab.AbstractScilabCodeHandler;
import org.scilab.modules.helptools.scilab.HTMLScilabCodeHandler;
import org.scilab.modules.helptools.scilab.ScilabLexer;

public class ScilabSourceBrowser extends HTMLScilabCodeHandler {

    private static final String entete = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n          \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"{$lang}\" lang=\"{$lang}\">\n  <head{$profile}>\n    <title><!--<title>--> <!--<subtitle>--></title>\n    <style type=\"text/css\" media=\"all\">\n      @import url(\"site.css\");\n      @import url(\"css/scilab_code.css\");\n      @import url(\"css/style.css\");\n    </style>\n    <!--[if IE]><![endif]><![endif]-->\n    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>\n    <!-- <link rel=\"shortcut icon\" href=\"{$_SERVER['STATIC_ROOT']}/favicon.ico\" />{$link}\n     $canonical\n    <script type=\"text/javascript\" src=\"{$_SERVER['STATIC_ROOT']}/userprefs.js\"></script>{$base}{$meta}{$moreheadtags}\n        -->\n  </head>\n  <body>";

    private List<String> files;
    private Map<String, Set<String>> macroUsage = new HashMap<String, Set<String>>();
    private ScilabLexer scilabLexer;
    private String outputDirectory;
    private Map<String, String> mapId;

    public static void main(String[] args) {
        ScilabSourceBrowser b = new ScilabSourceBrowser(args[0], args[1], args[2], args[3]);
        b.generateSource();
    }

    public ScilabSourceBrowser(String paths, String primFile, String macroFile, String outputDirectory) {
        super();
        this.outputDirectory = outputDirectory;
        scilabLexer = new ScilabLexer(primFile, macroFile);
        generateMapId(paths);
    }

    public void generateSource() {
        new FirstPass().getMacroUsage();
        for (String file : files) {
            try {
                File f = new File(file);
                System.out.println(f);
                Reader input = new BufferedReader(new FileReader(f));
                currentCommand = f.getName().split("\\.")[0];
                buffer = new FileWriter(outputDirectory + File.separator + currentCommand + ".html");
                buffer.append(entete);
                buffer.append("<div style=\"code\"><pre>");
                scilabLexer.convert(this, input, false);
                buffer.append("</pre></div>\n</body>\n</html>");
                ((Writer) buffer).flush();
                ((Writer) buffer).close();
                input.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void handleCommand(String seq) throws IOException {
        buffer.append("<span class=\"command\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionId(String seq) throws IOException {
        buffer.append("<a href=\"#");
        buffer.append(seq);
        buffer.append("\"><span class=\"functionid\">");
        buffer.append(seq);
        buffer.append("</span></a>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionIdDecl(String seq) throws IOException {
        buffer.append("<a name=\"");
        buffer.append(seq);
        buffer.append("\"></a><span class=\"functionid\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgs(String seq) throws IOException {
        buffer.append("<a href=\"#");
        buffer.append(seq);
        buffer.append("\"><span class=\"inputoutputargs\">");
        buffer.append(seq);
        buffer.append("</span></a>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgsDecl(String seq) throws IOException {
        buffer.append("<a name=\"");
        buffer.append(seq);
        buffer.append("\"></a><span class=\"inputoutputargs\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    private void generateMapId(String filename) {
        BufferedReader input = null;
        files = new ArrayList<String>();
        mapId = new HashMap<String, String>();
        try {
            input = new BufferedReader(new FileReader(filename));
            String line = null;
            while ((line = input.readLine()) != null) {
                files.add(line);
                String name = new File(line).getName().split("\\.")[0];
                mapId.put(name, name + ".html");
            }
        } catch (IOException e) {
            System.err.println(e);
        }
        if (input != null) {
            try {
                input.close();
            } catch (IOException e) {
                System.err.println(e);
            }
        }
    }

    class FirstPass extends AbstractScilabCodeHandler {

        FirstPass() { }

        void getMacroUsage() {
            for (String file : files) {
                try {
                    File f = new File(file);
                    Reader input = new BufferedReader(new FileReader(f));
                    currentCommand = f.getName().split("\\.")[0];
                    scilabLexer.convert(this, input, false);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        public void handleMacro(String macro) throws IOException {
            Set<String> set = macroUsage.get(macro);
            if (set == null) {
                set = new HashSet<String>();
                macroUsage.put(macro, set);
            }
            set.add(currentCommand);
        }
    }
}
