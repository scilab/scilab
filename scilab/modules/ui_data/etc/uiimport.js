// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

// --- Virtual scroll constants ---
const ROW_HEIGHT = 24;
const BUFFER_ROWS = 10;   // extra rows rendered above/below viewport
const FETCH_CHUNK = 100;  // rows per fetch request
const MAX_CACHED_ROWS = 1000; // max rows kept in cache
const MAX_SCROLL_HEIGHT = 10_000_000; // browser max element height (~16M in Chromium, use 10M for safety)

// --- State ---

let state = {
    filetype: "csv",
    columns: [],     // {name, originalName, type, selected, inputFormat, outputFormat, formatRef, convert}
    rowtimes: "No",
    totalRows: 0,
    rowCache: {},    // rowIndex -> [cell0, cell1, ...]
    pendingFetch: null,  // {start, end} of current pending fetch
    lastVisibleRange: null,
};

// --- Scilab communication ---

function toScilab(msg) {
    Scilab({request: JSON.stringify(msg)});
}

function fromScilab(msg) {
    switch (msg.type) {
        case "preview":
            updatePreview(msg.data);
            break;
        case "rows":
            onRowsReceived(msg.start, msg.rows);
            break;
        case "error":
            if (msg.clear) clearPreview();
            showError(msg.message);
            break;
        case "progress":
            setProgress(msg.value);
            break;
        case "clear":
            clearPreview();
            break;
        case "refresh":
            // format/convert changed: clear cache, re-render visible rows
            state.rowCache = {};
            renderVisibleRows();
            buildToolbarTimeVarDropdown();
            break;
        case "updateformat":
            // Scilab rejected a format edit and is asking us to revert the field
            updateFormatFields(msg.column, msg.inputFormat, msg.outputFormat);
            break;
        default:
            console.log("unhandled message", msg);
    }
}

// --- Initialization ---

