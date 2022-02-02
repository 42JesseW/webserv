#include "../includes/Config.hpp"

int Config::m_client_max_body_size = DFL_MAX_BODY_SIZE;

Config::Config(void)
{

}

Config::Config(const Config &config) :
    m_file_path(config.m_file_path),
    m_error_files(config.m_error_files),
    m_servers(config.m_servers)
{

}

Config::~Config(void)
{

}

Config&                         Config::operator = (const Config &config)
{
    if (this != &config)
    {
        m_file_path = config.m_file_path;
        m_error_files = config.m_error_files;
        m_servers = config.m_servers;
    }
    return (*this);
}

std::map<int, std::string>&     Config::getErrorFiles(void)
{
    return (m_error_files);
}

std::vector<Server>&            Config::getServers(void)
{
    return (m_servers);
}

void                            Config::setFilePath(const char *file_path)
{
    if (!file_path || std::strlen(file_path) == 0)
        throw std::invalid_argument("File path is either NULL or empty");
    m_file_path += file_path;
}

void                            Config::loadFile(void)
{
    std::ifstream   config_file;

    if (m_file_path.size() == 0)
        throw std::logic_error("File path is not yet set");
    /* set this ifstream instance to throw if opening file fails */
    config_file.exceptions( std::ifstream::badbit );
    try
    {
        config_file.open(m_file_path);
    }
    catch (const std::ifstream::failure& e)
    {
        std::cout << "Failed to open config file " << m_file_path << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
