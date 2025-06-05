#ifndef MATCH_QUEUE_HPP
#define MATCH_QUEUE_HPP

#include "TeamScheduler.hpp"
#include <cstddef>
#include <stdexcept>

constexpr std::size_t MAX_MATCHES = 128;

class MatchQueue {
public:
    MatchQueue(): head(0), tail(0), count(0) {}

    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == MAX_MATCHES; }

    void enqueue(const Tournament::Match& m) {
        if (isFull()) throw std::overflow_error("MatchQueue full");
        buffer[tail] = m;
        tail = (tail + 1) % MAX_MATCHES;
        ++count;
    }

    Tournament::Match dequeue() {
        if (isEmpty()) throw std::underflow_error("MatchQueue empty");
        auto m = buffer[head];
        head = (head + 1) % MAX_MATCHES;
        --count;
        return m;
    }

private:
    Tournament::Match buffer[MAX_MATCHES];
    std::size_t       head, tail, count;
};

#endif // MATCH_QUEUE_HPP