document.addEventListener("DOMContentLoaded", () => {
    // Toolbar
    document.getElementById("btn-open").addEventListener("click", () => {
        toScilab({type: "selectfile"});
    });

    document.getElementById("btn-import").addEventListener("click", () => {
        toScilab({
            type: "variable",
            varname: document.getElementById("input-varname").value,
            vartype: document.getElementById("info-vartype").value,
            config: getConfig()
        });
    });

    document.getElementById("btn-function").addEventListener("click", () => {
        toScilab({
            type: "function",
            varname: document.getElementById("input-varname").value,
            vartype: document.getElementById("info-vartype").value,
            config: getConfig()
        });
    });

    document.getElementById("btn-help").addEventListener("click", () => {
        toScilab({type: "help"});
    });

    document.getElementById("info-vartype").addEventListener("change", (e) => {
        const val = e.target.value;
        const timeGroup = document.getElementById("toolbar-time-group");
        if (val === "timeseries") {
            timeGroup.classList.remove("d-none");
            timeGroup.classList.add("d-flex");
            buildTimeVarDropdown();
            buildToolbarTimeVarDropdown();
        } else {
            timeGroup.classList.add("d-none");
            timeGroup.classList.remove("d-flex");
            state.rowtimes = "No";
            const sel = document.getElementById("select-timevar");
            if (sel) sel.value = "No";
            onTimeVarChange();
        }
    });
    document.getElementById("input-varname").addEventListener("change", (e) => {
        let val = e.target.value.trim();
        if (val === "") {
            val = e.target.getAttribute("data-last-valid") || "data";
        } else {
            val = val.replace(/[^a-zA-Z0-9_]/g, '_');
            if (!/^[a-zA-Z_]/.test(val)) {
                val = 'var_' + val;
            }
            const keywords = [
                "if", "then", "else", "elseif", "end", "for", "while", "function", 
                "endfunction", "select", "case", "try", "catch", "return", "resume", 
                "abort", "break", "continue", "global", "clear"
            ];
            if (keywords.includes(val.toLowerCase())) {
                val = 'var_' + val;
            }
        }
        e.target.value = val;
        e.target.setAttribute("data-last-valid", val);
    });

    // Delimiter change
    document.getElementById("select-delimiter").addEventListener("change", (e) => {
        const val = e.target.value;
        document.getElementById("row-custom-delim").classList.toggle("d-none", val !== "custom");
        if (val !== "custom") {
            requestPreview();
        }
    });

    // Custom delimiter
    document.getElementById("input-custom-delim").addEventListener("change", () => {
        if (document.getElementById("input-custom-delim").value !== "") {
            requestPreview();
        }
    });

    // Decimal change
    document.getElementById("select-decimal").addEventListener("change", () => {
        requestPreview();
    });

    // Sheet change (XLSX)
    const selectSheetEl = document.getElementById("select-sheet");
    if (selectSheetEl) {
        selectSheetEl.addEventListener("change", (e) => {
            let val = e.target.value;
            const num = parseInt(val);
            if (!isNaN(num) && String(num) === val) {
                val = num;
            }
            toScilab({type: "sheet", sheet: val});
        });
    }

    // Header change
    document.getElementById("input-header").addEventListener("change", () => {
        requestPreview();
    });

    // Row numbers toggle
    document.getElementById("toggle-rownum").addEventListener("change", (e) => {
        document.getElementById("preview-table").classList.toggle("hide-rownum", !e.target.checked);
        if (state.totalRows > 0) freezeColumnWidths();
    });

    // Select all / none
    document.getElementById("btn-selectall").addEventListener("click", () => {
        setAllColumns(true);
    });

    document.getElementById("btn-selectnone").addEventListener("click", () => {
        setAllColumns(false);
    });

    // Data conversion
    document.getElementById("select-data-conversion").addEventListener("change", onTimeVarChange);
    
    // Time reference (Toolbar)
    document.getElementById("select-timevar").addEventListener("change", (e) => {
        state.rowtimes = e.target.value;
    });

    // Convert
    document.getElementById("select-convert").addEventListener("change", () => {
        const tv = document.getElementById("select-data-conversion").value;
        const idx = state.columns.findIndex(c => c.name === tv);
        if (idx >= 0) {
            state.columns[idx].convert = document.getElementById("select-convert").value;
            toScilab({type: "convert", column: idx, convert: state.columns[idx].convert});
        }
    });

    // Input/output format
    document.getElementById("input-inputformat").addEventListener("change", () => {
        const tv = document.getElementById("select-data-conversion").value;
        const idx = state.columns.findIndex(c => c.name === tv);
        if (idx >= 0) {
            toScilab({type: "inputformat", column: idx, format: document.getElementById("input-inputformat").value});
        }
    });

    document.getElementById("input-outputformat").addEventListener("change", () => {
        const tv = document.getElementById("select-data-conversion").value;
        const idx = state.columns.findIndex(c => c.name === tv);
        if (idx >= 0) {
            toScilab({type: "outputformat", column: idx, format: document.getElementById("input-outputformat").value});
        }
    });

    document.getElementById("btn-resetinput").addEventListener("click", () => {
        const tv = document.getElementById("select-data-conversion").value;
        const idx = state.columns.findIndex(c => c.name === tv);
        if (idx >= 0) {
            toScilab({type: "resetinput", column: idx, format: state.columns[idx].formatRef});
        }
    });

    document.getElementById("btn-resetoutput").addEventListener("click", () => {
        const tv = document.getElementById("select-data-conversion").value;
        const idx = state.columns.findIndex(c => c.name === tv);
        if (idx >= 0) {
            toScilab({type: "resetoutput", column: idx, format: state.columns[idx].formatRef});
        }
    });

    // Virtual scroll handler
    document.getElementById("preview-scroll").addEventListener("scroll", onScroll);

    const previewScroll = document.getElementById("preview-scroll");

    new ResizeObserver(() => {
        requestAnimationFrame(() => {
            freezeColumnWidths();
        });
    }).observe(previewScroll);

});

// --- Actions ---

function getDelimiter() {
    const sel = document.getElementById("select-delimiter");
    if (sel.value === "custom") {
        return document.getElementById("input-custom-delim").value;
    }
    return sel.value;
}

function getDecimal() {
    return document.getElementById("select-decimal").value;
}

function getHeaderLines() {
    return parseInt(document.getElementById("input-header").value) || 0;
}

function requestPreview() {
    toScilab({
        type: "preview",
        delimiter: getDelimiter(),
        decimal: getDecimal(),
        numheaderlines: getHeaderLines()
    });
}

function getConfig() {
    return {
        keepcols: state.columns.map((c, i) => c.selected ? i : -1).filter(i => i >= 0),
        varnames: state.columns.map(c => c.name),
        rowtimes: state.rowtimes,
        converts: state.columns.map(c => c.convert || ""),
        formatRef: state.columns.map(c => c.formatRef || ""),
        outputFormats: state.columns.map(c => c.outputFormat || ""),
        delimiter: getDelimiter(),
        decimal: getDecimal(),
        numheaderlines: getHeaderLines()
    };
}

