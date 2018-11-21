#pragma once

#include <memory>

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Binning.h"
//#include "CAFAna/Vars/GenieWeights.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/SpectrumLoader.h"

namespace ana
{

  class Loaders;
  class IPrediction;

  /** \brief Given loaders and an MC shift, Generate() generates an IPrediction

      All other arguments needed to construct the prediction are passed to the
      IPredictionGenerator constructor, and are identical for all generated
      predictions. For standalone use or use with PredictionInterp. **/
  class IPredictionGenerator
  {
    public:
    virtual std::unique_ptr<IPrediction> Generate(
    						  Loaders& loaders, const SystShifts& shiftMC = kNoShift ) const = 0;

    //virtual std::unique_ptr<IPrediction> Generate(
    //                                              Loaders& loaders, const SystShifts& shiftMC = kNoShift, const Var& wei = kUnweighted ) const = 0;


  };

  //---------------------------------------------------------------------------

  /// Generates FD-only predictions (no extrapolation)
  class NoExtrapGenerator: public IPredictionGenerator
  {
    public:
    NoExtrapGenerator(
      const HistAxis axis,
      //const Binning bins,
      const Cut cut,
      const Var wei = kUnweighted );

    std::unique_ptr<IPrediction> Generate(
    					  Loaders& loaders,
					  const SystShifts& shiftMC = kNoShift ) const override;

    private:
    const HistAxis fAxis;
    //const Binning fBins;
    const Cut fCut;
    const Var fWei;
  };


  class OffExtrapGenerator: public IPredictionGenerator
  {
    public:
    OffExtrapGenerator(
      const HistAxis axis,
      const Cut cut,
      const Var wei = kUnweighted );

    OffExtrapGenerator(
      const HistAxis axis,
      const Cut cut,
      std::map <float, std::map <float, std::map<float, float>>> map,
      int offLocation,
      const Var wei);

    std::unique_ptr<IPrediction> Generate(
                                          Loaders& loaders,
                                          const SystShifts& shiftMC = kNoShift ) const override;

    std::unique_ptr<IPrediction> Generate(
                                          SpectrumLoaderBase& loaderNonswap, SpectrumLoaderBase& loaderNue, SpectrumLoaderBase& loaderNuTau,
                                          const SystShifts& shiftMC = kNoShift ) const;

    private:
    const HistAxis fAxis;
    //const Binning fBins;
    const Cut fCut;
    const Var fWei;
    std::map <float, std::map <float, std::map<float, float>>> fMap;
    int fOffLocation; 
  };
}
