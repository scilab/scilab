#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <string>
#include <fstream>
#include <map>
#include <iterator>
#include <vector>
#include <filesystem>
#include <mutex>

#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libxml/parser.h>
#include <libxml/xpathInternals.h>

#include "configvariable.hxx"
#include "json.hxx"
#include "struct.hxx"
#include "UTF8.hxx"

#include "inlinehelp.hxx"

extern "C"
{
#include "dynlib_core.h"
#include "charEncoding.h"
#include "localization.h"
#include "setgetlanguage.h"
#include "sci_malloc.h"
#include "sciprint.h"
#include "inlinehelp.h"
}

//generate_inline_links
// The index is read by the interpreter (help, toolbox loading) and by the LSP
// thread (hover, signature help): every access goes through g_linksMutex.
static std::unordered_map<std::string, types::Struct*> g_linksByLanguage;
static std::mutex g_linksMutex;

std::once_flag g_xmlInitOnce;

// One compiled stylesheet per file, reparsed only when the file changes. A
// compiled stylesheet can be applied concurrently by several threads, so the
// selection made by loadStyleSheet is kept per thread: the console help and an
// LSP hover each transform with their own sheet.
struct CachedStyleSheet
{
    xsltStylesheetPtr sheet;
    std::filesystem::file_time_type writeTime;
};
static std::map<std::string, CachedStyleSheet> g_stylesheets;
static std::mutex g_stylesheetsMutex;
static thread_local xsltStylesheetPtr t_stylesheet = nullptr;

//used in initscilab
void ensureLibxmlInitialized()
{
    std::call_once(g_xmlInitOnce, []()
    {
        xmlInitParser();
        xsltInit();
    });
}

std::string normalizeSeparators(const std::string& value)
{
    std::string ret(value);
    std::replace(ret.begin(), ret.end(), '\\', '/');
    return ret;
}

std::string toSystemPath(const std::string& value)
{
    std::string ret(value);
#ifdef _MSC_VER
    std::replace(ret.begin(), ret.end(), '/', '\\');
#endif
    return ret;
}

std::string getSciPathUtf8()
{
    std::wstring& sciW = ConfigVariable::getSCIPath();
    char* sciC = wide_string_to_UTF8(sciW.c_str());
    std::string result(sciC ? sciC : "");
    if (sciC)
    {
        FREE(sciC);
    }
    return result;
}

std::string buildAbsolutePath(const std::string& sciPath, std::string stored, bool* isScilab)
{
    std::string normalizedSci = normalizeSeparators(sciPath);
    std::string normalizedStored = normalizeSeparators(std::move(stored));
    if(isScilab)
    {
        *isScilab = false;
    }

    if (normalizedStored.rfind("SCI/", 0) == 0)
    {
        normalizedStored.erase(0, 4);
        if(isScilab)
        {
            *isScilab = true;
        }
    }
    else if (normalizedStored.rfind("${SCI}", 0) == 0)
    {
        normalizedStored.erase(0, 6);
        if (!normalizedStored.empty() && normalizedStored[0] == '/')
        {
            normalizedStored.erase(0, 1);
        }

        if(isScilab)
        {
            *isScilab = true;
        }
    }

    if (normalizedStored.size() > 1 && normalizedStored[1] == ':')
    {
        return toSystemPath(normalizedStored);
    }

    if (!normalizedStored.empty() && normalizedStored[0] == '/')
    {
        return toSystemPath(normalizedStored);
    }

    if (!normalizedStored.empty())
    {
        if (!normalizedSci.empty() && normalizedSci.back() != '/')
        {
            normalizedSci.push_back('/');
        }
        normalizedSci += normalizedStored;
    }

    return toSystemPath(normalizedSci);
}

std::string buildInlineFilePath(const std::string& sciPath, const std::string& lang, const std::string& fileName)
{
    std::string relative = "modules/core/inline/" + lang + "/" + fileName;
    return buildAbsolutePath(sciPath, relative);
}