// --- Virtual Scrolling ---

function onScroll() {
    renderVisibleRows();
}

function getScrollScale() {
    const naturalHeight = state.totalRows * ROW_HEIGHT;
    if (naturalHeight <= MAX_SCROLL_HEIGHT) return 1;
    return naturalHeight / MAX_SCROLL_HEIGHT;
}

function getVisibleRange() {
    const scrollEl = document.getElementById("preview-scroll");
    const scrollTop = scrollEl.scrollTop;
    const viewportHeight = scrollEl.clientHeight;
    const scale = getScrollScale();

    // Map scroll position to row index using scale factor
    const startRow = Math.max(0, Math.floor((scrollTop * scale) / ROW_HEIGHT) - BUFFER_ROWS);
    const endRow = Math.min(state.totalRows, Math.ceil(((scrollTop + viewportHeight) * scale) / ROW_HEIGHT) + BUFFER_ROWS);

    return {start: startRow, end: endRow};
}

function renderVisibleRows() {
    if (state.totalRows === 0) return;

    const range = getVisibleRange();

    // Check which rows are missing from cache
    let missingStart = -1;
    let missingEnd = -1;
    for (let i = range.start; i < range.end; i++) {
        if (!state.rowCache[i]) {
            if (missingStart === -1) missingStart = i;
            missingEnd = i + 1;
        }
    }

    // Request missing rows from Scilab
    if (missingStart !== -1) {
        fetchRows(missingStart, missingEnd);
    }

    // Render what we have
    renderRows(range.start, range.end);
    state.lastVisibleRange = range;
}

function fetchRows(start, end) {
    // Extend to chunk boundary for efficiency
    const chunkStart = Math.floor(start / FETCH_CHUNK) * FETCH_CHUNK;
    const chunkEnd = Math.min(state.totalRows, Math.ceil(end / FETCH_CHUNK) * FETCH_CHUNK);

    // Don't re-request if we already have a pending fetch covering this range
    if (state.pendingFetch && state.pendingFetch.start <= start && state.pendingFetch.end >= end) {
        return;
    }

    state.pendingFetch = {start: chunkStart, end: chunkEnd};
    toScilab({type: "fetchrows", start: chunkStart, count: chunkEnd - chunkStart});
}

function evictCache() {
    const keys = Object.keys(state.rowCache);
    if (keys.length <= MAX_CACHED_ROWS) return;

    // Evict rows furthest from current visible range
    const range = getVisibleRange();
    const center = (range.start + range.end) / 2;

    keys.sort((a, b) => Math.abs(a - center) - Math.abs(b - center));

    // Keep the closest MAX_CACHED_ROWS, delete the rest
    const toRemove = keys.slice(MAX_CACHED_ROWS);
    for (const k of toRemove) {
        delete state.rowCache[k];
    }
}

function onRowsReceived(startIndex, rows) {
    state.pendingFetch = null;

    // Store rows in cache (rehydrate single-row / single-col collapses)
    rows = asRowMatrix(rows, state.columns.length);
    rows.forEach((row, i) => {
        state.rowCache[startIndex + i] = row;
    });

    // Purge if cache too large
    evictCache();

    // Re-render visible area
    const range = getVisibleRange();
    renderRows(range.start, range.end);

    // Check if there are still missing rows in the visible range
    let stillMissing = false;
    for (let i = range.start; i < range.end; i++) {
        if (!state.rowCache[i]) {
            stillMissing = true;
            break;
        }
    }

    if (stillMissing) {
        renderVisibleRows();
    }
}

