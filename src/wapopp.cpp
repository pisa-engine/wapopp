#include <wapopp/detail.hpp>
#include <iostream>

using namespace wapopp;

template <class C, class Fn>
void append_content(nlohmann::json const &node, std::vector<Content> &contents, Fn read_content)
{
    auto content = read_content(node);
    if (detail::holds<C>(content)) {
        contents.push_back(detail::take<C>(std::move(content)));
    }
}

[[nodiscard]] auto Record::read(std::istream &is) -> Result
{
    std::string line;
    std::getline(is, line);
    try {
        nlohmann::json data = nlohmann::json::parse(line);
        std::vector<Content> contents;
        for (auto &&content_entry : data["contents"]) {
            if (auto type_pos = content_entry.find("type"); type_pos != content_entry.end()) {
                auto type = type_pos->get<std::string>();
                if (type == "kicker") {
                    append_content<Kicker>(
                        content_entry, contents, detail::read_simple_content<Kicker>);
                } else if (type == "title") {
                    append_content<Title>(
                        content_entry, contents, detail::read_simple_content<Title>);
                } else if (type == "byline") {
                    append_content<Byline>(
                        content_entry, contents, detail::read_simple_content<Byline>);
                } else if (type == "sanitized_html") {
                    append_content<Text>(
                        content_entry, contents, detail::read_simple_content<Text>);
                } else if (type == "date") {
                    append_content<Date>(content_entry, contents, detail::read_date);
                } else if (type == "author_info") {
                    append_content<AuthorInfo>(content_entry, contents, detail::read_author_info);
                } else if (type == "image") {
                    append_content<Image>(content_entry, contents, detail::read_image);
                }
            }
        }
        return Record{data["id"],
                      detail::read_field_or<std::string>(data, "article_url", ""),
                      detail::read_field_or<std::string>(data, "title", ""),
                      detail::read_field_or<std::string>(data, "author", ""),
                      detail::read_field_or<std::string>(data, "type", ""),
                      detail::read_field_or<std::string>(data, "source", ""),
                      detail::read_field_or<std::uint64_t>(data, "published_date", 0u),
                      std::move(contents)};
    } catch (nlohmann::detail::exception const& error) {
        return Error{error.what(), line};
    }
}
