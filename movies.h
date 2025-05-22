#ifndef MOVIES_H
#define MOVIES_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Movie {
  string title;
  double rating;
};

// Sort, Add and Print + data storing
class MovieCollection {
public:
  void addMovie(const string &title, double rating);
  void sortByTitle(); // A-Z
  void printAll(ostream &os = cout) const;
  vector<const Movie *> queryPrefix(const string &p) const;

private:
  vector<Movie> data;
};

#endif