function renderRows(start, end) {
    const tbody = document.getElementById("preview-tbody");
    const spacerTop = document.getElementById("spacer-top");
    const spacerBottom = document.getElementById("spacer-bottom");
    const scrollEl = document.getElementById("preview-scroll");
    const nbcols = state.columns.length;

    // Save horizontal scroll position before DOM changes
    const savedScrollLeft = scrollEl.scrollLeft;

    // Remove old data rows (everything between spacer-top and spacer-bottom)
    while (spacerTop.nextElementSibling && spacerTop.nextElementSibling !== spacerBottom) {
        spacerTop.nextElementSibling.remove();
    }

    // Set spacer heights using scale factor for large files
    const scale = getScrollScale();
    const topHeight = (start * ROW_HEIGHT) / scale;
    const bottomHeight = Math.max(0, ((state.totalRows - end) * ROW_HEIGHT) / scale);
    spacerTop.firstElementChild.style.height = topHeight + "px";
    spacerBottom.firstElementChild.style.height = bottomHeight + "px";

    // Insert data rows before spacer-bottom
    const fragment = document.createDocumentFragment();
    for (let i = start; i < end; i++) {
        const row = state.rowCache[i];
        const tr = document.createElement("tr");
        tr.style.height = ROW_HEIGHT + "px";

        // Row number column
        const tdNum = document.createElement("td");
        tdNum.className = "row-number";
        tdNum.textContent = i + 1;
        tr.appendChild(tdNum);

        for (let j = 0; j < nbcols; j++) {
            const td = document.createElement("td");
            td.dataset.col = j;
            if (!state.columns[j].selected) {
                td.className = "col-hidden";
            }
            if (row) {
                td.textContent = row[j] || "\u00A0";
            } else {
                td.textContent = "\u2026";
                td.style.color = "#ccc";
            }
            tr.appendChild(td);
        }
        fragment.appendChild(tr);
    }
    tbody.insertBefore(fragment, spacerBottom);

    // Restore horizontal scroll position after DOM changes
    scrollEl.scrollLeft = savedScrollLeft;
}

function escapeHtml(text) {
    const div = document.createElement("div");
    div.textContent = text;
    return div.innerHTML;
}

// Scilab JSON marshalling collapses 1-element matrices to scalars and
// 1-row/1-col matrices to flat arrays. These helpers re-inflate them.
function asArray(v) {
    if (v === undefined || v === null) return [];
    return Array.isArray(v) ? v : [v];
}

function asRowMatrix(v, ncols) {
    if (v === undefined || v === null) return [];
    if (!Array.isArray(v)) {
        return [[v]];
    }
    if (v.length === 0) return [];
    if (Array.isArray(v[0])) return v;
    // Flat array: either a single row (ncols cells) or a single column (1 cell per row)
    if (ncols === 1) {
        return v.map(cell => [cell]);
    }
    return [v];
}

// --- UI Updates ---

function clearPreview() {
    document.getElementById("preview-scroll").classList.add("d-none");
    document.getElementById("preview-empty").classList.remove("d-none");
    document.getElementById("panel-columns").classList.add("d-none");
    document.getElementById("info-rows").textContent = "-";
    document.getElementById("info-cols").textContent = "-";
    document.getElementById("btn-import").disabled = true;
    document.getElementById("btn-function").disabled = true;
    document.getElementById("toolbar-separator").classList.add("d-none");
    document.getElementById("toolbar-variable-info").classList.remove("d-flex");
    document.getElementById("toolbar-variable-info").classList.add("d-none");
    state.columns = [];
    state.totalRows = 0;
    state.rowCache = {};
    state.pendingFetch = null;

    // Reset variable type and hide time reference group
    const infoVartype = document.getElementById("info-vartype");
    if (infoVartype) infoVartype.value = "table";
    const timeGroup = document.getElementById("toolbar-time-group");
    if (timeGroup) {
        timeGroup.classList.add("d-none");
        timeGroup.classList.remove("d-flex");
    }
}

function showError(message) {
    alert(message);
}

function setProgress(value) {
    document.getElementById("progressbar").style.width = (value * 100) + "%";
}

