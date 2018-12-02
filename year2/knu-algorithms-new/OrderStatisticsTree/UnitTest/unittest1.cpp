#include "stdafx.h"
#include "CppUnitTest.h"
#include "../redblacktree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
      RedBlackTree<int> t;
      for (int i = 0; i < 10; ++i)
        t.insert(10 - i);
      for (int i = 0; i < 10; i += 3)
        t.remove(i);
      for (int i = 0; i < 3; ++i)
        t.find_index(i);
      for (int i = 0; i < 5; ++i)
        t.contains(10 - i);
		}

	};
}