#include "movies.h"

#include <cstring>
using namespace std;

static bool byTitle(const Movie &a, const Movie &b) {
  return a.title < b.title;
}

void MovieCollection::addMovie(const string &title, double rating) {
  data.push_back({title, rating});
}

void MovieCollection::sortByTitle() { sort(data.begin(), data.end(), byTitle); }

void MovieCollection::printAll(ostream &os) const {
  for (const auto &m : data) {
    os << m.title << ',' << fixed << setprecision(1) << m.rating << '\n';
  }
}

static string afterPrefix(const string &p) {
  string up = p;
  up.push_back(char(0xFF));
  return up;
}

static bool ratingDescTitleAsc(const Movie *a, const Movie *b) {
  if (a->rating != b->rating)
    return a->rating > b->rating;
  return a->title < b->title;
}

vector<const Movie *> MovieCollection::queryPrefix(const string &p) const {
  vector<const Movie *> res;
  if (p.empty())
    return res;

  // data sorted by title
  auto lo = lower_bound(
      data.begin(), data.end(), p,
      [](const Movie &m, const string &key) { return m.title < key; });

  auto hi = lower_bound(
      lo, data.end(), afterPrefix(p),
      [](const Movie &m, const string &key) { return m.title < key; });

  for (auto it = lo; it != hi; ++it) {
    res.push_back(&*it);
  }

  stable_sort(res.begin(), res.end(), ratingDescTitleAsc);
  return res;
}