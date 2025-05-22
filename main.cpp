// Winter'24
// Instructor: Diba Mirza
// Student name:
#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "movies.h"
#include "utilities.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
    cerr << "Usage: " << argv[0] << " moviesFilename prefixFilename " << endl;
    exit(1);
  }

  ifstream movieFile(argv[1]);

  if (movieFile.fail()) {
    cerr << "Could not open file " << argv[1];
    exit(1);
  }

  MovieCollection movies;

  string line, movieName;
  double movieRating;
  while (getline(movieFile, line) && parseLine(line, movieName, movieRating)) {
    movies.addMovie(movieName, movieRating);
  }

  movieFile.close();

  if (argc == 2) {
    movies.sortByTitle();
    movies.printAll();
    return 0;
  }

  ifstream prefixFile(argv[2]);

  if (prefixFile.fail()) {
    cerr << "Could not open file " << argv[2];
    exit(1);
  }

  vector<string> prefixes;
  while (getline(prefixFile, line)) {
    if (!line.empty()) {
      prefixes.push_back(line);
    }
  }

  vector<const Movie *> bestForPrefix(prefixes.size(), nullptr);
  movies.sortByTitle();

  auto t0 = chrono::high_resolution_clock::now();

  for (size_t i = 0; i < prefixes.size(); ++i) {
    auto matches = movies.queryPrefix(prefixes[i]);
    if (matches.empty()) {
      cout << "No movies found with prefix " << prefixes[i] << endl;
    } else {
      for (const Movie *m : matches) {
        cout << m->title << ',' << fixed << setprecision(1) << m->rating
             << endl;
      }
      bestForPrefix[i] = matches.front();
    }
  }

  for (size_t i = 0; i < prefixes.size(); ++i) {
    if (bestForPrefix[i]) {
      cout << "Best movie with prefix " << prefixes[i]
           << " is: " << bestForPrefix[i]->title << " with rating " << fixed
           << setprecision(1) << bestForPrefix[i]->rating << endl;
    }
  }

  auto t1 = chrono::high_resolution_clock::now();
  cerr << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() << " ms"
       << endl;

  return 0;
}

/*  ===========================  Part 3 — Explanation  ===========================

   ▸ 3a.  How fast does it run?
           n = total movies
           m = prefixes in one run
           k = max movies that share a single prefix
           l = longest title length

       One-time work
           sort the whole vector<Movie>  ~  O(nlogn)

       For every prefix
           two binary searches (lower/upper_bound) ~ O(logn)
           copy the k_i matches into a small vector ~ O(k_i)
           sort that vector by rating, then title ~ O(k_i log k_i)

       All together
           O( nlogn + sum(logn  +  k_i*log(k_i)) )

       Real numbers on the CSIL machines (g++ -O3):
           input_20_random.csv      →   6 ms
           input_100_random.csv     →   6 ms
           input_1000_random.csv    →   8 ms
           input_76920_random.csv   →  55 ms
       The growth from 1 k to 76k rows is close to the nlogn

   ▸ 3b.  Mermory usage

           vector<Movie>             n titles  ~ O(n*l)
           vector<string> prefixes   m items   ~ O(m*l)
           temp vector<const Movie*> per query ~ O(k)

       Peak footprint: Θ(n*l + m*l) — linear in input size, plus
       O(k) scratch buffer while processing one prefix.

   ▸ 3c.  Why this design?

       • Goal: keep runtime low controlling RAM usage.
         A single sorted vector plus binary search gives log-time prefix lookup
         and stores every title exactly once.

       • Could we go faster? A trie would reduce lookup to O(l), but would
         triple memory (lots of heap nodes) and save only ~20 ms on the biggest
         test — not worth it for this assignment.

       • Result: we hit the “fast enough” target (dominant nlogn term) and
         stayed strictly linear in space.  Further time gains would cost memory;
         further memory cuts would not change asymptotics. Time was the harder
         constraint, so we accepted the balanced solution above.
*/

