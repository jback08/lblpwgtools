#include "CAFAna/Analysis/CalcsNuFit.h"

#include "Utilities/func/MathUtil.h"

#include "OscLib/func/OscCalculatorPMNSOpt.h"

#include "CAFAna/Vars/FitVars.h"

namespace ana
{
  //----------------------------------------------------------------------
  osc::IOscCalculatorAdjustable* NuFitOscCalc(int hie, int oct, int asimov_set)
  {
    assert(hie == +1 || hie == -1);
    assert(oct == +1 || oct == -1);

    osc::IOscCalculatorAdjustable* ret = new osc::OscCalculatorPMNSOpt;
    ret->SetL(kBaseline);
    ret->SetRho(kEarthDensity);

    ret->SetDmsq21(kNuFitDmsq21CV);
    ret->SetTh12(kNuFitTh12CV);

    if(hie > 0){
      ret->SetDmsq32(kNuFitDmsq32CVNH);
      ret->SetTh23(kNuFitTh23CVNH);
      ret->SetTh13(kNuFitTh13CVNH);
      ret->SetdCP(kNuFitdCPCVNH);
    }
    else{
      ret->SetDmsq32(kNuFitDmsq32CVIH);
      ret->SetTh23(kNuFitTh23CVIH);
      ret->SetTh13(kNuFitTh13CVIH);
      ret->SetdCP(kNuFitdCPCVIH);
    }

    if (asimov_set == 1 && hie == +1) ret->SetTh23(kNuFitTh23LoNH);
    if (asimov_set == 2 && hie == +1) ret->SetTh23(kNuFitTh23HiNH);
    if (asimov_set == 1 && hie == -1) ret->SetTh23(kNuFitTh23LoIH);
    if (asimov_set == 2 && hie == -1) ret->SetTh23(kNuFitTh23HiIH);
    if (asimov_set == 3) ret->SetTh23(kNuFitTh23MM);
    if (asimov_set == 4 && hie == +1) ret->SetTh23(kNuFitTh23MinNH);
    if (asimov_set == 4 && hie == -1) ret->SetTh23(kNuFitTh23MinIH);
    if (asimov_set == 5 && hie == +1) ret->SetTh23(kNuFitTh23MaxNH);
    if (asimov_set == 5 && hie == -1) ret->SetTh23(kNuFitTh23MaxIH);
    if (asimov_set == 6 && hie == +1) ret->SetTh13(kNuFitTh13MinNH);
    if (asimov_set == 6 && hie == -1) ret->SetTh13(kNuFitTh13MinIH);
    if (asimov_set == 7 && hie == +1) ret->SetTh13(kNuFitTh13MaxNH);
    if (asimov_set == 7 && hie == -1) ret->SetTh13(kNuFitTh13MaxIH);
    if (asimov_set == 8 && hie == +1) ret->SetDmsq32(kNuFitDmsq32MinNH);
    if (asimov_set == 8 && hie == -1) ret->SetDmsq32(kNuFitDmsq32MinIH);
    if (asimov_set == 9 && hie == +1) ret->SetDmsq32(kNuFitDmsq32MaxNH);
    if (asimov_set == 9 && hie == -1) ret->SetDmsq32(kNuFitDmsq32MaxIH);



    if (oct < 0) {
      double dum = TMath::Pi()/2 - ret->GetTh23();
      ret->SetTh23(dum);
    }
    
    return ret;
  }

  bool HasVar(std::vector<const IFitVar*> oscVars, std::string name){
    for(auto *s :oscVars ) if(s->ShortName() == name) return true;
    return false;
  }

