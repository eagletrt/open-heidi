#ifndef REQUESTS_DEVICE_H
#define REQUESTS_DEVICE_H

#include "requests.h"
#include <nlohmann/json.hpp>

response_t
device_flow_init(
  const std::string &url,
  const std::string &client_id,
  const std::vector<std::string> &scopes
);

#endif // !REQUESTS_DEVICE_H
