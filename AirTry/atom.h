#pragma once

#include <atomic>
#include <mutex>

extern std::atomic<bool> end;
extern std::mutex mut;