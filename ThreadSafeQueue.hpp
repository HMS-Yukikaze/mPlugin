#pragma once

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue(int maxSize) : MaxSize(maxSize) { mq.reserve(maxSize); }

    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mt_linklist);
        not_full_cv.wait(lock, [this] { return mq.size() < MaxSize; });
        mq.emplace_back(item);
        not_empty_cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mt_linklist);
        not_empty_cv.wait(lock, [this] { return !mq.empty(); });
        T item = mq.front();
        not_full_cv.notify_one();
        return item;
    }

private:
    std::mutex mt_linklist;
    std::condition_variable not_empty_cv;
    std::condition_variable not_full_cv;
    std::vector<T> mq;
    int MaxSize;
};
