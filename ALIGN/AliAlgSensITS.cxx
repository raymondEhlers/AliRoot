/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#include "AliAlgSensITS.h"
#include "AliAlgAux.h"
#include "AliLog.h"
#include "AliTrackPointArray.h"
#include "AliESDtrack.h"
#include "AliAlgPoint.h"
#include "AliAlgDet.h"

ClassImp(AliAlgSensITS)

using namespace AliAlgAux;
using namespace TMath;

//_________________________________________________________
AliAlgSensITS::AliAlgSensITS(const char* name,Int_t vid, Int_t iid) 
  : AliAlgSens(name,vid,iid)
{
  // def c-tor
}

//_________________________________________________________
AliAlgSensITS::~AliAlgSensITS()
{
  // d-tor
}
/*
//__________________________________________________________________
void AliAlgSensITS::SetTrackingFrame()
{
  // define tracking frame of the sensor
  double tra[3]={0},loc[3],glo[3];
  // ITS defines tracking frame with origin in sensor, others at 0
  GetMatrixT2L().LocalToMaster(tra,loc);
  GetMatrixL2GIdeal().LocalToMaster(loc,glo);
  fX = Sqrt(glo[0]*glo[0]+glo[1]*glo[1]);
  fAlp = ATan2(glo[1],glo[0]);
  AliAlgAux::BringToPiPM(fAlp);
}
*/

//____________________________________________
AliAlgPoint* AliAlgSensITS::TrackPoint2AlgPoint(int pntId, const AliTrackPointArray* trpArr, const AliESDtrack*)
{
  // convert the pntId-th point to AliAlgPoint
  //
  AliAlgDet* det = GetDetector();
  AliAlgPoint* pnt = det->GetPointFromPool();
  pnt->SetSensor(this);
  //
  double tra[3],locId[3],loc[3],
    glo[3] = {trpArr->GetX()[pntId], trpArr->GetY()[pntId], trpArr->GetZ()[pntId]};
  const TGeoHMatrix& matL2Grec = GetMatrixL2GReco(); // local to global matrix used for reconstruction
  const TGeoHMatrix& matT2L    = GetMatrixT2L();     // matrix for tracking to local frame translation
  //
  // undo reco-time alignment
  matL2Grec.MasterToLocal(glo,locId); // go to local frame using reco-time matrix, here we recover ideal measurement 
  //
  GetMatrixClAlg().LocalToMaster(locId,loc);   // apply alignment
  //
  matT2L.MasterToLocal(loc,tra);  // go to tracking frame 
  //
  /*
  double gloT[3]; 
  TGeoHMatrix t2g;
  GetMatrixT2G(t2g); t2g.LocalToMaster(tra,gloT);
  printf("\n%5d %s\n",GetVolID(), GetSymName());
  printf("GloOR: %+.4e %+.4e %+.4e\n",glo[0],glo[1],glo[2]);
  printf("LocID: %+.4e %+.4e %+.4e\n",locId[0],locId[1],locId[2]);
  printf("LocML: %+.4e %+.4e %+.4e\n",loc[0],loc[1],loc[2]);
  printf("Tra  : %+.4e %+.4e %+.4e\n",tra[0],tra[1],tra[2]);
  printf("GloTR: %+.4e %+.4e %+.4e\n",gloT[0],gloT[1],gloT[2]);
  */
  //
  if (!det->GetUseErrorParam()) {
    // convert error
    TGeoHMatrix hcov;
    Double_t hcovel[9];
    const Float_t *pntcov = trpArr->GetCov()+pntId*6; // 6 elements per error matrix
    hcovel[0] = double(pntcov[0]);
    hcovel[1] = double(pntcov[1]);
    hcovel[2] = double(pntcov[2]);
    hcovel[3] = double(pntcov[1]);
    hcovel[4] = double(pntcov[3]);
    hcovel[5] = double(pntcov[4]);
    hcovel[6] = double(pntcov[2]);
    hcovel[7] = double(pntcov[4]);
    hcovel[8] = double(pntcov[5]);
    hcov.SetRotation(hcovel);
    hcov.Multiply(&matL2Grec);                
    hcov.MultiplyLeft(&matL2Grec.Inverse());    // errors in local frame
    hcov.Multiply(&matT2L);
    hcov.MultiplyLeft(&matT2L.Inverse());       // errors in tracking frame
    //
    Double_t *hcovscl = hcov.GetRotationMatrix();
    const double *sysE = GetAddError(); // additional syst error
    pnt->SetYZErrTracking(hcovscl[4]+sysE[0]*sysE[0],hcovscl[5],hcovscl[8]+sysE[1]*sysE[1]);
  }
  else { // errors will be calculated just before using the point in the fit, using track info
    pnt->SetYZErrTracking(0,0,0);
    pnt->SetNeedUpdateFromTrack();
  }
  pnt->SetXYZTracking(tra[0],tra[1],tra[2]);
  pnt->SetAlphaSens(GetAlpTracking());
  pnt->SetXSens(GetXTracking());
  pnt->SetDetID(det->GetDetID());
  pnt->SetSID(GetSID());
  //
  pnt->SetContainsMeasurement();
  //
  pnt->Init();
  //
  return pnt;
  //
}
