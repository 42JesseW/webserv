#include <catch.hpp>
#include <Config.hpp>

#include <utility>

TEST_CASE("Default construction")
{
    Config  default_config;

    REQUIRE(default_config.getFilePath().empty());
    REQUIRE(default_config.getErrorFiles().empty());
    REQUIRE(default_config.getServers().empty());
}

TEST_CASE("Copy construction")
{
    Config  config_with_stuff;
    Server  some_server;

    config_with_stuff.setFilePath("some_file.conf");
    config_with_stuff.getErrorFiles().insert(std::make_pair(500, "some_error_file.html"));
    config_with_stuff.getServers().push_back(some_server);

    Config  copy_config(config_with_stuff);

}

TEST_CASE("Assignment operation")
{

}
