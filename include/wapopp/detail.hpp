#pragma once

#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <variant>

#include <nlohmann/json.hpp>

#include "wapopp.hpp"

namespace wapopp {
namespace detail {

    // Due to AppleClang's lack of support
    template <class T, class... Types>
    [[nodiscard]] auto take(std::variant<Types...> &&v) -> T &&
    {
        if (auto ptr = std::get_if<T>(&v); ptr != nullptr) {
            return std::move(*ptr);
        } else {
            throw std::invalid_argument("Couldn't access required type");
        }
    }

    // Due to AppleClang's lack of support
    template <class T, class... Types>
    [[nodiscard]] auto holds(std::variant<Types...> const &v) -> bool
    {
        return std::get_if<T>(&v) != nullptr;
    }

    template <class T>
    [[nodiscard]] auto read_field_or(nlohmann::json const &node,
                                     std::string const &field,
                                     T default_value) -> T
    {
        if (auto pos = node.find(field); pos != node.end()) {
            try {
                return pos->get<T>();
            } catch (std::exception const &) {
                return default_value;
            }
        } else {
            return default_value;
        }
    }

    template <class T>
    [[nodiscard]] auto read_mandatory_field(nlohmann::json const &node, std::string const &field)
        -> std::variant<T, Error>
    {
        if (auto pos = node.find(field); pos != node.end()) {
            try {
                return pos->get<T>();
            } catch (std::exception const &) {
                std::ostringstream os;
                os << "Failed to parse " << field << " (" << *pos << ") to a requested type";
                return Error{os.str(), node.dump()};
            }
        } else {
            return Error{std::string("Missing ") + field, node.dump()};
        }
    }

    template <class T>
    [[nodiscard]] auto read_simple_content(nlohmann::json const &node) -> std::variant<T, Error>
    {
        auto content = read_mandatory_field<std::string>(node, "content");
        if (holds<Error>(content)) {
            return take<Error>(std::move(content));
        }
        auto mime = read_mandatory_field<std::string>(node, "mime");
        if (holds<Error>(mime)) {
            return take<Error>(std::move(mime));
        }
        return T{take<std::string>(std::move(content)), take<std::string>(std::move(mime))};
    }

    [[nodiscard]] auto read_date(nlohmann::json const &node) -> std::variant<Date, Error>
    {
        auto date = read_mandatory_field<std::uint64_t>(node, "content");
        if (holds<Error>(date)) {
            return take<Error>(std::move(date));
        }
        return Date{take<std::uint64_t>(std::move(date))};
    }

    [[nodiscard]] auto read_author_info(nlohmann::json const &node)
        -> std::variant<AuthorInfo, Error>
    {
        auto role = read_mandatory_field<std::string>(node, "role");
        if (holds<Error>(role)) {
            return take<Error>(std::move(role));
        }
        auto name = read_mandatory_field<std::string>(node, "name");
        if (holds<Error>(name)) {
            return take<Error>(std::move(name));
        }
        auto bio = read_mandatory_field<std::string>(node, "bio");
        if (holds<Error>(bio)) {
            return take<Error>(std::move(bio));
        }
        return AuthorInfo{take<std::string>(std::move(role)),
                          take<std::string>(std::move(name)),
                          take<std::string>(std::move(bio))};
    }

    [[nodiscard]] auto read_image(nlohmann::json const &node)
        -> std::variant<Image, Error>
    {
        auto caption = read_mandatory_field<std::string>(node, "fullcaption");
        if (holds<Error>(caption)) {
            return take<Error>(std::move(caption));
        }
        auto blurb = read_mandatory_field<std::string>(node, "blurb");
        if (holds<Error>(blurb)) {
            return take<Error>(std::move(blurb));
        }
        auto url = read_mandatory_field<std::string>(node, "imageURL");
        if (holds<Error>(url)) {
            return take<Error>(std::move(url));
        }
        auto height = read_mandatory_field<int>(node, "imageHeight");
        if (holds<Error>(height)) {
            return take<Error>(std::move(height));
        }
        auto width = read_mandatory_field<int>(node, "imageWidth");
        if (holds<Error>(width)) {
            return take<Error>(std::move(width));
        }
        return Image{take<std::string>(std::move(caption)),
                     take<std::string>(std::move(blurb)),
                     take<std::string>(std::move(url)),
                     take<int>(std::move(height)),
                     take<int>(std::move(width))};
    }

} // namespace detail

} // namespace wapopp
