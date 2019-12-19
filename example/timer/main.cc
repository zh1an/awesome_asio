#include <InvokeTimer.hpp>
#include <logging.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
  LOGGING_INIT;

  LOGGING_DEBUG("Program starting...");

  asio::io_context io;

  auto timer_1 = awesome_asio::InvokeTimer::CreateTimer(io, std::chrono::seconds(2), []{
    static int count = 1;
    LOGGING_INFO("timer_1 is executed {} times", count++);
    }, 10);

  timer_1->Start();
  io.run();

  LOGGING_DEBUG("Program ending...");
  return 0;
}

