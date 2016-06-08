#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"

#include <ThreadManager.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily
{
	namespace test
	{
		TEST_CLASS(ThreadManagerUnitTest)
		{
		public:
			
			TEST_METHOD(ThreadManager_Create_Success)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				// create the thread
				auto thread = threadManager->getOrCreateThread(0);
				Assert::IsTrue(thread.use_count() != 0, L"Make sure the object actually exists");
			}

			// lower thread id's only notify the thread +1 from it unless it is specified not to do otherwise
			TEST_METHOD(ThreadManager_Create_Success_Notification)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				// create threads
				auto thread0 = threadManager->getOrCreateThread(0, std::set<unsigned int>({ 1 }));
				auto thread5 = threadManager->getOrCreateThread(5, std::set<unsigned int>({ 0 }));
				auto thread2 = threadManager->getOrCreateThread(2, std::set<unsigned int>({ 3 }));
				auto thread3 = threadManager->getOrCreateThread(3, std::set<unsigned int>({ 2 }));
				auto thread1 = threadManager->getOrCreateThread(1, std::set<unsigned int>({ 2 }));

				Assert::IsTrue(thread0->isNotifiedBy(thread5), L"Thread0 should be notified by thread 5");
				Assert::IsFalse(thread0->isNotifiedBy(thread1), L"Thread0 should not be notified by thread 1");
				Assert::IsFalse(thread0->isNotifiedBy(thread2), L"Thread0 should not be notified by thread 2");
				Assert::IsFalse(thread0->isNotifiedBy(thread3), L"Thread0 should not be notified by thread 3");

				Assert::IsFalse(thread2->isNotifiedBy(thread0), L"Thread2 should not be notified by thread 0");
				Assert::IsTrue(thread2->isNotifiedBy(thread3), L"Thread2 should be notified by thread 3");
				Assert::IsFalse(thread2->isNotifiedBy(thread5), L"Thread2 should not be notified by thread 5");

				Assert::IsFalse(thread5->isNotifiedBy(thread0), L"Thread5 should not be notified by thread 0");
				Assert::IsFalse(thread5->isNotifiedBy(thread2), L"Thread5 should not be notified by thread 2");
				Assert::IsFalse(thread5->isNotifiedBy(thread3), L"Thread5 should not be notified by thread 3");
			}

			TEST_METHOD(ThreadManager_Create_Success_Notification_2)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				auto uiThread = threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>({ ThreadIds::App }), nullptr);
				auto appThread = threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::Service, ThreadIds::UI }), nullptr);
				auto serviceThread = threadManager->getOrCreateThread(ThreadIds::Service, std::set<unsigned int>({ ThreadIds::App }), nullptr);

				Assert::IsTrue(uiThread->isNotifiedBy(appThread));
				Assert::IsFalse(uiThread->isNotifiedBy(serviceThread));

				Assert::IsTrue(appThread->isNotifiedBy(serviceThread));
				Assert::IsTrue(appThread->isNotifiedBy(uiThread));

				Assert::IsFalse(serviceThread->isNotifiedBy(uiThread));
				Assert::IsTrue(serviceThread->isNotifiedBy(appThread));
			}

			TEST_METHOD(ThreadManager_Lifecycle_Success)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				// create the thread
				auto thread = threadManager->getOrCreateThread(0);
				Assert::IsTrue(thread.use_count() != 0, L"Make sure the object actually exists");

				// now we check to make sure the thread stays alive even after we destroy our local copy of it
				auto beforeThreadNull = thread.get();
				thread = nullptr;
				thread = threadManager->getOrCreateThread(0);
				Assert::IsTrue(beforeThreadNull == thread.get(), L"Thread should stay alive without any external references to it");

				threadManager->clear();
			}
		};
	}
}