/**
 * Scilab Assistant GUI — JCEF bridge application
 *
 * Bridge: assistantQuery({request, onSuccess, onFailure}) provided by CefMessageRouter
 * (not named toScilab, which is used by browser uicontrols: see ScilabBrowser.js)
 * Handler: AssistantMessageHandler.java
 */

// ============================================
// State
// ============================================
let isProcessing = false;
let providersLoaded = false;     // the provider list arrived from Java
let activeTaskId = null;
let pollTimerId = null;
let stopRequested = false;
let streamingText = '';          // accumulated text during streaming
let streamingMessageEl = null;   // live assistant message element
let pendingImages = [];          // base64 data URLs of attached images
let pendingFiles = [];           // [{path, name, content}] attached via @
let mentionActive = false;       // is the @ dropdown open?
let fileListCache = null;        // cached file list from Java
let mentionSelectedIdx = 0;      // selected index in dropdown
let currentSessionId = '';       // current agent session ID
let currentConversationId = '';  // the panel's own id of the conversation
let replayContext = false;       // the agent may have lost the conversation
                                 // (reopened, agent changed): send it the gist
let conversations = [];          // [{id, title, provider, date}]
let providers = [];              // agents: [{id, name, found, path, custom_path, customized, custom}]
let currentProviderId = '';      // active AI provider id
let currentModelId = '';         // active model id for that provider
let savedModels = {};            // provider id -> last model used, from assistant.json
let savedEfforts = {};           // provider id -> last effort used, from assistant.json
let currentEffort = '';          // effort level of the agent, '' for its default
let probeCache = {};             // provider id -> live check result (config page)
let assistantSettings = null;    // Java-side settings ({session_logs, open_with_scinotes})
let logsDir = '';                // session logs directory (display only)

// ============================================
// DOM Elements
// ============================================
const chatContainer = document.getElementById('chat-container');
const messagesContainer = document.getElementById('messages');
const stickyPrompt = document.getElementById('sticky-prompt');
const promptInput = document.getElementById('prompt-input');
const sendBtn = document.getElementById('send-btn');
const providerChip = document.getElementById('provider-chip');
const providerChipLabel = document.getElementById('provider-chip-label');
const configView = document.getElementById('config-view');
const configList = document.getElementById('config-list');
// theme toggle button removed — theme will be controlled globally by Scilab
const clearBtn = document.getElementById('clear-btn');
const attachBtn = document.getElementById('attach-btn');
const fileInput = document.getElementById('file-input');
const imagePreview = document.getElementById('image-preview');
const filePreview = document.getElementById('file-preview');
const mentionDropdown = document.getElementById('mention-dropdown');
const editorContextBar = document.getElementById('editor-context');
const editorContextFile = document.getElementById('editor-context-file');
const editorContextSelection = document.getElementById('editor-context-selection');

const SEND_ICON = `<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M22 2L11 13M22 2l-7 20-4-9-9-4 20-7z" /></svg>`;
const STOP_ICON = `<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="6" y="6" width="12" height="12" rx="1" ry="1" fill="currentColor" stroke="none"></rect></svg>`;

// ============================================
// Bridge
// ============================================
function sendToJava(msg, callback) {
    const json = typeof msg === 'string' ? msg : JSON.stringify(msg);
    if (typeof assistantQuery === 'function') {
        assistantQuery({
            request: json,
            onSuccess: function(response) {
                try {
                    const data = JSON.parse(response);
                    if (callback) callback(data);
                } catch (e) {
                    if (callback) callback({ error: 'JSON parse error: ' + e.message });
                }
            },
            onFailure: function(errorCode, errorMessage) {
                if (callback) callback({ error: errorMessage || 'Bridge error ' + errorCode });
            }
        });
    } else {
        console.error('assistantQuery bridge not available');
        if (callback) callback({ error: 'Bridge not available' });
    }
}

// ============================================
// Initialization
// ============================================
document.addEventListener('DOMContentLoaded', () => {
    initTheme();
    setupEventListeners();
    setupStickyPrompt();
    autoResizeTextarea();
    loadConversationList();
    updateConversationList();
    loadProviders();
    // Note: initial editor context is sent via CefLoadHandler.onLoadEnd() in Java
});

// ============================================
// Providers & models
// ============================================
/** Link opening the assistant help page, where each agent's installation is
 *  described (handled by a document-wide listener). */
const HELP_LINK = '<a href="#" class="open-help">help page</a>';

function getProvider(id) {
    return providers.find(p => p.id === id) || null;
}

function selectedProvider() {
    return getProvider(currentProviderId);
}

/** Models an agent offers, as {id, label}: known once the agent runs
 *  (probe_provider), empty before. */
function providerModels(p) {
    const probe = probeCache[p.id];
    return probe && probe.models ? probe.models : [];
}

/** Effort levels an agent offers, as {id, label}: same origin. */
function providerEfforts(p) {
    const probe = probeCache[p.id];
    return probe && probe.efforts ? probe.efforts : [];
}

function modelLabel(p, id) {
    const m = providerModels(p).find(m => m.id === id);
    return m ? m.label : id;
}

/** Fetch the provider list once, then restore the last selection. */
function loadProviders() {
    renderProviderChip();
    sendBtn.disabled = true;
    sendToJava({ type: 'get_providers' }, (resp) => {
        // Never leave the panel locked, even on a bad answer
        providersLoaded = true;
        sendBtn.disabled = false;
        if (!resp || !resp.providers || resp.providers.length === 0) {
            renderProviderChip();
            return;
        }
        providers = resp.providers;

        // The user's last choice is kept in assistant.json (Java side)
        const selection = resp.selection || {};
        savedModels = selection.models || {};
        savedEfforts = selection.efforts || {};
        const saved = selection.provider;

        // Restore the user's last choice; else the first INSTALLED provider
        let id;
        if (saved && getProvider(saved)) {
            id = saved;
        } else {
            const firstFound = providers.find(p => p.found);
            id = firstFound ? firstFound.id
                : (resp.current && getProvider(resp.current) ? resp.current : providers[0].id);
        }
        setProvider(id, false);
    });
}

/** Fetch the agent list again after its configuration changed (agent
 *  added or removed, customized flag), keeping the live checks. Then
 *  selects `selectId` when given, else keeps the current agent (or the
 *  first installed one when it is gone). */
function refreshProviders(selectId) {
    sendToJava({ type: 'get_providers' }, (resp) => {
        if (!resp || !resp.providers || resp.providers.length === 0) return;
        resp.providers.forEach(np => {
            const old = getProvider(np.id);
            // The live check wins over the snapshot, as in probeProvider
            if (old && probeCache[np.id] && probeCache[np.id].done) {
                np.found = old.found;
                np.path = old.path;
            }
        });
        providers = resp.providers;
        if (selectId && getProvider(selectId)) {
            setProvider(selectId, true);
        } else if (getProvider(currentProviderId)) {
            setProvider(currentProviderId, false);
        } else {
            const first = providers.find(p => p.found) || providers[0];
            setProvider(first.id, true);
        }
        renderSettings();
    });
}

/** Save the active agent in assistant.json, with its model and effort
 *  ('' goes back to the agent's default). */
function saveSelection() {
    sendToJava({ type: 'set_selection', provider: currentProviderId,
                 model: currentModelId, effort: currentEffort }, () => {});
}

/** Make an agent active and restore its last used model and effort.
 *  Session IDs are not portable across agents: the Java side starts a
 *  fresh conversation on the next ask. The agent is started right away
 *  (probe): its models are listed, and the first request does not wait. */
function setProvider(id, save) {
    const p = getProvider(id);
    if (!p) return;
    if (id !== currentProviderId && currentConversationId) replayContext = true;
    currentProviderId = id;

    // '' = the agent's default; a saved model is kept even when not listed
    currentModelId = savedModels[id] || '';
    currentEffort = savedEfforts[id] || '';
    if (save !== false) saveSelection();
    renderProviderChip();
    updateCliBanner();
    if (p.found) probeProvider(id);
}

function setModel(id) {
    currentModelId = id || '';
    savedModels[currentProviderId] = id;
    saveSelection();
    renderProviderChip();
}

/** The value of a combo from its typed text: an item's label (any case)
 *  or id gives that item, any other text is taken as an id. */
function comboValue(name, text) {
    const t = text.trim();
    const items = comboItems[name] || [];
    const it = items.find(i => i.label.toLowerCase() === t.toLowerCase())
        || items.find(i => i.value === t);
    return it ? it.value : t;
}

/** Effort level of the current agent, '' for its default. */
function setEffort(value) {
    currentEffort = value || '';
    savedEfforts[currentProviderId] = currentEffort;
    saveSelection();
    renderProviderChip();
}

/** Compact "Provider · model · effort" chip in the composer (opens the
 *  settings page); a dot marks a customized command line. */
function renderProviderChip() {
    if (!providerChipLabel) return;
    const p = selectedProvider();
    if (!p) {
        providerChipLabel.textContent = providersLoaded ? 'AI provider…' : 'Checking AI providers…';
        return;
    }
    const probe = probeCache[p.id] || {};
    const shown = currentModelId || probe.currentModel || '';
    const label = shown ? modelLabel(p, shown) : '';
    providerChipLabel.textContent = p.name + (label ? ' · ' + label : '')
        + (currentEffort ? ' · ' + currentEffort : '');
    providerChip.classList.toggle('provider-chip-missing', !p.found);
    providerChip.classList.toggle('provider-chip-custom', !!p.customized);
    providerChip.title = p.customized
        ? 'Agent settings: provider, model, effort (customized command line)'
        : 'Agent settings: provider, model, effort';
}

// ============================================
// Settings page: provider, model, effort, command line, general options
// ============================================
const REFRESH_ICON = '<svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="23 4 23 10 17 10"/><path d="M20.49 15a9 9 0 1 1-2.12-9.36L23 10"/></svg>';
const CARET_ICON = '<svg width="10" height="10" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><polyline points="6 9 12 15 18 9"/></svg>';
const CHEVRON_ICON = '<svg width="10" height="10" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><polyline points="9 6 15 12 9 18"/></svg>';

/** Argument lists of agents.json, in display order. */
const ARG_GROUPS = [
    { key: 'args', label: 'Arguments' }
];

let providerMenuOpen = false;    // provider list of the settings page
let openCombo = null;            // 'model' | 'effort' while its list is open
let commandLineOpen = false;     // "Command line" section unfolded
let providerConfig = null;       // get_provider_config answer (current provider)
let commandLineError = '';       // last error of the "Command line" section
let commandLineNotice = '';      // last confirmation ("Fix applied to ...")
let fixOpen = false;             // "Paste a fix" box shown
let addAgentOpen = false;        // "Add an agent" form shown
let addAgentError = '';          // last error of that form
let removeArmed = false;         // "Remove this agent" clicked once (confirm)

function toggleConfigView() {
    if (!configView) return;
    if (configView.hidden) openConfigView(); else closeConfigView();
}

function openConfigView() {
    providerMenuOpen = false;
    openCombo = null;
    renderSettings();
    configView.hidden = false;
    sendToJava({ type: 'get_settings' }, (resp) => {
        if (resp && resp.settings) {
            assistantSettings = resp.settings;
            logsDir = resp.logs_dir || '';
            renderSettings();
        }
    });
    // Check the active provider right away; the others when chosen
    probeProvider(currentProviderId);
    if (commandLineOpen) loadProviderConfig();
}

function closeConfigView() {
    configView.hidden = true;
    promptInput.focus();
}

/** Ask Java for a live check of one agent: program lookup, then the agent
 *  is started to read its version, models and effort levels. */