function updatePreview(data) {
    // data: {filetype, sheetnames, currentSheet, nbrows, nbcols, headerlines, delimiter, decimal, varnames, vartypes, inputFormats, totalDataRows, initialRows, initialStart}

    state.filetype = data.filetype || "csv";

    // Scilab JSON collapses single-element vectors to scalars; rehydrate them.
    data.varnames = asArray(data.varnames);
    data.vartypes = asArray(data.vartypes);
    data.inputFormats = asArray(data.inputFormats);
    data.initialRows = asRowMatrix(data.initialRows, data.varnames.length);

    // Update file info
    document.getElementById("info-rows").textContent = data.nbrows;
    document.getElementById("info-cols").textContent = data.nbcols;
    document.getElementById("input-header").value = data.headerlines;

    // Show/hide controls based on filetype
    if (state.filetype === "xlsx") {
        document.getElementById("row-delimiter").classList.add("d-none");
        document.getElementById("row-custom-delim").classList.add("d-none");
        document.getElementById("row-decimal").classList.add("d-none");

        const rowSheet = document.getElementById("row-sheet");
        if (rowSheet) {
            rowSheet.classList.remove("d-none");
            const selectSheet = document.getElementById("select-sheet");
            selectSheet.innerHTML = "";
            const sheets = asArray(data.sheetnames);
            sheets.forEach((sh) => {
                const opt = document.createElement("option");
                opt.value = sh;
                opt.textContent = sh;
                selectSheet.appendChild(opt);
            });
            let targetSheet = data.currentSheet;
            if (typeof targetSheet === "number" && sheets[targetSheet - 1]) {
                targetSheet = sheets[targetSheet - 1];
            }
            if (targetSheet) {
                selectSheet.value = targetSheet;
            }
        }
    } else {
        document.getElementById("row-delimiter").classList.remove("d-none");
        document.getElementById("row-decimal").classList.remove("d-none");
        const rowSheet = document.getElementById("row-sheet");
        if (rowSheet) rowSheet.classList.add("d-none");

        // Set delimiter in dropdown
        setDelimiterUI(data.delimiter);
        setDecimalUI(data.decimal);
    }

    // Build column state
    state.columns = data.varnames.map((name, i) => ({
        name: name,
        originalName: name,
        type: data.vartypes[i],
        selected: true,
        inputFormat: data.inputFormats ? data.inputFormats[i] : "",
        outputFormat: data.inputFormats ? data.inputFormats[i] : "",
        formatRef: data.inputFormats ? data.inputFormats[i] : "",
        convert: ""
    }));
    state.rowtimes = "No";
    state.totalRows = data.totalDataRows;
    state.rowCache = {};
    state.pendingFetch = null;

    // Store initial rows in cache
    if (data.initialRows) {
        data.initialRows.forEach((row, i) => {
            state.rowCache[data.initialStart + i] = row;
        });
    }

    // Build columns list
    buildColumnsList();

    // Build time variable dropdowns
    buildTimeVarDropdown();
    buildToolbarTimeVarDropdown();

    // Hide toolbar time group initially since default type is 'table'
    const timeGroup = document.getElementById("toolbar-time-group");
    if (timeGroup) {
        timeGroup.classList.add("d-none");
        timeGroup.classList.remove("d-flex");
    }

    // Setup virtual scroll
    setupVirtualScroll();

    // Build header
    buildPreviewHeader(data.varnames);

    // Show panels
    document.getElementById("panel-columns").classList.remove("d-none");
    document.getElementById("preview-scroll").classList.remove("d-none");
    document.getElementById("preview-empty").classList.add("d-none");

    // Enable buttons
    document.getElementById("btn-import").disabled = false;
    document.getElementById("btn-function").disabled = false;

    // Show toolbar variable info
    document.getElementById("toolbar-separator").classList.remove("d-none");
    document.getElementById("toolbar-variable-info").classList.remove("d-none");
    document.getElementById("toolbar-variable-info").classList.add("d-flex");

    // Compute and set default variable name
    const defaultVarName = cleanVariableName(data.path);
    const varnameInput = document.getElementById("input-varname");
    varnameInput.value = defaultVarName;
    varnameInput.setAttribute("data-last-valid", defaultVarName);


    setProgress(0);

    // Initial render, then freeze column widths
    renderVisibleRows();
    freezeColumnWidths();
}

function setupVirtualScroll() {
    const tbody = document.getElementById("preview-tbody");
    const nbcols = state.columns.length;

    // Reset tbody: just the two spacers
    tbody.innerHTML = "";

    const spacerTop = document.createElement("tr");
    spacerTop.id = "spacer-top";
    const tdTop = document.createElement("td");
    tdTop.colSpan = nbcols + 1; // +1 for row number column
    tdTop.style.height = "0px";
    spacerTop.appendChild(tdTop);
    tbody.appendChild(spacerTop);

    const spacerBottom = document.createElement("tr");
    spacerBottom.id = "spacer-bottom";
    const tdBottom = document.createElement("td");
    tdBottom.colSpan = nbcols + 1; // +1 for row number column
    const totalHeight = Math.min(state.totalRows * ROW_HEIGHT, MAX_SCROLL_HEIGHT);
    tdBottom.style.height = totalHeight + "px";
    spacerBottom.appendChild(tdBottom);
    tbody.appendChild(spacerBottom);

    // Reset scroll position
    document.getElementById("preview-scroll").scrollTop = 0;
}

