#ifndef FAIRGEOBASICSHAPE_H
#define FAIRGEOBASICSHAPE_H

#include "TNamed.h"                     // for TNamed

#include "Riosfwd.h"                    // for fstream
#include "Rtypes.h"                     // for Int_t, etc

#include <fstream>                      // for fstream

class FairGeoTransform;
class FairGeoVolume;
class TArrayD;
/**
 * base class for all shapes
 * @author Ilse koenig
 */
class FairGeoBasicShape : public TNamed
{
  protected:
    Int_t nPoints;            // number of points describing the shape
    Int_t nParam;             // number of parameters needed to create the ROOT shape
    TArrayD* param;           // parameters needed to create the ROOT shape
    FairGeoTransform* center;   // position of the volume center in the technical KS
    FairGeoTransform* position; // position of the ROOT volume in its ROOT mother
  public:
    FairGeoBasicShape();
    virtual ~FairGeoBasicShape();
    Int_t getNumPoints() {return nPoints;}
    Int_t getNumParam() {return nParam;}
    TArrayD* getParam() {return param;}
    FairGeoTransform* getCenterPosition() {return center;}
    FairGeoTransform* getVoluPosition() {return position;}
    virtual Int_t readPoints(fstream*,FairGeoVolume*);
    virtual Bool_t writePoints(fstream*,FairGeoVolume*);
    virtual void printPoints(FairGeoVolume* volu);
    virtual TArrayD* calcVoluParam(FairGeoVolume*) {return 0;}
    virtual void calcVoluPosition(FairGeoVolume*,const FairGeoTransform&);
    virtual void calcVoluPosition(FairGeoVolume*,const FairGeoTransform&,
                                  const FairGeoTransform&) {return;}
    void printParam();
  protected:
    void posInMother(const FairGeoTransform&,const FairGeoTransform&);
    ClassDef(FairGeoBasicShape,0) // base class for all shapes
  private:
    FairGeoBasicShape(const FairGeoBasicShape&);
    FairGeoBasicShape& operator=(const FairGeoBasicShape&);

};

#endif  /* !FAIRGEOBASICSHAPE_H */
