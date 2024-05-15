#ifndef REQUESTS_TOKEN_H
#define REQUESTS_TOKEN_H

#include "requests.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <curl/curl.h>

struct device_flow_params_t {
  std::string device_code;
};

response_t
request_token(
  const std::string &url,
  const std::string &client_id,
  const std::variant<device_flow_params_t> &params
);

#endif // !REQUESTS_TOKEN_H