function freezeColumnWidths() {
    const table = document.getElementById("preview-table");
    const scrollEl = document.getElementById("preview-scroll");
    const hideRowNum = table.classList.contains("hide-rownum");
    const ths = document.querySelectorAll("#preview-thead-row th");
    const containerWidth = scrollEl.clientWidth;

    // Save horizontal scroll position (safety net for layout changes)
    const savedScrollLeft = scrollEl.scrollLeft;

    // Build a hidden measurement table with no width constraint
    const measTable = document.createElement("table");
    measTable.className = "table table-sm table-bordered mb-0";
    measTable.style.cssText = "position:absolute;top:-9999px;left:-9999px;width:auto;white-space:nowrap;font-size:12px;visibility:hidden;";

    // Determine which columns are visible
    const isVisible = [];
    ths.forEach(th => {
        if (hideRowNum && th.classList.contains("row-number")) {
            isVisible.push(false);
        } else if (th.classList.contains("col-hidden")) {
            isVisible.push(false);
        } else {
            isVisible.push(true);
        }
    });

    // Header row (only visible columns)
    const measHead = document.createElement("thead");
    const headTr = document.createElement("tr");
    ths.forEach((th, i) => {
        if (!isVisible[i]) return;
        const cell = document.createElement("th");
        if (th.classList.contains("row-number")) {
            cell.textContent = String(state.totalRows);
        } else {
            cell.textContent = th.textContent;
        }
        headTr.appendChild(cell);
    });
    measHead.appendChild(headTr);
    measTable.appendChild(measHead);

    // Sample data rows (only visible columns)
    const measBody = document.createElement("tbody");
    const dataRows = document.querySelectorAll("#preview-tbody tr:not(#spacer-top):not(#spacer-bottom)");
    const sampleCount = Math.min(dataRows.length, 20);
    for (let r = 0; r < sampleCount; r++) {
        const tr = document.createElement("tr");
        const tds = dataRows[r].querySelectorAll("td");
        tds.forEach((td, i) => {
            if (!isVisible[i]) return;
            const cell = document.createElement("td");
            cell.textContent = td.textContent;
            tr.appendChild(cell);
        });
        measBody.appendChild(tr);
    }
    measTable.appendChild(measBody);

    document.body.appendChild(measTable);
    measTable.offsetWidth;

    // Read unconstrained widths from measurement table
    const measWidths = [];
    headTr.querySelectorAll("th").forEach(th => {
        measWidths.push(Math.ceil(th.getBoundingClientRect().width));
    });
    document.body.removeChild(measTable);

    // Map measured widths back: 0 for hidden columns
    const colWidths = [];
    let mi = 0;
    ths.forEach((th, i) => {
        if (!isVisible[i]) {
            colWidths.push(0);
        } else {
            // Add 4px of safety margin (2px on each side)
            colWidths.push(measWidths[mi++] + 4);
        }
    });

    // If visible columns fit, distribute extra space across data columns only
    // (row-number stays at its natural width so it doesn't bloat with few columns)
    let totalWidth = colWidths.reduce((a, b) => a + b, 0);
    if (totalWidth < containerWidth) {
        const dataCount = ths.length === 0 ? 0 : Array.from(ths).filter((th, i) =>
            colWidths[i] > 0 && !th.classList.contains("row-number")).length;
        if (dataCount > 0) {
            const extra = containerWidth - 1  - totalWidth;
            const perCol = Math.floor(extra / dataCount);
            ths.forEach((th, i) => {
                if (colWidths[i] > 0 && !th.classList.contains("row-number")) {
                    colWidths[i] += perCol;
                }
            });
            totalWidth = colWidths.reduce((a, b) => a + b, 0);
        }
    }

    // Apply fixed widths
    ths.forEach((th, i) => { th.style.width = colWidths[i] + "px"; });
    table.style.tableLayout = "fixed";
    table.style.width = totalWidth + "px";

    // Restore horizontal scroll position
    scrollEl.scrollLeft = savedScrollLeft;
}

function buildPreviewHeader(varnames) {
    const tr = document.getElementById("preview-thead-row");
    tr.innerHTML = "";

    // Row number header
    const thNum = document.createElement("th");
    thNum.className = "row-number";
    thNum.textContent = "#";
    tr.appendChild(thNum);

    varnames.forEach((name, i) => {
        const th = document.createElement("th");
        th.textContent = name;
        th.dataset.col = i;
        tr.appendChild(th);
    });
}

