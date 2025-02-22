/*
 * Copyright 2023 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef DisplayParams_DEFINED
#define DisplayParams_DEFINED

#include "include/core/SkColorSpace.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurfaceProps.h"
#include "include/gpu/ganesh/GrContextOptions.h"

#if defined(SK_GRAPHITE)
#include "include/gpu/graphite/ContextOptions.h"
#include "include/private/gpu/graphite/ContextOptionsPriv.h"
#endif

namespace skwindow {

#if defined(SK_GRAPHITE)
struct GraphiteContextOptions {
    GraphiteContextOptions() {
        fOptions.fOptionsPriv = &fPriv;
    }

    GraphiteContextOptions(const GraphiteContextOptions& other)
        : fOptions(other.fOptions)
        , fPriv(other.fPriv) {
        fOptions.fOptionsPriv = &fPriv;
    }

    GraphiteContextOptions& operator=(const GraphiteContextOptions& other) {
        fOptions = other.fOptions;
        fPriv = other.fPriv;
        fOptions.fOptionsPriv = &fPriv;
        return *this;
    }

    skgpu::graphite::ContextOptions     fOptions;
    skgpu::graphite::ContextOptionsPriv fPriv;
};

#endif

struct DisplayParams {
    DisplayParams()
        : fColorType(kN32_SkColorType)
        , fColorSpace(nullptr)
        , fMSAASampleCount(1)
        , fSurfaceProps(0, kRGB_H_SkPixelGeometry)
        , fDisableVsync(false)
        , fDelayDrawableAcquisition(false)
        , fCreateProtectedNativeBackend(false)
    {}

    SkColorType            fColorType;
    sk_sp<SkColorSpace>    fColorSpace;
    int                    fMSAASampleCount;
    GrContextOptions       fGrContextOptions;
#if defined(SK_GRAPHITE)
    GraphiteContextOptions fGraphiteContextOptions;
#if defined(SK_DAWN)
    bool fDisableTintSymbolRenaming = false;
#endif
#endif
    SkSurfaceProps         fSurfaceProps;
    bool                   fDisableVsync;
    bool                   fDelayDrawableAcquisition;
    bool                   fCreateProtectedNativeBackend = false;
};

}  // namespace skwindow

#endif
