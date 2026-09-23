/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Compact, pointer-free view of the Scilab runtime used while the interpreter
// owns the live context.  Document positions and LSP results do not belong
// here: they are rebuilt from the current document analysis.

#ifndef __RUNTIME_SNAPSHOT_HXX__
#define __RUNTIME_SNAPSHOT_HXX__

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

enum class RuntimeSymbolKind : uint8_t
{
    Variable,
    Function,
    Macro
};

struct RuntimeMacroSnapshot
{
    std::string sourcePath;
    std::string signature;
    int sourceStartLine = 0;
    int sourceStartColumn = 0;
    int sourceEndLine = 0;
    int sourceEndColumn = 0;
};

struct RuntimeSymbolSnapshot
{
    static constexpr uint32_t NoMacro = std::numeric_limits<uint32_t>::max();

    std::string name;
    uint32_t macroIndex = NoMacro;
    RuntimeSymbolKind kind = RuntimeSymbolKind::Variable;
    bool readonly = false;
};

class RuntimeSnapshot
{
public:
    // The caller must own RuntimeAccess' read lock.
    void capture();

    bool empty() const;
    const RuntimeSymbolSnapshot* find(const std::string& name) const;
    const std::vector<RuntimeSymbolSnapshot>& symbols() const;
    const RuntimeMacroSnapshot* macro(const RuntimeSymbolSnapshot& symbol) const;

private:
    std::vector<RuntimeSymbolSnapshot> m_symbols;
    std::vector<RuntimeMacroSnapshot> m_macros;
};

#endif /* !__RUNTIME_SNAPSHOT_HXX__ */
