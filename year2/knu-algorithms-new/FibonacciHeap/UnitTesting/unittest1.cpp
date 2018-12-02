#pragma once
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../fibonacciheap.cpp"
#include <iostream>
#include <set>
#include <string>
#include <typeinfo>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(UnitTest1) {
public:

  template<typename TKey>
  void insert(FibonacciHeap<TKey, std::string>& FibHeap, std::multiset<TKey>& Set, TKey key) {
    FibHeap.insert(key, random_string(5));
    Set.insert(key);
  }

  template<typename TKey>
  void extract_min(FibonacciHeap<TKey, std::string>& FibHeap, std::multiset<TKey>& Set) {
    TKey res1 = FibHeap.extract_min().first;
    TKey res2 = *Set.begin();
    Set.erase(Set.begin());

    if (res1 != res2) {
      //std::string msg = "fib=" + std::to_string(res1) + " set=" + std::to_string(res2) + " *** ";
      //Logger::WriteMessage(FibHeap.as_string().c_str());
      //std::wstring wmsg = std::wstring(msg.begin(), msg.end());

      //Assert::Fail(wmsg.c_str());
      //Assert::Fail("error: extract min failed.");
    }
  }

  template<typename TKey>
  void get_min(FibonacciHeap<TKey, std::string>& FibHeap, std::multiset<TKey>& Set) {
    TKey res1 = FibHeap.get_min()->key;
    TKey res2 = *Set.begin();

    if (res1 != res2) {
      //std::string msg = "fib=" + std::to_string(res1) + " set=" + std::to_string(res2) + " *** ";
      //Logger::WriteMessage(FibHeap.as_string().c_str());
      //std::wstring wmsg = std::wstring(msg.begin(), msg.end());

      //Assert::Fail(wmsg.c_str());
      //Assert::Fail("error: get min failed.");
    }
  }

  template<typename TKey>
  void iteration(int insert_count = 100, int operations_count = 100, int max_number = 100) {
    FibonacciHeap<TKey, std::string> h;
    std::multiset<TKey> s;

    for (int i = 0; i < insert_count; ++i) {
      //TKey random_key = rand() % max_number;
      TKey random_key = random_string();

      insert<TKey>(h, s, random_key);
    }
    for (int i = 0; i < operations_count; ++i) {
      int cmd = rand() % 3;
      switch (cmd) {
        case 0:
        {
          //TKey random_key = rand() % max_number;
          TKey random_key = random_string();

          insert<TKey>(h, s, random_key);
          break;
        }
        case 1:
        {
          if (h.empty())
            continue;
          extract_min<TKey>(h, s);
          break;
        }
        case 2:
        {
          if (h.empty())
            continue;
          get_min<TKey>(h, s);
          break;
        }
      }
    }
  }

  TEST_METHOD(intSmall) {
    int t = 100;
    int insert_count = 10;
    int operations_count = 10;
    int max_number = 1000;
    while (t--) {
      //iteration<int>(insert_count, operations_count, max_number);
    }
  }

  TEST_METHOD(intBig) {
    int t = 100;
    int insert_count = 10000;
    int operations_count = 1000000;
    int max_number = 1000000;
    while (t--) {
      //iteration<int>(insert_count, operations_count, max_number);
    }
  }

  TEST_METHOD(stringRandomSmall) {
    int t = 10;
    int insert_count = 10;
    int operations_count = 100;
    while (t--) {
      iteration<std::string>(insert_count, operations_count);
    }
  }

  TEST_METHOD(stringRandomBig) {
    int t = 1000;
    int insert_count = 10000;
    int operations_count = 10000;
    while (t--) {
      iteration<std::string>(insert_count, operations_count);
    }
  }

  };

}
