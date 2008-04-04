#ifndef ALITRDDIGITSMANAGER_H
#define ALITRDDIGITSMANAGER_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

/////////////////////////////////////////////////////////////
//  Manages the TRD digits                                 //
/////////////////////////////////////////////////////////////

#include <TObject.h>

class TFile;
class TTree;

class AliTRDsegmentArray;
class AliTRDdataArray;
class AliTRDdataArrayDigits;
class AliTRDdigit;
class AliTRDSignalIndex;

class AliTRDdigitsManager : public TObject {

 public:

  enum { kNDict = 3 };

  AliTRDdigitsManager();
  AliTRDdigitsManager(const AliTRDdigitsManager &m);
  virtual ~AliTRDdigitsManager();
  AliTRDdigitsManager &operator=(const AliTRDdigitsManager &m);

  virtual void                Copy(TObject &m) const;

  virtual void                CreateArrays();
  virtual void                ResetArrays();
  virtual Bool_t              BuildIndexes(Int_t det);

  virtual Bool_t              MakeBranch(TTree *tree);
  virtual Bool_t              ReadDigits(TTree *tree);
  virtual Bool_t              WriteDigits();

  virtual void                SetEvent(Int_t evt)             { fEvent           = evt;  };
  virtual void                SetSDigits(Int_t v = 1)         { fHasSDigits      = v;    };
  virtual void                SetUseDictionaries(Bool_t kval) { fUseDictionaries = kval; };

  virtual Bool_t              UsesDictionaries() const        { return fUseDictionaries; };
  virtual Bool_t              HasSDigits() const              { return fHasSDigits;      };
  static  Int_t               NDict()                         { return fgkNDict;         }; 

  virtual AliTRDsegmentArray *GetDigits() const               { return fDigits;          };
  virtual AliTRDsegmentArray *GetDictionary(Int_t i) const    { return fDictionary[i];   };

          AliTRDdigit        *GetDigit(Int_t row, Int_t col, Int_t time, Int_t det) const;
          Int_t               GetTrack(Int_t track, Int_t row, Int_t col
                                     , Int_t time, Int_t det) const;

          AliTRDdataArrayDigits    *GetDigits(Int_t det) const;
          AliTRDdataArray    *GetDictionary(Int_t det, Int_t i) const;

	  AliTRDSignalIndex  *GetIndexes(Int_t det);
	  TObjArray          *GetIndexes()                    { return fSignalIndexes;   };

	  void                RemoveDigits(Int_t det);
	  void                RemoveDictionaries(Int_t det);
	  void                ClearIndexes(Int_t det);

          Int_t               GetTrack(Int_t track, AliTRDdigit *digit) const;
          Short_t             GetDigitAmp(Int_t row, Int_t col, Int_t time, Int_t det) const;
					UChar_t             GetPadStatus(Int_t row, Int_t col, Int_t time, Int_t det) const;

 protected:

  static const Int_t  fgkNDict;            //  Number of track dictionary arrays

  Int_t               fEvent;              //  Event number
  TTree              *fTree;               //! Tree for the digits arrays

  AliTRDsegmentArray *fDigits;             //! Digits data array
  AliTRDsegmentArray *fDictionary[kNDict]; //! Track dictionary data array

  Bool_t              fHasSDigits;         //  Switch for the summable digits

  TObjArray          *fSignalIndexes;      //  Provides access to the active pads and tbins
  Bool_t              fUseDictionaries;    //  Use dictionaries or not (case of real data)

  ClassDef(AliTRDdigitsManager,7)          //  Manages the TRD digits

};

#endif