function probeProvider(id, force) {
    const p = getProvider(id);
    if (!p) return;
    const cached = probeCache[id];
    if (cached && (cached.probing || (cached.done && !force))) return;
    probeCache[id] = { probing: true };
    renderSettings();

    sendToJava({ type: 'probe_provider', provider: id }, (resp) => {
        let data = resp;
        if (resp && resp.json_b64) {
            try {
                const bytes = Uint8Array.from(atob(resp.json_b64), c => c.charCodeAt(0));
                data = JSON.parse(new TextDecoder('utf-8').decode(bytes));
            } catch (e) {
                data = { error: e.message };
            }
        }
        if (!data || (data.error && data.found === undefined)) {
            probeCache[id] = { done: true, found: p.found,
                               error: (data && data.error) || 'check failed' };
        } else {
            probeCache[id] = { done: true, found: data.found, path: data.path,
                               version: data.version, auth: data.auth,
                               models: data.models || null, efforts: data.efforts || null,
                               authMethods: data.auth_methods || null,
                               currentModel: data.current_model || '',
                               currentEffort: data.current_effort || '',
                               error: data.error };
            // The live result wins over the startup snapshot (the CLI may
            // have been installed or removed since)
            p.found = data.found;
            p.path = data.path || '';
            p.custom_path = data.custom_path || '';
        }
        if (id === currentProviderId) {
            updateCliBanner();
            renderProviderChip();
        }
        renderSettings();
        // The executable may have changed: so does the command line
        if (id === currentProviderId && commandLineOpen) loadProviderConfig();
    });
}

/** Store the provider's executable path (empty: auto-detect), then re-check it. */
function saveProviderPath(id, path) {
    sendToJava({ type: 'set_provider_path', provider: id, path: path.trim() }, (resp) => {
        if (resp && resp.error) {
            probeCache[id] = Object.assign({}, probeCache[id], { error: resp.error });
            renderSettings();
            return;
        }
        const p = getProvider(id);
        if (p) p.custom_path = path.trim();
        probeProvider(id, true);
    });
}

// -- Command line (agents.json) ------------------------------------------

/** Fetch the command line configuration of the current agent, with its
 *  effective command. */
function loadProviderConfig() {
    const id = currentProviderId;
    sendToJava({ type: 'get_provider_config', provider: id }, (resp) => {
        if (id !== currentProviderId) return;
        applyProviderConfig(resp);
    });
}

function applyProviderConfig(resp) {
    if (resp && resp.defaults) {
        providerConfig = resp;
        commandLineError = resp.error || '';
    } else {
        commandLineError = (resp && resp.error) || 'The configuration could not be read';
    }
    renderSettings();
}

/** Save the user's overrides of the current provider ({} resets it). */
function saveOverrides(overrides) {
    commandLineNotice = '';
    sendToJava({ type: 'set_provider_overrides', provider: currentProviderId,
                 overrides: overrides }, (resp) => {
        applyProviderConfig(resp);
        if (resp && !resp.error) refreshProviders();
    });
}

/** "Paste a fix": a snippet given on a forum, applied by Java. */
function applyFix(text) {
    if (!text.trim()) return;
    commandLineNotice = '';
    sendToJava({ type: 'apply_provider_fix', provider: currentProviderId, fix: text }, (resp) => {
        if (resp && !resp.error) {
            fixOpen = false;
            const names = (resp.changed || []).map(id => (getProvider(id) || { name: id }).name);
            commandLineNotice = 'Fix applied to ' + names.join(', ') + '.';
            refreshProviders();
        }
        applyProviderConfig(resp);
    });
}

/** Arguments of a group as stored: an array, or {"args": [...]}. */
function groupArgs(value) {
    if (Array.isArray(value)) return value;
    if (value && Array.isArray(value.args)) return value.args;
    return [];
}

