/*
 * @CopyRight:
 * FISCO-BCOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FISCO-BCOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2018 fisco-dev contributors.
 */
/**
 * @brief : keycenter for disk encrytion
 * @author: jimmyshi
 * @date: 2018-12-03
 */
#pragma once
#include "Common.h"
#include <libdevcore/Common.h>
#include <libdevcore/Guards.h>
#include <libethcore/CommonJS.h>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <cstdlib>
#include <memory>
#include <string>

namespace Json
{
class Value;
}

namespace dev
{
#define KC_LOG(_OBV)        \
    LOG(_OBV) << "[g:null]" \
              << "[p:null][KeyManager]"

class KeyCenterHttpClient
{
public:
    KeyCenterHttpClient(const std::string& _ip, int _port);
    ~KeyCenterHttpClient();
    void connect();
    void close();
    Json::Value callMethod(const std::string& _method, Json::Value _params);

private:
    std::string m_ip;
    int m_port;
    boost::asio::io_context m_ioc;
    boost::asio::ip::tcp::socket m_socket;
    mutable SharedMutex x_clinetSocket;
};

class KeyCenter
{
public:
    using Ptr = std::shared_ptr<KeyCenter>;

    KeyCenter(){};
    virtual ~KeyCenter(){};
    virtual const dev::bytes getDataKey(const std::string& _cipherDataKey);
    virtual const std::string generateCipherDataKey();
    void setIpPort(const std::string& _ip, int _port);
    const std::string url() { return m_ip + ":" + std::to_string(m_port); }

    static std::shared_ptr<KeyCenter> instance();

    void clearCache()
    {
        m_lastQueryCipherDataKey.clear();
        m_lastRcvDataKey.clear();
    }

private:
    std::string m_ip;
    int m_port;
    std::string m_url;

    // Query cache
    std::string m_lastQueryCipherDataKey;
    dev::bytes m_lastRcvDataKey;

protected:
    dev::bytes uniformDataKey(const dev::bytes& _readableDataKey);
};

#define g_keyCenter KeyCenter::instance()  // Only one keycenter in a node

}  // namespace dev