---
name: data-import
description: |
  Importing/exporting data and working with table & timeseries types (Scilab
  2024.0+): readtable, readtimeseries, csvRead, Excel xlsx, Parquet, joins,
  pivot, groupsummary, retime. Trigger words: "CSV", "Excel", "xlsx", "read
  file", "import data", "export", "table", "timeseries", "dataframe", "parquet",
  "load data", "spreadsheet", "resample".
triggers: csv, excel, xlsx, xls, parquet, table, tables, timeseries, dataframe, spreadsheet, tableur, feuille, sheet, read file, import, importer, importe, importes, export, exporter, exporte, readtable, writetable, csvread, série temporelle, séries temporelles, resample, rééchantillonne, rééchantillonner, retime, synchronize, sod, load data, charger les données, valeurs manquantes, missing values, jointure, join, pivot, groupsummary, readmatrix, xlsread, encodage, encoding
---

# Scilab data import/export, table & timeseries

IMPORTANT: `table` and `timeseries` were introduced in Scilab 2024.0, and
several functions below (xlsxRead, parquetRead, readtable for .xlsx) in 2026.1.
They may be newer than your training data — do NOT claim they don't exist, and
verify signatures with scilab_help instead of trusting your prior.

## Which function to use

| Data | Read | Write |
|------|------|-------|
| Columns with headers, mixed types (.csv, .txt, .dat, .xlsx) | `readtable` → table | `writetable` |
| Time-stamped data | `readtimeseries` → timeseries | `writetimeseries` |
| Pure numeric matrix in CSV/text | `csvRead` | `csvWrite` |
| Excel .xlsx (low level, cell ranges) | `xlsxRead`, `xlsxInfo`, `xlsxSheet` (2026.1) | `xlsxWrite` |
| Legacy Excel .xls (BIFF8 / Excel 2003) | `readxls` | — |
| Parquet | `parquetRead` (2026.1) | `parquetWrite` |
| Raw text lines | `mgetl` | `mputl` |
| Numeric matrix in whitespace text | `fscanfMat` | `fprintfMat` |

MATLAB functions that DO NOT exist in Scilab: `readmatrix`, `readcell`,
`xlsread`, `importdata`, `dlmread`, `textscan` (Scilab has `csvTextScan` —
capital S — for strings in csvRead format), `fillmissing`, `rmmissing`,
`ismissing`, `innerjoin`, `outerjoin`, `stack`/`unstack`. But
`readtable`/`writetable`/`join`/`pivot`/`sortrows`/`head`/`tail`/
`stackedplot` DO exist.

## readtable / detectImportOptions
`readtable` accepts ONLY these options: `VariableNames`, `ReadRowNames`,
`Sheet`, `Range`. Everything else (`Delimiter`, `Decimal`,
`NumHeaderLines`) goes through `detectImportOptions` — `readtable(f,
"Delimiter", ";")` errors.
```scilab
t = readtable("data.csv")                       // header row -> variable names
t = readtable("data.csv", "VariableNames", ["Name", "Area"])  // subset of columns
t = readtable("data.csv", "ReadRowNames", %t)   // 1st column -> row names
t = readtable("data.xlsx", "Sheet", "Sheet1")   // Excel sheet by name or index
t = readtable("data.xlsx", "Sheet", "Budget", "Range", "B3:F20")
opts = detectImportOptions("data.csv", "Delimiter", ";", "Decimal", ",", "NumHeaderLines", 3)
t = readtable("data.csv", opts)
```
Auto-detection is good: plain `readtable` already handles `;` + decimal
comma, `dd/MM/yyyy` dates, and leading `#` comment lines. Try it first, and
inspect `detectImportOptions(f)` (fields `delimiter`, `decimal`,
`inputFormat`, `variableTypes`) when the result looks wrong. There is no
`"InputFormat"` option on readtable/readtimeseries — the date format is
detected.

**Silently dropped rows**: header detection treats any of the first 20
lines whose FIRST character is not `[A-Za-z0-9".,;:|+-]` or space as a
comment. A data row starting with an accent (`Épinal`), `(`, `_`, `€`...
vanishes without warning, even in UTF-8. When the first column holds free
text, force the header count and check the row count against `mgetl`:
```scilab
opts = detectImportOptions(f, "NumHeaderLines", 0);
t = readtable(f, opts);
size(t, 1) == size(mgetl(f), 1) - 1     // 1 = the variable-name row
```

**Missing values**: a single `NA` or empty cell turns the WHOLE numeric
column into strings. Convert back with `strtod` (non-numeric -> `%nan`):
```scilab
t.Montant = strtod(t.Montant);          // "NA", "" -> %nan
t = t(~isnan(t.Montant) & t.Region <> "", :);   // drop incomplete rows
```

Encodings: Latin-1 files are decoded correctly by readtable/mgetl, no
conversion needed.

## csvRead pitfalls
```scilab
M = csvRead(filename, separator, decimal, conversion)
M = csvRead("data.csv")                  // default: "," separator, "." decimal
M = csvRead("data.csv", ";", ",")        // European CSV: ";" sep, "," decimal
S = csvRead("data.csv", ",", ".", "string")  // read everything as strings
M = csvRead("data.csv", ",", ".", "double", [], [], [], 1)  // skip 1 header line
```
csvRead returns a homogeneous matrix — for files mixing text columns and
numeric columns, use `readtable` instead. Tab-separated numbers:
`csvRead(f, ascii(9))` or `fscanfMat(f)`.

