#include <atomic>
#include <condition_variable>
#include <mutex>

class Semaphore
{
public:
  Semaphore(int n_consumers = 1)
    : counter_(0), n_threads_(n_consumers)
  {}
  Semaphore(Semaphore&& toMove)
    : counter_(toMove.counter_.load() ), n_threads_(toMove.n_threads_)
  {}
  void wait_one(){
    std::unique_lock<std::mutex> lk(mutex_);
    int current = counter_;
    cv_.wait(lk, [this, current] {return counter_ > current;} );
    --counter_;
  }
  void wait_all(){
    std::unique_lock<std::mutex> lk(mutex_);
    cv_.wait(lk, [this] {return counter_ >= n_threads_;} );
    counter_ -= n_threads_;
  }
  void notify_one() {
    std::unique_lock<std::mutex> lk(mutex_);
    ++counter_ ;
    cv_.notify_one();  
  }
  void notify_all() {
    std::unique_lock<std::mutex> lk(mutex_);
    counter_ += n_threads_;
    cv_.notify_all();  
  }
private:
  std::atomic<int> counter_;
  std::mutex mutex_;
  std::condition_variable cv_;
  int n_threads_;
};

