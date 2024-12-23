#ifndef _SERVER_H_
#define _SERVER_H_

#include <condition_variable>
#include <mutex>
#include <optional>
#include <thread>

#include "../external/cpp-httplib/httplib.h"

struct CallbackResponse {
  std::string scope;
  std::string state;
  std::string code;
};

class AuthCallbackServer {
 private:
  httplib::Server srv;
  std::thread server_thread;
  std::optional<CallbackResponse> response;
  std::mutex lock;
  std::condition_variable cv;

  std::string client_id;
  std::string redirect_uri;
  std::string state;

  static int random_int();

 public:
  AuthCallbackServer(const std::string &client_id);
  std::string start();
  void stop();
  bool is_running();
  CallbackResponse wait_response(int timeout);
};

#endif  // _SERVER_H_