std::vector<std::wstring> listToolboxXmlForLanguage(const std::string& lang, const std::filesystem::path& path)
{
    std::vector<std::wstring> files;
    std::filesystem::path langDir = path / L"help" / lang;
    std::error_code langEc;
    if (!std::filesystem::is_directory(langDir, langEc))
    {
        return files;
    }

    std::filesystem::recursive_directory_iterator rend;
    for (std::filesystem::recursive_directory_iterator it(langDir, langEc); !langEc && it != rend; it.increment(langEc))
    {
        std::error_code fileEc;
        if (!it->is_regular_file(fileEc))
        {
            if (fileEc)
            {
                fileEc.clear();
            }
            continue;
        }

        if (it->path().extension() == L".xml")
        {
            std::string xml = scilab::UTF8::toUTF8(it->path().wstring());
            xml = normalizeSeparators(xml);
            files.push_back(scilab::UTF8::toWide(xml));
        }
    }

    std::sort(files.begin(), files.end());
    files.erase(std::unique(files.begin(), files.end()), files.end());
    return files;
}

std::vector<std::wstring> listModuleXmlForLanguage(const std::string& lang)
{
    std::vector<std::wstring> files;
    if (lang.empty())
    {
        return files;
    }

    std::wstring& sciPathW = ConfigVariable::getSCIPath();
    std::filesystem::path modulesRoot = std::filesystem::path(sciPathW) / L"modules";

    std::error_code ec;
    if (!std::filesystem::is_directory(modulesRoot, ec))
    {
        return files;
    }

    std::wstring langW(lang.begin(), lang.end());

    std::filesystem::directory_iterator end;
    for (std::filesystem::directory_iterator moduleIt(modulesRoot, ec); !ec && moduleIt != end; moduleIt.increment(ec))
    {
        std::error_code moduleEc;
        if (!moduleIt->is_directory(moduleEc))
        {
            if (moduleEc)
            {
                moduleEc.clear();
            }
            continue;
        }

        std::filesystem::path langDir = moduleIt->path() / L"help" / langW;
        std::error_code langEc;
        if (!std::filesystem::is_directory(langDir, langEc))
        {
            continue;
        }

        std::filesystem::recursive_directory_iterator rend;
        for (std::filesystem::recursive_directory_iterator it(langDir, langEc); !langEc && it != rend; it.increment(langEc))
        {
            std::error_code fileEc;
            if (!it->is_regular_file(fileEc))
            {
                if (fileEc)
                {
                    fileEc.clear();
                }
                continue;
            }

            if (it->path().extension() == L".xml")
            {
                std::string xml = scilab::UTF8::toUTF8(it->path().wstring());
                xml = normalizeSeparators(xml);
                files.push_back(scilab::UTF8::toWide(xml));
            }
        }
    }

    std::sort(files.begin(), files.end());
    files.erase(std::unique(files.begin(), files.end()), files.end());
    return files;
}

bool loadStyleSheet(const std::string& sheetPath)
{
    std::string stylesheetPath = normalizeSeparators(sheetPath);
    //(sciPath, "modules/core/etc/help.xsl");
    std::error_code ec;
    std::filesystem::file_time_type writeTime = std::filesystem::last_write_time(stylesheetPath, ec);

    std::lock_guard<std::mutex> lock(g_stylesheetsMutex);
    auto it = g_stylesheets.find(stylesheetPath);
    if (it != g_stylesheets.end() && !ec && it->second.writeTime == writeTime)
    {
        t_stylesheet = it->second.sheet;
        return true;
    }

    xsltStylesheetPtr sheet = xsltParseStylesheetFile(reinterpret_cast<const xmlChar*>(stylesheetPath.c_str()));
    if (sheet == nullptr)
    {
        t_stylesheet = nullptr;
        return false;
    }

    // A replaced sheet may still be applied by another thread: it is not freed.
    // This only happens when the .xsl file is edited while Scilab runs.
    g_stylesheets[stylesheetPath] = {sheet, writeTime};
    t_stylesheet = sheet;
    return true;
}