function setDelimiterUI(delim) {
    const sel = document.getElementById("select-delimiter");
    let found = false;
    for (let opt of sel.options) {
        if (opt.value === delim) {
            sel.value = delim;
            found = true;
            break;
        }
    }
    if (!found) {
        sel.value = "custom";
        document.getElementById("input-custom-delim").value = delim;
        document.getElementById("row-custom-delim").classList.remove("d-none");
    } else {
        document.getElementById("row-custom-delim").classList.add("d-none");
    }
}

function setDecimalUI(decim) {
    if (decim) {
        document.getElementById("select-decimal").value = decim;
    }
}

function buildColumnsList() {
    const container = document.getElementById("columns-list");
    container.innerHTML = "";

    state.columns.forEach((col, i) => {
        const div = document.createElement("div");
        div.className = "column-item";

        const cb = document.createElement("input");
        cb.type = "checkbox";
        cb.className = "form-check-input";
        cb.checked = col.selected;
        cb.addEventListener("change", () => {
            state.columns[i].selected = cb.checked;
            input.disabled = !cb.checked;
            updateColumnVisibility(i, cb.checked);
            updateButtonState();
            buildTimeVarDropdown();
            buildToolbarTimeVarDropdown();
        });

        const input = document.createElement("input");
        input.type = "text";
        input.className = "col-name-input";
        input.value = col.name;
        input.addEventListener("change", () => {
            if (input.value === "") {
                input.value = col.name;
                return;
            }
            const oldName = col.name;
            state.columns[i].name = input.value;
            updateHeaderName(i, input.value);
            if (state.rowtimes === oldName) {
                state.rowtimes = input.value;
            }
            buildTimeVarDropdown();
            buildToolbarTimeVarDropdown();
        });

        div.appendChild(cb);
        div.appendChild(input);
        container.appendChild(div);
    });
    container.scrollTop = 0;
}

function buildToolbarTimeVarDropdown() {
    const sel = document.getElementById("select-timevar");
    if (!sel) return;

    const prevValue = sel.value;
    sel.innerHTML = "";

    // Find all eligible columns (datetime, duration, converted double)
    const eligibleCols = state.columns.filter(col => {
        if (!col.selected) return false;
        const isDateTimeOrDuration = (col.type === "datetime" || col.type === "duration");
        const isConvertedDouble = (col.type === "double" && col.convert && col.convert !== "");
        return isDateTimeOrDuration || isConvertedDouble;
    });

    if (eligibleCols.length === 0) {
        // No time columns detected: show only "No"
        const noOpt = document.createElement("option");
        noOpt.value = "No";
        noOpt.textContent = "No";
        sel.appendChild(noOpt);
        sel.value = "No";
        state.rowtimes = "No";
    } else {
        // At least one time column detected: do NOT show "No"
        let foundPrev = false;
        eligibleCols.forEach(col => {
            const opt = document.createElement("option");
            opt.value = col.name;
            opt.textContent = col.name;
            sel.appendChild(opt);

            if (col.name === prevValue) {
                foundPrev = true;
            }
        });

        // Set default selection
        const vartype = document.getElementById("info-vartype").value;
        if (vartype === "timeseries") {
            if (foundPrev && prevValue !== "No") {
                sel.value = prevValue;
                state.rowtimes = prevValue;
            } else {
                // Default to the first found time column
                sel.value = eligibleCols[0].name;
                state.rowtimes = eligibleCols[0].name;
            }
        } else {
            state.rowtimes = "No";
        }
    }
}

function buildTimeVarDropdown() {
    const sel = document.getElementById("select-data-conversion");
    const prevValue = sel.value;
    const prevValueIdx = state.columns.findIndex(
        col => 
            (col.name === prevValue || col.originalName === prevValue) &&
             ["double", "datetime", "duration"].includes(col.type) && col.selected
    );
    
    sel.innerHTML = "";

    const noOpt = document.createElement("option");
    noOpt.value = "No";
    noOpt.textContent = "No";
    sel.appendChild(noOpt);

    state.columns.forEach(col => {
        if (col.selected && (col.type === "double" || col.type === "datetime" || col.type === "duration")) {
            const opt = document.createElement("option");
            opt.value = col.name;
            opt.textContent = col.name;
            sel.appendChild(opt);
        }
    });

    // Restore selection if still available
    let found = (prevValueIdx === -1) ? false : true;
    
    if (!found) {
        sel.value = "No";
        state.rowtimes = "No";
        document.getElementById("panel-format").classList.add("d-none");
        document.getElementById("panel-convert").classList.add("d-none");
    }
    else
    {
        sel.value = state.columns[prevValueIdx].name;
    }

    onTimeVarChange();
}

