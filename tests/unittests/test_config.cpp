#include <catch.hpp>
#include <Config.hpp>

#include <utility>

TEST_CASE("Default construction")
{
    Config  default_config;

    CHECK(default_config.getFilePath().empty());
    CHECK(default_config.getErrorFiles().empty());
    CHECK(default_config.getServers().empty());
}

TEST_CASE("Copy construction")
{
    Config  config_with_stuff;
    Server  some_server;

    config_with_stuff.setFilePath("some_file.conf");
    config_with_stuff.getErrorFiles().insert(std::make_pair(500, "some_error_file.html"));
    config_with_stuff.getServers().push_back(some_server);

    Config  copy_config(config_with_stuff);
    CHECK(config_with_stuff.getServers() == copy_config.getServers());
    CHECK(config_with_stuff.getErrorFiles() == copy_config.getErrorFiles());
    CHECK(config_with_stuff.getFilePath() == copy_config.getFilePath());
}

TEST_CASE("Assignment operation")
{
    Config  config_with_stuff;
    Server  some_server;

    config_with_stuff.setFilePath("some_file.conf");
    config_with_stuff.getErrorFiles().insert(std::make_pair(500, "some_error_file.html"));
    config_with_stuff.getServers().push_back(some_server);

    Config  assign_config;

    assign_config = config_with_stuff;
    CHECK(config_with_stuff.getServers() == assign_config.getServers());
    CHECK(config_with_stuff.getErrorFiles() == assign_config.getErrorFiles());
    CHECK(config_with_stuff.getFilePath() == assign_config.getFilePath());
}
