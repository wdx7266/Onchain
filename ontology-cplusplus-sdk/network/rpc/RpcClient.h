#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include "../../network/connect/IConnector.h"
#include "RpcInterfaces.h"
#include "boost/any.hpp"
#include <iostream>
#include <map>
#include <vector>

class RpcClient : public IConnector {
private:
  RpcInterfaces rpc;

public:
  RpcClient() {}
  RpcClient(std::string url) {
    try {
      rpc.setUrl(url);
    } catch (const char *err) {
      std::cerr << err << std::endl;
    }
  }

  std::string getUrl() override { return rpc.getHost(); }

  boost::any getBalance(std::string address) override {
    boost::any result;
    result = rpc.call("getbalance", address);
    return result;
  }

  boost::any sendRawTransaction(bool preExec, const std::string &userid,
                                const std::string &sData) override {
    boost::any result;
    std::vector<boost::any> any_vec;
    nlohmann::json json_array;
    json_array.push_back(sData);
    if (preExec) {
      json_array.push_back(1);
      result = rpc.call("sendrawtransaction", json_array);
    } else {
      result = rpc.call("sendrawtransaction", json_array);
    }
    return result;
  }

  std::string sendRawTransaction(const std::string &sData) {
    boost::any val;
    std::string value;
    val = rpc.call("sendrawtransaction", sData);
    if (val.type() == typeid(std::string)) {
      value = boost::any_cast<std::string>(val);
    } else {
      throw "sendRawTransaction: return type error!";
    }
    return value;
  }
};

#endif