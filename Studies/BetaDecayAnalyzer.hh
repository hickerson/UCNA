#ifndef BETADECAYANALYZER_HH
#define BETADECAYANALYZER_HH 1

#include "MuonAnalyzer.hh"
#include "PositionAnalyzer.hh"
#include "WirechamberGainAnalyzer.hh"
#include "AsymmetryAnalyzer.hh"
#include "SimAsymmetryAnalyzer.hh"

/// analyzer for beta decay data
class BetaDecayAnalyzer: public OctetAnalyzer {
public:
	/// constructor
	BetaDecayAnalyzer(OutputManager* pnt, const std::string& nm = "BetaDecayAnalyzer", const std::string& inflName = "");
	/// create a new instance of this object (cloning self settings) for given directory
	virtual SegmentSaver* makeAnalyzer(const std::string& nm, const std::string& inflname) { return new BetaDecayAnalyzer(this,nm,inflname); }
	/// get processed data location for background estimation
	virtual std::string estimatorHistoLocation() const { return processedLocation; }
	static std::string processedLocation;	//< processed data location for background estimation
	
	MuonAnalyzer* myMuons;			//< muons plugin
	PositionAnalyzer* myPos;		//< positions plugin
	WirechamberGainAnalyzer* myWG;	//< wirechamber gain plugin
	AsymmetryAnalyzer* myAsym;		//< asymmetry plugin
};

/// analyzer for beta decay simulation
class SimBetaDecayAnalyzer: public BetaDecayAnalyzer {
public:
	/// constructor
	SimBetaDecayAnalyzer(OutputManager* pnt, const std::string& nm = "BetaDecayAnalyzer", const std::string& inflName = "");
	/// create a new instance of this object (cloning self settings) for given directory
	virtual SegmentSaver* makeAnalyzer(const std::string& nm, const std::string& inflname) { return new SimBetaDecayAnalyzer(this,nm,inflname); }
	
	SimAsymmetryAnalyzer* mySimAsym;	//< simulated asymmetry plugin
};

#endif
