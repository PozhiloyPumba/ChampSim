#include "plru.h"

#include <algorithm>
#include <cassert>


plru::plru(CACHE* cache) : plru(cache, cache->NUM_SET, cache->NUM_WAY) {}

plru::plru(CACHE* cache, long sets, long ways) : replacement(cache), NUM_WAY(std::max(long(2), ways)), halfSz_(NUM_WAY >> 1), trees_(static_cast<std::size_t>(sets * NUM_WAY), 0) {}

long plru::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,
                      champsim::address full_addr, access_type type)
{
  long idx = 1;

  auto *setTree = trees_.data() + set * NUM_WAY;
  while (idx < halfSz_) {
    idx = 2 * idx + setTree[idx];
  }

  return ((idx - halfSz_) << 1) + setTree[idx];
}

void plru::update(long set, long way) {
  long idx = (way >> 1) + halfSz_;
  auto *setTree = trees_.data() + set * NUM_WAY;

  while (idx >= 1) {
    setTree[idx] = 1 - setTree[idx];
    idx >>= 1;
  }
}

void plru::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr,
                                 access_type type)
{
  update(set, way);
}

void plru::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                   champsim::address victim_addr, access_type type, uint8_t hit)
{
  if (hit && type != access_type::WRITE) // Skip this for writeback hits
    update(set, way);
}
