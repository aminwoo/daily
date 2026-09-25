#include <bits/stdc++.h>
using namespace std;

namespace {
struct Parser {
  const string& input;
  size_t position = 0;

  void skip_spaces() {
    while (position < input.size() && input[position] == ' ') ++position;
  }

  long long factor() {
    skip_spaces();
    if (input[position] == '(') {
      ++position;
      long long value = expression();
      skip_spaces();
      ++position;  // Skip ')'.
      return value;
    }
    long long value = 0;
    while (position < input.size() &&
           isdigit(static_cast<unsigned char>(input[position]))) {
      value = value * 10 + (input[position++] - '0');
    }
    return value;
  }

  long long term() {
    long long value = factor();
    for (;;) {
      skip_spaces();
      if (position < input.size() && input[position] == '*') {
        ++position;
        value *= factor();
      } else if (position < input.size() && input[position] == '/') {
        ++position;
        value /= factor();
      } else {
        return value;
      }
    }
  }

  long long expression() {
    long long value = term();
    for (;;) {
      skip_spaces();
      if (position < input.size() && input[position] == '+') {
        ++position;
        value += term();
      } else if (position < input.size() && input[position] == '-') {
        ++position;
        value -= term();
      } else {
        return value;
      }
    }
  }
};
}  // namespace

long long evaluate(const string& s) {
  Parser parser{s};
  return parser.expression();
}

string decode_string(const string& s) {
  vector<pair<long long, size_t>> stack;  // (repeat count, segment start)
  string result;
  long long repeat_count = 0;
  for (char c : s) {
    if (isdigit(static_cast<unsigned char>(c))) {
      repeat_count = repeat_count * 10 + (c - '0');
    } else if (c == '[') {
      stack.push_back({repeat_count, result.size()});
      repeat_count = 0;
    } else if (c == ']') {
      auto [repetitions, start] = stack.back();
      stack.pop_back();
      const size_t length = result.size() - start;
      result.reserve(start + length * repetitions);
      for (long long copy = 1; copy < repetitions; ++copy) {
        result.append(result, start, length);
      }
    } else {
      result += c;
    }
  }
  return result;
}
