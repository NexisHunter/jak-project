#pragma once

#ifndef JAK1_MATCHPARAM_H
#define JAK1_MATCHPARAM_H

template <typename T>
struct MatchParam {
  MatchParam() { is_wildcard = true; }

  // intentionally not explicit so you don't have to put MatchParam<whatever>(blah) everywhere
  MatchParam(T x) {
    value = x;
    is_wildcard = false;
  }

  T value;
  bool is_wildcard = true;

  bool operator==(const T& other) const { return is_wildcard || (value == other); }
  bool operator!=(const T& other) const { return !(*this == other); }
};

#endif  // JAK1_MATCHPARAM_H
