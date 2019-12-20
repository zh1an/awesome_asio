//
// Created by Sky Lan on 2019/12/20.
// Copyright (c) 2019 GLTech.All Rights Reserved.
//

#ifndef BLOCK_TCP_CLIENT_BLOCKINGTCPCLIENT_HPP
#define BLOCK_TCP_CLIENT_BLOCKINGTCPCLIENT_HPP

#include <asio.hpp>
#include <logging.hpp>

#include "noncopyable.hpp"

namespace awesome_asio {

class BlockingTCPClient : public noncopyable {
public:
  explicit BlockingTCPClient(asio::io_context &io, const std::string &addr,
                             int port);
  bool connect();

  std::string read(asio::error_code &e);
  std::string read(asio::error_code &e, const timeval &timeout);
  size_t write(const std::string &msg, asio::error_code &e);

  inline bool isConnected() const { return isConnected_; }

private:
  asio::ip::tcp::socket socket_;
  asio::ip::tcp::endpoint endpoint_;

  bool isConnected_;
};

BlockingTCPClient::BlockingTCPClient(asio::io_context &io,
                                     const std::string &addr, int port)
    : socket_(io), endpoint_(asio::ip::address::from_string(addr), port),
      isConnected_(false) {
  LOGGING_TRACE;
}

bool BlockingTCPClient::connect() {
  LOGGING_TRACE;
  try {
    asio::error_code e;
    socket_.connect(endpoint_, e);
    isConnected_ = !static_cast<bool>(e.value());
    LOGGING_IF_DEBUG(!isConnected_, "{}:{} cannot be connected. message: {}",
                     endpoint_.address().to_string(), endpoint_.port(),
                     e.message());
    return isConnected_;
  } catch (const std::exception &e) {
    LOGGING_ERROR("program is terminated. message: {}", e.what());
    isConnected_ = false;
    return isConnected_;
  }
}

std::string BlockingTCPClient::read(asio::error_code &e) {
  LOGGING_TRACE;
  LOGGING_IF_FATAL(!isConnected_, "{}:{} is not available",
                   endpoint_.address().to_string(), endpoint_.port());

  if (!isConnected_) {
    e = std::error_code(-1, std::generic_category());
    return std::string();
  }
  //! if buf is type of std::string, then read_some is non-blocking;
  //! thus std::array, blocking.
  //std::string buf;
  std::array<char, 128> buf = {0};
  size_t len = socket_.read_some(asio::buffer(buf), e);

  LOGGING_IF_WARN(len != buf.size(),
                  "read msg length is {}, but buf.size() is {}", len,
                  buf.size());

  if (e == asio::error::eof) {
    LOGGING_FATAL("{}:{} is disconnected.", endpoint_.address().to_string(),
                  endpoint_.port());
    return std::string();
  }

  LOGGING_IF_FATAL(e.value(), "error message is {}", e.message());
  return {buf.begin(), buf.end()};
}

std::string BlockingTCPClient::read(asio::error_code &e,
                                    const timeval &timeout) {
  LOGGING_TRACE;
  LOGGING_IF_FATAL(!isConnected_, "{}:{} is not available",
                   endpoint_.address().to_string(), endpoint_.port());

  if (!isConnected_) {
    e = std::error_code(-1, std::generic_category());
    return std::string();
  }

  fd_set fdRead;
  FD_ZERO(&fdRead);
  FD_SET(socket_.native_handle(), &fdRead);

  int nRet = 0;
  nRet = select(socket_.native_handle() + 1, &fdRead, NULL, NULL, &timeout);
  if (nRet == 0) {
    LOGGING_ERROR("read timeout");
    e = std::error_code(-2, std::generic_category());
    return std::string();
  }

  std::string buf;
  size_t len = socket_.read_some(asio::buffer(buf), e);

  LOGGING_IF_WARN(len != buf.size(),
                  "read msg length is {}, but buf.size() is {}", len,
                  buf.size());

  if (e == asio::error::eof) {
    LOGGING_FATAL("{}:{} is disconnected.", endpoint_.address().to_string(),
                  endpoint_.port());
    return std::string();
  }

  LOGGING_IF_FATAL(e.value(), "error message is {}", e.message());
  return buf;
}

size_t BlockingTCPClient::write(const std::string &msg, asio::error_code &e) {
  LOGGING_TRACE;
  LOGGING_IF_FATAL(!isConnected_, "{}:{} is not available",
                   endpoint_.address().to_string(), endpoint_.port());

  if (!isConnected_) {
    e = std::error_code(-1, std::generic_category());
    return 0;
  }

  auto sz = socket_.write_some(asio::buffer(msg), e);
  if (e == asio::error::eof) {
    LOGGING_FATAL("{}:{} is disconnected.", endpoint_.address().to_string(),
                  endpoint_.port());
    return 0;
  }

  LOGGING_IF_FATAL(e.value(), "error message is {}", e.message());
  return sz;
}

} // namespace awesome_asio

#endif // BLOCK_TCP_CLIENT_BLOCKINGTCPCLIENT_HPP
