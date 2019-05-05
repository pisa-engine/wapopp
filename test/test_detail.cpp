#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "wapopp/detail.hpp"

using namespace wapopp;
using namespace wapopp::detail;

TEST_CASE("Read simple content", "[unit]")
{
    SECTION("Missing content")
    {
        auto j = R"({
            "mime": "text/plain",
            "type": "kicker"
        })"_json;
        auto content = read_simple_content<Kicker>(j);
        REQUIRE(std::get_if<Error>(&content) != nullptr);
        REQUIRE(std::get_if<Error>(&content)->msg == "Missing content");
    }
    SECTION("Missing mime")
    {
        auto j = R"({
            "content": "Colleges",
            "type": "kicker"
        })"_json;
        auto content = read_simple_content<Kicker>(j);
        REQUIRE(std::get_if<Error>(&content) != nullptr);
        REQUIRE(std::get_if<Error>(&content)->msg == "Missing mime");
    }
    SECTION("Kicker")
    {
        auto j = R"({
            "content": "Colleges",
            "mime": "text/plain",
            "type": "kicker"
        })"_json;
        auto content = read_simple_content<Kicker>(j);
        REQUIRE(std::get_if<Kicker>(&content) != nullptr);
        REQUIRE(std::get_if<Kicker>(&content)->content == j["content"]);
        REQUIRE(std::get_if<Kicker>(&content)->mime == j["mime"]);
    }
    SECTION("Title")
    {
        auto j = R"({
            "content": "Danny Coale, Jarrett Boykin are a perfect 1-2 punch for Virginia Tech",
            "mime": "text/plain",
            "type": "title"
        })"_json;
        auto content = read_simple_content<Title>(j);
        REQUIRE(std::get_if<Title>(&content) != nullptr);
        REQUIRE(std::get_if<Title>(&content)->content == j["content"]);
        REQUIRE(std::get_if<Title>(&content)->mime == j["mime"]);
    }
    SECTION("Byline")
    {
        auto j = R"({
            "content": "By Mark Giannotto",
            "mime": "text/plain",
            "type": "title"
        })"_json;
        auto content = read_simple_content<Byline>(j);
        REQUIRE(std::get_if<Byline>(&content) != nullptr);
        REQUIRE(std::get_if<Byline>(&content)->content == j["content"]);
        REQUIRE(std::get_if<Byline>(&content)->mime == j["mime"]);
    }
    SECTION("Text")
    {
        auto j = R"({
            "content": "<span>paragraph</span>",
            "subtype":"paragraph",
            "mime": "text/html",
            "type": "sanitized_html"
        })"_json;
        auto content = read_simple_content<Text>(j);
        REQUIRE(std::get_if<Text>(&content) != nullptr);
        REQUIRE(std::get_if<Text>(&content)->content == j["content"]);
        REQUIRE(std::get_if<Text>(&content)->mime == j["mime"]);
    }
}

TEST_CASE("Read date content", "[unit]")
{
    SECTION("Missing date field")
    {
        auto j = R"({
            "mime": "text/plain",
            "type": "date"
        })"_json;
        auto content = read_date(j);
        REQUIRE(std::get_if<Error>(&content) != nullptr);
        REQUIRE(std::get_if<Error>(&content)->msg == "Missing content");
    }
    SECTION("Correct date")
    {
        auto j = R"({
            "mime": "text/plain",
            "content": 1325376562000,
            "type": "date"
        })"_json;
        auto content = read_date(j);
        REQUIRE(std::get_if<Date>(&content) != nullptr);
        REQUIRE(std::get_if<Date>(&content)->timestamp == j["content"]);
    }
    SECTION("Corrupted date")
    {
        auto j = R"({
            "mime": "text/plain",
            "content": "oops",
            "type": "date"
        })"_json;
        auto content = read_date(j);
        REQUIRE(std::get_if<Error>(&content) != nullptr);
        REQUIRE(std::get_if<Error>(&content)->msg ==
                "Failed to parse content (\"oops\") to a requested type");
    }
}

TEST_CASE("Read author info", "[unit]")
{
    SECTION("Correct author info")
    {
        auto j = R"({
            "role":"Reporter",
            "type":"author_info",
            "name":"Mark Giannotto",
            "bio":"Mark Giannotto is a Montgomery County native ..."
        })"_json;
        auto content = read_author_info(j);
        REQUIRE(std::get_if<AuthorInfo>(&content) != nullptr);
        REQUIRE(std::get_if<AuthorInfo>(&content)->role == "Reporter");
        REQUIRE(std::get_if<AuthorInfo>(&content)->name == "Mark Giannotto");
        REQUIRE(std::get_if<AuthorInfo>(&content)->bio ==
                "Mark Giannotto is a Montgomery County native ...");
    }
    SECTION("Missing role")
    {
        auto j = R"({
            "type":"author_info",
            "name":"Mark Giannotto",
            "bio":"Mark Giannotto is a Montgomery County native ..."
        })"_json;
        auto content = read_author_info(j);
        REQUIRE(std::get_if<Error>(&content) != nullptr);
        REQUIRE(std::get_if<Error>(&content)->msg == "Missing role");
    }
    SECTION("Missing name")
    {
        auto j = R"({
            "role":"Reporter",
            "type":"author_info",
            "bio":"Mark Giannotto is a Montgomery County native ..."
        })"_json;
        auto content = read_author_info(j);
        REQUIRE(std::get_if<Error>(&content) != nullptr);
        REQUIRE(std::get_if<Error>(&content)->msg == "Missing name");
    }
    SECTION("Missing bio")
    {
        auto j = R"({
            "role": "Reporter",
            "type": "author_info",
            "name": "Mark Giannotto"
        })"_json;
        auto content = read_author_info(j);
        REQUIRE(std::get_if<Error>(&content) != nullptr);
        REQUIRE(std::get_if<Error>(&content)->msg == "Missing bio");
    }
}

TEST_CASE("Read image", "[unit]")
{
    SECTION("Correct image")
    {
        auto j = R"({
            "fullcaption": "Full Caption",
            "imageURL": "https://img.washingtonpost.com/somepic.jpg",
            "mime": "image/jpeg",
            "imageHeight": 2260,
            "imageWidth": 2775,
            "type": "image",
            "blurb": "Virginia Tech wide receiver Danny Coale..."
        })"_json;
        auto content = read_image(j);
        if (holds<Error>(content)) {
            std::cerr << take<Error>(std::move(content)).msg << std::endl;
            FAIL("");
        }
        REQUIRE(std::get_if<Image>(&content) != nullptr);
        REQUIRE(std::get_if<Image>(&content)->caption == "Full Caption");
        REQUIRE(std::get_if<Image>(&content)->url == "https://img.washingtonpost.com/somepic.jpg");
        REQUIRE(std::get_if<Image>(&content)->blurb ==
                "Virginia Tech wide receiver Danny Coale...");
        REQUIRE(std::get_if<Image>(&content)->height == 2260);
        REQUIRE(std::get_if<Image>(&content)->width == 2775);
    }
}
