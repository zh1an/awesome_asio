#include <BlockingTcpClient.hpp>
#include <asio.hpp>
#include <logging.hpp>

#include <array>

#ifdef _WIN32
#define _WIN32_WINNT 0x0501
#endif

int main(int argc, char *argv[]) {
  LOGGING_INIT;

  LOGGING_DEBUG("Program Starting...");
  asio::io_context io;
  awesome_asio::BlockingTCPClient btc(io, "127.0.0.1", 10003);
  btc.connect();
  LOGGING_IF_FATAL(!btc.isConnected(), "tcp client cannot work");

  for (;;) {
    asio::error_code e;

    auto msg = btc.read(e/*, {0, 1000 * 500}*/);
    if (e == asio::error::eof) {
      break;
    }

    LOGGING_INFO("rcv: {}", msg);
    auto sz = btc.write(msg, e);
    if (e == asio::error::eof)
      break;
    else if (e) {
      LOGGING_ERROR(e.message());
      break;
    }
    LOGGING_INFO("write size is {} and msg size is {}", sz, msg.size());
  }

  LOGGING_DEBUG("Program ending...");
  return 0;
}