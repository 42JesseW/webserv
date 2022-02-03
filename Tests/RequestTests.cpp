#include "catch.hpp"
#include "Request.hpp"

TEST_CASE("parsetester")
{
	SECTION("return")
	{
		Request NewRequest;
        int fd = open("Request/RequestFile.txt", O_RDONLY);

        NewRequest.copyRequest(fd);

		REQUIRE(NewRequest.getStartLine() == "GET /home.html HTTP/1.1");
	}
}