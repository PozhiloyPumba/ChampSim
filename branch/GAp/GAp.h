#ifndef BRANCH_BIMODAL_H
#define BRANCH_BIMODAL_H

#include <array>
#include <bitset>

#include "address.h"
#include "modules.h"
#include "msl/fwcounter.h"

class GAp : champsim::modules::branch_predictor
{
  // GAp indexing from this 
  // https://comp.anu.edu.au/courses/comp3710-uarch/assets/lectures/week4.pdf

  [[nodiscard]] static auto hash(std::bitset<7> bhr, champsim::address ip) { 
    return (bhr.to_ulong()) | ((ip.to<unsigned long>() & MASK) << 7);
  }

  static constexpr std::size_t MASK = (1 << 7) - 1;
  static constexpr std::size_t TABLE_SIZE = 16384;
  static constexpr std::size_t BITS = 2;

  std::bitset<7> bhr_;
  std::array<champsim::msl::fwcounter<BITS>, TABLE_SIZE> pht_;

public:
  using branch_predictor::branch_predictor;

  // void initialize_branch_predictor();
  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif
