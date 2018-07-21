//http://www.paulbridger.com/monitor_object/#ixzz2CeN1rr4P

#ifndef PCL_OUTOFCORE_MONITOR_QUEUE_IMPL_H_
#define PCL_OUTOFCORE_MONITOR_QUEUE_IMPL_H_

#include <queue>
#include <condition_variable>

template<typename DataT>
class MonitorQueue : boost::noncopyable
{
public:
  void
  push (const DataT& newData)
  {
    std::unique_lock<std::mutex> lock (monitor_mutex_);
    queue_.push (newData);
    item_available_.notify_one ();
  }

  DataT
  pop ()
  {
    std::unique_lock<std::mutex> lock (monitor_mutex_);

    if (queue_.empty ())
    {
      item_available_.wait (lock);
    }

    DataT temp (queue_.front ());
    queue_.pop ();

    return temp;
  }

private:
  std::queue<DataT> queue_;
  std::mutex monitor_mutex_;
  std::condition_variable item_available_;
};

#endif //PCL_OUTOFCORE_MONITOR_QUEUE_IMPL_H_
