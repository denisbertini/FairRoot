/*
 * FairMCPointsDraw.cpp
 *
 *  Created on: Apr 17, 2009
 *      Author: stockman
 */

#include "FairHitPointSetDraw.h"

#include "FairHit.h"                    // for FairHit

#include "Riosfwd.h"                    // for ostream
#include "TVector3.h"                   // for TVector3

#include <iostream>                     // for operator<<, basic_ostream, etc

using std::cout;
using std::endl;

FairHitPointSetDraw::FairHitPointSetDraw()
{
  // TODO Auto-generated constructor stub

}

FairHitPointSetDraw::~FairHitPointSetDraw()
{
  // TODO Auto-generated destructor stub
}

TVector3 FairHitPointSetDraw::GetVector(TObject* obj)
{
  FairHit* p = (FairHit*)obj;
  if (fVerbose > 1) {
    cout << "-I- FairHitPointSetDraw::GetVector: " << p->GetX() << " " << p->GetY() << " " << p->GetZ() << endl;
  }
  return TVector3(p->GetX(), p->GetY(), p->GetZ());
}


ClassImp(FairHitPointSetDraw)
