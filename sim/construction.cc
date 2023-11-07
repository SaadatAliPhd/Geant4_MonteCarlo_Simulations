#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    nCols=10;
    nRows=10;
    fMessenger=new G4GenericMessenger(this,"/detector/","DetectorConstruction");

    fMessenger->DeclareProperty("nCols",nCols,"Number of columns");
    fMessenger->DeclareProperty("nRows",nRows,"Number of rows");
    fMessenger->DeclareProperty("isCherenkov",isCherenkov,"Toggle Cherenkove Setup");
    fMessenger->DeclareProperty("isScintillator",isScintillator,"Toggle Scintillator Setup");
    DefineMaterials();
    xWorld=0.5*m;
    yWorld=0.5*m;
    zWorld=0.5*m;
    isCherenkov=false;
    isScintillator=false;
}
MyDetectorConstruction::~MyDetectorConstruction()
{}
void MyDetectorConstruction::DefineMaterials()
{
G4NistManager *nist=G4NistManager::Instance();
// building silicon dioxide material
SiO2= new G4Material("SiO2",2.2018*g/cm3,2);
SiO2->AddElement(nist->FindOrBuildElement("Si"),1);
SiO2->AddElement(nist->FindOrBuildElement("O"),2);
// building water material
H2O= new G4Material("H2O",1.000*g/cm3,2);
H2O->AddElement(nist->FindOrBuildElement("H"),2);
H2O->AddElement(nist->FindOrBuildElement("O"),1);
// defining element Carbon in this case
C=nist->FindOrBuildElement("C");
// defining aerogel material by mixing above materials siO2 + H2O
Aerogel=new G4Material("Aerogel",0.200*g/cm3,3);
Aerogel->AddMaterial(SiO2,62.5*perCent);
Aerogel->AddMaterial(H2O,37.4*perCent);
Aerogel->AddElement(C,0.1*perCent);
worldMat=nist->FindOrBuildMaterial("G4_AIR");

// defining refractive index for cherenkove radiations,divided by 0.2 for blue light and 0.9 for red light
G4double energy[2]={1.239841939*eV/0.2,1.239841939*eV/0.9};
G4double rindexAerogel[2]={1.1,1.1};
// defining refrective index of motherVolume mptWorld
G4double rindexWorld[2]={1.0,1.0};
G4MaterialPropertiesTable *mptAerogel= new G4MaterialPropertiesTable();
// adding a propert to a material,refractive index in this case
mptAerogel->AddProperty("RINDEX",energy,rindexAerogel,2);
G4MaterialPropertiesTable *mptWorld=new G4MaterialPropertiesTable();
mptWorld->AddProperty("RINDEX",energy,rindexWorld,2);
Aerogel->SetMaterialPropertiesTable(mptAerogel);
worldMat->SetMaterialPropertiesTable(mptWorld);
Na=nist->FindOrBuildElement("Na");
I=nist->FindOrBuildElement("I");
NaI= new G4Material("NaI",3.67*g/cm3,2);
NaI->AddElement(Na,1);
NaI->AddElement(I,1);

}
void MyDetectorConstruction::ConstructScintillator()
{
    solidScintillator = new G4Tubs("solidScintillator", 10*cm, 20*cm, 30*cm,0*deg,360*deg);

    logicScintillator = new G4LogicalVolume(solidScintillator, NaI, "logicalScintillator");

    // G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface);

    fScoringVolume = logicScintillator;
    physScintillator = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicScintillator,"physScintillator",logicWorld,false,0,true);

 }
    

void MyDetectorConstruction::ConstructCherenkov()
{
    solidRadiator=new G4Box("solidRadiator",0.4*m,0.4*m,0.01*m);
    logicRadiator=new G4LogicalVolume(solidRadiator,Aerogel,"logicRadiator");
    fScoringVolume= logicRadiator;
    physRadiator =new G4PVPlacement(0,G4ThreeVector(0.,0.,0.25*m),logicRadiator,"physRadiator",logicWorld,false,0,true);
// creating a sensitive detector,photodetector for cerenkove light 
    solidDetector =new G4Box("solidDetector",xWorld/nRows,yWorld/nCols,0.01*m);
// logicDetector defined in construction.hh
    logicDetector=new G4LogicalVolume(solidDetector,worldMat,"logicDetector") ; 
// creating array of  detectors using for loop
    for (G4int i=0;i<nRows;i++)

    {
    for (G4int j=0; j<nCols;j++)
    {
        physDetector=new G4PVPlacement(0,G4ThreeVector(-0.5*m+(i+0.5)*m/nRows,-0.5*m+(j+0.5)*m/nCols,0.49*m),logicDetector,"physDetector",logicWorld,false,j+i*nCols,true);

        }
    }   
 }

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{

    solidWorld = new G4Box("solidWorld",xWorld,yWorld,zWorld);

    logicWorld=new G4LogicalVolume(solidWorld,worldMat,"logicWorld");

    physWorld=new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"physWorld",0,false,0,true);

// to make this detector sensitive to detect cerenkove light check files detector.hh and detector.cc
if( isCherenkov)
    ConstructCherenkov();
 if(isScintillator)
        ConstructScintillator();

return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
   MySensitiveDetector *sensDet= new MySensitiveDetector("SensitiveDetector");
   if( isCherenkov)
        logicDetector->SetSensitiveDetector(sensDet);
}