/** One argument as typed in a shell: quoted when it holds spaces or quotes. */
function quoteArg(a) {
    if (a !== '' && !/[\s"']/.test(a)) return a;
    return '"' + a.replace(/\\/g, '\\\\').replace(/"/g, '\\"') + '"';
}

/** Split a command line into arguments ("..." and '...' quoting). */
function parseArgs(text) {
    const args = [];
    let cur = '';
    let inArg = false;
    let quote = null;
    for (let i = 0; i < text.length; i++) {
        const c = text[i];
        if (quote) {
            if (c === quote) {
                quote = null;
            } else if (c === '\\' && quote === '"' && (text[i + 1] === '"' || text[i + 1] === '\\')) {
                cur += text[++i];
            } else {
                cur += c;
            }
        } else if (c === '"' || c === "'") {
            quote = c;
            inArg = true;
        } else if (/\s/.test(c)) {
            if (inArg) {
                args.push(cur);
                cur = '';
                inArg = false;
            }
        } else {
            cur += c;
            inArg = true;
        }
    }
    if (quote) throw new Error('a quote is not closed');
    if (inArg) args.push(cur);
    return args;
}

/** NAME=value lines (empty lines and # comments ignored). */
function parseEnv(text) {
    const env = {};
    for (const raw of text.split(/\r?\n/)) {
        const line = raw.trim();
        if (!line || line.startsWith('#')) continue;
        const eq = line.indexOf('=');
        if (eq <= 0) throw new Error('"' + line + '" is not NAME=value');
        env[line.substring(0, eq).trim()] = line.substring(eq + 1).trim();
    }
    return env;
}

function sameJson(a, b) {
    return JSON.stringify(a) === JSON.stringify(b);
}

/** Commit an edited field of the "Command line" section: a value equal to
 *  the default removes the override. */
function commitConfigField(key, text) {
    if (!providerConfig) return;
    const defaults = providerConfig.defaults || {};
    const overrides = JSON.parse(JSON.stringify(providerConfig.overrides || {}));
    let value;
    let def;
    try {
        if (key === 'env') {
            value = parseEnv(text);
            def = defaults.env || {};
        } else {
            value = parseArgs(text);
            def = groupArgs(defaults[key]);
        }
    } catch (e) {
        commandLineError = 'Not saved: ' + e.message;
        renderSettings();
        return;
    }
    if (sameJson(value, def)) {
        delete overrides[key];
    } else {
        overrides[key] = value;
    }
    if (sameJson(overrides, providerConfig.overrides || {})) return;
    saveOverrides(overrides);
}

function resetConfigField(key) {
    if (!providerConfig) return;
    const overrides = JSON.parse(JSON.stringify(providerConfig.overrides || {}));
    delete overrides[key];
    saveOverrides(overrides);
}

function commandText() {
    if (!providerConfig || !providerConfig.command) return '';
    let text = providerConfig.command.map(quoteArg).join(' ');
    const env = providerConfig.env || {};
    for (const k of Object.keys(env)) text += '\n' + k + '=' + env[k];
    return text;
}

// -- Rendering ------------------------------------------------------------

/** Rebuild the settings page, keeping the focus and the text being typed. */
function renderSettings() {
    if (!configList) return;
    const active = document.activeElement;
    const focusId = active && active.id && configList.contains(active) ? active.id : null;
    const typed = focusId && 'value' in active ? active.value : null;
    const sel = focusId && typeof active.selectionStart === 'number'
        ? [active.selectionStart, active.selectionEnd] : null;

    configList.innerHTML = settingsHtml();

    if (focusId) {
        const el = document.getElementById(focusId);
        if (el) {
            if (typed !== null && el.value !== typed) el.value = typed;
            el.focus();
            if (sel) {
                try { el.setSelectionRange(sel[0], sel[1]); } catch (e) { /* not a text field */ }
            }
        }
    }
}

function settingsHtml() {
    const p = selectedProvider();
    let html = '<section class="settings-section">'
             + '<div class="settings-section-title">AI provider</div>'
             + providerSelectHtml(p);
    if (addAgentOpen) {
        html += addAgentHtml();
    } else if (p) {
        const probe = probeCache[p.id] || {};
        const found = probe.done ? probe.found : p.found;
        html += providerMetaHtml(p, probe);
        if (!found && !probe.probing) {
            html += '<div class="provider-install">' + escapeHtml(p.name) + ' is not installed, or not found. '
                  + (p.custom ? 'Check the path of its program below.'
                              : 'See the ' + HELP_LINK + ' to install it, or give the path of its program below.')
                  + '</div>';
        }
        if (found) {
            // Until a value is chosen, the agent keeps its own: shown as the
            // placeholder. A list the agent gave replaces the bare "Default".
            const choicesOf = (list) => list.length > 0
                ? list.map(c => ({ value: c.id, label: c.label }))
                : [{ value: '', label: 'Default', hint: 'agent setting' }];
            const placeholderOf = (list, current) => {
                const c = list.find(x => x.id === current);
                return c ? c.label : (current || 'Default');
            };
            const models = providerModels(p);
            const efforts = providerEfforts(p);
            html += '<div class="settings-pair">'
                  + comboHtml('model', 'Model', currentModelId, choicesOf(models),
                              probe.probing ? 'Starting the agent…' : placeholderOf(models, probe.currentModel))
                  + (efforts.length > 0 || currentEffort
                      ? comboHtml('effort', 'Effort', currentEffort, choicesOf(efforts),
                                  placeholderOf(efforts, probe.currentEffort))
                      : '')
                  + '</div>';
        }
        html += executableHtml(p, found);
        html += commandLineHtml(p);
        if (p.custom) {
            html += '<div class="cmdline-actions">'
                  + '<button id="agent-remove" class="provider-path-btn">'
                  + (removeArmed ? 'Click again to remove ' + escapeHtml(p.name) : 'Remove this agent')
                  + '</button></div>';
        }
    }
    html += '</section>' + generalHtml();
    return html;
}

/** Form adding an agent of the user's choice (any ACP program). */
function addAgentHtml() {
    let html = '<div class="add-agent">'
             + '<div class="cmdline-hint">Any program that speaks the Agent Client Protocol '
             + '(list on agentclientprotocol.com). It must be installed and logged in.</div>';
    if (addAgentError) html += '<div class="cmdline-error">' + escapeHtml(addAgentError) + '</div>';
    const field = (id, label, placeholder, multiline) =>
        '<div class="settings-field"><label class="settings-label" for="' + id + '">' + label + '</label>'
        + (multiline
            ? '<textarea id="' + id + '" class="cmdline-input" rows="2" spellcheck="false" placeholder="'
              + escapeHtml(placeholder) + '"></textarea>'
            : '<input id="' + id + '" class="cmdline-input" type="text" spellcheck="false" placeholder="'
              + escapeHtml(placeholder) + '">')
        + '</div>';
    html += field('agent-name', 'Name', 'e.g. OpenCode', false)
          + field('agent-program', 'Program', 'Name found on the PATH (opencode), or its full path', false)
          + field('agent-args', 'Arguments', 'Arguments starting its ACP mode, e.g. acp or --acp', false)
          + field('agent-env', 'Environment', 'NAME=value, one per line (optional)', true)
          + '<div class="cmdline-actions">'
          + '<button id="agent-add" class="provider-path-btn primary">Add</button>'
          + '<button id="agent-cancel" class="provider-path-btn">Cancel</button></div>';
    return html + '</div>';
}

/** Send the "Add an agent" form; the new agent is selected and checked. */
function submitAddAgent() {
    const val = (id) => (document.getElementById(id) || { value: '' }).value;
    let args, env;
    try {
        args = parseArgs(val('agent-args'));
        env = parseEnv(val('agent-env'));
    } catch (e) {
        addAgentError = e.message;
        renderSettings();
        return;
    }
    sendToJava({ type: 'add_agent', name: val('agent-name').trim(), program: val('agent-program').trim(),
                 args: args, env: env }, (resp) => {
        if (!resp || resp.error || !resp.id) {
            addAgentError = (resp && resp.error) || 'The agent could not be added';
            renderSettings();
            return;
        }
        addAgentOpen = false;
        addAgentError = '';
        refreshProviders(resp.id);
    });
}

function providerSelectHtml(p) {
    const dotOf = (q) => {
        const probe = probeCache[q.id] || {};
        if (probe.probing) return 'probing';
        return (probe.done ? probe.found : q.found) ? 'ok' : 'missing';
    };
    let html = '<div class="provider-select">'
             + '<button id="provider-select-btn" class="provider-select-btn" aria-haspopup="listbox"'
             + ' aria-expanded="' + providerMenuOpen + '">'
             + (p ? '<span class="status-dot ' + dotOf(p) + '"></span>'
                    + '<span class="provider-select-name">' + escapeHtml(p.name) + '</span>'
                  : '<span class="provider-select-name">AI provider…</span>')
             + '<span class="provider-select-caret">' + CARET_ICON + '</span></button>'
             + '<button class="icon-btn provider-refresh" title="Check again">' + REFRESH_ICON + '</button>';
    if (providerMenuOpen) {
        html += '<div class="provider-menu" role="listbox">' + providers.map(q => {
            const probe = probeCache[q.id] || {};
            const found = probe.done ? probe.found : q.found;
            const status = probe.probing ? 'checking…'
                : (!found ? 'not installed' : (probe.version || ''));
            return '<div class="provider-menu-item' + (q.id === currentProviderId ? ' selected' : '')
                 + (found ? '' : ' missing') + '" role="option" data-id="' + escapeHtml(q.id) + '">'
                 + '<span class="status-dot ' + dotOf(q) + '"></span>'
                 + '<span class="provider-menu-name">' + escapeHtml(q.name) + '</span>'
                 + (q.customized ? '<span class="custom-dot" title="Customized command line"></span>' : '')
                 + '<span class="provider-menu-status">' + escapeHtml(status) + '</span></div>';
        }).join('')
          + '<div class="provider-menu-item provider-menu-add" role="option">'
          + '<span class="provider-menu-name">Add an agent…</span></div>'
          + '</div>';
    }
    return html + '</div>';
}

function providerMetaHtml(p, probe) {
    const head = [];
    if (probe.probing) head.push('checking…');
    if (probe.version) head.push(escapeHtml(probe.version));
    if (probe.auth) head.push(escapeHtml(probe.auth));
    let html = '<div class="provider-meta">';
    if (head.length > 0) html += '<div>' + head.join(' · ') + '</div>';
    if (probe.path || p.path) html += '<div class="provider-meta-path">' + escapeHtml(probe.path || p.path) + '</div>';
    if (probe.error) html += '<div class="provider-meta-error">' + escapeHtml(probe.error) + '</div>';
    if (probe.authMethods && probe.authMethods.length > 0) {
        if (probe.signingIn) {
            html += '<div class="sign-in">Signing in… a browser page may open.</div>';
        } else {
            const terminal = probe.authMethods.filter(m => m.terminal);
            const direct = probe.authMethods.filter(m => !m.terminal);
            html += '<div class="sign-in">';
            if (terminal.length > 0) {
                html += '<div>' + terminal.map(m => 'Run <code>' + escapeHtml(m.terminal) + '</code> in a terminal')
                      .join(', or ') + ', then check again.</div>';
            }
            if (direct.length > 0) {
                html += '<div class="cmdline-actions">' + direct.map(m =>
                    '<button class="provider-path-btn sign-in-btn" data-method="' + escapeHtml(m.id)
                    + '" title="' + escapeHtml(m.description || '') + '">' + escapeHtml(m.name) + '</button>')
                    .join('') + '</div>';
            }
            html += '</div>';
        }
    }
    return html + '</div>';
}

/** Items of each combo as last rendered, to read typed labels back. */
const comboItems = {};

/** Text field with a list of suggestions: any value can be typed. The
 *  field shows the label of the chosen item, the id when none matches. */
function comboHtml(name, label, value, items, placeholder) {
    comboItems[name] = items;
    const chosen = items.find(it => it.value === (value || ''));
    const shown = value ? (chosen ? chosen.label : value) : '';
    let html = '<div class="settings-field">'
             + '<label class="settings-label" for="combo-' + name + '">' + label + '</label>'
             + '<div class="combo' + (openCombo === name ? ' open' : '') + '" data-combo="' + name + '">'
             + '<input id="combo-' + name + '" class="combo-input" type="text" spellcheck="false"'
             + ' autocomplete="off" value="' + escapeHtml(shown) + '" placeholder="'
             + escapeHtml(placeholder) + '">'
             + '<button class="combo-toggle" tabindex="-1" title="Choose">' + CARET_ICON + '</button>';
    if (openCombo === name) {
        html += '<div class="combo-list" role="listbox">' + items.map(it =>
            '<div class="combo-item' + (it.value === (value || '') ? ' selected' : '')
            + '" role="option" data-value="' + escapeHtml(it.value) + '">'
            + '<span>' + escapeHtml(it.label) + '</span>'
            + (it.hint ? '<span class="combo-hint">' + escapeHtml(it.hint) + '</span>'
               : (it.label !== it.value ? '<span class="combo-hint">' + escapeHtml(it.value) + '</span>' : ''))
            + '</div>').join('') + '</div>';
    }
    return html + '</div></div>';
}

function executableHtml(p, found) {
    return '<div class="settings-field">'
         + '<label class="settings-label" for="provider-path-input">Executable</label>'
         + '<div class="provider-path">'
         + '<input id="provider-path-input" class="provider-path-input" type="text" spellcheck="false" value="'
         + escapeHtml(p.custom_path || '') + '" placeholder="'
         + escapeHtml(found ? 'Auto-detected. Or the full path of the agent program'
                            : 'Full path of the agent program, if installed elsewhere') + '">'
         + '<button class="provider-path-btn" data-action="save" title="Use this executable">Save</button>'
         + (p.custom_path
             ? '<button class="provider-path-btn" data-action="reset" title="Forget the path, detect the program automatically">Auto-detect</button>'
             : '')
         + '</div></div>';
}

function commandLineHtml(p) {
    let html = '<div class="cmdline' + (commandLineOpen ? ' open' : '') + '">'
             + '<button id="cmdline-toggle" class="cmdline-head" aria-expanded="' + commandLineOpen + '">'
             + '<span class="cmdline-chevron">' + CHEVRON_ICON + '</span>Command line'
             + (p.customized ? '<span class="cmdline-badge">customized</span>' : '')
             + '</button>';
    if (!commandLineOpen) return html + '</div>';

    html += '<div class="cmdline-body">';
    if (!providerConfig || providerConfig.provider !== p.id) {
        html += '<div class="cmdline-hint">Loading…</div>';
        if (commandLineError) html += '<div class="cmdline-error">' + escapeHtml(commandLineError) + '</div>';
        return html + '</div></div>';
    }
    const defaults = providerConfig.defaults || {};
    const overrides = providerConfig.overrides || {};
    if (commandLineError) html += '<div class="cmdline-error">' + escapeHtml(commandLineError) + '</div>';
    if (commandLineNotice) html += '<div class="cmdline-notice">' + escapeHtml(commandLineNotice) + '</div>';
    html += '<div class="cmdline-hint">How the ' + escapeHtml(p.name) + ' program is started, from agents.json. '
          + 'Change it when an agent update breaks the assistant, for example as advised on the Scilab forum.</div>';

    for (const g of ARG_GROUPS) {
        const meta = defaults[g.key] && !Array.isArray(defaults[g.key]) ? defaults[g.key] : {};
        const overridden = g.key in overrides;
        const value = groupArgs(overridden ? overrides[g.key] : defaults[g.key]);
        let pills = '';
        if (meta.security) pills += '<span class="arg-pill security" title="What the AI is allowed to do (shell, web...)">security</span>';
        if (meta.required) pills += '<span class="arg-pill required" title="The assistant reads the CLI output in this format: another one breaks it">required by Scilab</span>';
        html += configFieldHtml(g.key, g.label, pills, value.map(quoteArg).join(' '), overridden,
                                'None', false);
        if (overridden && meta.security) {
            html += '<div class="cmdline-warning">Changed security arguments may let the AI run shell commands, '
                  + 'browse the web or change files without asking. Only apply changes from a source you trust.</div>';
        }
    }
    const env = 'env' in overrides ? overrides.env : (defaults.env || {});
    html += configFieldHtml('env', 'Environment', '', Object.keys(env).map(k => k + '=' + env[k]).join('\n'),
                            'env' in overrides, 'NAME=value, one per line', true);

    html += '<div class="settings-label">Effective command</div>'
          + '<pre class="cmdline-preview">' + escapeHtml(commandText()) + '</pre>'
          + '<div class="cmdline-hint">Then sent over the Agent Client Protocol: the Scilab MCP server, the system prompt and your requests.</div>';

    if (fixOpen) {
        html += '<div class="cmdline-fix">'
              + '<label class="settings-label" for="fix-input">Paste the fix (JSON)</label>'
              + '<textarea id="fix-input" class="cmdline-input" rows="5" spellcheck="false" placeholder="'
              + escapeHtml('{"' + p.id + '": {"env": {"NAME": "value"}}}') + '"></textarea>'
              + '<div class="cmdline-actions">'
              + '<button id="fix-apply" class="provider-path-btn primary">Apply</button>'
              + '<button id="fix-cancel" class="provider-path-btn">Cancel</button></div></div>';
    }
    html += '<div class="cmdline-actions">'
          + (fixOpen ? '' : '<button id="cmdline-fix" class="provider-path-btn">Paste a fix…</button>')
          + '<button id="cmdline-copy" class="provider-path-btn">Copy</button>'
          + '<button id="cmdline-reset-all" class="provider-path-btn"'
          + (Object.keys(overrides).length > 0 ? '' : ' disabled') + '>Reset all</button>'
          + '</div>';
    return html + '</div></div>';
}

function configFieldHtml(key, label, pills, text, overridden, placeholder, multiline) {
    const id = 'cfg-' + key;
    const field = multiline
        ? '<textarea id="' + id + '" class="cmdline-input' + (overridden ? ' custom' : '') + '" data-key="' + key
          + '" rows="' + Math.max(2, text.split('\n').length) + '" spellcheck="false" placeholder="'
          + escapeHtml(placeholder) + '">' + escapeHtml(text) + '</textarea>'
        : '<input id="' + id + '" class="cmdline-input' + (overridden ? ' custom' : '') + '" data-key="' + key
          + '" type="text" spellcheck="false" value="' + escapeHtml(text) + '" placeholder="'
          + escapeHtml(placeholder) + '">';
    return '<div class="settings-field">'
         + '<div class="settings-label-row"><label class="settings-label" for="' + id + '">' + label + '</label>'
         + pills
         + (overridden ? '<button class="cmdline-reset" data-key="' + key + '" title="Back to the default value">↺ default</button>' : '')
         + '</div>' + field + '</div>';
}

function generalHtml() {
    if (!assistantSettings) return '';
    return '<section class="settings-section">'
        + '<div class="settings-section-title">General</div>'
        + '<label class="config-option">'
        + '<input type="checkbox" id="open-with-scinotes-toggle"'
        + (assistantSettings.open_with_scinotes ? ' checked' : '') + '>'
        + '<span>Open the assistant with SciNotes</span></label>'
        + '<label class="config-option">'
        + '<input type="checkbox" id="session-logs-toggle"'
        + (assistantSettings.session_logs ? ' checked' : '') + '>'
        + '<span>Log conversations to disk for later analysis</span></label>'
        + '<div class="config-option-hint">JSONL transcripts of prompts, tool calls '
        + 'and results, kept on this machine only'
        + (logsDir ? ' in <code>' + escapeHtml(logsDir) + '</code>' : '')
        + ' (50 MB cap, oldest pruned).</div>'
        + '</section>';
}

/** Warn prominently when the selected agent is not installed. */
function updateCliBanner() {
    const old = messagesContainer.querySelector('.cli-banner');
    if (old) old.remove();

    const p = selectedProvider();
    if (!p || p.found) return;

    const banner = document.createElement('div');
    banner.className = 'cli-banner';
    banner.innerHTML =
        '<div class="cli-banner-title">' + escapeHtml(p.name) + ' not found</div>' +
        '<p>The assistant needs the ' + escapeHtml(p.name) + ' agent program to work. '
        + (p.custom ? 'Check the path of its program in the settings page.'
                    : 'See the ' + HELP_LINK + ' to install it, then check again from the settings page, or restart Scilab.')
        + '</p>' +
        '<p>You can also pick another AI provider in the selector below.</p>';
    messagesContainer.insertBefore(banner, messagesContainer.firstChild);
}

function initTheme() {
    const saved = localStorage.getItem('theme') || 'light';
    document.documentElement.setAttribute('data-theme', saved);
}

function setupEventListeners() {
    sendBtn.addEventListener('click', handleSend);
    promptInput.addEventListener('keydown', (e) => {
        if (mentionActive) return; // let mention handler deal with it
        handlePromptHistoryKeydown(e);
        if (e.defaultPrevented) return;
        if (isProcessing) return;
        // isComposing: Enter validating an IME candidate (Chinese, Japanese...) must not send
        if (e.key === 'Enter' && !e.shiftKey && !e.isComposing) {
            e.preventDefault();
            handleSend();
        }
    });
    promptInput.addEventListener('input', () => {
        historyIdx = -1; // typing exits history browsing
        autoResizeTextarea();
        handleMentionInput();
    });
    promptInput.addEventListener('keydown', handleMentionKeydown);
    clearBtn.addEventListener('click', clearConversation);
    const convMenuBtn = document.getElementById('conv-menu-btn');
    if (convMenuBtn) convMenuBtn.addEventListener('click', toggleConvDropdown);

    // Settings page
    if (providerChip) providerChip.addEventListener('click', toggleConfigView);
    const settingsBtn = document.getElementById('settings-btn');
    if (settingsBtn) settingsBtn.addEventListener('click', toggleConfigView);
    const configBack = document.getElementById('config-back');
    if (configBack) configBack.addEventListener('click', closeConfigView);
    const configHelp = document.getElementById('config-help');
    if (configHelp) configHelp.addEventListener('click', () => sendToJava({ type: 'open_help' }, () => {}));
    document.addEventListener('click', (e) => {
        if (!e.target.closest('.open-help')) return;
        e.preventDefault();
        sendToJava({ type: 'open_help' }, () => {});
    });
    document.addEventListener('keydown', (e) => {
        if (e.key !== 'Escape' || !configView || configView.hidden) return;
        // First close an open list, then the page
        if (providerMenuOpen || openCombo) {
            providerMenuOpen = false;
            openCombo = null;
            renderSettings();
        } else {
            closeConfigView();
        }
    });
    // A click out of the settings page closes its open lists (inside, the
    // click handler does it: rebuilding the page on mousedown would remove
    // the button being clicked)
    document.addEventListener('mousedown', (e) => {
        if ((!providerMenuOpen && !openCombo) || (configList && configList.contains(e.target))) return;
        providerMenuOpen = false;
        openCombo = null;
        renderSettings();
    });
    if (configList) configList.addEventListener('click', (e) => {
        const t = e.target;
        if ((providerMenuOpen || openCombo) && !t.closest('.provider-select') && !t.closest('.combo')) {
            providerMenuOpen = false;
            openCombo = null;
            renderSettings();
        }
        if (t.closest('#provider-select-btn')) {
            providerMenuOpen = !providerMenuOpen;
            openCombo = null;
            renderSettings();
            return;
        }
        const signInBtn = t.closest('.sign-in-btn');
        if (signInBtn) {
            const id = currentProviderId;
            probeCache[id] = Object.assign({}, probeCache[id], { signingIn: true, error: '' });
            renderSettings();
            sendToJava({ type: 'agent_sign_in', provider: id, method: signInBtn.dataset.method }, (resp) => {
                if (resp && resp.error) {
                    probeCache[id] = Object.assign({}, probeCache[id], { signingIn: false, error: resp.error });
                    renderSettings();
                    return;
                }
                probeProvider(id, true);
            });
            return;
        }
        if (t.closest('.provider-menu-add')) {
            providerMenuOpen = false;
            addAgentOpen = true;
            addAgentError = '';
            renderSettings();
            const name = document.getElementById('agent-name');
            if (name) name.focus();
            return;
        }
        if (t.closest('#agent-cancel')) {
            addAgentOpen = false;
            addAgentError = '';
            renderSettings();
            return;
        }
        if (t.closest('#agent-add')) {
            submitAddAgent();
            return;
        }
        if (t.closest('#agent-remove')) {
            if (!removeArmed) {
                removeArmed = true;
                renderSettings();
                return;
            }
            removeArmed = false;
            const id = currentProviderId;
            sendToJava({ type: 'remove_agent', provider: id }, (resp) => {
                if (resp && resp.error) {
                    commandLineError = resp.error;
                    renderSettings();
                    return;
                }
                delete probeCache[id];
                refreshProviders();
            });
            return;
        }
        if (removeArmed && !t.closest('#agent-remove')) {
            removeArmed = false;
            renderSettings();
        }
        const item = t.closest('.provider-menu-item');
        if (item) {
            providerMenuOpen = false;
            addAgentOpen = false;
            if (item.dataset.id !== currentProviderId) {
                setProvider(item.dataset.id, true);
                providerConfig = null;
                commandLineError = '';
                commandLineNotice = '';
                fixOpen = false;
                if (commandLineOpen) loadProviderConfig();
            }
            renderSettings();
            probeProvider(currentProviderId);
            return;
        }
        if (t.closest('.provider-refresh')) {
            probeProvider(currentProviderId, true);
            return;
        }
        const toggle = t.closest('.combo-toggle');
        if (toggle) {
            const name = toggle.closest('.combo').dataset.combo;
            openCombo = openCombo === name ? null : name;
            providerMenuOpen = false;
            renderSettings();
            return;
        }
        const option = t.closest('.combo-item');
        if (option) {
            const name = option.closest('.combo').dataset.combo;
            openCombo = null;
            if (name === 'model') setModel(option.dataset.value); else setEffort(option.dataset.value);
            renderSettings();
            if (commandLineOpen) loadProviderConfig();
            return;
        }
        const pathBtn = t.closest('.provider-path-btn[data-action]');
        if (pathBtn) {
            const input = document.getElementById('provider-path-input');
            saveProviderPath(currentProviderId,
                             pathBtn.dataset.action === 'reset' ? '' : (input ? input.value : ''));
            return;
        }
        if (t.closest('#cmdline-toggle')) {
            commandLineOpen = !commandLineOpen;
            renderSettings();
            if (commandLineOpen) loadProviderConfig();
            return;
        }
        const reset = t.closest('.cmdline-reset');
        if (reset) {
            resetConfigField(reset.dataset.key);
            return;
        }
        if (t.closest('#cmdline-reset-all')) {
            saveOverrides({});
            return;
        }
        if (t.closest('#cmdline-copy')) {
            navigator.clipboard.writeText(commandText()).catch(() => {});
            return;
        }
        if (t.closest('#cmdline-fix')) {
            fixOpen = true;
            commandLineNotice = '';
            renderSettings();
            const fix = document.getElementById('fix-input');
            if (fix) fix.focus();
            return;
        }
        if (t.closest('#fix-apply')) {
            const fix = document.getElementById('fix-input');
            applyFix(fix ? fix.value : '');
            return;
        }
        if (t.closest('#fix-cancel')) {
            fixOpen = false;
            renderSettings();
        }
    });
    if (configList) configList.addEventListener('keydown', (e) => {
        if (e.key !== 'Enter' || e.isComposing) return;
        const t = e.target;
        if (t.tagName === 'INPUT' && t.closest('.add-agent')) {
            e.preventDefault();
            submitAddAgent();
            return;
        }
        if (t.id === 'provider-path-input') {
            e.preventDefault();
            saveProviderPath(currentProviderId, t.value);
        } else if (t.classList.contains('combo-input')
                   || (t.classList.contains('cmdline-input') && t.tagName === 'INPUT')) {
            // Committed by the change event
            e.preventDefault();
            t.blur();
        }
    });
    if (configList) configList.addEventListener('change', (e) => {
        const t = e.target;
        if (t.id === 'session-logs-toggle' && assistantSettings) {
            assistantSettings.session_logs = t.checked;
            // Sent as a string: the Java-side minimal JSON parser only
            // reads quoted values
            sendToJava({ type: 'set_setting', key: 'session_logs',
                         value: t.checked ? 'true' : 'false' }, () => {});
        } else if (t.id === 'open-with-scinotes-toggle' && assistantSettings) {
            assistantSettings.open_with_scinotes = t.checked;
            sendToJava({ type: 'set_setting', key: 'open_with_scinotes',
                         value: t.checked ? 'true' : 'false' }, () => {});
        } else if (t.id === 'combo-model') {
            setModel(comboValue('model', t.value));
            renderSettings();
            if (commandLineOpen) loadProviderConfig();
        } else if (t.id === 'combo-effort') {
            setEffort(comboValue('effort', t.value));
            renderSettings();
            if (commandLineOpen) loadProviderConfig();
        } else if (t.classList.contains('cmdline-input') && t.dataset.key) {
            commitConfigField(t.dataset.key, t.value);
        }
    });

    // Unified attach button: dispatches images vs text files
    attachBtn.addEventListener('click', () => fileInput.click());
    fileInput.addEventListener('change', () => {
        addMixedFiles(fileInput.files);
        fileInput.value = '';
    });

    // Paste image from clipboard
    promptInput.addEventListener('paste', (e) => {
        const files = [];
        for (const item of e.clipboardData.items) {
            if (item.type.startsWith('image/')) {
                files.push(item.getAsFile());
            }
        }
        if (files.length > 0) addMixedFiles(files);
    });

    // Drag & drop files/images
    const composerShell = document.querySelector('.composer-shell');
    composerShell.addEventListener('dragover', (e) => {
        e.preventDefault();
        composerShell.classList.add('drag-over');
    });
    composerShell.addEventListener('dragleave', () => {
        composerShell.classList.remove('drag-over');
    });
    composerShell.addEventListener('drop', (e) => {
        e.preventDefault();
        composerShell.classList.remove('drag-over');
        addMixedFiles(e.dataTransfer.files);
    });

    renderSendButton();
}

// ============================================
// Prompt history (ArrowUp/ArrowDown recall)
// ============================================
let promptHistory = [];
let historyIdx = -1;     // -1 = not browsing
let historyDraft = '';   // text present before browsing started

function handlePromptHistoryKeydown(e) {
    if (e.key !== 'ArrowUp' && e.key !== 'ArrowDown') return;
    if (promptHistory.length === 0) return;
    // Only take over when empty or already browsing — never hijack multi-line editing
    if (historyIdx === -1 && promptInput.value !== '') return;

    if (e.key === 'ArrowUp') {
        if (historyIdx === -1) {
            historyDraft = promptInput.value;
            historyIdx = promptHistory.length - 1;
        } else if (historyIdx > 0) {
            historyIdx--;
        }
        promptInput.value = promptHistory[historyIdx];
    } else {
        if (historyIdx === -1) return;
        if (historyIdx < promptHistory.length - 1) {
            historyIdx++;
            promptInput.value = promptHistory[historyIdx];
        } else {
            historyIdx = -1;
            promptInput.value = historyDraft;
        }
    }
    e.preventDefault();
    promptInput.selectionStart = promptInput.selectionEnd = promptInput.value.length;
    autoResizeTextarea();
}

function autoResizeTextarea() {
    promptInput.style.height = 'auto';
    promptInput.style.height = Math.min(promptInput.scrollHeight, 200) + 'px';
}

// Recalculate textarea height when the panel is resized
new ResizeObserver(() => autoResizeTextarea()).observe(document.body);

function toggleTheme() {
    const current = document.documentElement.getAttribute('data-theme');
    const next = current === 'dark' ? 'light' : 'dark';
    document.documentElement.setAttribute('data-theme', next);
    localStorage.setItem('theme', next);
}

// ============================================
// Chat
// ============================================
let currentActivityEl = null;

function handleSend() {
    if (isProcessing) {
        requestStop();
        return;
    }

    const prompt = promptInput.value.trim();
    if (!prompt) return;
    // Not ready until the provider list arrived: keep the prompt, it can be
    // sent in a moment
    if (!providersLoaded) return;

    if (promptHistory[promptHistory.length - 1] !== prompt) {
        promptHistory.push(prompt);
    }
    historyIdx = -1;

    // Read before the new message is shown: the gist of what came before
    const history = replayContext ? conversationTranscript() : '';
    if (!currentConversationId) currentConversationId = newConversationId();

    const imagesToSend = pendingImages.slice();
    pendingImages = [];
    renderImagePreview();

    // Don't send attachments whose read/staging failed or is still running
    const filesToSend = pendingFiles.filter(f => !f.error && !f.loading);
    pendingFiles = [];
    renderFilePreview();
    hideMentionDropdown();

    addMessage('user', prompt, imagesToSend.length > 0 ? imagesToSend : null,
               filesToSend.length > 0 ? filesToSend : null);
    scrollToBottom(true);
    promptInput.value = '';
    autoResizeTextarea();

    currentActivityEl = addActivityBlock();
    isProcessing = true;
    stopRequested = false;
    renderSendButton();

    const model = currentModelId;

    // Prepend file context to prompt:
    // - inline @mentions still present in the text are resolved to absolute
    //   paths; the LLM reads them on demand or uses the path in Scilab code
    // - picker/drag&drop files have no usable path: content is embedded
    const refs = mentionedFiles.filter(m => prompt.includes('@' + m.relPath));
    const stagedFiles = filesToSend.filter(f => f.stagedPath);
    const inlineFiles = filesToSend.filter(f => !f.stagedPath);
    let fullPrompt = prompt;
    if (refs.length > 0 || filesToSend.length > 0) {
        let fileContext = '';
        if (refs.length > 0) {
            fileContext += '<referenced_files>\n'
                + refs.map(m => '@' + m.relPath + ' = ' + m.absPath).join('\n')
                + '\n</referenced_files>\n';
        }
        if (stagedFiles.length > 0) {
            fileContext += '<attached_files>\n'
                + stagedFiles.map(f => f.name + ' = ' + f.stagedPath).join('\n')
                + '\n</attached_files>\n';
        }
        if (inlineFiles.length > 0) {
            fileContext += inlineFiles.map(f =>
                '<attached_file path="' + f.path + '">\n' + f.content + '\n</attached_file>'
            ).join('\n') + '\n';
        }
        fullPrompt = fileContext + '\n' + prompt;
    }

    const msg = { type: 'ask', prompt: fullPrompt, model: model,
                  effort: currentEffort, provider: currentProviderId };
    if (imagesToSend.length > 0) {
        msg.images = imagesToSend;
    }
    if (history) {
        msg.history = history;
    }

    sendToJava(msg, (response) => {
        if (response && response.streaming && response.request_id) {
            activeTaskId = response.request_id;
            // Skills auto-loaded into the system prompt: show them as steps
            if (response.skills && response.skills.length > 0) {
                response.skills.forEach(s =>
                    updateActivity({ type: 'tool_use', message: 'Skill loaded: ' + s }));
            }
            pollResult(response.request_id);
        } else if (response && response.error) {
            finishChat(null, response.error);
        }
    });
}

function pollResult(taskId) {
    if (stopRequested || taskId !== activeTaskId) return;

    sendToJava({ type: 'poll', request_id: taskId }, (response) => {
        if (stopRequested || taskId !== activeTaskId) return;

        try {
            // Handle base64-encoded JSON
            let data = response;
            if (response && response.json_b64) {
                const binaryStr = atob(response.json_b64);
                const bytes = Uint8Array.from(binaryStr, c => c.charCodeAt(0));
                const jsonStr = new TextDecoder('utf-8').decode(bytes);
                data = JSON.parse(jsonStr);
            }

            // Process activity events
            if (data && data.events && data.events.length > 0) {
                data.events.forEach(evt => updateActivity(evt));
            }

            // Live output-token counter (shows the work is progressing)
            if (data && typeof data.live_tokens === 'number') {
                updateWorkingTokens(data.live_tokens);
            }

            if (data && data.done) {
                // Capture session ID for conversation history
                if (data.session_id) {
                    currentSessionId = data.session_id;
                }

                // The agent now holds the conversation
                if (!data.cancelled && !data.error) replayContext = false;

                if (data.cancelled) {
                    finishChat(null, 'Request cancelled.');
                } else if (data.final_result) {
                    finishChat(data.final_result, null,
                               { in: data.tokens_in || 0, out: data.tokens_out || 0 });
                } else if (data.error) {
                    finishChat(null, data.error);
                } else {
                    finishChat('Task completed.', null);
                }

                // Save AFTER finishChat so the final assistant message
                // is part of the persisted conversation
                if (currentSessionId) {
                    saveCurrentConversation();
                }
            } else {
                // Still running, poll again
                pollTimerId = setTimeout(() => pollResult(taskId), 300);
            }
        } catch (e) {
            finishChat(null, 'Error: ' + e.message);
        }
    });
}

function finishChat(text, error, tokens) {
    stopWorkingIndicator();
    expireQuestionCards();
    if (currentActivityEl) {
        // Mark last step as done, keep the block visible
        const lastStep = currentActivityEl.querySelector('.activity-step.current');
        if (lastStep) {
            lastStep.classList.remove('current');
            lastStep.classList.add('done');
        }
        currentActivityEl.classList.add('activity-done');
        currentActivityEl = null;
    }

    if (error) {
        // Remove incomplete streaming message on error
        if (streamingMessageEl) {
            streamingMessageEl.remove();
            streamingMessageEl = null;
        }
        addMessage('assistant', 'Error: ' + error);
    } else if (text) {
        let msgEl;
        if (streamingMessageEl) {
            // Finalize the streaming message with the authoritative result
            const contentEl = streamingMessageEl.querySelector('.message-content');
            contentEl.classList.remove('streaming');
            contentEl.innerHTML = renderMarkdown(text);
            addCopyButtons(contentEl);
            msgEl = streamingMessageEl;
            streamingMessageEl = null;
        } else {
            msgEl = addMessage('assistant', text);
        }
        appendMessageMeta(msgEl, tokens);
    }

    streamingText = '';
    streamingMessageEl = null;
    resetState();
}

function requestStop() {
    if (!isProcessing) return;
    stopRequested = true;

    if (pollTimerId) {
        clearTimeout(pollTimerId);
        pollTimerId = null;
    }

    if (activeTaskId) {
        sendToJava({ type: 'cancel', request_id: activeTaskId }, () => {});
    }

    finishChat(null, 'Request cancelled.');
}

function resetState() {
    isProcessing = false;
    stopRequested = false;
    activeTaskId = null;
    if (pollTimerId) {
        clearTimeout(pollTimerId);
        pollTimerId = null;
    }
    renderSendButton();
}

function renderSendButton() {
    if (isProcessing) {
        sendBtn.classList.add('stop-mode');
        sendBtn.title = 'Stop';
        sendBtn.innerHTML = STOP_ICON;
    } else {
        sendBtn.classList.remove('stop-mode');
        sendBtn.title = 'Send (Enter)';
        sendBtn.innerHTML = SEND_ICON;
    }
}

// ============================================
// Messages
// ============================================
function addMessage(role, content, images, files) {
    const messageEl = document.createElement('div');
    messageEl.className = 'message ' + role;

    const contentEl = document.createElement('div');
    contentEl.className = 'message-content';

    if (role === 'assistant') {
        contentEl.innerHTML = renderMarkdown(content || '');
        setTimeout(() => addCopyButtons(contentEl), 0);
    } else {
        let html = '';
        if (files && files.length > 0) {
            html += '<div class="message-files">' +
                files.map(f =>
                    '<span class="message-file-chip">' +
                    '<svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">' +
                    '<path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/>' +
                    '<polyline points="14 2 14 8 20 8"/></svg>' +
                    escapeHtml(f.name) + '</span>'
                ).join('') +
                '</div>';
        }
        if (images && images.length > 0) {
            html += '<div class="message-images">' +
                images.map(src => '<img src="' + src + '" alt="image">').join('') +
                '</div>';
        }
        // Render known @mentions as styled links inside the message
        let bodyHtml = escapeHtml(content);
        mentionedFiles.forEach(m => {
            const token = escapeHtml('@' + m.relPath);
            bodyHtml = bodyHtml.split(token).join(
                '<span class="file-mention" title="' + escapeHtml(m.absPath) + '">'
                + token + '</span>');
        });
        html += '<p>' + bodyHtml.replace(/\n/g, '<br>') + '</p>';
        contentEl.innerHTML = html;
    }

    messageEl.appendChild(contentEl);
    messagesContainer.appendChild(messageEl);
    if (role === 'user') observeUserMessage(messageEl);
    scrollToBottom();
    return messageEl;
}

function addActivityBlock() {
    const el = document.createElement('div');
    el.className = 'message assistant activity-block';
    el.innerHTML = `<div class="message-content">
        <div class="activity-container">
            <div class="activity-spinner"></div>
            <div class="activity-steps">
            </div>
            <div class="activity-working">
                <span class="activity-working-label">Working…</span>
                <span class="activity-working-time"></span>
                <span class="activity-working-tokens"></span>
            </div>
        </div>
    </div>`;
    messagesContainer.appendChild(el);
    startWorkingIndicator(el);
    scrollToBottom();
    return el;
}

// ============================================
// Working indicator (elapsed time while processing)
// ============================================
let workingTimerId = null;
let workingStartTime = 0;

function startWorkingIndicator(activityEl) {
    // Only clear the timer here — stopWorkingIndicator() would also remove
    // the .activity-working element just created inside activityEl.
    if (workingTimerId) {
        clearInterval(workingTimerId);
        workingTimerId = null;
    }
    workingStartTime = Date.now();
    const timeEl = activityEl.querySelector('.activity-working-time');
    if (!timeEl) return;
    timeEl.textContent = '0s';
    workingTimerId = setInterval(() => {
        const s = Math.floor((Date.now() - workingStartTime) / 1000);
        timeEl.textContent = formatElapsed(s);
    }, 1000);
}

function stopWorkingIndicator() {
    if (workingTimerId) {
        clearInterval(workingTimerId);
        workingTimerId = null;
    }
    document.querySelectorAll('.activity-working').forEach(el => el.remove());
}

function formatElapsed(seconds) {
    if (seconds < 60) return seconds + 's';
    const m = Math.floor(seconds / 60);
    const s = seconds % 60;
    return m + 'm ' + (s < 10 ? '0' : '') + s + 's';
}

/**
 * Discreet duration + token-usage line under a finished assistant message.
 * Tokens rather than dollars: with CLI subscriptions a price is not
 * representative, but consumption-conscious users can track tokens.
 */
function appendMessageMeta(msgEl, tokens) {
    if (!msgEl) return;
    const parts = [];
    if (workingStartTime > 0) {
        parts.push(formatElapsed(Math.max(1, Math.round((Date.now() - workingStartTime) / 1000))));
    }
    // Only output tokens: the real measure of produced work ("in" is mostly
    // context replay and reads as alarming for trivial requests)
    if (tokens && tokens.out > 0) {
        parts.push(formatTokens(tokens.out) + ' tokens');
    }
    if (parts.length === 0) return;
    const meta = document.createElement('div');
    meta.className = 'message-meta';
    meta.textContent = parts.join(' · ');
    msgEl.appendChild(meta);
}

function formatTokens(n) {
    if (n >= 1000000) return (n / 1000000).toFixed(1) + 'M';
    if (n >= 1000) return (n / 1000).toFixed(1) + 'k';
    return String(n);
}

/** Update the live output-token counter in the Working… line. */
function updateWorkingTokens(n) {
    if (n <= 0) return;
    const el = document.querySelector('.activity-working-tokens');
    if (el) el.textContent = '· ' + formatTokens(n) + ' tokens';
}

function updateActivity(evt) {
    // Text delta — append to streaming message
    if (evt.type === 'text') {
        // The model is writing its reply: whatever tool step was shown as
        // "current" is finished — stop displaying it as in-progress.
        if (currentActivityEl) {
            const cur = currentActivityEl.querySelector('.activity-step.current');
            if (cur) {
                cur.classList.remove('current');
                cur.classList.add('done');
            }
        }
        streamingText += evt.message;
        renderStreamingMessage();
        return;
    }

    if (!currentActivityEl) return;
    const stepsEl = currentActivityEl.querySelector('.activity-steps');
    if (!stepsEl) return;

    const prevStep = stepsEl.querySelector('.activity-step.current');

    if (evt.type === 'permission' || evt.type === 'auth') {
        if (prevStep) {
            prevStep.classList.remove('current');
            prevStep.classList.add('done');
        }
        stepsEl.appendChild(questionCard(evt));
        scrollToBottom();
        return;
    }
    if (evt.type === 'status') {
        if (prevStep) {
            prevStep.classList.remove('current');
            prevStep.classList.add('done');
        }
        const step = document.createElement('div');
        step.className = 'activity-step activity-step-status current';
        step.innerHTML = '<span class="activity-bullet"></span>'
            + '<span class="activity-step-label">' + escapeHtml(evt.message) + '</span>';
        stepsEl.appendChild(step);
        scrollToBottom();
        return;
    }

    const prevIsThinkingPlaceholder = prevStep
        && prevStep.classList.contains('activity-step-thinking')
        && prevStep.dataset.placeholder === '1';

    if (evt.type === 'thinking') {
        // Empty message = "thinking phase started" (streamed before the block
        // completes, so the previous tool step doesn't look stuck)
        if (!evt.message && prevIsThinkingPlaceholder) return; // already shown
        if (evt.message && prevIsThinkingPlaceholder) {
            // The completed block fills the placeholder announced earlier
            delete prevStep.dataset.placeholder;
            const content = prevStep.nextElementSibling;
            if (content && content.classList.contains('activity-thinking-content')) {
                content.textContent = evt.message;
            }
            return;
        }
    }

    // Mark previous step as done
    if (prevStep) {
        prevStep.classList.remove('current');
        prevStep.classList.add('done');
    }

    if (evt.type === 'thinking') {
        // Collapsible thinking step
        const step = document.createElement('div');
        step.className = 'activity-step activity-step-thinking current';
        if (!evt.message) step.dataset.placeholder = '1';
        step.innerHTML =
            '<span class="activity-bullet"></span>' +
            '<span class="activity-step-label">Thinking\u2026</span>' +
            '<span class="activity-chevron">&rsaquo;</span>';

        const content = document.createElement('div');
        content.className = 'activity-thinking-content';
        content.textContent = evt.message || '';

        step.addEventListener('click', () => {
            step.classList.toggle('expanded');
            content.classList.toggle('visible');
        });

        stepsEl.appendChild(step);
        stepsEl.appendChild(content);
    } else {
        // Tool use / other steps — split "Tool name: context"
        const step = document.createElement('div');
        step.className = 'activity-step current';
        const parts = evt.message.split(': ');
        const toolName = parts[0];
        const context = parts.length > 1 ? parts.slice(1).join(': ') : '';

        // Determine if context should be shown as a code preview
        const isCode = toolName === 'Executing Scilab code' ||
                       toolName === 'Running command' ||
                       toolName === 'Editing file' ||
                       toolName === 'Editing in editor' ||
                       toolName === 'Writing file';

        if (isCode && context) {
            step.classList.add('activity-step-with-preview');
            step.innerHTML =
                '<span class="activity-bullet"></span>' +
                '<span class="activity-step-label">' +
                    '<span class="activity-step-tool-name">' + escapeHtml(toolName) + '</span>' +
                '</span>' +
                '<span class="activity-chevron">&rsaquo;</span>';

            const preview = document.createElement('div');
            preview.className = 'activity-code-preview';
            const code = document.createElement('code');
            code.textContent = context;
            preview.appendChild(code);

            step.addEventListener('click', () => {
                step.classList.toggle('expanded');
                preview.classList.toggle('visible');
            });

            stepsEl.appendChild(step);
            stepsEl.appendChild(preview);
        } else {
            step.innerHTML =
                '<span class="activity-bullet"></span>' +
                '<span class="activity-step-label">' +
                    '<span class="activity-step-tool-name">' + escapeHtml(toolName) + '</span>' +
                    (context ? ': ' + escapeHtml(context) : '') +
                '</span>';
            stepsEl.appendChild(step);
        }
    }

    scrollToBottom();
}

/**
 * A question of the agent in the activity feed: a permission (run a
 * command, write a file...) or how to sign in. The request waits for the
 * answer; Stop or the end of the request withdraws the question.
 */
function questionCard(evt) {
    const card = document.createElement('div');
    card.className = 'ask-card';
    const taskId = activeTaskId;
    let html;
    if (evt.type === 'permission') {
        html = '<div class="ask-title">' + escapeHtml(evt.agent || 'The agent') + ' asks permission: '
             + '<strong>' + escapeHtml(evt.title || '') + '</strong></div>';
        if (evt.detail && evt.detail !== evt.title) {
            html += '<pre class="ask-detail">' + escapeHtml(evt.detail) + '</pre>';
        }
        html += '<div class="ask-actions">' + (evt.options || []).map(o =>
            '<button class="provider-path-btn' + (o.kind === 'allow_once' ? ' primary' : '')
            + '" data-value="' + escapeHtml(o.id) + '">' + escapeHtml(o.name) + '</button>').join('')
            + '</div>';
    } else {
        html = '<div class="ask-title"><strong>' + escapeHtml(evt.agent || 'The agent')
             + '</strong> needs you to sign in</div>';
        const methods = evt.methods || [];
        const terminal = methods.filter(m => m.terminal);
        const direct = methods.filter(m => !m.terminal);
        if (terminal.length > 0) {
            html += '<div class="ask-hint">' + terminal.map(m =>
                'Run <code>' + escapeHtml(m.terminal) + '</code> in a terminal').join(', or ') + ', then:</div>';
        }
        html += '<div class="ask-actions">' + direct.map(m =>
            '<button class="provider-path-btn" data-value="' + escapeHtml(m.id) + '" title="'
            + escapeHtml(m.description || '') + '">' + escapeHtml(m.name) + '</button>').join('')
            + '<button class="provider-path-btn" data-value="__signed_in">I\'ve signed in</button></div>';
    }
    card.innerHTML = html;
    card.addEventListener('click', (e) => {
        const btn = e.target.closest('button[data-value]');
        if (!btn || card.classList.contains('answered')) return;
        card.classList.add('answered');
        sendToJava({ type: 'answer', request_id: taskId, question_id: evt.id,
                     value: btn.dataset.value }, () => {});
        const actions = card.querySelector('.ask-actions');
        if (actions) actions.innerHTML = '<span class="ask-answer">' + escapeHtml(btn.textContent) + '</span>';
    });
    return card;
}

/** Questions left unanswered when a request ends are withdrawn. */
function expireQuestionCards() {
    document.querySelectorAll('.ask-card:not(.answered)').forEach(card => {
        card.classList.add('answered', 'expired');
        const actions = card.querySelector('.ask-actions');
        if (actions) actions.innerHTML = '<span class="ask-answer">No answer</span>';
    });
}

function renderStreamingMessage() {
    if (!streamingMessageEl) {
        streamingMessageEl = document.createElement('div');
        streamingMessageEl.className = 'message assistant';
        const contentEl = document.createElement('div');
        contentEl.className = 'message-content streaming';
        streamingMessageEl.appendChild(contentEl);
        messagesContainer.appendChild(streamingMessageEl);
    }

    const contentEl = streamingMessageEl.querySelector('.message-content');
    contentEl.innerHTML = renderMarkdown(streamingText);
    scrollToBottom();
}

function scrollToBottom(force) {
    // Only auto-scroll if user is near the bottom (or forced).
    // chatContainer is the scrolling element, not messagesContainer.
    const threshold = 80;
    const distanceFromBottom = chatContainer.scrollHeight
        - chatContainer.scrollTop - chatContainer.clientHeight;
    if (force || distanceFromBottom < threshold) {
        if (force) {
            chatContainer.scrollTo({ top: chatContainer.scrollHeight, behavior: 'smooth' });
        } else {
            // Instant during streaming: smooth scrolling lags behind rapid
            // updates and would break the near-bottom check above.
            chatContainer.scrollTop = chatContainer.scrollHeight;
        }
    }
}

function clearConversation() {
    // Save current conversation before clearing
    saveCurrentConversation();

    const welcome = messagesContainer.querySelector('.message');
    messagesContainer.innerHTML = '';
    if (welcome) {
        messagesContainer.appendChild(welcome.cloneNode(true));
    }
    stickyPrompt.hidden = true;
    lastVisibleUserMsg = null;
    currentSessionId = '';
    currentConversationId = '';
    replayContext = false;
    mentionedFiles = [];
    sendToJava({ type: 'clear' }, () => {});
    updateConversationList();
}

/** A new id for a conversation of the panel. */
function newConversationId() {
    return 'c' + Date.now().toString(36) + Math.random().toString(36).slice(2, 8);
}

/**
 * The conversation shown in the panel as plain text, most recent part
 * first kept: given to an agent that lost it (its session could not be
 * resumed, or another agent takes over).
 */
function conversationTranscript() {
    const parts = [];
    let started = false;  // the welcome message comes before the first question
    messagesContainer.querySelectorAll('.message').forEach(el => {
        if (el.classList.contains('user')) started = true;
        if (!started || el.classList.contains('activity-block')) return;
        const c = el.querySelector('.message-content');
        let t = c ? (c.innerText || c.textContent || '').trim() : '';
        if (!t) return;
        if (t.length > 1500) t = t.slice(0, 1500) + ' [...]';
        parts.push((el.classList.contains('user') ? 'User: ' : 'Assistant: ') + t);
    });
    const kept = [];
    let total = 0;
    for (let i = parts.length - 1; i >= 0 && total + parts[i].length <= 12000; i--) {
        kept.unshift(parts[i]);
        total += parts[i].length;
    }
    return kept.join('\n\n');
}

function saveCurrentConversation() {
    if (!currentConversationId || !currentSessionId) return;

    // Extract title from first user message
    const firstUserMsg = messagesContainer.querySelector('.message.user .message-content');
    let title = firstUserMsg ? firstUserMsg.textContent.trim() : 'Untitled';
    if (title.length > 50) title = title.substring(0, 50) + '...';

    // Collect messages as JSON array
    const msgs = [];
    messagesContainer.querySelectorAll('.message').forEach(el => {
        const role = el.classList.contains('user') ? 'user' : 'assistant';
        const content = el.querySelector('.message-content');
        if (content) {
            msgs.push({ role: role, text: content.innerHTML });
        }
    });

    sendToJava({
        type: 'save_conversation',
        conversation_id: currentConversationId,
        session_id: currentSessionId,
        title: title,
        provider: currentProviderId,
        messages: JSON.stringify(msgs)
    }, () => {});
}

function loadConversationList() {
    sendToJava({ type: 'list_conversations' }, (response) => {
        if (response && response.conversations) {
            conversations = response.conversations;
        } else {
            conversations = [];
        }
    });
}

function resumeConversation(conversationId) {
    // Save current conversation first
    saveCurrentConversation();

    // Conversations saved before had the agent session as their id
    const conv = conversations.find(c => c.id === conversationId);
    currentConversationId = conversationId;
    currentSessionId = (conv && conv.session) || conversationId;

    // Switch back to the agent this conversation was held with: session
    // IDs only mean something to their own agent
    if (conv && conv.provider && getProvider(conv.provider)
        && currentProviderId !== conv.provider) {
        setProvider(conv.provider, true);
    }
    // The agent may have dropped the session meanwhile: the first request
    // carries the gist, used only if the session cannot be resumed
    replayContext = true;

    sendToJava({ type: 'resume', session_id: currentSessionId,
                 provider: conv && conv.provider ? conv.provider : '' }, () => {});

    // Load messages from SCIHOME
    sendToJava({ type: 'load_conversation', conversation_id: conversationId }, (response) => {
        let data = response;
        if (response && response.json_b64) {
            const binaryStr = atob(response.json_b64);
            const bytes = Uint8Array.from(binaryStr, c => c.charCodeAt(0));
            data = JSON.parse(new TextDecoder('utf-8').decode(bytes));
        }

        if (data && data.messages && data.messages.length > 0) {
            messagesContainer.innerHTML = '';
            data.messages.forEach(msg => {
                const el = document.createElement('div');
                el.className = 'message ' + msg.role;
                const content = document.createElement('div');
                content.className = 'message-content';
                content.innerHTML = msg.text;
                el.appendChild(content);
                messagesContainer.appendChild(el);
                if (msg.role === 'assistant') addCopyButtons(content);
            });
            scrollToBottom(true);
        }
    });
}

function updateConversationList() {
    // Dropdown is rebuilt each time it's opened
}

function toggleConvDropdown() {
    const dropdown = document.getElementById('conv-dropdown');
    if (!dropdown) return;

    if (!dropdown.hidden) {
        dropdown.hidden = true;
        return;
    }

    // Always fetch a fresh list: conversations saved during this session
    // would otherwise never appear (the startup list is stale).
    sendToJava({ type: 'list_conversations' }, (response) => {
        conversations = (response && response.conversations) ? response.conversations : [];
        renderConvDropdown(dropdown);
    });
}

function renderConvDropdown(dropdown) {
    if (conversations.length === 0) {
        dropdown.innerHTML = '<div class="conv-dropdown-empty">No conversations yet</div>';
    } else {
        dropdown.innerHTML = conversations.map(c =>
            '<div class="conv-item' + (c.id === currentConversationId ? ' active' : '') + '" data-id="' + c.id + '">' +
                '<span class="conv-title">' + escapeHtml(c.title) + '</span>' +
                (c.provider && getProvider(c.provider)
                    ? '<span class="conv-provider">' + escapeHtml(getProvider(c.provider).name) + '</span>'
                    : '') +
                '<span class="conv-actions">' +
                    '<button class="conv-action-btn" data-action="rename" data-id="' + c.id + '" title="Rename">' +
                        '<svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">' +
                        '<path d="M17 3a2.83 2.83 0 1 1 4 4L7.5 20.5 2 22l1.5-5.5L17 3z"/></svg>' +
                    '</button>' +
                    '<button class="conv-action-btn" data-action="delete" data-id="' + c.id + '" title="Delete">' +
                        '<svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">' +
                        '<path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6M8 6V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg>' +
                    '</button>' +
                '</span>' +
            '</div>'
        ).join('');
    }

    dropdown.hidden = false;

    // Bind clicks
    dropdown.querySelectorAll('.conv-item').forEach(el => {
        el.addEventListener('click', (e) => {
            // Don't trigger if clicking action buttons
            if (e.target.closest('.conv-action-btn')) return;
            dropdown.hidden = true;
            resumeConversation(el.dataset.id);
        });
    });

    dropdown.querySelectorAll('.conv-action-btn').forEach(btn => {
        btn.addEventListener('click', (e) => {
            e.stopPropagation();
            const id = btn.dataset.id;
            if (btn.dataset.action === 'rename') {
                // Inline edit — window.prompt() is not implemented in JCEF
                renameConversation(id, btn.closest('.conv-item'));
            } else if (btn.dataset.action === 'delete') {
                // Two-step: a quick misclick must not delete a conversation
                if (!btn.classList.contains('confirm')) {
                    armDeleteButton(btn);
                    return;
                }
                deleteConversation(id);
                dropdown.hidden = true;
            }
        });
    });

    // Close on click outside
    setTimeout(() => {
        const closeHandler = (e) => {
            if (!dropdown.contains(e.target) && e.target.id !== 'conv-menu-btn') {
                dropdown.hidden = true;
                document.removeEventListener('click', closeHandler);
            }
        };
        document.addEventListener('click', closeHandler);
    }, 0);
}

function renameConversation(id, itemEl) {
    const conv = conversations.find(c => c.id === id);
    if (!conv || !itemEl) return;
    const titleEl = itemEl.querySelector('.conv-title');
    if (!titleEl) return;

    const input = document.createElement('input');
    input.type = 'text';
    input.className = 'conv-rename-input';
    input.value = conv.title;
    titleEl.replaceWith(input);
    input.focus();
    input.select();

    const commit = () => {
        const t = input.value.trim();
        if (t && t !== conv.title) {
            conv.title = t;
            // Empty messages = title-only update (Java keeps stored messages)
            sendToJava({
                type: 'save_conversation',
                conversation_id: id,
                title: t,
                messages: ''
            }, () => {});
        }
        const span = document.createElement('span');
        span.className = 'conv-title';
        span.textContent = conv.title;
        input.replaceWith(span);
    };

    input.addEventListener('keydown', (e) => {
        e.stopPropagation();
        if (e.key === 'Enter') {
            e.preventDefault();
            input.blur(); // triggers commit
        } else if (e.key === 'Escape') {
            input.value = conv.title;
            input.blur();
        }
    });
    input.addEventListener('click', (e) => e.stopPropagation());
    input.addEventListener('blur', commit);
}

/**
 * First click on a delete button: turn it into a "Delete?" confirmation
 * that a second click validates. Disarmed after 3 s or when the pointer
 * leaves the conversation row.
 */
function armDeleteButton(btn) {
    const icon = btn.innerHTML;
    const title = btn.title;
    const item = btn.closest('.conv-item');
    let timer = null;
    const disarm = () => {
        clearTimeout(timer);
        if (item) item.removeEventListener('mouseleave', disarm);
        btn.classList.remove('confirm');
        btn.innerHTML = icon;
        btn.title = title;
    };
    btn.classList.add('confirm');
    btn.textContent = 'Delete?';
    btn.title = 'Click again to delete this conversation';
    timer = setTimeout(disarm, 3000);
    if (item) item.addEventListener('mouseleave', disarm);
}

function deleteConversation(id) {
    conversations = conversations.filter(c => c.id !== id);
    sendToJava({ type: 'delete_conversation', conversation_id: id }, () => {});
    if (currentConversationId === id) {
        clearConversation();
    }
}

// ============================================
// File & Image Attachments
// ============================================

/** Document/binary extensions staged to disk rather than inlined: the
 *  LLM reads them from their saved path (Read handles PDF; Scilab loads
 *  xlsx/parquet/sod...). */
const STAGED_EXTENSIONS = ['pdf', 'zip', '7z', 'gz', 'tar', 'sod', 'h5', 'mat',
    'doc', 'docx', 'xls', 'xlsx', 'ppt', 'pptx', 'odt', 'ods', 'parquet',
    'bin', 'dat'];
const IMAGE_EXTENSIONS = ['png', 'jpg', 'jpeg', 'gif', 'webp', 'bmp'];
/** Inline attachments ride in the CEF query with the prompt: keep them
 *  small or the request gets truncated and the ask is rejected. */
const MAX_INLINE_FILE_BYTES = 512 * 1024;
/** Staged files travel once as base64 in their own message. */
const MAX_STAGED_FILE_BYTES = 15 * 1024 * 1024;

/** Dispatch files: images → base64 for vision, small text → inline
 *  content, documents/binaries → saved to disk Java-side and referenced
 *  by absolute path in the prompt (<attached_files>). */
function addMixedFiles(fileList) {
    for (const file of fileList) {
        const ext = (file.name.split('.').pop() || '').toLowerCase();
        if (file.type.startsWith('image/') || IMAGE_EXTENSIONS.includes(ext)) {
            const reader = new FileReader();
            reader.onload = (e) => {
                pendingImages.push(e.target.result);
                renderImagePreview();
            };
            reader.readAsDataURL(file);
            continue;
        }
        if (pendingFiles.some(f => f.name === file.name)) continue;
        const entry = { path: file.name, name: file.name, content: '', loading: true };
        pendingFiles.push(entry);
        renderFilePreview();

        if (file.size > MAX_STAGED_FILE_BYTES) {
            entry.error = 'too large (max 15 MB) — reference it with @path instead';
            entry.loading = false;
            renderFilePreview();
            continue;
        }
        if (STAGED_EXTENSIONS.includes(ext) || file.size > MAX_INLINE_FILE_BYTES) {
            stageFile(entry, file);
            continue;
        }
        const reader = new FileReader();
        reader.onload = (e) => {
            const text = e.target.result;
            // Binary content without a known extension: NUL bytes or many
            // undecodable characters — stage it instead of inlining garbage
            if (text.indexOf('\u0000') >= 0
                || (text.match(/�/g) || []).length > 20) {
                stageFile(entry, file);
            } else {
                entry.content = text;
                entry.loading = false;
                renderFilePreview();
            }
        };
        reader.onerror = () => {
            entry.error = 'could not read file';
            entry.loading = false;
            renderFilePreview();
        };
        reader.readAsText(file);
    }
}

/** Ship the file's bytes to Java, which saves them locally and returns
 *  the absolute path referenced at send time. */
function stageFile(entry, file) {
    const reader = new FileReader();
    reader.onload = (e) => {
        const dataUrl = e.target.result;
        const b64 = dataUrl.substring(dataUrl.indexOf(',') + 1);
        sendToJava({ type: 'stage_file', name: file.name, data_b64: b64 }, (resp) => {
            if (resp && resp.path) {
                entry.stagedPath = resp.path;
            } else {
                entry.error = (resp && resp.error) ? resp.error : 'could not save file';
            }
            entry.loading = false;
            renderFilePreview();
        });
    };
    reader.onerror = () => {
        entry.error = 'could not read file';
        entry.loading = false;
        renderFilePreview();
    };
    reader.readAsDataURL(file);
}

function renderImagePreview() {
    if (pendingImages.length === 0) {
        imagePreview.hidden = true;
        imagePreview.innerHTML = '';
        return;
    }
    imagePreview.hidden = false;
    imagePreview.innerHTML = pendingImages.map((dataUrl, idx) =>
        `<div class="image-thumb">
            <img src="${dataUrl}" alt="image">
            <button class="image-remove" onclick="removeImage(${idx})">&times;</button>
        </div>`
    ).join('');
}

function removeImage(idx) {
    pendingImages.splice(idx, 1);
    renderImagePreview();
}

// ============================================
// Sticky Prompt
// ============================================
let stickyObserver = null;
let lastVisibleUserMsg = null;

function setupStickyPrompt() {
    // IntersectionObserver: track which user messages are above the viewport
    stickyObserver = new IntersectionObserver(() => {
        updateStickyPrompt();
    }, {
        root: chatContainer,
        threshold: [0, 1]
    });
}

function observeUserMessage(el) {
    if (stickyObserver) stickyObserver.observe(el);
}

function updateStickyPrompt() {
    const userMessages = messagesContainer.querySelectorAll('.message.user');
    const containerTop = chatContainer.getBoundingClientRect().top;

    // Find the last user message that has scrolled above the visible area
    let candidate = null;
    for (const msg of userMessages) {
        const rect = msg.getBoundingClientRect();
        // Message is above the container top (scrolled past)
        if (rect.bottom < containerTop + 10) {
            candidate = msg;
        }
    }

    if (candidate && candidate !== lastVisibleUserMsg) {
        lastVisibleUserMsg = candidate;
        const text = candidate.querySelector('.message-content').textContent.trim();
        const display = text.length > 120 ? text.substring(0, 120) + '...' : text;
        stickyPrompt.innerHTML = '<div class="sticky-card">' + escapeHtml(display) + '</div>';
        stickyPrompt.hidden = false;
    } else if (!candidate) {
        lastVisibleUserMsg = null;
        stickyPrompt.hidden = true;
    }
}

// Also update on scroll directly (observer fires on visibility change,
// but scroll gives smoother updates)
document.addEventListener('DOMContentLoaded', () => {
    chatContainer.addEventListener('scroll', () => {
        requestAnimationFrame(updateStickyPrompt);
    });
});

// ============================================
// Markdown Rendering
// ============================================
let markedConfigured = false;

/**
 * Code blocks are highlighted in marked's code renderer: marked 5+ dropped
 * the "highlight" option, which was silently ignored (no colors).
 */
function configureMarked() {
    if (markedConfigured) return;
    markedConfigured = true;
    marked.use({
        breaks: true,
        gfm: true,
        renderer: {
            code(token) {
                const lang = (token.lang || '').trim().split(/\s+/)[0].toLowerCase();
                const cls = lang ? ' class="language-' + escapeHtml(lang) + '"' : '';
                return '<pre><code' + cls + '>' + highlightCode(token.text, lang) + '</code></pre>\n';
            }
        }
    });
}

/** Highlighted HTML of a code block; unlabelled blocks are Scilab code. */
function highlightCode(code, lang) {
    if (typeof hljs !== 'undefined') {
        const language = lang && hljs.getLanguage(lang) ? lang
            : (hljs.getLanguage('scilab') ? 'scilab' : null);
        if (language) {
            try {
                return hljs.highlight(code, { language: language, ignoreIllegals: true }).value;
            } catch (e) {
                // fall back to plain text
            }
        }
    }
    return escapeHtml(code);
}

function renderMarkdown(text) {
    if (typeof marked === 'undefined') {
        return '<p>' + escapeHtml(text).replace(/\n/g, '<br>') + '</p>';
    }

    configureMarked();
    const raw = marked.parse(text);
    if (typeof DOMPurify !== 'undefined') {
        return DOMPurify.sanitize(raw, { ADD_TAGS: ['code'], ADD_ATTR: ['class'] });
    }
    return raw;
}

/** Escape text for HTML content and attribute values (quotes included:
 *  innerHTML leaves them as they are). */
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML.replace(/"/g, '&quot;').replace(/'/g, '&#39;');
}

function addCopyButtons(container) {
    container.querySelectorAll('pre').forEach(pre => {
        if (pre.querySelector('.code-actions')) return; // already equipped
        const actions = document.createElement('div');
        actions.className = 'code-actions';
        actions.innerHTML =
            '<button class="code-btn" data-action="run" title="Run in Scilab">Run</button>' +
            '<button class="code-btn" data-action="insert" title="Insert at cursor / replace selection">Insert</button>' +
            '<button class="code-btn" data-action="copy" title="Copy to clipboard">Copy</button>';
        pre.style.position = 'relative';
        pre.appendChild(actions);
    });
}

// Links of the answers must never navigate the panel itself: web links
// open in the system browser (window.open -> Java onBeforePopup), and
// anything else (empty href, relative or file: path) is ignored, since
// Chromium refuses to load file: URLs from this page ("Unsafe attempt to
// load URL ... 'file:' URLs are treated as unique security origins").
messagesContainer.addEventListener('click', (e) => {
    const a = e.target.closest('a[href]');
    if (!a) return;
    const href = a.getAttribute('href') || '';
    if (href.startsWith('#') && href.length > 1) return; // in-page anchor
    e.preventDefault();
    if (/^(https?:|mailto:)/i.test(href)) {
        window.open(a.href, '_blank');
    }
});

// Delegated handler: also works for conversations restored from history,
// where per-button listeners would have been lost in serialization.
messagesContainer.addEventListener('click', (e) => {
    const btn = e.target.closest('.code-btn');
    if (!btn) return;
    const pre = btn.closest('pre');
    const codeEl = pre ? pre.querySelector('code') : null;
    if (!codeEl) return;
    const code = codeEl.textContent;

    if (btn.dataset.action === 'copy') {
        navigator.clipboard.writeText(code);
        flashButton(btn, 'Copied!');
    } else if (btn.dataset.action === 'insert') {
        sendToJava({ type: 'insert_code', code: code }, (resp) => {
            if (resp && resp.error) {
                flashButton(btn, 'No editor');
            } else {
                flashButton(btn, 'Inserted!');
            }
        });
    } else if (btn.dataset.action === 'run') {
        btn.disabled = true;
        btn.textContent = 'Running…';
        sendToJava({ type: 'run_code', code: code }, (resp) => {
            btn.disabled = false;
            btn.textContent = 'Run';
            let data = resp;
            if (resp && resp.json_b64) {
                try {
                    const bytes = Uint8Array.from(atob(resp.json_b64), c => c.charCodeAt(0));
                    data = JSON.parse(new TextDecoder('utf-8').decode(bytes));
                } catch (err) {
                    data = { error: err.message };
                }
            }
            showRunOutput(pre, data);
        });
    }
});

function flashButton(btn, text) {
    const original = btn.textContent;
    btn.textContent = text;
    setTimeout(() => { btn.textContent = original; }, 2000);
}

/** Show (or update) the execution output block right below a code block. */
function showRunOutput(pre, data) {
    let out = pre.nextElementSibling;
    if (!out || !out.classList.contains('code-run-output')) {
        out = document.createElement('div');
        out.className = 'code-run-output';
        pre.parentNode.insertBefore(out, pre.nextSibling);
    }
    const text = (data && data.error) ? ('Error: ' + data.error)
               : (data && data.output && data.output.trim()) ? data.output
               : '(no output)';
    out.innerHTML = '<div class="code-run-output-header">Scilab output'
        + '<button class="code-run-output-close" title="Close">&times;</button></div>'
        + '<pre></pre>';
    out.querySelector('pre').textContent = text;
    out.querySelector('.code-run-output-close').addEventListener('click', () => out.remove());
}

// ============================================
// @ File Mentions
// ============================================

/** Get the @mention context at the cursor position */
function getMentionContext() {
    const text = promptInput.value;
    const cursor = promptInput.selectionStart;
    // Walk backwards from cursor to find @; stop at whitespace so a completed
    // mention followed by normal typing does not keep re-opening the dropdown
    for (let i = cursor - 1; i >= 0; i--) {
        const ch = text[i];
        if (ch === '@') {
            // Must be at start or after whitespace
            if (i === 0 || /\s/.test(text[i - 1])) {
                return { atPos: i, query: text.substring(i + 1, cursor).toLowerCase() };
            }
            return null;
        }
        if (/\s/.test(ch)) return null; // don't cross whitespace
    }
    return null;
}

/** Handle input events for @ detection */
function handleMentionInput() {
    const ctx = getMentionContext();
    if (!ctx) {
        hideMentionDropdown();
        return;
    }

    // Always refresh file list (pwd or files may have changed)
    sendToJava({ type: 'list_files' }, (response) => {
        if (response && response.files) {
            fileListCache = { files: response.files, base: response.base || '' };
            // Re-check context (user may have kept typing)
            const ctx2 = getMentionContext();
            if (ctx2) showMentionDropdown(ctx2.query);
        }
    });
}

/** Handle keyboard navigation in the dropdown */
function handleMentionKeydown(e) {
    if (!mentionActive) return;

    if (e.key === 'ArrowDown') {
        e.preventDefault();
        const items = mentionDropdown.querySelectorAll('.mention-item');
        mentionSelectedIdx = Math.min(mentionSelectedIdx + 1, items.length - 1);
        updateMentionSelection();
    } else if (e.key === 'ArrowUp') {
        e.preventDefault();
        mentionSelectedIdx = Math.max(mentionSelectedIdx - 1, 0);
        updateMentionSelection();
    } else if (e.key === 'Enter' && mentionActive) {
        e.preventDefault();
        const items = mentionDropdown.querySelectorAll('.mention-item');
        if (items[mentionSelectedIdx]) {
            selectMentionFile(items[mentionSelectedIdx].dataset.path);
        }
    } else if (e.key === 'Escape') {
        e.preventDefault();
        hideMentionDropdown();
    } else if (e.key === 'Tab' && mentionActive) {
        e.preventDefault();
        const items = mentionDropdown.querySelectorAll('.mention-item');
        if (items[mentionSelectedIdx]) {
            selectMentionFile(items[mentionSelectedIdx].dataset.path);
        }
    }
}

/** Show/update the mention dropdown with filtered results */
function showMentionDropdown(query) {
    if (!fileListCache || !fileListCache.files) return;

    // Filter files by query
    let filtered = fileListCache.files;
    if (query) {
        filtered = fileListCache.files.filter(f => f.toLowerCase().includes(query));
    }

    // Limit display
    const maxItems = 8;
    const shown = filtered.slice(0, maxItems);

    if (shown.length === 0) {
        hideMentionDropdown();
        return;
    }

    mentionSelectedIdx = 0;
    mentionActive = true;
    mentionDropdown.hidden = false;
    mentionDropdown.innerHTML = shown.map((filePath, idx) => {
        const name = filePath.split('/').pop();
        const dir = filePath.includes('/') ? filePath.substring(0, filePath.lastIndexOf('/')) : '';
        return '<div class="mention-item' + (idx === 0 ? ' selected' : '') + '" data-path="' + escapeHtml(filePath) + '">' +
            '<svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">' +
            '<path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/>' +
            '<polyline points="14 2 14 8 20 8"/></svg>' +
            '<span class="mention-name">' + escapeHtml(name) + '</span>' +
            (dir ? '<span class="mention-dir">' + escapeHtml(dir) + '</span>' : '') +
            '</div>';
    }).join('');

    // Click handler
    mentionDropdown.querySelectorAll('.mention-item').forEach(item => {
        item.addEventListener('click', () => selectMentionFile(item.dataset.path));
    });
}

function updateMentionSelection() {
    const items = mentionDropdown.querySelectorAll('.mention-item');
    items.forEach((item, idx) => {
        item.classList.toggle('selected', idx === mentionSelectedIdx);
    });
    // Scroll selected into view
    if (items[mentionSelectedIdx]) {
        items[mentionSelectedIdx].scrollIntoView({ block: 'nearest' });
    }
}

function hideMentionDropdown() {
    mentionActive = false;
    mentionDropdown.hidden = true;
    mentionDropdown.innerHTML = '';
}

/** Files mentioned with @ this session: {relPath, absPath} */
let mentionedFiles = [];

/**
 * Select a file from the dropdown: the mention STAYS INLINE in the prompt
 * ("charge @data/file.dat et trace ...") — it reads as part of the sentence,
 * like in Claude Code. At send time, mentions still present in the text are
 * resolved to absolute paths in a <referenced_files> block; the LLM reads
 * them on demand or uses the path directly in Scilab code. No size limit.
 */
function selectMentionFile(filePath) {
    hideMentionDropdown();

    // Replace the @query with the complete @mention
    const ctx = getMentionContext();
    if (ctx) {
        const text = promptInput.value;
        const insert = '@' + filePath + ' ';
        promptInput.value = text.substring(0, ctx.atPos) + insert
            + text.substring(promptInput.selectionStart);
        promptInput.selectionStart = promptInput.selectionEnd = ctx.atPos + insert.length;
        autoResizeTextarea();
    }

    const base = (fileListCache && fileListCache.base)
        ? fileListCache.base.replace(/\\/g, '/') : '';
    const absPath = base ? base + '/' + filePath : filePath;
    if (!mentionedFiles.some(m => m.relPath === filePath)) {
        mentionedFiles.push({ relPath: filePath, absPath: absPath });
    }
    promptInput.focus();
}

function renderFilePreview() {
    if (pendingFiles.length === 0) {
        filePreview.hidden = true;
        filePreview.innerHTML = '';
        return;
    }
    filePreview.hidden = false;
    filePreview.innerHTML = pendingFiles.map((f, idx) => {
        const state = f.error ? ' file-chip-error' : (f.loading ? ' file-chip-loading' : '');
        const tooltip = f.error ? ('Error: ' + f.error) : (f.stagedPath || f.path);
        return '<div class="file-chip' + state + '" title="' + escapeHtml(tooltip) + '">' +
            '<svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">' +
            '<path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/>' +
            '<polyline points="14 2 14 8 20 8"/></svg>' +
            '<span class="file-chip-name">' + escapeHtml(f.name) + (f.error ? ' (error)' : '') + '</span>' +
            '<button class="file-chip-remove" onclick="removeFile(' + idx + ')">&times;</button>' +
        '</div>';
    }).join('');
}

function removeFile(idx) {
    pendingFiles.splice(idx, 1);
    renderFilePreview();
}

// ============================================
// Events from Java (SciNotes → Assistant)
// ============================================
function fromScilab(event) {
    if (!event || !event.type) return;

    if (event.type === 'editor_context') {
        updateEditorContext(event);
    }
}

function updateEditorContext(ctx) {
    if (!editorContextBar) return;
    const sep = document.getElementById('editor-context-sep');

    const name = ctx.shortName || '';
    const selLines = ctx.selectionLines || 0;

    if (!name) {
        editorContextBar.hidden = true;
        if (sep) sep.hidden = true;
        return;
    }

    editorContextBar.hidden = false;
    if (sep) sep.hidden = false;
    editorContextFile.textContent = name;

    if (selLines > 0) {
        editorContextSelection.textContent =
            selLines + ' line' + (selLines > 1 ? 's' : '') +
            ' selected (L' + ctx.selStart + '-L' + ctx.selEnd + ')';
    } else {
        editorContextSelection.textContent = '';
    }
}