  //----------------------------------------------------------------------
  osc::IOscCalculatorAdjustable* ThrownNuFitOscCalc(int hie, std::vector<const IFitVar*> oscVars, int asimov_set)
  {
    assert(hie == +1 || hie == -1);

    osc::IOscCalculatorAdjustable* ret = NuFitOscCalc(hie);//new osc::OscCalculatorPMNSOpt;

    // Throw 12 and rho within errors
    if (HasVar(oscVars, kFitRho.ShortName()))
      ret->SetRho(kEarthDensity*(1+0.02*gRandom->Gaus()));

    if (HasVar(oscVars, kFitDmSq21.ShortName()))
      ret->SetDmsq21(kNuFitDmsq21CV*(1+kNuFitDmsq21Err*gRandom->Gaus()));

    if (HasVar(oscVars, kFitSinSq2Theta12.ShortName()))
      ret->SetTh12(kNuFitTh12CV*(1+kNuFitTh12Err*gRandom->Gaus()));

    // Uniform throws within +/-3 sigma
    if(hie > 0){
      if (HasVar(oscVars, kFitDmSq32Scaled.ShortName())) {
        if (asimov_set ==8) {
	  ret->SetDmsq32(gRandom->Uniform(kNuFitDmsq32MinNH-3*kNuFitDmsq32ErrNH,
					  kNuFitDmsq32MinNH+3*kNuFitDmsq32ErrNH));
	}
        else if (asimov_set ==9) {
	  ret->SetDmsq32(gRandom->Uniform(kNuFitDmsq32MaxNH-3*kNuFitDmsq32ErrNH,
					  kNuFitDmsq32MaxNH+3*kNuFitDmsq32ErrNH));
	}
	else {
	  ret->SetDmsq32(gRandom->Uniform(kNuFitDmsq32CVNH-3*kNuFitDmsq32ErrNH,
					  kNuFitDmsq32CVNH+3*kNuFitDmsq32ErrNH));
	}
      }


      if (HasVar(oscVars, kFitSinSqTheta23.ShortName())) {
        if (asimov_set == 1) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23LoNH-3*kNuFitTh23ErrNH,
					kNuFitTh23LoNH+3*kNuFitTh23ErrNH));
	}
        else if (asimov_set ==2) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23HiNH-3*kNuFitTh23ErrNH,
					kNuFitTh23HiNH+3*kNuFitTh23ErrNH));
	}
        else if (asimov_set ==3) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23MM-3*kNuFitTh23ErrNH,
					kNuFitTh23MM+3*kNuFitTh23ErrNH));
	}
        else if (asimov_set ==4) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23MinNH-3*kNuFitTh23ErrNH,
					kNuFitTh23MinNH+3*kNuFitTh23ErrNH));
	}
        else if (asimov_set ==5) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23MaxNH-3*kNuFitTh23ErrNH,
					kNuFitTh23MaxNH+3*kNuFitTh23ErrNH));
	}
	else {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23CVNH-3*kNuFitTh23ErrNH,
					kNuFitTh23CVNH+3*kNuFitTh23ErrNH));
	}
      }
        
      if (HasVar(oscVars, kFitTheta13.ShortName())) {
        if (asimov_set ==6) {
	  ret->SetTh13(gRandom->Uniform(kNuFitTh13MinNH-3*kNuFitTh13ErrNH,
					kNuFitTh13MinNH+3*kNuFitTh13ErrNH));
	}
        else if (asimov_set ==7) {
	  ret->SetTh13(gRandom->Uniform(kNuFitTh13MaxNH-3*kNuFitTh13ErrNH,
					kNuFitTh13MaxNH+3*kNuFitTh13ErrNH));
	}
	else {
	  ret->SetTh13(gRandom->Uniform(kNuFitTh13CVNH-3*kNuFitTh13ErrNH,
					kNuFitTh13CVNH+3*kNuFitTh13ErrNH));
	}

      }

      if (HasVar(oscVars, kFitDeltaInPiUnits.ShortName()))
        ret->SetdCP(gRandom->Uniform(-1*TMath::Pi(), TMath::Pi()));

    } else {
      if (HasVar(oscVars, kFitDmSq32Scaled.ShortName())) {
        if (asimov_set ==8) {
	  ret->SetDmsq32(gRandom->Uniform(kNuFitDmsq32MinIH-3*kNuFitDmsq32ErrIH,
					  kNuFitDmsq32MinIH+3*kNuFitDmsq32ErrIH));
	}
        else if (asimov_set ==9) {
	  ret->SetDmsq32(gRandom->Uniform(kNuFitDmsq32MaxIH-3*kNuFitDmsq32ErrIH,
					  kNuFitDmsq32MaxIH+3*kNuFitDmsq32ErrIH));
	}
	else {
	  ret->SetDmsq32(gRandom->Uniform(kNuFitDmsq32CVIH-3*kNuFitDmsq32ErrIH,
					kNuFitDmsq32CVIH+3*kNuFitDmsq32ErrIH));
	}
      }
      if (HasVar(oscVars, kFitSinSqTheta23.ShortName())) {
        if (asimov_set == 1) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23LoIH-3*kNuFitTh23ErrIH,
					kNuFitTh23LoIH+3*kNuFitTh23ErrIH));
	}
        else if (asimov_set ==2) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23HiIH-3*kNuFitTh23ErrIH,
					kNuFitTh23HiIH+3*kNuFitTh23ErrIH));
	}
        else if (asimov_set ==3) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23MM-3*kNuFitTh23ErrIH,
					kNuFitTh23MM+3*kNuFitTh23ErrIH));
	}
        else if (asimov_set ==4) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23MinIH-3*kNuFitTh23ErrIH,
					kNuFitTh23MinIH+3*kNuFitTh23ErrIH));
	}
        else if (asimov_set ==5) {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23MaxIH-3*kNuFitTh23ErrIH,
					kNuFitTh23MaxIH+3*kNuFitTh23ErrIH));
	}
	else {
	  ret->SetTh23(gRandom->Uniform(kNuFitTh23CVIH-3*kNuFitTh23ErrIH,
					kNuFitTh23CVIH+3*kNuFitTh23ErrIH));
	}
      }

      if (HasVar(oscVars, kFitTheta13.ShortName())) {
        if (asimov_set ==6) {
	  ret->SetTh13(gRandom->Uniform(kNuFitTh13MinIH-3*kNuFitTh13ErrIH,
					kNuFitTh13MinIH+3*kNuFitTh13ErrIH));
	}
        else if (asimov_set ==7) {
	  ret->SetTh13(gRandom->Uniform(kNuFitTh13MaxIH-3*kNuFitTh13ErrIH,
					kNuFitTh13MaxIH+3*kNuFitTh13ErrIH));
	}
	else {
	  ret->SetTh13(gRandom->Uniform(kNuFitTh13CVIH-3*kNuFitTh13ErrIH,
					kNuFitTh13CVIH+3*kNuFitTh13ErrIH));
	}
      }

      if (HasVar(oscVars, kFitDeltaInPiUnits.ShortName()))
	ret->SetdCP(gRandom->Uniform(-1*TMath::Pi(), TMath::Pi()));
    }
    return ret;
  }

  //----------------------------------------------------------------------
  osc::IOscCalculatorAdjustable* NuFitOscCalcPlusOneSigma(int hie)
  {
    assert(hie == +1 || hie == -1);

    osc::IOscCalculatorAdjustable* ret = new osc::OscCalculatorPMNSOpt;
    ret->SetL(kBaseline);
    ret->SetRho(kEarthDensity);

    ret->SetDmsq21(kNuFitDmsq21CV + kNuFitDmsq21Err);
    ret->SetTh12(kNuFitTh12CV + kNuFitTh12Err);

    if(hie > 0){
      ret->SetDmsq32(kNuFitDmsq32CVNH + kNuFitDmsq32ErrNH);
      ret->SetTh23(kNuFitTh23CVNH + kNuFitTh23ErrNH);
      ret->SetTh13(kNuFitTh13CVNH + kNuFitTh13ErrNH);
    }
    else{
      ret->SetDmsq32(kNuFitDmsq32CVIH + kNuFitDmsq32ErrIH);
      ret->SetTh23(kNuFitTh23CVIH + kNuFitTh23ErrIH);
      ret->SetTh13(kNuFitTh13CVIH + kNuFitTh13ErrIH);
    }

    ret->SetdCP(0); // a little ambiguous in the instructions

    return ret;
  }

  //----------------------------------------------------------------------
  double NuFitPenalizer::ChiSq(osc::IOscCalculatorAdjustable* calc,
                               const SystShifts& /*syst*/) const
  {
    double ret =
      util::sqr((calc->GetDmsq21() - kNuFitDmsq21CV)/kNuFitDmsq21Err) +
      util::sqr((calc->GetTh12() - kNuFitTh12CV)/kNuFitTh12Err);

    if(calc->GetDmsq32() > 0){
      ret +=
        util::sqr((calc->GetDmsq32() - kNuFitDmsq32CVNH)/kNuFitDmsq32ErrNH) +
        util::sqr((calc->GetTh23() - kNuFitTh23CVNH)/kNuFitTh23ErrNH) +
        util::sqr((calc->GetTh13() - kNuFitTh13CVNH)/kNuFitTh13ErrNH);
    }
    else{
      ret +=
        util::sqr((calc->GetDmsq32() - kNuFitDmsq32CVIH)/kNuFitDmsq32ErrIH) +
        util::sqr((calc->GetTh23() - kNuFitTh23CVIH)/kNuFitTh23ErrIH) +
        util::sqr((calc->GetTh13() - kNuFitTh13CVIH)/kNuFitTh13ErrIH);
    }

    // No term in delta

    return ret;
  }

  //----------------------------------------------------------------------
  Penalizer_GlbLike::Penalizer_GlbLike(int hietrue, int octtrue, bool useTh13, 
				       bool useTh23, bool useDmsq, int asimov_set) 
    : fTh13Pen(useTh13),fTh23Pen(useTh23), fDmsqPen(useDmsq){

    fDmsq21 = kNuFitDmsq21CV;
    fTh12 = kNuFitTh12CV;
    fRho = kEarthDensity;


    fDmsq32 = (hietrue > 0) ? kNuFitDmsq32CVNH : kNuFitDmsq32CVIH;
    fTh13 = (hietrue > 0) ? kNuFitTh13CVNH : kNuFitTh13CVIH;
    fTh23 = (hietrue > 0) ? kNuFitTh23CVNH : kNuFitTh23CVIH;

    if (asimov_set == 1) {
      fTh23 = (hietrue > 0) ? kNuFitTh23LoNH : kNuFitTh23LoIH;
    }
    else if (asimov_set == 2) {
      fTh23 = (hietrue > 0) ? kNuFitTh23HiNH : kNuFitTh23HiIH;
    }
    else if (asimov_set == 3) {
      fTh23 = kNuFitTh23MM;
    }
    else if (asimov_set == 4) {
      fTh23 = (hietrue > 0) ? kNuFitTh23MinNH : kNuFitTh23MinIH;
    }
    else if (asimov_set == 5) {
      fTh23 = (hietrue > 0) ? kNuFitTh23MaxNH : kNuFitTh23MaxIH;
    }
    else if (asimov_set == 6) {
      fTh13 = (hietrue > 0) ? kNuFitTh13MinNH : kNuFitTh13MinIH;
    }
    else if (asimov_set == 7) {
      fTh13 = (hietrue > 0) ? kNuFitTh13MaxNH : kNuFitTh13MaxIH;
    }
    else if (asimov_set == 8) {
      fDmsq32 = (hietrue > 0) ? kNuFitDmsq32MinNH : kNuFitDmsq32MinIH;
    }
    else if (asimov_set == 9) {
      fDmsq32 = (hietrue > 0) ? kNuFitDmsq32MaxNH : kNuFitDmsq32MaxIH;
    }

    // This is sort of an assumption...
    if (octtrue < 0) fTh23 = TMath::Pi()/2 - fTh23;

    fDmsq21Err = kNuFitDmsq21Err;
    fTh12Err = kNuFitTh12Err;
    fDmsq32Err = (hietrue > 0) ? kNuFitDmsq32ErrNH : kNuFitDmsq32ErrIH;
    fTh13Err = (hietrue > 0) ? kNuFitTh13ErrNH : kNuFitTh13ErrIH;
    fTh23Err = (hietrue > 0) ? kNuFitTh23ErrNH : kNuFitTh23ErrIH;

    fRhoErr = 0.02*kEarthDensity;

  }

  Penalizer_GlbLike::Penalizer_GlbLike(osc::IOscCalculatorAdjustable* cvcalc, 
				       int hietrue, bool useTh13,
                                       bool useTh23, bool useDmsq) 
    : fTh13Pen(useTh13), fTh23Pen(useTh23), fDmsqPen(useDmsq){

    fDmsq21 = cvcalc->GetDmsq21();
    fTh12 = cvcalc->GetTh12();
    fDmsq32 = cvcalc->GetDmsq32();
    fTh23 = cvcalc->GetTh23();
    fTh13 = cvcalc->GetTh13();
    fRho = cvcalc->GetRho();

    fDmsq21Err = kNuFitDmsq21Err;
    fTh12Err = kNuFitTh12Err;
    fDmsq32Err = (hietrue > 0) ? kNuFitDmsq32ErrNH : kNuFitDmsq32ErrIH;
    fTh13Err = (hietrue > 0) ? kNuFitTh13ErrNH : kNuFitTh13ErrIH;
    fTh23Err = (hietrue > 0) ? kNuFitTh23ErrNH : kNuFitTh23ErrIH;

    fRhoErr = 0.02*kEarthDensity;
  }


  double Penalizer_GlbLike::ChiSq(osc::IOscCalculatorAdjustable* calc,
				  const SystShifts& /*syst*/) const {

  //Usage: calc is the fit parameters as above
  //Starting fit parameters and errors are set in constructor - this is equivalent to SetCentralValues in globes

    double ret =
      util::sqr((calc->GetDmsq21() - fDmsq21)/fDmsq21Err) +
      util::sqr((calc->GetTh12() - fTh12)/fTh12Err) +
      util::sqr((calc->GetRho() - fRho)/fRhoErr);

    if (fDmsqPen) ret += util::sqr((calc->GetDmsq32() - fDmsq32)/fDmsq32Err);
    if (fTh23Pen) ret += util::sqr((calc->GetTh23() - fTh23)/fTh23Err);
    if (fTh13Pen) ret += util::sqr((calc->GetTh13() - fTh13)/fTh13Err);

    //std::cout << calc->GetTh23() << " " << fTh23 << " " << fTh23Err << " " << ret << std::endl;

    return ret;
  }


}
