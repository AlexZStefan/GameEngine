#include "ThreadPool.h"

void ThreadPool::enqueue(Task task) {
	{
		// move is used to indicate that an object t may be "moved from",
		// i.e. allowing the efficient transfer of resources from t to another object. 
		std::unique_lock<std::mutex> lock(mMutex);
		mTasks.emplace(std::move(task));
	}

	// notify one waiting thread if there is one
	// can notify_all for all threads waiting
	mCondition.notify_one();
}

void ThreadPool::Start(short threadCount) {
	for (short i = 0; i < threadCount; i++) {

		mThreads.emplace_back([=] {

			Task task;
			while (true) {
				{
					auto id = mThreads[i].get_id();
					std::cout << " - Crrent thread running" << id << "\n" << std::endl;

					// lock thread so it cannot be used by other thread 
					// in order to prevent memory conflicts
					// unlocks after scope
					std::unique_lock<std::mutex> lock(mMutex);
					mCondition.notify_all();
					// wait for notification - spurious wake 
					// if queue empty put thread back to sleep else pop task
					mCondition.wait(lock, [=] {
						auto id = mThreads[i].get_id();
						std::cout << "... Waiting, thread id : " << id << "\n" << std::endl;
						return mRunning || !mTasks.empty();
						});

					if (mRunning && mTasks.empty()) {
						auto id = mThreads[i].get_id();
						std::cout <<" - Closing pool loop. thread id :" << id << "\n" << std::endl;
						break;
					}

					// return a referance of the next element in queue 
					task = std::move(mTasks.front());
					mTasks.pop();
					try {
						auto id = mThreads[i].get_id();
						std::cout << " - Working on task. Thread id :" << id << "\n" << std::endl;
						task();
						lock.unlock();
					}
					catch (const std::exception& e) {
						std::cerr << " - Exception caught in thread: " << e.what() << std::endl;
					}
					catch (...) {
						std::cerr << " - Unknown exception caught in thread" << std::endl;
					}
				}
			}
			});
		auto id = mThreads[i].get_id();
		std::cout << id  << "\n" << std::endl;

	}
}

 void ThreadPool::Stop() {
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mRunning = true;
	}
	mCondition.notify_all();

	for (auto& thread : mThreads)
		if (thread.joinable())
			thread.join();
}
