//
// Created by Sky Lan on 2019/12/20.
// Copyright (c) 2019 GLTech.All Rights Reserved.
//

#ifndef BLOCK_TCP_CLIENT_NONCOPYABLE_HPP
#define BLOCK_TCP_CLIENT_NONCOPYABLE_HPP

namespace awesome_asio {
class noncopyable {
protected:
  noncopyable() {}
  virtual ~noncopyable() = default;

public:
  noncopyable(const noncopyable &) = delete;
  const noncopyable &operator=(const noncopyable &) = delete;
};
} // namespace awesome_asio

#endif // BLOCK_TCP_CLIENT_NONCOPYABLE_HPP
