#include "PenelopeToPMT.hh"
#include "Types.hh"
#include "strutils.hh"

void PenelopeToPMT::setReadpoints() {
	
	for(Side s = EAST; s <= WEST; ++s) {
		Tch->SetBranchAddress(sideSubst("Ep%c",s,true).c_str(),&fEdep[s]);
		Tch->SetBranchAddress(sideSubst("nph%c",s,true).c_str(),&fEquench[s]);
		Tch->SetBranchAddress(sideSubst("Ep%cd",s,true).c_str(),&fedepDeadScint[s]);
		
		Tch->SetBranchAddress(sideSubst("Eg%ca",s,true).c_str(),&fEW[s]);
		Tch->SetBranchAddress(sideSubst("Eg%cdf",s,true).c_str(),&fEMWPCDead[s][0]);
		Tch->SetBranchAddress(sideSubst("Eg%cdb",s,true).c_str(),&fEMWPCDead[s][1]);
		
		Tch->SetBranchAddress(sideSubst("E%c1",s,true).c_str(),&fEMWPCWires[s][0]);
		Tch->SetBranchAddress(sideSubst("E%c2",s,true).c_str(),&fEMWPCWires[s][1]);
		Tch->SetBranchAddress(sideSubst("E%an",s,true).c_str(),&fEMWPCWires[s][2]);
		
		Tch->SetBranchAddress(sideSubst("Em%c1",s,true).c_str(),&fedepWinIn[s]);
		Tch->SetBranchAddress(sideSubst("Em%c2",s,true).c_str(),&fedepWinOut[s]);
		
		for(AxisDirection d=X_DIRECTION; d<=X_DIRECTION; ++d)
			Tch->SetBranchAddress((sideSubst("%cpos",s,true)+(d==X_DIRECTION?"x":"y")).c_str(),&fMWPCpos[s][d]);
		
		Tch->SetBranchAddress(sideSubst("Efl%c",s,true).c_str(),&fedepFoils[s]);
		Tch->SetBranchAddress(sideSubst("t%c",s,true).c_str(),&fTime[s]);
	}
	
	for(AxisDirection d=X_DIRECTION; d<=Z_DIRECTION; ++d)
		Tch->SetBranchAddress(d==X_DIRECTION?"X":d==Y_DIRECTION?"Y":"Z",&fPrimPos[d]);
		
	Tch->SetBranchAddress("E",&fEprim);
	Tch->SetBranchAddress("W",&fCostheta);
}

void PenelopeToPMT::doUnits() {
	const double posConversion = 10.0;
	for(AxisDirection d=X_DIRECTION; d<=Z_DIRECTION; ++d) {
		primPos[d] = fPrimPos[d]*posConversion;
		for(Side s = EAST; s <= WEST; ++s)
			mwpcPos[s][d] = fMWPCpos[s][d]*posConversion*sqrt(0.6);
	}
	for(Side s = EAST; s <= WEST; ++s) {
		eDep[s] = fEdep[s]*0.001;
		eQ[s] = fEquench[s];
		eW[s] = fEW[s]*0.001;
		time[s] = fTime[s]*1e-9; // conver from nanoseconds to seconds
		for(AxisDirection d=X_DIRECTION; d<=Y_DIRECTION; ++d)
			scintPos[s][d] = mwpcPos[s][d];	// fake scintillator pos from MWPC
	}
	ePrim = fEprim*0.001;
	costheta = fCostheta;
	for(Side s = EAST; s <= WEST; ++s) {
		edepWires[s] = (fEMWPCWires[s][0]+fEMWPCWires[s][1]+fEMWPCWires[s][2])*0.001;
		edepDeadMWPC[s] = fEMWPCDead[s][0]*0.001 + fEMWPCDead[s][1]*0.001;
		edepFoils[s]  = fedepFoils[s]*0.001;
		edepWinOut[s] = fedepWinOut[s]*0.001;
		edepWinIn[s]  = fedepWinIn[s]*0.001;
		edepDeadScint[s] = fedepDeadScint[s]*0.001;
		/*
		 need to get these in the simulation code
		 double cosThetaInFoils[2];
		 double cosThetaInWinOut[2];
		 double cosThetaInWinIn[2];
		 double cosThetaInScint[2];
		 
		 double cosThetaOutFoils[2];
		 double cosThetOutWinOut[2];
		 double cosThetaOutWinIn[2];
		 double cosThetaOutScint[2];
		 */
	}
}
