// SPDX-FileCopyrightText: 2021 CERN
// SPDX-License-Identifier: Apache-2.0

#ifndef TESTEM3_H
#define TESTEM3_H

#include <VecGeom/base/Config.h>
#include <VecGeom/volumes/PlacedVolume.h>

#ifdef VECGEOM_ENABLE_CUDA
#include <VecGeom/management/CudaManager.h> // forward declares vecgeom::cxx::VPlacedVolume
#endif

struct ScoringPerVolume {
  double *energyDeposit;
  double *chargedTrackLength;
};

void TestEm3(const vecgeom::cxx::VPlacedVolume *world, int particles, double energy, double startX, const int *MCIndex,
             ScoringPerVolume *scoringPerVolume, int NumVolumes);

#endif
