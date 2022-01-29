#include "Config.hpp"

int Config::m_client_max_body_size = DFL_MAX_BODY_SIZE;

Config::Config()
{

}

Config::Config(const Config &config) :
    m_error_files(config.m_error_files),
    m_servers(config.m_servers)
{

}

Config::~Config()
{

}

Config&                         Config::operator = (const Config &config)
{
    if (this != &config)
    {
        m_error_files = config.m_error_files;
        m_servers = config.m_servers;
    }
    return (*this);
}

std::map<int, std::string>&     Config::getErrorFiles()
{
    return (m_error_files);
}

std::vector<Server>&            Config::getServers()
{
    return (m_servers);
}
