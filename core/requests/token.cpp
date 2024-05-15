#include "token.h"
#include "requests.h"
#include <variant>

static
size_t
_write_callback(
  char *ptr,
  size_t size,
  size_t nmemb,
  void *data
) {
  nlohmann::json *body = static_cast<nlohmann::json *>(data);
  *body = nlohmann::json::parse(std::string(ptr));
  return size * nmemb;
}

response_t
request_token(
  const std::string &url,
  const std::string &client_id,
  const std::variant<device_flow_params_t, password_flow_params_t, refresh_flow_params_t> &params
) {

  size_t return_code;
  nlohmann::json body;

  CURLcode curl_ret;
  CURL *hnd;
  struct curl_slist *headers;

  headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());

  std::string post_str;

  if (std::holds_alternative<device_flow_params_t>(params)) {
    post_str = "client_id=" + client_id + "&grant_type=urn:ietf:params:oauth:grant-type:device_code"
      + "&device_code=" + std::get<device_flow_params_t>(params).device_code;
  } 
  else if (std::holds_alternative<password_flow_params_t>(params)) {
    post_str = "client_id=" + client_id + "&grant_type=password"
      + "&username=" + std::get<password_flow_params_t>(params).username
      + "&password=" + std::get<password_flow_params_t>(params).password;
  }
  else if (std::holds_alternative<refresh_flow_params_t>(params)) {
    post_str = "client_id=" + client_id + "&grant_type=refresh_token"
      + "&refresh_token=" + std::get<refresh_flow_params_t>(params).refresh_token;
  }
  else {
    return {0, body};
  }

  curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, post_str.c_str());
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, _write_callback);
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &body);

  curl_ret = curl_easy_perform(hnd);
  curl_easy_getinfo (hnd, CURLINFO_RESPONSE_CODE, &return_code);

  curl_easy_cleanup(hnd);
  hnd = NULL;
  curl_slist_free_all(headers);
  headers = NULL;

  return {return_code, body};
}

