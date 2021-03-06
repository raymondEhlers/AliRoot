#ifndef ALIEMCALQADATAMAKERSIM_H
#define ALIEMCALQADATAMAKERSIM_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//______________________________________________________________
/// \class AliEMCALQADataMakerSim
/// \ingroup EMCALsim
/// \brief Simulation QA output maker
///
/// Produces the data needed to calculate the quality assurance. 
/// All data must be mergeable objects.
///
/// Based on PHOS code 
/// \author  Yyves Schutz CERN July 2007
//______________________________________________________________

// --- ROOT system ---
class TH1F ; 
class TH1I ; 
class TObjArray ; 

// --- Standard library ---

// --- AliRoot header files ---
#include "AliQADataMakerSim.h"

class AliEMCALQADataMakerSim: public AliQADataMakerSim 
{

public:
  
  AliEMCALQADataMakerSim() ;          // ctor
  AliEMCALQADataMakerSim(const AliEMCALQADataMakerSim& qadm) ;   
  AliEMCALQADataMakerSim& operator = (const AliEMCALQADataMakerSim& qadm) ;
  virtual ~AliEMCALQADataMakerSim() {;} // dtor
  
private:
  
  virtual void   EndOfDetectorCycle(AliQAv1::TASKINDEX_t, TObjArray ** list) ;
  virtual void   InitHits() ; 
  virtual void   InitDigits() ; 
  virtual void   InitSDigits() ; 
  virtual void   MakeHits() ;
  virtual void   MakeHits(TTree * hitTree) ;
  virtual void   MakeDigits() ; 
  virtual void   MakeDigits(TTree * digitTree) ; 
  virtual void   MakeSDigits() ; 
  virtual void   MakeSDigits(TTree * sigitTree) ; 
  virtual void   StartOfDetectorCycle() ; 

  /// \cond CLASSIMP
  ClassDef(AliEMCALQADataMakerSim,1) ;
  /// \endcond

};

#endif // AliEMCALQADATAMAKERSIM_H