bool loadLinksFile(const std::string& sciPath, const std::string& lang, types::Struct*& out, std::string& error)
{
    std::string linksPath = buildInlineFilePath(sciPath, lang, "links.json");
    std::ifstream file(linksPath, std::ios::binary);
    if (!file)
    {
        error = linksPath;
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    types::InternalType* f = fromJSON(content);
    out = f->getAs<types::Struct>();
    return true;
}

bool ensureLinks(const std::string& sciPath, const std::string& lang, types::Struct*& mapPtr, std::string& error)
{
    auto it = g_linksByLanguage.find(lang);
    if (it != g_linksByLanguage.end())
    {
        mapPtr = it->second;
        return true;
    }

    types::Struct* loaded;
    if (!loadLinksFile(sciPath, lang, loaded, error))
    {
        return false;
    }

    auto result = g_linksByLanguage.emplace(lang, std::move(loaded));
    mapPtr = result.first->second;
    return true;
}

bool resolveXmlPath(const std::string& sciPath, const std::string& lang, const std::wstring& page, std::string& xmlPath, std::string& error, bool* isScilab)
{
    types::Struct* links = nullptr;
    if (ensureLinks(sciPath, lang, links, error))
    {
        if (links->exists(page))
        {
            char* p = wide_string_to_UTF8(links->get(0)->get(page)->getAs<types::String>()->get(0));
            xmlPath = buildAbsolutePath(sciPath, p, isScilab);
            FREE(p);
            return true;
        }
    }

    // fallback default language
    if(lang != "en_US")
    {
        error.clear();
        return resolveXmlPath(sciPath, "en_US", page, xmlPath, error, isScilab);
    }

    return false;
}

bool transformXmlToText(const std::string& xmlPath, std::string& output, std::string& error, bool withLinks)
{
    xsltStylesheetPtr sheet = t_stylesheet;
    if (sheet == nullptr)
    {
        error = xmlPath;
        return false;
    }

    xmlDocPtr xmlDoc = xmlParseFile(xmlPath.c_str());
    if (!xmlDoc)
    {
        error = xmlPath;
        return false;
    }

    // "with-links" is an XPath string literal, not a raw value: quote it.
    const char* params[] = {"with-links", withLinks ? "'1'" : "'0'", nullptr};
    xmlDocPtr resultDoc = xsltApplyStylesheet(sheet, xmlDoc, params);
    if (!resultDoc)
    {
        xmlFreeDoc(xmlDoc);
        error = xmlPath;
        return false;
    }

    xmlChar* buffer = nullptr;
    int bufferLength = 0;
    if (xsltSaveResultToString(&buffer, &bufferLength, resultDoc, sheet) < 0)
    {
        xmlFreeDoc(resultDoc);
        xmlFreeDoc(xmlDoc);
        if (buffer)
        {
            xmlFree(buffer);
        }
        error = xmlPath;
        return false;
    }

    output.assign(reinterpret_cast<char*>(buffer), static_cast<size_t>(bufferLength));
    xmlFree(buffer);
    xmlFreeDoc(resultDoc);
    xmlFreeDoc(xmlDoc);

    return true;
}

std::string trimTrailingSeparators(std::string path)
{
    while (!path.empty() && (path.back() == '/' || path.back() == '\\'))
    {
        path.pop_back();
    }
    return path;
}

std::string toLowerAscii(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
                   { return static_cast<char>(std::tolower(c)); });
    return value;
}

std::string replaceRoot(const std::string& path, const std::string& sciRoot, const std::string& replace)
{
    if (sciRoot.empty())
    {
        return path;
    }

    std::string normalizedPath = normalizeSeparators(path);
    std::string normalizedRoot = normalizeSeparators(sciRoot);

    normalizedRoot = trimTrailingSeparators(normalizedRoot);
    normalizedPath = trimTrailingSeparators(normalizedPath);

    std::string pathLower = toLowerAscii(normalizedPath);
    std::string rootLower = toLowerAscii(normalizedRoot);

    if (pathLower.rfind(rootLower, 0) == 0)
    {
        std::string remainder = normalizedPath.substr(normalizedRoot.length());
        if (!remainder.empty() && remainder.front() == '/')
        {
            remainder.erase(0, 1);
        }
        if (remainder.empty())
        {
            return replace;
        }
        return replace + "/" + remainder;
    }

    return normalizedPath;
}

