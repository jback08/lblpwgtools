#include "CAFAna/Prediction/PredictionExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

namespace ana
{
  class Loaders;

  class DUNERunPOTSpectrumLoader;

  /// Prediction that just uses FD MC, with no extrapolation
  class PredictionNoExtrap: public PredictionExtrap
  {
  public:
    PredictionNoExtrap(PredictionExtrap* pred);

    // This is the DUNE constructor
    PredictionNoExtrap(SpectrumLoaderBase& loaderNonswap,
                       SpectrumLoaderBase& loaderNue,
                       SpectrumLoaderBase& loaderNuTau,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,             
    		       std::map <float, std::map <float, std::map<float, float>>> map ,          
                       const SystShifts& shift = kNoShift,
		       int offLocation = 0, 
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(SpectrumLoaderBase& loaderNonswap,
                       SpectrumLoaderBase& loaderNue,
                       SpectrumLoaderBase& loaderNuTau,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var, int offLocation,
                       const Cut& cut,
                       std::map <float, std::map <float, std::map<float, float>>> map ,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(SpectrumLoaderBase& loaderNonswap,
                       SpectrumLoaderBase& loaderNue,
                       SpectrumLoaderBase& loaderNuTau,
		       const HistAxis& axis,
		       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(SpectrumLoaderBase& loaderNonswap,
                       SpectrumLoaderBase& loaderNue,
                       SpectrumLoaderBase& loaderNuTau,
                       const std::string& label,
		       const Binning& bins,
		       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(Loaders& loaders,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(Loaders& loaders,
                       const HistAxis& axis,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    virtual ~PredictionNoExtrap();

    static std::unique_ptr<PredictionNoExtrap> LoadFrom(TDirectory* dir);
    virtual void SaveTo(TDirectory* dir) const override;

    std::map <float, std::map <float, std::map<float, float>>> fExtrapMap;

  };

  class NoExtrapPredictionGenerator: public IPredictionGenerator
  {
  public:
    NoExtrapPredictionGenerator(HistAxis axis, Cut cut)
      : fAxis(axis), fCut(cut)
    {
    }

    virtual std::unique_ptr<IPrediction>
    Generate(Loaders& loaders, const SystShifts& shiftMC = kNoShift) const override
    {
      return std::unique_ptr<IPrediction>(
                                          new PredictionNoExtrap(loaders, fAxis, fCut, shiftMC));
    }

  protected:
    HistAxis fAxis;
    Cut fCut;
    //std::map <float, std::map <float, std::map<float, float>>> fExtrapMap;
  };

  class DUNENoExtrapPredictionGenerator: public IPredictionGenerator
  {
  public:

    DUNENoExtrapPredictionGenerator(SpectrumLoaderBase& loaderBeam,
                                    SpectrumLoaderBase& loaderNue,
                                    SpectrumLoaderBase& loaderNuTau,
                                    SpectrumLoaderBase& loaderNC,
				    std::string label, Binning bins, Var var, Cut cut, Var wei)
      : fLoaderBeam(loaderBeam), fLoaderNue(loaderNue),
        fLoaderNuTau(loaderNuTau), fLoaderNC(loaderNC), 
        fLabel(label), fBins(bins), fVar(var), fCut(cut), fWei(wei)
    {
    }
 
    virtual std::unique_ptr<IPrediction>
    Generate(Loaders& loaders, const SystShifts& shiftMC = kNoShift /*, const Var& wei = kUnweighted*/)  const override
    {
      return std::unique_ptr<IPrediction>(new PredictionNoExtrap(fLoaderBeam, fLoaderNue, fLoaderNuTau,
                                          fLabel, fBins, fVar, fCut));
    }
/*
    virtual std::unique_ptr<IPrediction>
    Generate(Loaders& loaders, const SystShifts& shiftMC = kNoShift ) const override
    {
      return std::unique_ptr<IPrediction>( new PredictionOffExtrap(fLoaderBeam, fLoaderNue, fLoaderNuTau,
                              fBins, fCut, shiftMC, 0 ));
      //return new PredictionOffExtrap(fLoaderBeam, fLoaderNue, fLoaderNuTau, fBins, fCut, shiftMC, 0);
    }
*/

  protected:
    SpectrumLoaderBase& fLoaderBeam;
    SpectrumLoaderBase& fLoaderNue;
    SpectrumLoaderBase& fLoaderNuTau;
    SpectrumLoaderBase& fLoaderNC;
    std::string fLabel;
    Binning fBins;
    Var fVar;
    Cut fCut;
    Var fWei;
  };

}
