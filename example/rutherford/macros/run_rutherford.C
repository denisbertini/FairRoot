void run_rutherford(Int_t nEvents = 10)
{
  
  TString dir = gSystem->Getenv("VMCWORKDIR");
  TString tutdir = dir + "/rutherford/macros";

  TString tut_geomdir = dir + "/geometry";
  gSystem->Setenv("GEOMPATH",tut_geomdir.Data());

  TString tut_configdir = dir + "/gconfig";
  gSystem->Setenv("CONFIG_DIR",tut_configdir.Data());

  TString outDir  = "data";
  TString outFile = outDir + "/test.mc.root";
  TString parFile = outDir + "/params.root";

  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

   FairLogger *logger = FairLogger::GetLogger();
  // define log file name
  logger->SetLogFileName("MyLog.log");
  // log to screen and to file 
  logger->SetLogToScreen(kTRUE);
  logger->SetLogToFile(kTRUE);
  // Print very accurate output. Levels are LOW, MEDIUM and HIGH
  logger->SetLogVerbosityLevel("HIGH");
 
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");              // Transport engine
  run->SetOutputFile(outFile);          // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------

  run->SetWriteRunInfoFile(kFALSE);  
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------
  
  // -----   Create geometry   ----------------------------------------------

  FairModule* cave= new FairCave("CAVE");
  cave->SetGeometryFileName("cave_vacuum.geo"); 
  run->AddModule(cave);

  FairModule* target= new FairTarget("Target");
  target->SetGeometryFileName("target_rutherford.geo"); 
  run->AddModule(target);

  FairDetector* rutherford = new FairRutherford("RutherfordDetector", kTRUE);
  rutherford->SetGeometryFileName("rutherford.geo"); 
  run->AddModule(rutherford);
  // ------------------------------------------------------------------------

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  run->SetGenerator(primGen);


  // Ion Generator
                                               
  FairIonGenerator *fIongen= new FairIonGenerator(2, 4, 2, 1, 0., 0., 1./20., 0., 0., -1.); 
  primGen->AddGenerator(fIongen);


  FairBoxGenerator* boxGen1 = new FairBoxGenerator(2212, 1);
  boxGen1->SetPRange(.005,.005);
  boxGen1->SetPhiRange(0.,0.);
  boxGen1->SetThetaRange(0.,0.);
  boxGen1->SetXYZ(0.,0.,-3.);
  primGen->AddGenerator(boxGen1);


  // ------------------------------------------------------------------------

  run->SetStoreTraj(kTRUE);

  // -----   Run initialisation   -------------------------------------------
  run->Init();
  // ------------------------------------------------------------------------
  
  // Set cuts for storing the trajectories.
  // Switch this on only if trajectories are stored.
  // Choose this cuts according to your needs, but be aware
  // that the file size of the output file depends on these cuts

  FairTrajFilter* trajFilter = FairTrajFilter::Instance();
 // trajFilter->SetStepSizeCut(0.01); // 1 cm
  // trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
  // trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
  // trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
  trajFilter->SetStorePrimaries(kTRUE);
  trajFilter->SetStoreSecondaries(kTRUE);
  // ------------------------------------------------------------------------

  // -----   Runtime database   ---------------------------------------------

  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------
   
  // -----   Start run   ----------------------------------------------------
  run->Run(nEvents);
  // ------------------------------------------------------------------------
  run->CreateGeometryFile("data/geofile_full.root");
  
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime 
       << "s" << endl << endl;
  // ------------------------------------------------------------------------
}


