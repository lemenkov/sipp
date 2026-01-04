/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  XML parsing helpers using pugixml
 */

#ifndef __XML_HELPERS_HPP__
#define __XML_HELPERS_HPP__

#include <pugixml.hpp>
#include <cstring>
#include <cstdlib>

// Forward declaration - ERROR is defined in sipp.hpp
extern void ERROR(const char *fmt, ...) __attribute__((noreturn));

namespace xml {

// Get required string attribute (caller must free)
inline char* get_string(pugi::xml_node node, const char* name, const char* what) {
    auto attr = node.attribute(name);
    if (!attr) {
        ERROR("%s is missing the required '%s' parameter.", what, name);
    }
    return strdup(attr.value());
}

// Get optional string attribute (returns nullptr if missing)
inline const char* get_value(pugi::xml_node node, const char* name) {
    auto attr = node.attribute(name);
    if (!attr) return nullptr;
    return attr.value();
}

// Get required long attribute
inline long get_long(pugi::xml_node node, const char* name, const char* what) {
    auto attr = node.attribute(name);
    if (!attr) {
        ERROR("%s is missing the required '%s' parameter.", what, name);
    }
    return attr.as_llong();
}

// Get optional long attribute with default
inline long get_long(pugi::xml_node node, const char* name, const char* what, long defval) {
    auto attr = node.attribute(name);
    if (!attr) return defval;
    return attr.as_llong(defval);
}

// Get required double attribute
inline double get_double(pugi::xml_node node, const char* name, const char* what) {
    auto attr = node.attribute(name);
    if (!attr) {
        ERROR("%s is missing the required '%s' parameter.", what, name);
    }
    return attr.as_double();
}

// Get optional double attribute with default
inline double get_double(pugi::xml_node node, const char* name, const char* what, double defval) {
    auto attr = node.attribute(name);
    if (!attr) return defval;
    return attr.as_double(defval);
}

// Get required bool attribute
inline bool get_bool(pugi::xml_node node, const char* name, const char* what) {
    auto attr = node.attribute(name);
    if (!attr) {
        ERROR("%s is missing the required '%s' parameter.", what, name);
    }
    const char* val = attr.value();
    if (!strcasecmp(val, "true")) return true;
    if (!strcasecmp(val, "false")) return false;
    return attr.as_bool();
}

// Get optional bool attribute with default
inline bool get_bool(pugi::xml_node node, const char* name, const char* what, bool defval) {
    auto attr = node.attribute(name);
    if (!attr) return defval;
    const char* val = attr.value();
    if (!strcasecmp(val, "true")) return true;
    if (!strcasecmp(val, "false")) return false;
    return attr.as_bool(defval);
}

// Get CDATA content from element (returns nullptr if not found)
inline const char* get_cdata(pugi::xml_node node) {
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
        if (child.type() == pugi::node_cdata) {
            return child.value();
        }
    }
    return nullptr;
}

// Get text content (for elements with just text, not CDATA)
inline const char* get_text(pugi::xml_node node) {
    return node.child_value();
}

// Convert byte offset to line number for error reporting
inline int offset_to_line(const char* content, ptrdiff_t offset) {
    int line = 1;
    for (ptrdiff_t i = 0; i < offset && content[i]; ++i) {
        if (content[i] == '\n') {
            line++;
        }
    }
    return line;
}

} // namespace xml

#endif // __XML_HELPERS_HPP__
