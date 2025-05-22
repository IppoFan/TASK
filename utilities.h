#ifndef UTILITIES_H
#define UTILITIES_H

#include <chrono>
#include <string>
using namespace std;

bool parseLine(const string &line, string &movieName, double &movieRating);

using TimePoint = chrono::high_resolution_clock::time_point;
inline long long msSince(const TimePoint &start) {
  return chrono::duration_cast<chrono::milliseconds>(
             chrono::high_resolution_clock::now() - start)
      .count();
}

#endif
