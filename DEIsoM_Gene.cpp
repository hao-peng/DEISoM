/*
 Author: Hao Peng (pengh@purdue.edu)
 Date: May 8, 2013
 Version: 1.0v
 */
#include "DEIsoM_Gene.h"
#include <sstream>

using namespace std;

DEIsoM_Record::DEIsoM_Record() {
	seqid = string();
	source = string();
	type = string();
	start = 0;
	end = 0;
	score = 0;
    strand = '.';
    phase = -1;
	ID = string();
    name = string();
    parent = string();
}

string DEIsoM_Record::toString() const {
	stringstream sstm;
	sstm << seqid << "\t" << source << "\t" << type << "\t" << start << "\t" << end << "\t";
	if (score < 0) {
		sstm << ".";
	} else {
		sstm << score;
	}
	sstm << "\t" << strand << "\t";
	if (phase < 0) {
		sstm << ".";
	} else {
		sstm << phase;
	}
	sstm << "\t";
	bool hasAttribute = false;
	if (!ID.empty()) {
		sstm  << "ID=" << ID;
		hasAttribute = true;
	}
	if (!name.empty()) {
		if (hasAttribute) {
			sstm << ";";
		}
		sstm << "Name=" << name;
		hasAttribute = true;
	}
	if (!parent.empty()) {
		if (hasAttribute) {
			sstm << ";";
		}
		sstm << "Parent=" << parent;
		hasAttribute = true;
	}
	return sstm.str();
}

string DEIsoM_Exon::toString() const {
	stringstream sstm;
	sstm << DEIsoM_Record::toString() << endl;
	/*
	for (list<DEIsoM_CDS>::iterator ii = DEIsoM_CDSs.begin(); ii != DEIsoM_CDSs.end(); ii++) {
		sstm << (*ii).toString() << endl;
	}
	 */
	return sstm.str();
}

string DEIsoM_MRNA::toString() const{
	stringstream sstm;
	sstm << DEIsoM_Record::toString() << endl;
	for (list<DEIsoM_Exon>::const_iterator ii = exons.begin(); ii != exons.end(); ii++) {
		sstm << (*ii).toString();
	}
	/*
	for (list<DEIsoM_CDS>::iterator ii = DEIsoM_CDSs.begin(); ii != DEIsoM_CDSs.end(); ii++) {
		sstm << (*ii).toString() << endl;
	}
	 */
	return sstm.str();
}

int DEIsoM_MRNA::getLength() const {
	int len = 0;
	for (list<DEIsoM_Exon>::const_iterator ii = exons.begin(); ii != exons.end(); ii++) {
		len += ii->end-ii->start+1;
	}
	return len;
}

string DEIsoM_Gene::toString() const{
	stringstream sstm;
	sstm << DEIsoM_Record::toString() << endl;
	for (list<DEIsoM_MRNA>::const_iterator ii = mRNAs.begin(); ii != mRNAs.end(); ii++) {
		sstm << (*ii).toString();
	}
	return sstm.str();
}

// return the possible bounds for this gene
// beg and end are 1 based
void DEIsoM_Gene::getBounds(int &beg, int &end) const {
	beg = -1;
	end = -1;
	for (list<DEIsoM_MRNA>::const_iterator ii = mRNAs.begin();
		 ii != mRNAs.end(); ii++) {
		if (beg == -1 || ii->start < beg) {
			beg = ii->start;
		}
		if (end == -1 || ii->end > end) {
			end = ii->end;
		}
	}
}