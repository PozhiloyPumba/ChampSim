#include "GAp.h"


bool GAp::predict_branch(champsim::address ip)
{
  unsigned long index = hash(bhr_, ip);
  // Predict taken if MSB of the counter is 1
  return (pht_[index].value() >> 1) & 1;
}

void GAp::last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type)
{
  unsigned long index = hash(bhr_, ip);
  pht_[index] += taken ? 1 : -1;

  // Update BHR
  bhr_ <<= 1;
  bhr_[0] = taken;

}
