#include "utilities.h"
#include <cstdlib>
using namespace std;

bool parseLine(const string &line, string &movieName, double &movieRating) {
  size_t comma = line.find_last_of(',');
  if (comma == string::npos)
    return false;

  movieName = line.substr(0, comma);
  if (!movieName.empty() && movieName.front() == '"') {
    movieName = movieName.substr(1, movieName.length() - 2);
  }
  movieRating = stod(line.substr(comma + 1));
  return true;
}
