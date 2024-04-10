#pragma once
#include <thread>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <iostream>

class ThreadPool
{
public:
	using Task = std::function<void()>;

	ThreadPool(short threads) {
		Start(threads);
	}

	~ThreadPool() {
		std::cout << "Deconstructing threadpool \n" << std::endl;
		Stop();
	}

	void enqueue(Task task);

	void updateThreads() {
		threads = std::thread::hardware_concurrency();
	}

	short threads;
private:
	std::vector<std::thread> myThreads;
	std::vector<std::thread> mThreads;
	std::condition_variable mCondition;
	// mutex used by all threads - used to manage threads usage of data 
	std::mutex mMutex;
	std::queue<Task> mTasks;

	bool mRunning = false;

	void Start(short threadCount);
	void Stop();

};

