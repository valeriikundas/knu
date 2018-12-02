#include <iostream>
#include <thread>
#include <mutex>
#include <deque>

class Buffer
{
private:
  std::mutex mu;
  std::deque<int> mBuf;
  const int mMaxSize = 10;

public:
  Buffer() {}

  void push(int x)
  {
    mu.lock();
    mBuf.push_back(x);
    mu.unlock();
  }

  int pop()
  {
    mu.lock();
    int val = mBuf.back();
    mBuf.pop_back();
    mu.unlock();
    return val;
  }
};

class Producer
{
private:
  Buffer mBuf;

public:
  Producer(Buffer buffer)
  {
  }

  void produce()
  {
  }
};

class Consumer
{
public:
  Consumer(Buffer *buffer) {}

  void consume() {}
};

int main()
{
  return 0;
}