## Excel .xlsx (low level)
```scilab
xlsxInfo(f).sheet_names                              // list sheets
M = xlsxRead(f, "sheet", "Budget", "conversion", "string")  // options are key/value
xlsxSheet(f, "create", "Notes")                      // add a sheet (also "delete", "rename")
xlsxWrite(data, f, "sheet", "Notes", "range", "B3")  // data: double, string or cell
writetable(t, "out.xlsx", "Sheet", "Ventes")         // table -> new file
```
- **Empty edge rows/columns are TRIMMED** by `xlsxRead` and `readtable`:
  if column A is empty, result column 1 is sheet column B. Result positions
  do not map to sheet letters — do NOT conclude to a range/offset bug. Get
  the real extent with `xlsxSheet(f, "info", sheet)` (`max_row`,
  `max_col`), then read with an explicit `Range`.
- A title above the table (e.g. in B1) becomes the header of a plain
  `readtable(f, "Sheet", s)`: pass `"Range"` starting at the header row.
- `xlsxRead` converts to double by default: text cells come back as `Nan`.
  Use `"conversion", "string"` to see the layout.
- `xlsxWrite` creates a NAMED sheet only in a new file. On an existing file
  the sheet must exist: create it first with `xlsxSheet(f, "create", name)`.
- `"writemode", "append"` IGNORES `range`: it writes after the last used
  row, starting at column A (the help page says otherwise). To place data
  at a given cell, omit writemode — other cells are preserved.

Legacy `.xls`: `s = readxls(f)` returns an "xls" object; `s(k)` is sheet k
with fields `name`, `value` (numbers, text as Nan) and `text` (strings);
the sheet count is `size(s.sheets)` (`size(s)` is not overloaded).
```scilab
sh = s(1);
t = matrix2table(sh.value(3:$, :), "VariableNames", sh.text(2, :));
```

## Parquet
Filename FIRST (unlike writetable):
```scilab
parquetWrite("data.parquet", t)
t = parquetRead("data.parquet")
ts = parquetRead("data.parquet", "OutputType", "timeseries")
```

## table type (since 2024.0)
```scilab
t = table(Code, Area, Pop, "VariableNames", ["Code", "Area", "Pop"])
t = table(m)               // from matrix (= matrix2table)
t = table(st)              // from struct (= struct2table); also cell2table

// Indexing: like a matrix, plus names
t(2, 3);  t.Area;  t("Area");  t("Row1", "Area");  t(1, ["Code", "Area"])
t(6, :) = {"d", 2, -2};    // insert heterogeneous row: use { }
t.NewCol = (1:6)';         // add a column
t.Code = [];               // remove a column
t.Properties               // Description, VariableNames, VariableUnits, RowNames
```
Manipulation: `addvars`, `removevars`, `join` (combine two tables), `pivot`,
`groupsummary` / `groupcounts` (stats by group), `varfun` / `rowfun` (apply a
function), `summary`, `combinations`, `table2matrix` / `table2struct` /
`table2cell`, `writetable`.

```scilab
g = groupsummary(t, "Region", "sum", "Montant")               // or ["Quantite","Montant"]
p = pivot(t, Rows="Region", Columns="Produit", DataVariable="Montant", Method="sum")
s = sortrows(t, "Montant", "descend");  head(s, 10)          // not gsort on tables
writetable(t, "out.csv", "Delimiter", ";")
```
- `pivot` takes NAMED arguments (`Rows=...`), not "Rows", "..." pairs.
- `join(left, right, "Keys", "ID")` requires EVERY left key to exist in the
  right table (no innerjoin/outerjoin). Filter orphans first:
  `left = left(members(left.ID, right.ID) > 0, :)`.

## timeseries type (since 2024.0)
A table indexed by time (`datetime` or `duration` values from the time module:
`datetime()`, `hours()`, `seconds()`, ...).
```scilab
ts = timeseries(time, var1, ..., varN)                 // explicit time vector
ts = timeseries(var1, ..., varN, "TimeStep", seconds(1))
ts = timeseries(var1, ..., varN, "SampleRate", 100)
ts = readtimeseries("log.csv")                          // from file
ts = table2timeseries(t)

// resample: interpolation on a regular grid
tsout = retime(ts, "regular", "linear", "TimeStep", minutes(1))
// aggregate per period: pass a FUNCTION, not a string
tsout = retime(ts, "minutely", mean)        // also "hourly", "daily", ...
tsout = synchronize(ts1, ts2)               // union of times, NaN where missing
tsout = synchronize(ts1, ts2, "regular", "linear", "TimeStep", seconds(30))
isregular(ts)
fh = stackedplot(ts, "Title", "Capteur", "DisplayLabels", ["T (°C)", "HR (%)"])
// ^ creates its OWN figure and returns it: no scf/clf before
//   (an scf(0) left empty would be the figure you capture)
```
String methods are ONLY "fillwithmissing", "fillwithconstant", "linear",
"spline", "count", "firstvalue", "lastvalue", "mode". `"mean"`/`"sum"` are
rejected: use the functions `mean`/`sum`. `retime(ts, minutes(1), mean)`
fails ("Invalid index") — use a named step ("minutely") or the
`"regular", ..., "TimeStep", dt` form.

## Guidelines
- Before calling something a Scilab bug, check the simpler explanations
  above (trimmed empty columns, text columns, default numeric conversion)
  with a minimal read of the raw cells/lines
- Prefer `readtable`/`timeseries` over csvRead + manual parsing whenever the
  data has named columns or timestamps
- These types print nicely — display the result to the user after importing,
  and use `summary(t)` to describe an imported dataset
- For a quick look at an unknown file, `mgetl(filename, 5)` shows the first
  lines (delimiter, decimal separator, header)
- Version guard: `table`/`timeseries` need Scilab >= 2024.0; xlsxRead/
  parquetRead/readtable-on-xlsx need >= 2026.1
