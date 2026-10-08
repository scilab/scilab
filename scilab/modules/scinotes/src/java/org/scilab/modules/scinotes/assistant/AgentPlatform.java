package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;

/**
 * Operating system helpers to find and start the agent programs: install
 * directories, PATH lookup, a clean environment, process tree kill.
 */
final class AgentPlatform {

    private AgentPlatform() {
    }

    static boolean isWindows() {
        return System.getProperty("os.name", "").toLowerCase().contains("win");
    }

    /**
     * The user's real home directory. Scilab sets the user.home property to
     * SCIHOME on every OS: use USERPROFILE on Windows, HOME elsewhere, and
     * ScilabConstants.USERHOME (read before the override) as a fallback.
     */
    static String userHome() {
        String home = System.getenv(isWindows() ? "USERPROFILE" : "HOME");
        if (home == null || home.isEmpty()) {
            home = org.scilab.modules.commons.ScilabConstants.USERHOME;
        }
        return home;
    }

    /** Search PATH for one of the given file names; null when absent. */
    static String findOnPath(String... names) {
        String path = System.getenv("PATH");
        if (path == null) {
            return null;
        }
        for (String dir : path.split(File.pathSeparator)) {
            if (dir.isEmpty()) {
                continue;
            }
            for (String name : names) {
                File f = new File(dir, name);
                if (f.isFile()) {
                    return f.getAbsolutePath();
                }
            }
        }
        return null;
    }

    /**
     * Usual install directories of the programs on Linux and macOS: native
     * installers and pipx/uv (~/.local/bin), Homebrew (/opt/homebrew/bin on
     * Apple Silicon, /usr/local/bin), npm prefixes, volta, bun and nvm
     * (newest node first). Needed because Scilab launched from the macOS
     * Dock or a desktop menu does not get the user's shell PATH.
     */
    static List<String> unixBinDirs() {
        String home = userHome();
        List<String> dirs = new ArrayList<>(Arrays.asList(
            home + "/.local/bin",
            "/opt/homebrew/bin",
            "/usr/local/bin",
            home + "/.npm-global/bin",
            home + "/.volta/bin",
            home + "/.bun/bin"));
        File[] nodes = new File(home, ".nvm/versions/node").listFiles(File::isDirectory);
        if (nodes != null) {
            Arrays.sort(nodes, (a, b) -> b.getName().compareTo(a.getName()));
            for (File n : nodes) {
                dirs.add(new File(n, "bin").getPath());
            }
        }
        return dirs;
    }

    /**
     * Find an executable on Linux/macOS: the usual install directories,
     * then the PATH. Null when absent.
     */
    static String findUnix(String... names) {
        for (String dir : unixBinDirs()) {
            for (String name : names) {
                File f = new File(dir, name);
                if (f.isFile() && f.canExecute()) {
                    return f.getPath();
                }
            }
        }
        return findOnPath(names);
    }

    /**
     * Adapt the environment of an agent process to Linux/macOS (no-op on
     * Windows):
     * - PATH: the program's own directory and the usual install directories
     *   come first. npm programs are "#!/usr/bin/env node" scripts: without
     *   node's directory in PATH (Scilab started from the macOS Dock gets
     *   /usr/bin:/bin only), they die with "env: node: No such file".
     * - LD_LIBRARY_PATH / DYLD_*: drop the entries bin/scilab put for
     *   Scilab's own libraries (its OpenSSL, libxml2... must not be loaded
     *   by the agents or the tools they spawn).
     */
    static void adaptUnixEnvironment(ProcessBuilder pb, String binary) {
        if (isWindows()) {
            return;
        }
        Map<String, String> env = pb.environment();

        LinkedHashSet<String> path = new LinkedHashSet<>();
        if (binary != null) {
            File dir = new File(binary).getAbsoluteFile().getParentFile();
            if (dir != null) {
                path.add(dir.getPath());
            }
        }
        path.addAll(unixBinDirs());
        String current = env.get("PATH");
        if (current != null) {
            path.addAll(Arrays.asList(current.split(File.pathSeparator)));
        }
        path.remove("");
        env.put("PATH", String.join(File.pathSeparator, path));

        List<File> sciRoots = scilabRoots();
        for (String var : new String[] {"LD_LIBRARY_PATH", "DYLD_LIBRARY_PATH", "DYLD_FALLBACK_LIBRARY_PATH"}) {
            String value = env.get(var);
            if (value == null) {
                continue;
            }
            List<String> kept = new ArrayList<>();
            for (String entry : value.split(File.pathSeparator)) {
                if (!entry.isEmpty() && !entry.equals(".") && !isInsideAny(entry, sciRoots)) {
                    kept.add(entry);
                }
            }
            if (kept.isEmpty()) {
                env.remove(var);
            } else {
                env.put(var, String.join(File.pathSeparator, kept));
            }
        }
    }

    /** SCI, and the install root when SCI is <root>/share/scilab. */
    private static List<File> scilabRoots() {
        List<File> roots = new ArrayList<>();
        File sci = org.scilab.modules.commons.ScilabConstants.SCI;
        if (sci != null) {
            roots.add(sci);
            File share = sci.getParentFile();
            if (share != null && "share".equals(share.getName()) && share.getParentFile() != null) {
                roots.add(share.getParentFile());
            }
        }
        return roots;
    }

    private static boolean isInsideAny(String path, List<File> roots) {
        try {
            String p = new File(path).getCanonicalPath() + File.separator;
            for (File r : roots) {
                if (p.startsWith(r.getCanonicalPath() + File.separator)) {
                    return true;
                }
            }
        } catch (java.io.IOException e) {
            // unresolvable entry: keep it
        }
        return false;
    }

    /**
     * Kill a process and its whole tree: npm launchers and shell wrappers
     * start the real agent as a child, which would survive and keep calling
     * the Scilab tools.
     */
    static void killTree(Process p) {
        if (p == null) {
            return;
        }
        try {
            p.descendants().forEach(ProcessHandle::destroyForcibly);
        } catch (Exception e) {
            // best effort
        }
        p.destroyForcibly();
    }
}