std::string replaceSciRoot(const std::string& path, const std::string& sciRoot)
{
    return replaceRoot(path, sciRoot, "SCI");
}

std::string replaceToolboxRoot(const std::string& path, const std::string& sciRoot)
{
    return replaceRoot(path, sciRoot, "TBX");
}

std::string escapeJson(const std::string& value)
{
    std::string escaped;
    escaped.reserve(value.size() + 16);
    for (unsigned char c : value)
    {
        switch (c)
        {
            case '"':
                escaped.append("\\\"");
                break;
            case '\\':
                escaped.append("\\\\");
                break;
            case '\b':
                escaped.append("\\b");
                break;
            case '\f':
                escaped.append("\\f");
                break;
            case '\n':
                escaped.append("\\n");
                break;
            case '\r':
                escaped.append("\\r");
                break;
            case '\t':
                escaped.append("\\t");
                break;
            default:
            {
                if (c < 0x20)
                {
                    char buffer[7];
                    std::snprintf(buffer, sizeof(buffer), "\\u%04x", c);
                    escaped.append(buffer);
                }
                else
                {
                    escaped.push_back(static_cast<char>(c));
                }
                break;
            }
        }
    }
    return escaped;
}

void collectXmlFromStruct(types::Struct* st, std::vector<std::wstring>& xmlPaths);

void collectXmlFromNode(types::SingleStruct* node, std::vector<std::wstring>& xmlPaths)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->exists(L"xml_list"))
    {
        types::InternalType* xmlListIT = node->get(L"xml_list");
        if (xmlListIT != nullptr && xmlListIT->isString())
        {
            types::String* xmlList = xmlListIT->getAs<types::String>();
            if (xmlList != nullptr && xmlList->getSize() > 0)
            {
                int rows = xmlList->getRows();
                int cols = xmlList->getCols();
                int columnIndex = cols > 1 ? 1 : 0;
                if (cols > 0)
                {
                    for (int r = 0; r < rows; ++r)
                    {
                        const wchar_t* wpath = xmlList->get(r, columnIndex);
                        if (wpath != nullptr && wpath[0] != L'\0')
                        {
                            xmlPaths.emplace_back(wpath);
                        }
                    }
                }
            }
        }
    }

    auto& fields = node->getFields();
    for (const auto& entry : fields)
    {
        const std::wstring& name = entry.first;
        if (name.rfind(L"dir_", 0) == 0)
        {
            types::InternalType* child = node->get(name);
            if (child != nullptr && child->isStruct())
            {
                collectXmlFromStruct(child->getAs<types::Struct>(), xmlPaths);
            }
        }
    }
}

void collectXmlFromStruct(types::Struct* st, std::vector<std::wstring>& xmlPaths)
{
    if (st == nullptr)
    {
        return;
    }

    int size = st->getSize();
    for (int i = 0; i < size; ++i)
    {
        types::SingleStruct* node = st->get(i);
        collectXmlFromNode(node, xmlPaths);
    }
}

bool computeToolboxBase(const std::filesystem::path& modulePath, std::filesystem::path& toolboxBase)
{
    toolboxBase = std::filesystem::path();
    std::error_code ec;
    std::filesystem::path parent = std::filesystem::weakly_canonical(modulePath, ec);
    if (ec)
    {
        return false;
    }

    toolboxBase = parent;
    for (int i = 0; i < 2; ++i)
    {
        toolboxBase = toolboxBase.parent_path();
        if (toolboxBase.empty())
        {
            return false;
        }
    }
    return true;
}

bool writeJsonFile(const std::filesystem::path& filePath, const std::map<std::string, std::string>& links)
{
    std::ofstream stream(filePath, std::ios::binary | std::ios::trunc);
    if (!stream.good())
    {
        return false;
    }

    stream << "{";
    if (!links.empty())
    {
        stream << '\n';
        bool first = true;
        for (const auto& entry : links)
        {
            if (!first)
            {
                stream << ",\n";
            }
            first = false;
            stream << "  \"" << escapeJson(entry.first) << "\": \"" << escapeJson(entry.second) << "\"";
        }
        stream << '\n';
    }
    stream << "}\n";
    return stream.good();
}

