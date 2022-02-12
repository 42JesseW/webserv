#include "catch.hpp"
#include "Request.hpp"

TEST_CASE("Request Parser")
{
    Request NewRequest;
    int fd = open("Request/RequestFile.txt", O_RDONLY);

    NewRequest.copyRequest(fd);

	SECTION("Start line")
	{
		REQUIRE(NewRequest.getStartLine() == "GET /home.html HTTP/1.1");
	}

    SECTION("Headers")
	{
        std::map<std::string, std::string> TestMap =
        {
            {"Host", "developer.mozilla.org"},
            {"User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:50.0) Gecko/20100101 Firefox/50.0"},
            {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
            {"Accept-Language", "en-US,en;q=0.5"}
        };
		REQUIRE(NewRequest.getHeaders() == TestMap);
	}

    SECTION("Body")
	{
		REQUIRE(NewRequest.getBody() == "Lorem Ipsum is simply dummy text of the printing and typesetting industry. \nLorem Ipsum has been the industry's standard dummy text ever since the 1500s, \nwhen an unknown printer took a galley of type and scrambled it to make a type specimen book.");
	}
}