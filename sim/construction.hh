#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH
#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "detector.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4GenericMessenger.hh"


class MyDetectorConstruction: public G4VUserDetectorConstruction
{
public:
     MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

    virtual G4VPhysicalVolume *Construct();
private:
    G4Box *solidWorld,*solidRadiator,*solidDetector;
    G4Tubs *solidScintillator;
    G4LogicalVolume *logicWorld,*logicRadiator,*logicDetector,*logicScintillator;
    G4VPhysicalVolume *physWorld,*physRadiator,*physDetector,*physScintillator;
    G4Material *SiO2,*H2O,*Aerogel,*worldMat,*NaI;
    G4Element *C,*Na,*I;
   void DefineMaterials(); 

   void ConstructCherenkov();
   void ConstructScintillator();
    virtual void ConstructSDandField();
    G4GenericMessenger *fMessenger;
    G4LogicalVolume *fScoringVolume;
    G4int nCols,nRows;
    G4double xWorld,yWorld,zWorld;
    G4bool isCherenkov, isScintillator;
};
#endif