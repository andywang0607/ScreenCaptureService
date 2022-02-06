#ifndef __MESSAGEHELPER_HPP
#define __MESSAGEHELPER_HPP

#include <nlohmann/json.hpp>
#include <zmqpp/zmqpp.hpp>

#include <string>

class MessageHelper
{
public:
    MessageHelper() = default;

    MessageHelper(const zmqpp::message &msg)
    {
        if (msg.parts() == 1) {
            std::string content;
            msg.get(content, 0);

            body_ = nlohmann::json::parse(content);
        } else if (msg.parts() == 2) {
            msg.get(connectToken_, 0);
            std::string content;
            msg.get(content, 1);

            body_ = nlohmann::json::parse(content);
        }
    }

    bool contains(const std::string &key) const
    {
        return body_.contains(key);
    }

    nlohmann::json getBody() const
    {
        return body_;
    }

    void setBody(const nlohmann::json &body)
    {
        body_ = body;
    }

    void updateBody(const nlohmann::json &obj)
    {
        body_.update(obj);
    }

    void setConnectToken(const std::string &token)
    {
        connectToken_ = token;
    }

    std::string getConnectToken()
    {
        return connectToken_;
    }

    template <typename T>
    void set(const std::string &key, const T &value)
    {
        body_[key] = value;
    }

    template <typename T = std::string>
    T get(const std::string &key, T defaultValue) const
    {
        return (contains(key)) ? body_[key].get<T>() : defaultValue;
    }

    std::string get(const std::string &key) const
    {
        return (contains(key)) ? body_[key].get<std::string>() : "";
    }

    std::string toString() const
    {
        std::string ret;
        ret.append("connectToken: '" + connectToken_ + "',");
        ret.append("body: '" + body_.dump() + "'");

        return ret;
    }

    zmqpp::message toMessage() const
    {
        zmqpp::message ret;

        if (!connectToken_.empty()) {
            ret.push_back(connectToken_);
        }

        ret.push_back(body_.dump());

        return ret;
    }

private:
    std::string connectToken_;
    nlohmann::json body_;
};
#endif
