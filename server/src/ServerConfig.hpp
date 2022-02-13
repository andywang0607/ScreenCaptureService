#ifndef __SERVERCONFIG_H__
#define __SERVERCONFIG_H__

#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

class ServerConfig
{
public:
    static ServerConfig &getInstance()
    {
        static ServerConfig config;
        return config;
    }

    void updateConfig(const std::string &path = "server_config.json")
    {
        std::ifstream ifs(path);
        body_ = nlohmann::json::parse(ifs);
    }

    template <typename T = std::string>
    T get(const std::string &key, T defaultValue) const
    {
        return (body_.contains(key)) ? body_[key].get<T>() : defaultValue;
    }

    std::string get(const std::string &key) const
    {
        return (body_.contains(key)) ? body_[key].get<std::string>() : "";
    }

private:
    ServerConfig() = default;
    ServerConfig(const ServerConfig &) = delete;
    ServerConfig &operator=(const ServerConfig &) = delete;

    nlohmann::json body_;

};
#endif // __SERVERCONFIG_H__