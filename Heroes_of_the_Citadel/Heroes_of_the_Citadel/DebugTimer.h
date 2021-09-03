#pragma once

#include <chrono>
#include <unordered_map>
#include <string>

class RunData {
private:
public:
   int m_numAttempts{ 0 };
   uint64_t m_totalTime{ 0 };
};

class DebugTimer {
private:
   //std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime{};
   int64_t m_start{};
   static std::unordered_map<std::string, RunData> m_runData;
   std::string m_key{};

public:
   DebugTimer (std::string key);
   ~DebugTimer ();

   static std::string getResultsString ();
};

