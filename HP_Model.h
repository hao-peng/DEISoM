#ifndef _HP_MODEL
#define _HP_MODEL

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <map>
#include "HP_Read.h"
#include "HP_Gene.h"
#include "HP_Param.h"

#define MAX_NEWTON_ITER 1000

using namespace std;

class HP_Model {
private:
	HP_Param param;
	//sub->[readName, reads]
	vector<map<string, list<HP_Read> > > readsByName;
	//sub->read->isoform->insertedLength
	list<list<vector<int> > > insertedLensBySub;
	//sub->read->isoform->match
	list<list<vector <bool> > > alignmentsBySub;
	//log score ln P(R,lamda|isoform) or ln P(R|isoform)
	//sub->read->isoform->score
	vector<vector<vector<double> > > logScore;
	//num of subjects
	int numSubs;
	//num of isoforms
	int numIsos;
	//num of reads by subject
	vector<int> numReadsBySub;
	//isoform->alpha
	vector<double> alpha;
	//sub->isoform->beta
	vector<vector<double> >betasBySub;
	//sub->read->isoform->r
	vector<vector<vector<double> > > rsBySub;
	//isoform->(digamma(beta_k) - diagmma(sum(beta)))
	vector<double> weights;
	//isoform->(log_score_k + weights_k)
	vector<double> weightedScore;
	//isoform->sum(digamma(beta_k)-digamma(sum(beta)))
	vector<double> ss;
	//isoform->diagnal_Hessian
	vector<double> q;
	//isoform->gradient
	vector<double> g;
	bool isFinished;
	

	void loadGene();
	void loadReads();
	void computeAlignments();
	void computeLogScore();
	void initVariables();
	void updateRs(int subInd);
	void updateBetas(int subInd);
	void updateAlpha();
	void save();

public:
	HP_Gene gene;
	void addRead(const HP_Read &read, int index);
	HP_Model(const HP_Param &param);
	bool preprocessing();
	//perform Bayesian variational inference
	void performBVI();
};
#endif