// SPDX-FileCopyrightText: 2020 CERN
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <AdePT/BlockData.h>
#include <AdePT/Atomic.h>

/** @brief Data structures */
struct MyTrack {
  int index{0};
  float energy{0};
};

struct MyHit {
  adept::Atomic_t<float> edep;
};

// portable kernel functions have to reside in a backend-dependent inline namespace to avoid symbol duplication
// when building executables/libraries running same functions on different backends
inline namespace COPCORE_IMPL {

/** @brief Generate a number of primaries.
    @param id The thread id matching blockIdx.x * blockDim.x + threadIdx.x for CUDA
              The current index of the loop over the input data
    @param tracks Pointer to the container of tracks
  */
VECCORE_ATT_HOST_DEVICE
void generateAndStorePrimary(int id, adept::BlockData<MyTrack> *tracks)
{
  auto track = tracks->NextElement();
  if (!track) COPCORE_EXCEPTION("generateAndStorePrimary: Not enough space for tracks");

  track->index  = id;
  track->energy = 100.;
}

// Mandatory callable function decoration (storage for device function pointer in global variable)
COPCORE_CALLABLE_FUNC(generateAndStorePrimary)

/** @brief This demonstrates the use of a kernel function in a namespace */
namespace devfunc {

/** @brief Select a track based on its index and add it to a queue */
VECCORE_ATT_HOST_DEVICE
void selectTrack(int id, adept::BlockData<MyTrack> *tracks, int each_n, adept::mpmc_bounded_queue<int> *queue)
{
  auto &track   = (*tracks)[id];
  bool selected = (track.index % each_n == 0);
  if (selected) queue->enqueue(id);
}
COPCORE_CALLABLE_FUNC(selectTrack)
} // end namespace devfunc

/** @brief Process a track and sum-up some energy deposit */
VECCORE_ATT_HOST_DEVICE
void elossTrack(int id, adept::mpmc_bounded_queue<int> *queue, adept::BlockData<MyTrack> *tracks,
                adept::BlockData<MyHit> *hits)
{
  // Say there are 1024 hit objects and they accumulate energy deposits from specific track id's
  int track_id = (*queue)[id];
  auto &track  = (*tracks)[track_id];
  auto &hit    = (*hits)[id % 1024];
  float edep   = 0.1 * track.energy;
  hit.edep += edep;
  track.energy -= edep;
}
COPCORE_CALLABLE_FUNC(elossTrack)

} // End namespace COPCORE_IMPL
