#ifndef BRANCH_BIMODAL_H
#define BRANCH_BIMODAL_H

#include <array>
#include <bitset>

#include "address.h"
#include "modules.h"
#include "msl/fwcounter.h"

class GAg : champsim::modules::branch_predictor
{
  static constexpr std::size_t TABLE_SIZE = 16384;
  static constexpr std::size_t BITS = 2;

  std::bitset<14> bhr_;
  std::array<champsim::msl::fwcounter<BITS>, TABLE_SIZE> pht_;

public:
  using branch_predictor::branch_predictor;

  // void initialize_branch_predictor();
  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif
