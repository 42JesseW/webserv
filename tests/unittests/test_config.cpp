#include <catch.hpp>
#include <Config.hpp>

#include <utility>
#include <fstream>
#include <sys/stat.h>

#define SYS_ERROR -1

static std::string  basic_conf = ""
         "# start the configuration file. Only one http block is allowed\n"
         "http {\n"
         "\n"
         "\t# describes the maximum body size of a request in MB\n"
         "\tclient_max_body_size 10\n"
         "\n"
         "\t# error page in the form \"error_page <http_error_number> <full_path_to_file>.html\"\n"
         "\terror_page 400 /full/path/to/file.html\n"
         "\n"
         "\t# define a server block\n"
         "\tserver {\n"
         "\n"
         "\t\t# address to listen on in the form <address>:<port>. Only one listen is allowed\n"
         "\t\tlisten          *.80\n"
         "\n"
         "\t\t# server_name(s) used for matching server using \"Host\" Request header\n"
         "\t\tserver_name     example.org www.example.org\n"
         "\n"
         "        # setup a location with a base_pathname to match incoming request\n"
         "        location / {\n"
         "\n"
         "            # HTTP methods allowed for this (only GET POST DELETE are allowed)\n"
         "            allowed_methods GET POST DELETE\n"
         "\n"
         "            # sets the file_path to start looking for requested files\n"
         "            root            /data/w3\n"
         "\n"
         "            # provide directory listing if requested file is not found (either \"on\" or \"off\")\n"
         "            autoindex       on\n"
         "\n"
         "            # default file to answer if the request is a directory\n"
         "            index           index.html index.htm\n"
         "\n"
         "            # possible file extensions for which to pass to CGI\n"
         "            cgi_extensions  .php .py\n"
         "\n"
         "            # file_path of where to upload files. When set will allow uploading files\n"
         "            upload_path     /data/upload\n"
         "\n"
         "            # return a redirect in the form \"return 3<xx> <redirect_url>\"\n"
         "            return          301 www.google.com\n"
         "        }\n"
         "\t}\n"
         "}";

TEST_CASE("Default handle")
{
    CHECK(Config::getHandle().getFilePath().empty());
    CHECK(Config::getHandle().getErrorFiles().empty());
    CHECK(Config::getHandle().getServers().empty());
}

TEST_CASE(".loadFile() without m_file_path")
{
    CHECK_THROWS(Config::getHandle().loadFile());
}

TEST_CASE(".loadFile() with file that does not exists")
{
    Config::getHandle().setFilePath("FILE_THAT_DOES_NOT_EXIST");
    CHECK_THROWS(Config::getHandle().loadFile());
}

TEST_CASE(".loadFile() with file that has invalid permissions")
{
    std::string     file_name("INVALID_FILE");
    std::ofstream   invalid_file(file_name);

    REQUIRE(chmod(file_name.c_str(), 0000) != SYS_ERROR);
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with file that is valid")
{
    std::string     file_name("BRACKETS_FILE");
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);

    brackets_file << basic_conf;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_NOTHROW(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with file that has invalid brackets [1]")
{
    std::string     file_name("BRACKETS_FILE");
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "    server {\n"
        "    \n"
        "}";

    REQUIRE(brackets_file.good());
    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with file that has invalid brackets [2]")
{
    std::string     file_name("BRACKETS_FILE");
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "    server {\n"
        "\n"
        "        }\n"
        "    }\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with file that has invalid brackets [3]")
{
    std::string     file_name("BRACKETS_FILE");
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "    server {\n"
        "\n"
        "            }\n"
        "        }\n"
        "    {\n"
        "    }\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}
