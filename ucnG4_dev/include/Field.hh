//2-19-02 J. Yuan: magnet field for UCNA experiment
//Modified MPM 2011-2014
////////////////////////////////////////////////////

#ifndef FIELD_HH
#define FIELD_HH

#include <TString.h>
#include <vector>

#include <globals.hh>
#include <G4MagneticField.hh>
#include <G4ThreeVector.hh>

using namespace std;

/// class for describing magnetic field
class Field: public G4MagneticField {
public:
	/// constructor
	Field();
	/// constructor with fieldmap
	Field(const TString filename);
	/// get field at given point
	void GetFieldValue( const  G4double Point[3], G4double *Bfield ) const;
	/// zero out the field (via scaling factor)
	void SetFieldToZero() { fieldScale=0; };
	/// set fieldmap scaling factor
	void SetFieldScale(const G4double val) { fieldScale=val; }
	/// load fieldmap from file
	void LoadFieldMap(const TString filename);
	
	bool addAFP;	///< whether to add the AFP fringe field
	
private:
	/// add point to field profile
	void addPoint(G4double z, G4double B) { Zpoints.push_back(z); Bpoints.push_back(B); }
	vector<G4double> Bpoints;	///< field profile B values
	vector<G4double> Zpoints;	///< field profile z positions
	G4double rmax2;				///< max radius squared (position in world volume) to apply field
	G4double fieldScale;		///< scaling factor for field strength
};

#endif

