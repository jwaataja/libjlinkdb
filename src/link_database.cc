/*
 * Copyright (c) 2019 Jason Waataja
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "link_database.hh"

#include <libxml++/libxml++.h>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

using std::shared_ptr;
using std::string;
using std::vector;

namespace libjlinkdb {

LinkDatabase::LinkDatabase() : highest_id_(0)
{
}

LinkDatabase::LinkDatabase(std::istream& reader) : LinkDatabase{}
{
    xmlpp::DomParser parser;
    // TODO(jason): Handle exceptions for next two lines.
    parser.parse_stream(reader);
    set_from_document(*parser.get_document());
}

LinkDatabase::LinkDatabase(const string& path) : LinkDatabase{}
{
    xmlpp::DomParser parser{ path };
    set_from_document(*parser.get_document());
}

LinkDatabase::LinkEntryIterator
LinkDatabase::links_begin()
{
    return links_.begin();
}

LinkDatabase::LinkEntryIterator
LinkDatabase::links_end()
{
    return links_.end();
}

LinkDatabase::ConstLinkEntryIterator
LinkDatabase::links_cbegin() const
{
    return links_.cbegin();
}

LinkDatabase::ConstLinkEntryIterator
LinkDatabase::links_cend() const
{
    return links_.cend();
}

std::size_t
LinkDatabase::links_count() const
{
    return links_.size();
}

bool
LinkDatabase::has_entry(int id) const
{
    return links_.find(id) != links_.end();
}

shared_ptr<LinkEntry>
LinkDatabase::get_entry(int id) const
{
    auto position = links_.find(id);
    if (position != links_.end())
        return position->second;
    else
        return {};
}

int
LinkDatabase::add_entry(shared_ptr<LinkEntry> entry)
{
    int id = highest_id_;
    links_[id] = entry;
    ++highest_id_;
    return id;
}

void
LinkDatabase::delete_entry(int id)
{
    links_.erase(id);
}

vector<std::pair<int, shared_ptr<LinkEntry>>>
LinkDatabase::query(const Query& query) const
{
    vector<std::pair<int, shared_ptr<LinkEntry>>> result;
    std::copy_if(links_.begin(), links_.end(), std::back_inserter(result),
        [&query](const std::pair<int, shared_ptr<LinkEntry>>& entry) {
            return query.matches(*entry.second.get());
        });
    return result;
}

void
LinkDatabase::write_to_stream(std::ostream& writer) const
{
    xmlpp::Document document;
    auto root = document.create_root_node("links");
    for (const auto& entry : links_) {
        auto child = root->add_child_element("link");
        add_text_child_if_nonempty(
            child, "location", entry.second->location());
        add_text_child_if_nonempty(child, "name", entry.second->name());
        add_text_child_if_nonempty(
            child, "description", entry.second->description());
        for (const auto& tag : entry.second->tags())
            add_text_child(child, "tag", tag);

        for (const auto& attribute : entry.second->attributes()) {
            auto attribute_element = child->add_child_element("attribute");
            attribute_element->set_attribute("name", attribute.first);
            attribute_element->add_child_text(attribute.second);
        }
    }
    document.write_to_stream(writer);
}

void
LinkDatabase::write_to_file(const string& path) const
{
    std::ofstream writer{ path };

    // TODO(jason): Error handling here.

    write_to_stream(writer);
}

void
LinkDatabase::set_from_document(const xmlpp::Document& document)
{
    const xmlpp::Element* root = document.get_root_node();
    if (root->get_name() != "links") {
        // throw exception
    }

    for (const auto& child : root->get_children()) {
        if (dynamic_cast<const xmlpp::CommentNode*>(child) != nullptr)
            continue;

        if (child->get_name() != "link") {
            // throw exception
        }

        const xmlpp::Element* element =
            dynamic_cast<const xmlpp::Element*>(child);
        if (element != nullptr) {
            // throw exception
        }

        parse_link_node(element);
    }
}

void
LinkDatabase::parse_link_node(const xmlpp::Element* node)
{
    auto entry = std::make_shared<LinkEntry>();
    for (const auto& child : node->get_children())
        parse_link_node_child(child, entry);

    add_entry(entry);
}

void
LinkDatabase::parse_link_node_child(
    const xmlpp::Node* child, shared_ptr<LinkEntry> entry) const
{
    if (dynamic_cast<const xmlpp::CommentNode*>(child) != nullptr)
        return;

    if (child->get_name() == "location")
        entry->set_location(node_text(child));
    else if (child->get_name() == "name")
        entry->set_name(node_text(child));
    else if (child->get_name() == "description")
        entry->set_description(node_text(child));
    else if (child->get_name() == "tag")
        entry->add_tag(node_text(child));
    else if (child->get_name() == "attribute") {
        auto element = dynamic_cast<const xmlpp::Element*>(child);
        if (element == nullptr) {
            // throw exception
        }

        auto attribute = element->get_attribute("name");
        if (attribute == nullptr) {
            // throw exception
        }

        entry->set_attribute(attribute->get_value(), node_text(child));
    }
}

string
LinkDatabase::node_text(const xmlpp::Node* node) const
{
    auto element = dynamic_cast<const xmlpp::Element*>(node);
    if (element == nullptr) {
        // throw exception
    }

    auto text = element->get_first_child_text();
    if (text == nullptr) {
        // throw exception
    }

    return text->get_content();
}

void
LinkDatabase::add_text_child(xmlpp::Element* element, const std::string& name,
    const std::string& content) const
{
    auto container = element->add_child_element(name);
    container->add_child_text(content);
}

void
LinkDatabase::add_text_child_if_nonempty(xmlpp::Element* element,
    const std::string& name, const std::string& content) const
{
    if (!content.empty())
        add_text_child(element, name, content);
}

} // namespace libjlinkdb
