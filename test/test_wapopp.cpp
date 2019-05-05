#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <nlohmann/json.hpp>

#include "wapopp/wapopp.hpp"

using namespace wapopp;

TEST_CASE("Read record", "[unit]")
{
    auto r =
        R"({"id": "b2e89334-33f9-11e1-825f-dabc29fd7071",)"
        R"("article_url": "https://www.washingtonpost.com/truncated.html",)"
        R"("title": "Danny Coale, Jarrett Boykin are a perfect 1-2 punch for Virginia Tech",)"
        R"("author": null,)"
        R"("published_date": 1325376562000,)"
        R"("contents": [)"
        R"({)"
        R"(    "content":"Colleges",)"
        R"(    "mime":"text/plain",)"
        R"(    "type":"kicker")"
        R"(},)"
        R"({)"
        R"(    "content":"Danny Coale, Jarrett Boykin are a perfect 1-2 punch for Virginia Tech",)"
        R"(    "mime":"text/plain",)"
        R"(    "type":"title")"
        R"(},)"
        R"({)"
        R"(    "fullcaption":"Virginia Tech wide receiver Danny Coale",)"
        R"(    "imageURL":"https://img.washingtonpost.com/somepic.jpg",)"
        R"(    "mime":"image/jpeg",)"
        R"(    "imageHeight":2260,)"
        R"(    "imageWidth":2775,)"
        R"(    "type":"image",)"
        R"(    "blurb":"Virginia Tech Blurb")"
        R"(},)"
        R"({)"
        R"(    "content":"By Mark Giannotto",)"
        R"(    "mime":"text/plain",)"
        R"(    "type":"byline")"
        R"(},)"
        R"({)"
        R"(    "content":1325376562000,)"
        R"(    "mime":"text/plain",)"
        R"(    "type":"date")"
        R"(},)"
        R"({)"
        R"(    "content":"<span class=\"dateline\">NEW ORLEANS</span>.",)"
        R"(    "subtype":"paragraph",)"
        R"(    "type":"sanitized_html",)"
        R"(    "mime":"text/html")"
        R"(},)"
        R"({)"
        R"(    "corrupted":"content",)"
        R"(    "subtype":"paragraph",)"
        R"(    "type":"sanitized_html",)"
        R"(    "mime":"text/html")"
        R"(},)"
        R"({)"
        R"(    "type":"unknown")"
        R"(},)"
        R"({)"
        R"(    "role":"Reporter",)"
        R"(    "type":"author_info",)"
        R"(    "name":"Mark Giannotto",)"
        R"(    "bio":"Mark Giannotto's bio")"
        R"(})"
        R"(],)"
        R"("type": "article",)"
        R"("source": "The Washington Post")"
        "}";
    std::istringstream is(r);
    auto result = Record::read(is);
    REQUIRE(std::get_if<Record>(&result) != nullptr);
    auto record = *std::get_if<Record>(&result);

    auto j = nlohmann::json::parse(r);
    REQUIRE(record.id == j["id"]);
    REQUIRE(record.url == j["article_url"]);
    REQUIRE(record.title == j["title"]);
    REQUIRE(record.author == "");
    REQUIRE(record.published == j["published_date"]);
    REQUIRE(record.type == j["type"]);
    REQUIRE(record.source == j["source"]);

    {
        REQUIRE(std::get_if<Kicker>(&record.contents[0]) != nullptr);
        auto kicker = *std::get_if<Kicker>(&record.contents[0]);
        REQUIRE(kicker.content == "Colleges");
        REQUIRE(kicker.mime == "text/plain");
    }
    {
        REQUIRE(std::get_if<Title>(&record.contents[1]) != nullptr);
        auto title = *std::get_if<Title>(&record.contents[1]);
        REQUIRE(title.content ==
                "Danny Coale, Jarrett Boykin are a perfect 1-2 punch for Virginia Tech");
        REQUIRE(title.mime == "text/plain");
    }
    {
        REQUIRE(std::get_if<Image>(&record.contents[2]) != nullptr);
        auto image = *std::get_if<Image>(&record.contents[2]);
        REQUIRE(image.caption == "Virginia Tech wide receiver Danny Coale");
        REQUIRE(image.url == "https://img.washingtonpost.com/somepic.jpg");
        REQUIRE(image.height == 2260);
        REQUIRE(image.width == 2775);
        REQUIRE(image.blurb == "Virginia Tech Blurb");
    }
    {
        REQUIRE(std::get_if<Byline>(&record.contents[3]) != nullptr);
        auto byline = *std::get_if<Byline>(&record.contents[3]);
        REQUIRE(byline.content == "By Mark Giannotto");
        REQUIRE(byline.mime == "text/plain");
    }
    {
        REQUIRE(std::get_if<Date>(&record.contents[4]) != nullptr);
        auto date = *std::get_if<Date>(&record.contents[4]);
        REQUIRE(date.timestamp == 1325376562000);
    }
    {
        REQUIRE(std::get_if<Text>(&record.contents[5]) != nullptr);
        auto text = *std::get_if<Text>(&record.contents[5]);
        REQUIRE(text.content == "<span class=\"dateline\">NEW ORLEANS</span>.");
        REQUIRE(text.mime == "text/html");
    }
    {
        REQUIRE(std::get_if<AuthorInfo>(&record.contents[6]) != nullptr);
        auto author_info = *std::get_if<AuthorInfo>(&record.contents[6]);
        REQUIRE(author_info.role == "Reporter");
        REQUIRE(author_info.name == "Mark Giannotto");
        REQUIRE(author_info.bio == "Mark Giannotto's bio");
    }
}
