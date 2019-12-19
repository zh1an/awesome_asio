#ifndef AWESOME_ASIO_INVOKETIMER_HPP
#define AWESOME_ASIO_INVOKETIMER_HPP

#include <functional>
#include <memory>

#include <asio.hpp>

namespace awesome_asio {

class InvokeTimer;

typedef std::shared_ptr<InvokeTimer> InvokeTimerPtr;

class InvokeTimer : std::enable_shared_from_this<InvokeTimer> {
public:
  virtual ~InvokeTimer() = default;

  typedef std::function<void()> Function;
  static InvokeTimerPtr
  CreateTimer(asio::io_context &io,
              const asio::steady_timer::duration &duration, Function &f,
              bool period = false);
  static InvokeTimerPtr
  CreateTimer(asio::io_context &io,
              const asio::steady_timer::duration &duration, Function &&f,
              bool period = false);
  static InvokeTimerPtr
  CreateTimer(asio::io_context &io,
              const asio::steady_timer::duration &duration, Function &f,
              int count);
  static InvokeTimerPtr
  CreateTimer(asio::io_context &io,
              const asio::steady_timer::duration &duration, Function &&f,
              int count);

  void Start();

  void Cancel();

private:
  explicit InvokeTimer(asio::io_context &io,
                       const asio::steady_timer::duration &duration,
                       Function &f, bool period);
  explicit InvokeTimer(asio::io_context &io,
                       const asio::steady_timer::duration &duration,
                       Function &&f, bool period);

  explicit InvokeTimer(asio::io_context &io,
                       const asio::steady_timer::duration &duration,
                       Function &f, int count);
  explicit InvokeTimer(asio::io_context &io,
                       const asio::steady_timer::duration &duration,
                       Function &&f, int count);

  void OnTrigger(const asio::error_code &e);

private:
  asio::steady_timer timer_;
  asio::steady_timer::duration duration_;
  bool periodic_;
  std::shared_ptr<InvokeTimer> self_;
  Function callback_;
  int count_;
  int trigger_count_;
};

InvokeTimerPtr
InvokeTimer::CreateTimer(asio::io_context &io,
                         const asio::steady_timer::duration &duration,
                         awesome_asio::InvokeTimer::Function &f, bool period) {
  InvokeTimerPtr it(new InvokeTimer(io, duration, f, period));
  it->self_ = it;
  return it;
}

InvokeTimerPtr
InvokeTimer::CreateTimer(asio::io_context &io,
                         const asio::steady_timer::duration &duration,
                         awesome_asio::InvokeTimer::Function &&f, bool period) {
  InvokeTimerPtr it(new InvokeTimer(io, duration, f, period));
  it->self_ = it;
  return it;
}
awesome_asio::InvokeTimerPtr
InvokeTimer::CreateTimer(asio::io_context &io,
                         const asio::steady_timer::duration &duration,
                         awesome_asio::InvokeTimer::Function &f, int count) {
  InvokeTimerPtr it(new InvokeTimer(io, duration, f, count));
  it->self_ = it;
  return it;
}

awesome_asio::InvokeTimerPtr
InvokeTimer::CreateTimer(asio::io_context &io,
                         const asio::steady_timer::duration &duration,
                         awesome_asio::InvokeTimer::Function &&f, int count) {
  InvokeTimerPtr it(new InvokeTimer(io, duration, f, count));
  it->self_ = it;
  return it;
}

InvokeTimer::InvokeTimer(asio::io_context &io,
                         const asio::steady_timer::duration &duration,
                         awesome_asio::InvokeTimer::Function &f, bool period)
    : timer_(io, duration), duration_(duration), periodic_(period),
      callback_(f), count_(-1), trigger_count_(0) {}

InvokeTimer::InvokeTimer(asio::io_context &io,
                         const asio::steady_timer::duration &duration,
                         awesome_asio::InvokeTimer::Function &&f, bool period)
    : timer_(io, duration), duration_(duration), periodic_(period),
      callback_(std::move(f)), count_(-1), trigger_count_(0) {}

InvokeTimer::InvokeTimer(asio::io_context &io,
                         const asio::steady_timer::duration &duration,
                         awesome_asio::InvokeTimer::Function &f, int count)
    : timer_(io, duration), duration_(duration), periodic_(true),
      callback_(f), count_(count), trigger_count_(0) {}
InvokeTimer::InvokeTimer(asio::io_context &io,
                         const asio::steady_timer::duration &duration,
                         awesome_asio::InvokeTimer::Function &&f, int count)
    : timer_(io, duration), duration_(duration), periodic_(true),
      callback_(std::move(f)), count_(count), trigger_count_(0) {}

void InvokeTimer::Start() {
  timer_.async_wait(
      std::bind(&InvokeTimer::OnTrigger, this, std::placeholders::_1));
}

void InvokeTimer::Cancel() {
  periodic_ = false;

  timer_.cancel();
}

void InvokeTimer::OnTrigger(const asio::error_code &e) {
  if (e.value() != 0) {
    self_.reset();
    return;
  }

  trigger_count_++;

  if (callback_)
    callback_();

  if (periodic_) {
    if (count_ == -1 || trigger_count_ < count_) {
      timer_.expires_at(timer_.expiry() + duration_);
      Start();
    } else  {
      self_.reset();
    }
  } else {
    self_.reset();
  }
}

} // namespace awesome_asio

#endif // AWESOME_ASIO_INVOKETIMER_HPP