int generate_inline_links(const std::wstring& lang, const std::wstring& path)
{
    std::string sci = scilab::UTF8::toUTF8(ConfigVariable::getSCIPath());
    std::filesystem::path sciPath(sci);
    std::filesystem::path modulePath(path);

    bool isMainModule = normalizeSeparators(scilab::UTF8::toUTF8(path)) == normalizeSeparators(sci);

    std::filesystem::path outputPath;
    std::filesystem::path toolboxBase;
    std::vector<std::wstring> xmlPaths;

    if (isMainModule)
    {
        outputPath = sciPath / "modules" / "core" / "inline" / lang;
        xmlPaths = listModuleXmlForLanguage(scilab::UTF8::toUTF8(lang));
    }
    else
    {
        if (!computeToolboxBase(modulePath, toolboxBase))
        {
            // Scierror(999, _("%s: Cannot determine toolbox location from path '%s'.\n"), fname, scilab::UTF8::toUTF8(path).c_str());
            return -1;
        }

        outputPath = toolboxBase / "inline" / lang;
        xmlPaths = listToolboxXmlForLanguage(scilab::UTF8::toUTF8(lang), toolboxBase);
    }

    std::string sciUtf8 = normalizeSeparators(sci);
    std::string displayPath = replaceSciRoot(normalizeSeparators(outputPath.string()), sciUtf8);
    sciprint(_("\nBuilding the links file [%s] in %s.\n"), "inline", displayPath.c_str());

    std::map<std::string, std::string> links;
    for (const std::wstring& xmlPathW : xmlPaths)
    {
        std::string xmlUtf8 = scilab::UTF8::toUTF8(xmlPathW);
        std::string normalizedXmlPath = normalizeSeparators(xmlUtf8);

        xmlDocPtr doc = xmlParseFile(normalizedXmlPath.c_str());
        if (doc == nullptr)
        {
            // Scierror(999, _("%s: Unable to read XML file: %s.\n"), fname, xmlUtf8.c_str());
            return -2;
        }

        xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
        if (ctx == nullptr)
        {
            xmlFreeDoc(doc);
            // Scierror(999, _("%s: Unable to create XPath context for %s.\n"), fname, xmlUtf8.c_str());
            return -3;
        }

        xmlXPathRegisterNs(ctx, BAD_CAST "xml", BAD_CAST "http://www.w3.org/XML/1998/namespace");
        xmlXPathObjectPtr xp = xmlXPathEvalExpression(BAD_CAST "//@xml:id", ctx);

        if (xp == nullptr)
        {
            xmlXPathFreeContext(ctx);
            xmlFreeDoc(doc);
            // Scierror(999, _("%s: Unable to evaluate XPath on %s.\n"), fname, xmlUtf8.c_str());
            return -4;
        }

        xmlNodeSetPtr nodes = xp->nodesetval;
        if (nodes != nullptr)
        {
            for (int i = 0; i < nodes->nodeNr; ++i)
            {
                xmlAttrPtr attr = reinterpret_cast<xmlAttrPtr>(nodes->nodeTab[i]);
                if (attr == nullptr)
                {
                    continue;
                }

                xmlChar* value = xmlNodeListGetString(doc, attr->children, 1);
                if (value != nullptr)
                {
                    std::string id(reinterpret_cast<char*>(value));
                    xmlFree(value);

                    std::string storedPath;
                    if (isMainModule)
                    {
                        storedPath = replaceSciRoot(normalizedXmlPath, sciUtf8);
                    }
                    else
                    {
                        storedPath = replaceToolboxRoot(normalizedXmlPath, toolboxBase.string());
                    }
                    links[id] = storedPath;
                }
            }
        }

        xmlXPathFreeObject(xp);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
    }

    std::error_code mkdirEc;
    std::filesystem::create_directories(outputPath, mkdirEc);
    if (mkdirEc)
    {
        // Scierror(999, _("%s: Cannot create directory: %s.\n"), fname, scilab::UTF8::toUTF8(outputPath.wstring()).c_str());
        return -5;
    }

    std::filesystem::path linksPath = outputPath / "links.json";
    if (!writeJsonFile(linksPath, links))
    {
        // Scierror(999, _("%s: Cannot write file: %s.\n"), fname, scilab::UTF8::toUTF8(linksPath.wstring()).c_str());
        return -6;
    }

    return 0;
}

