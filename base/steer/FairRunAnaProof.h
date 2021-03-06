#ifndef FAIRRUNANAPROOF_H
#define FAIRRUNANAPROOF_H

/**
 * Configure and manage the  Analysis on proof
 * @author R. Karabowicz
 * @version 0.1
 * @since 30.04.2013
 */

#include "FairRunAna.h"

#include "TProof.h"
#include <iostream>

class FairRunAnaProof : public FairRunAna
{

  public:

    static FairRunAnaProof* Instance();
    virtual ~FairRunAnaProof();
    FairRunAnaProof(const char* proofName="");

    /**initialize the run manager*/
    void        Init();

    /**Run from event number NStart to event number NStop */
    void        Run(Int_t NStart ,Int_t NStop);
    /**Run for one event, used on PROOF nodes*/
    void        RunOneEvent(Long64_t entry);
    /**Run on proof from event NStart to event NStop*/
    void        RunOnProof(Int_t NStart, Int_t NStop);

    /** set the input tree of fRootManager when running on PROOF worker*/
    void        SetInTree (TTree* tempTree)   {
      fRootManager->SetInTree (tempTree);
    }

    /** GetProof */
    TProof* GetProof() {
      return fProof;
    }

    /** To be set to kTRUE only when running on PROOF worker, only by TSelector */
    void SetRunOnProofWorker(Bool_t tb = kTRUE) {
      fRunOnProofWorker = tb;
    }
    /** Set PROOF ARchive (PAR) file name*/
    void SetProofParName(TString parName) {
      fProofParName = parName;
    }
    /** Set directory for storing output files*/
    void SetOutputDirectory(TString dirName) {
      fOutputDirectory = dirName;
    }
    /** Set PROOF output status, possibilities: "copy","merge","dataset"*/
    void SetProofOutputStatus(TString outStat) {
      fProofOutputStatus = outStat;
    }

  protected:
    static FairRunAnaProof*                      fRAPInstance;

    /** PROOF **/
    TProof*                                 fProof;
    /** executing on PROOF worker*/
    Bool_t                                  fRunOnProofWorker; //!
    /** PROOF server name*/
    TString                                 fProofServerName; //!
    /** PROOF ARchive (PAR) file name*/
    TString                                 fProofParName; //!
    /** Output directory*/
    TString                                 fOutputDirectory; //!
    /** Output status indicator: "copy","merge","dataset"*/
    TString                                  fProofOutputStatus;

    ClassDef(FairRunAnaProof ,1)

};

#endif //FAIRRUNANAPROOF_H
