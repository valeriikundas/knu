#include "stdafx.h"
#include "CppUnitTest.h"
#include "../binomial-heap/binomial-heap.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
        TEST_METHOD(tm1234)
        {
            BinHeap bh;
            bh.insert(1);
            bh.insert(2);
            bh.insert(3);
            bh.insert(4);

            Node* node = bh.get_root_head();
            Assert::AreEqual(node->key, 1);
            Assert::AreEqual(node->child->key, 3);
            Assert::AreEqual(node->child->next->key, 2);
            Assert::AreEqual(node->child->child->key, 4);
        }

        TEST_METHOD(tm1243)
        {
            BinHeap bh;
            int a[4] = { 1,2,4,3 };
            for (int i = 0; i < 4; ++i)
                bh.insert(a[i]);

            Node* r = bh.get_root_head();
            Assert::AreEqual(r->key, 1);
            Assert::AreEqual(r->child->key, 3);
            Assert::AreEqual(r->child->next->key, 2);
            Assert::AreEqual(r->child->child->key, 4);
        }

        TEST_METHOD(tm43)
        {
            BinHeap bh;
            int a[2] = { 4,3 };
            for (int i = 0; i < 2; ++i)
                bh.insert(a[i]);

            Node* r = bh.get_root_head();
            Assert::AreEqual(r->key, 3);
            Assert::AreEqual(r->child->key, 4);
        }

        TEST_METHOD(tm4312)
        {
            BinHeap bh;
            int a[4] = { 4,3,1,2 };
            for (int i = 0; i < 4; ++i)
                bh.insert(a[i]);

            Node* r = bh.get_root_head();
            Assert::AreEqual(r->key, 1);
            Assert::AreEqual(r->child->key, 3);
            Assert::AreEqual(r->child->next->key, 2);
            Assert::AreEqual(r->child->child->key, 4);
        }
	};
}