void clearInlineHelpLinks()
{
    std::lock_guard<std::mutex> lock(g_linksMutex);
    for (auto&& s : g_linksByLanguage)
    {
        s.second->killMe();
    }

    g_linksByLanguage.clear();
}

int inlineHelp(const std::wstring& key, std::wstring& content, bool* isScilab, bool withLinks)
{
    wchar_t* langW = getlanguage();
    if (langW == nullptr)
    {
        return -1;
    }

    char* langC = wide_string_to_UTF8(langW);
    FREE(langW);

    if (langC == nullptr)
    {
        //Scierror(999, _("%s: No more memory.\n"), "help");
        return -2;
    }

    std::string language(langC);
    FREE(langC);

    std::string sciPath = getSciPathUtf8();
    std::string xmlPath;
    std::string lookupError;
    bool resolved = false;
    {
        std::lock_guard<std::mutex> lock(g_linksMutex);
        resolved = resolveXmlPath(sciPath, language, key, xmlPath, lookupError, isScilab);
    }

    if (!resolved)
    {
        if (!lookupError.empty())
        {
            return -4;
            //Scierror(999, _("%s: Cannot read inline help index: %s\n"), "help", lookupError.c_str());
        }
        else
        {
            return -5;
            //Scierror(999, _("%s: Unknown help page: %s.\n"), "help", page.c_str());
        }
    }

    std::string c;
    std::string transformError;
    if (!transformXmlToText(xmlPath, c, transformError, withLinks))
    {
        //Scierror(999, _("%s: Cannot transform help page: %s\n"), "help", transformError.c_str());
        return -6;
    }

    wchar_t* w = to_wide_string(c.data());
    content = w;
    FREE(w);
    return 0;
}

