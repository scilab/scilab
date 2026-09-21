/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.commons.utils;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.Set;
import java.util.function.Consumer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.scilab.modules.localization.Messages;

/**
 * "scilab:" links: pseudo URLs running a Scilab command when opened, such as
 * scilab:doc%20plot, whatever the component showing them.
 *
 * Any content can hold such a link (text read from a file, page loaded from
 * the web, ...) and its label does not have to tell what it runs, so opening
 * a link must never be able to run arbitrary code: only the commands opening
 * a Scilab internal tool are allowed, with plain word arguments (command
 * syntax, "doc plot") or literal string ones (call syntax,
 * "xcos('my file.zcos')").
 *
 * The command is run by the console, as if it had been typed, for the user
 * to see what a link has run. The console registers itself for that with
 * {@link #setConsole}.
 */
public final class ScilabLink {

    public static final String SCHEME = "scilab:";

    /**
     * Commands a link can run: Scilab internal tools only, nothing reaching
     * the system (host, unix, ...), the files (deletefile, ...) or evaluating
     * code (exec, execstr, ...).
     */
    private static final Set<String> ALLOWED_COMMANDS = Set.of(
                // help
                "help", "doc", "apropos", "helpbrowser",
                // editors and browsers
                "edit", "scinotes", "editvar", "browsevar", "filebrowser", "browsehistory",
                // tools
                "xcos", "atomsGui", "preferences", "plotbrowser", "uiimport", "m2sci_gui", "about");

    /**
     * Command syntax only: a name, then arguments made of plain word
     * characters. No separator (";" "," or new line), quote, parenthesis,
     * bracket nor operator, so a link holds a single call and no expression.
     */
    private static final Pattern SYNTAX = Pattern.compile("([A-Za-z_][A-Za-z0-9_]*)((?:[ \\t]+[A-Za-z0-9_%#!$?.:/\\\\~-]+)*)");

    /**
     * Call syntax, for the arguments the command syntax cannot hold (a file
     * name with spaces, ...): a name, then literal strings only, such as
     * xcos("my file.zcos"). A string holds no quote (so it cannot be closed
     * from inside) and nothing else is allowed out of the strings, so the
     * arguments are never evaluated: no variable, nested call nor operator.
     */
    private static final String STRING = "(?:\"[^\"'\\r\\n]*\"|'[^\"'\\r\\n]*')";
    private static final Pattern CALL_SYNTAX = Pattern.compile("([A-Za-z_][A-Za-z0-9_]*)[ \\t]*\\([ \\t]*(?:" + STRING + "(?:[ \\t]*,[ \\t]*" + STRING + ")*)?[ \\t]*\\)");

    private static volatile Consumer<String> executor;
    private static volatile Consumer<String> reporter;

    private ScilabLink() { }

    /**
     * Register the console running the commands of the links
     * @param executor runs a command as if it had been typed in the console
     * @param reporter displays a message in the console
     */
    public static void setConsole(Consumer<String> executor, Consumer<String> reporter) {
        ScilabLink.executor = executor;
        ScilabLink.reporter = reporter;
    }

    /**
     * @param url an URL
     * @return true if it is a "scilab:" link
     */
    public static boolean isScilabLink(String url) {
        return url != null && url.regionMatches(true, 0, SCHEME, 0, SCHEME.length());
    }

    /**
     * @param url a "scilab:" link, percent-encoded or not
     * @return the command it holds
     */
    public static String getCommand(String url) {
        String command = url.substring(SCHEME.length());
        try {
            // "+" is a plain character in a link, not an encoded space
            command = URLDecoder.decode(command.replace("+", "%2B"), "UTF-8");
        } catch (IllegalArgumentException | UnsupportedEncodingException e) {
            // not percent-encoded: keep it as is
        }

        return command.trim();
    }

    /**
     * @param command the command of a link
     * @return true if a link is allowed to run this command
     */
    public static boolean isAllowed(String command) {
        if (command == null) {
            return false;
        }

        Matcher call = CALL_SYNTAX.matcher(command.trim());
        if (call.matches()) {
            return ALLOWED_COMMANDS.contains(call.group(1));
        }

        Matcher matcher = SYNTAX.matcher(command.trim());
        if (!matcher.matches() || !ALLOWED_COMMANDS.contains(matcher.group(1))) {
            return false;
        }

        // ".." continues the line and "//" starts a comment: not word arguments
        String args = matcher.group(2);
        return !args.contains("..") && !args.contains("//");
    }

    /**
     * Open a "scilab:" link: run its command if allowed, tell what has been
     * refused otherwise.
     * @param url the link
     */
    public static void open(String url) {
        if (!isScilabLink(url)) {
            return;
        }

        String command = getCommand(url);
        if (command.isEmpty()) {
            // nothing to run
            return;
        }

        if (!isAllowed(command)) {
            Consumer<String> report = reporter;
            if (report != null) {
                report.accept(String.format(Messages.gettext("Link not allowed to run the command: %s"), command) + "\n");
            }
            return;
        }

        Consumer<String> exec = executor;
        if (exec != null) {
            exec.accept(command);
        }
    }
}
