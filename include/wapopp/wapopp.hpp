#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <variant>

namespace wapopp {

struct Error {
    std::string msg{};
    std::string json{};
};
struct Record;
using Result = std::variant<Record, Error>;

struct SimpleContent {
    std::string content;
    std::string mime;
};
struct Kicker : public SimpleContent {
};
struct Title : public SimpleContent {
};
struct Byline : public SimpleContent {
};
struct Text : public SimpleContent {
};
struct Date {
    std::uint64_t timestamp;
};
struct AuthorInfo {
    std::string role;
    std::string name;
    std::string bio;
};
struct Image {
    std::string caption;
    std::string blurb;
    std::string url;
    int height;
    int width;
};
using Content = std::variant<Kicker, Title, Byline, Text, Date, AuthorInfo, Image>;

struct Record {
    std::string id;
    std::string url;
    std::string title;
    std::string author;
    std::string type;
    std::string source;
    std::uint64_t published;
    std::vector<Content> contents;
    [[nodiscard]] static auto read(std::istream &is) -> Result;
};

} // namespace wapopp
