#include <InvokeTimer.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "Program starting..." << std::endl;

  asio::io_context io;

  auto timer_1 = awesome_asio::InvokeTimer::CreateTimer(io, std::chrono::seconds(2), []{
    static int count = 1;
    std::cout << "timer_1 is executed " << count++ << " times" << std::endl;
    }, 10);

  timer_1->Start();
  io.run();

  std::cout << "Program ending..." << std::endl;
  return 0;
}

