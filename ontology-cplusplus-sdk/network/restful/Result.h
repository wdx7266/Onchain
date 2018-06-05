#ifndef RESULT_H
#define RESULT_H

#include <nlohmann/json.hpp>
#include <string>

struct struct_result {
  std::string Action;
  long long Error;
  std::string Desc;
  std::string Version;
  std::string Result;
};

class Result {
public:
  struct_result result;
  std::string toString() {
    nlohmann::json json_result;
    json_result["Action"] = result.Action;
    json_result["Error"] = result.Error;
    json_result["Desc"] = result.Error;
    json_result["Version"] = result.Version;
    json_result["Result"] = boost::any_cast<std::string>(result.Result);
    std::string str_result = json_result.dump();
    return str_result;
  }
};

#endif // RESULT_H