function onTimeVarChange() {
    const tv = document.getElementById("select-data-conversion").value;

    if (tv === "No") {
        document.getElementById("panel-format").classList.add("d-none");
        document.getElementById("panel-convert").classList.add("d-none");
        return;
    }

    const col = state.columns.find(c => c.name === tv);
    if (!col) {
        return;
    }

    if (col.type === "double") {
        document.getElementById("panel-convert").classList.remove("d-none");
        document.getElementById("panel-format").classList.add("d-none");
        document.getElementById("select-convert").value = col.convert || "";
    } else {
        document.getElementById("panel-format").classList.remove("d-none");
        document.getElementById("panel-convert").classList.add("d-none");
        document.getElementById("input-inputformat").value = col.inputFormat || "";
        document.getElementById("input-outputformat").value = col.outputFormat || "";
    }
}

function updateColumnVisibility(colIndex, visible) {
    // Update header
    const th = document.querySelector(`#preview-thead-row th[data-col="${colIndex}"]`);
    if (th) th.classList.toggle("col-hidden", !visible);

    // Re-render visible rows and recalculate widths
    const range = getVisibleRange();
    renderRows(range.start, range.end);
    freezeColumnWidths();
}

function updateHeaderName(colIndex, newName) {
    const th = document.querySelector(`#preview-thead-row th[data-col="${colIndex}"]`);
    if (th) {
        th.textContent = newName;
    }

    // Re-render visible rows and recalculate widths
    const range = getVisibleRange();
    renderRows(range.start, range.end);
    freezeColumnWidths();
}

function updateButtonState() {
    const anySelected = state.columns.some(c => c.selected);
    document.getElementById("btn-import").disabled = !anySelected;
    document.getElementById("btn-function").disabled = !anySelected;
}

function setAllColumns(selected) {
    state.columns.forEach((col, i) => {
        col.selected = selected;
    });

    // Update checkboxes
    const checkboxes = document.querySelectorAll("#columns-list .form-check-input");
    checkboxes.forEach(cb => cb.checked = selected);

    const inputs = document.querySelectorAll("#columns-list .col-name-input");
    inputs.forEach(input => input.disabled = !selected);

    // Update header visibility (skip row-number column)
    document.querySelectorAll("#preview-thead-row th[data-col]").forEach(th => {
        th.classList.toggle("col-hidden", !selected);
    });

    updateButtonState();
    buildTimeVarDropdown();
    buildToolbarTimeVarDropdown();

    // Re-render and recalculate widths
    const range = getVisibleRange();
    renderRows(range.start, range.end);
    freezeColumnWidths();
}

// Update format fields from Scilab response
function updateFormatFields(colIndex, inputFormat, outputFormat) {
    const col = state.columns[colIndex];
    if (inputFormat !== undefined) col.inputFormat = inputFormat;
    if (outputFormat !== undefined) col.outputFormat = outputFormat;

    const tv = document.getElementById("select-data-conversion").value;
    if (col.name === tv) {
        if (inputFormat !== undefined) document.getElementById("input-inputformat").value = inputFormat;
        if (outputFormat !== undefined) document.getElementById("input-outputformat").value = outputFormat;
    }
}

function cleanVariableName(filename) {
    if (!filename) return "data";
    // Extract name without extension and path
    let name = filename.substring(filename.lastIndexOf('/') + 1);
    name = name.substring(name.lastIndexOf('\\') + 1);
    
    // Remove extension
    const dotIdx = name.lastIndexOf('.');
    if (dotIdx !== -1) {
        name = name.substring(0, dotIdx);
    }
    
    // Replace non-alphanumeric characters (excluding underscores) with underscores
    name = name.replace(/[^a-zA-Z0-9_]/g, '_');
    
    // Ensure it starts with a letter or underscore
    if (!/^[a-zA-Z_]/.test(name)) {
        name = 'var_' + name;
    }
    
    // Check against Scilab keywords
    const keywords = [
        "if", "then", "else", "elseif", "end", "for", "while", "function", 
        "endfunction", "select", "case", "try", "catch", "return", "resume", 
        "abort", "break", "continue", "global", "clear"
    ];
    if (keywords.includes(name.toLowerCase())) {
        name = 'var_' + name;
    }
    
    return name;
}


