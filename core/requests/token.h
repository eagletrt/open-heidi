#ifndef REQUESTS_TOKEN_H
#define REQUESTS_TOKEN_H

#include <nlohmann/json.hpp>
#include <optional>

struct auth_token_endpoint_body_t {
  std::string access_token;
  std::string token_type;
  std::string refresh_token;
  size_t expires_in;
  std::string scope;
};

std::optional<auth_token_endpoint_body_t> request_token(
    const std::string &host, const std::string &path, const std::string &code);

#endif  // !REQUESTS_TOKEN_H