int loadToolboxHelp(const std::wstring& path)
{
    std::filesystem::path inlinePath = std::filesystem::path(path) / L"inline";
    std::error_code ec;
    if (!std::filesystem::is_directory(inlinePath, ec))
    {
        return 0;
    }

    const std::string sciPath = getSciPathUtf8();
    bool hadError = false;
    std::lock_guard<std::mutex> lock(g_linksMutex);
    std::string toolboxRoot = trimTrailingSeparators(normalizeSeparators(scilab::UTF8::toUTF8(path)));

    std::filesystem::directory_iterator end;
    for (std::filesystem::directory_iterator langIt(inlinePath, ec); !ec && langIt != end; langIt.increment(ec))
    {
        if (!langIt->is_directory())
        {
            continue;
        }

        std::wstring langW = langIt->path().filename().wstring();
        if (langW.empty())
        {
            continue;
        }

        std::string lang = scilab::UTF8::toUTF8(langW);
        types::Struct* targetStruct = nullptr;

        auto existing = g_linksByLanguage.find(lang);
        if (existing != g_linksByLanguage.end())
        {
            targetStruct = existing->second;
        }
        else
        {
            std::string loadError;
            if (!loadLinksFile(sciPath, lang, targetStruct, loadError))
            {
                targetStruct = new types::Struct(1, 1);
            }

            auto insertResult = g_linksByLanguage.emplace(lang, targetStruct);
            if (!insertResult.second)
            {
                if (insertResult.first->second != targetStruct)
                {
                    targetStruct->killMe();
                }
                targetStruct = insertResult.first->second;
            }
        }

        if (targetStruct == nullptr)
        {
            hadError = true;
            continue;
        }

        if (targetStruct->getSize() == 0)
        {
            types::Struct* resized = targetStruct->resize(1, 1);
            if (resized == nullptr)
            {
                hadError = true;
                continue;
            }
            targetStruct = resized;
            g_linksByLanguage[lang] = targetStruct;
        }

        types::SingleStruct* targetNode = targetStruct->get(0);
        if (targetNode == nullptr)
        {
            hadError = true;
            continue;
        }

        std::filesystem::path linksPath = langIt->path() / L"links.json";
        std::ifstream stream(linksPath, std::ios::binary);
        if (!stream.good())
        {
            hadError = true;
            continue;
        }

        std::string content((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        if (content.empty())
        {
            continue;
        }

        types::InternalType* parsed = fromJSON(content);
        if (parsed == nullptr || !parsed->isStruct())
        {
            if (parsed != nullptr)
            {
                parsed->killMe();
            }
            hadError = true;
            continue;
        }

        types::Struct* toolboxStruct = parsed->getAs<types::Struct>();
        if (toolboxStruct == nullptr || toolboxStruct->getSize() == 0)
        {
            if (toolboxStruct != nullptr)
            {
                toolboxStruct->killMe();
            }
            continue;
        }

        types::SingleStruct* toolboxNode = toolboxStruct->get(0);
        if (toolboxNode == nullptr)
        {
            toolboxStruct->killMe();
            hadError = true;
            continue;
        }

        auto& sourceFields = toolboxNode->getFields();
        auto& sourceData = toolboxNode->getData();
        for (const auto& field : sourceFields)
        {
            const int index = field.second;
            if (index < 0 || static_cast<size_t>(index) >= sourceData.size())
            {
                continue;
            }

            types::InternalType* value = sourceData[index];
            if (value == nullptr)
            {
                continue;
            }

            if (!toolboxRoot.empty() && value->isString())
            {
                types::String* strValue = value->getAs<types::String>();
                if (strValue != nullptr)
                {
                    int total = strValue->getSize();
                    for (int idx = 0; idx < total; ++idx)
                    {
                        const wchar_t* current = strValue->get(idx);
                        if (current == nullptr)
                        {
                            continue;
                        }

                        std::string entry = scilab::UTF8::toUTF8(current);
                        if (entry.rfind("TBX", 0) != 0)
                        {
                            continue;
                        }

                        std::string suffix = entry.substr(3);
                        if (!suffix.empty() && (suffix[0] == '/' || suffix[0] == '\\'))
                        {
                            suffix.erase(0, 1);
                        }

                        std::string replaced = toolboxRoot;
                        if (!suffix.empty())
                        {
                            replaced.push_back('/');
                            replaced += suffix;
                        }

                        strValue->set(idx, replaced.c_str());
                    }
                }
            }

            if (!targetNode->exists(field.first))
            {
                targetNode->addField(field.first);
            }

            targetNode->set(field.first, value);
        }

        toolboxStruct->killMe();
    }

    if (ec)
    {
        hadError = true;
    }

    return hadError ? -1 : 0;
}

extern "C"
{
int getInlineHelp(const wchar_t* key, wchar_t** content, BOOL* isScilab)
{
    if (key == nullptr || content == nullptr)
    {
        return -1;
    }

    *content = nullptr;

    std::wstring result;
    bool scilab = false;
    int ret = inlineHelp(key, result, isScilab ? &scilab : nullptr);
    if (ret != 0)
    {
        return ret;
    }

    size_t len = result.size() + 1;
    wchar_t* buffer = static_cast<wchar_t*>(MALLOC(len * sizeof(wchar_t)));
    if (buffer == nullptr)
    {
        return -2;
    }

    memcpy(buffer, result.c_str(), len * sizeof(wchar_t));
    *content = buffer;

    if (isScilab != nullptr)
    {
        *isScilab = scilab ? TRUE : FALSE;
    }

    return 0;
}

BOOL loadInlineHelpStyleSheet(const char* sheetPath)
{
    if (sheetPath == nullptr)
    {
        return FALSE;
    }

    return loadStyleSheet(sheetPath) ? TRUE : FALSE;
}
}
