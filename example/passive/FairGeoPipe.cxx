//*-- AUTHOR : Ilse Koenig
//*-- Created : 10/11/2003

/////////////////////////////////////////////////////////////
// FairGeoPipe
//
// Class for geometry of support structure
//
/////////////////////////////////////////////////////////////
#include "FairGeoPipe.h"

#include "FairGeoInterface.h"           // for FairGeoInterface
#include "FairGeoLoader.h"              // for FairGeoLoader

#include "TList.h"                      // for TList
#include "TString.h"                    // for TString

#include <string.h>                     // for strcpy

class FairGeoBuilder;
class TObject;

ClassImp(FairGeoPipe)

FairGeoPipe::FairGeoPipe()
  : FairGeoSet()
{
  // Constructor
  fName="pipe";
  strcpy(modName,"p");
  strcpy(eleName,"p");
  maxSectors=0;
  maxModules=1;
}
Bool_t  FairGeoPipe::create ( FairGeoBuilder* build )
{
  Bool_t rc = FairGeoSet::create( build );
  if ( rc ) {
    FairGeoLoader* loader=FairGeoLoader::Instance();
    FairGeoInterface* GeoInterface =loader->getGeoInterface();

    GeoInterface->getMasterNodes()->Add( (TObject*)  getVolume("pipeCentral") );
  }
  return rc;
}

FairGeoPipe::~FairGeoPipe()
{

}

