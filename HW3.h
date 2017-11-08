/*
 * HW3.h
 *
 *  Created on: Oct 31, 2017
 *      Author: nakaj
 */

#ifndef HW3_H_
#define HW3_H_

#include <iostream>                         // for std::cout
#include <utility>                          // for std::pair
#include <algorithm>
#include <vector>
#include <string>
#include <time.h>       /* time */
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stack>
#include "set"
#include <random>
#include <tuple>
#include <iterator>
#include <map>



const char argStartDelim = '(';
const char argEndDelim = ')';
const char negationChar = '~';
const char predicateBoundaryDelim = '|';
const char argDelim = ',';
const char spaceChar = ' ';

//int varIndex = 0;

typedef bool isTrue;
typedef bool isConst;
typedef bool resble;
typedef std::map<std::string, std::string> subMap;
typedef std::pair<std::string, std::string> substitution;
typedef std::vector<substitution> substitutions;
typedef std::pair<isConst,std::string> argument;


struct argument1{
	isConst isConstant;
	std::string name;
	std::vector<int> relatedIndices;
};
typedef std::vector<argument1> arguments;
typedef int clauseId;
struct resblePair{
	bool resble;
	std::pair<int, int> indPair1;
	std::pair<int, int> indPair2;
};




struct predicate{
	predicate(const bool& hasVar_, const isTrue& truth_, const std::string& name_, const arguments& args_, const std::set<std::string>& varNames_)
	:hasVar(hasVar_), truth(truth_), name(name_), args(args_), varNames(varNames_)
	{}
	bool hasVar;
	isTrue truth;
	std::string name;
	arguments args;
	std::set<std::string> varNames;
};
struct mguOutput{
	mguOutput(const bool& possible_, const subMap& subs1_, const subMap& subs2_):
		possible(possible_), subs1(subs1_), subs2(subs2_){}
	bool possible;
	subMap subs1;
	subMap subs2;
};
typedef std::vector<predicate> clause;

struct sortedClause{
	int nPredicates;
	std::vector<clause> trueCl, falseCl;
	std::vector<std::string> pNames;
	std::set<std::string> varNames;
};


typedef std::vector<predicate> queries;
typedef std::pair<resble, sortedClause> resbleOutput;
typedef std::vector<sortedClause> clauses;
typedef std::pair<clauseId, sortedClause> clPair;
typedef std::vector<clPair> KB;
typedef std::map<clauseId, sortedClause> KB1;
typedef std::pair<clauseId, sortedClause> resPair; //! Result of resolving with sortedClause
typedef std::vector<resPair> resPairs; 		//! set of the past resolution result.
typedef std::pair<bool, resPairs> resOutput; //! Output of Resolution function.



struct clPair_less {
    bool operator ()(clPair const& scl1, clPair const& scl2) const {
        if (scl1.second.nPredicates < scl2.second.nPredicates) return true;
        return false;
    }
};

struct unifiableOutput{
	bool consistent;
	substitutions subs;
};

struct instance {
	queries qs;
	KB kb;
	std::set<std::string> constants;
};


//!Functions

resOutput Resolution(KB kb, sortedClause cl, resPairs pastPairs);
KB GetRelevantKB(KB kb, const sortedClause& cl);
KB GetResolvableKB(const KB& kb, const sortedClause& cl);
bool SolvabilityTest(const KB& kb, const sortedClause& cl, const resPairs& results);
KB Reorder(KB kb, sortedClause cl);
bool IsRepeated(const resPair& result, const resPairs& results);
sortedClause MergeClause(sortedClause cl1, sortedClause cl2);
mguOutput Mgu(predicate p1, predicate p2);
bool SubPropagate(const subMap& sub, predicate& p1, predicate& p2,
		subMap& subs1, subMap& subs2, const int& index, const bool& isForOne, std::set<int>& checkedIndices);
void ApplySubstitutions(sortedClause& cl,  const subMap& subs);
resbleOutput Resolvable(sortedClause cl1, sortedClause cl2);
instance GetInput(const std::string& fName, int& varIndex);
predicate GetPredicate(std::string line);
sortedClause GetSortedClause(std::string line, int& varIndex);
void CoutSortedClause(const sortedClause& scl);
sortedClause PredicateToClause(const std::vector<predicate>& ps, const bool& flip);
bool areIdenticalPredicates(const predicate& p1, const predicate& p2);
bool areIdenticalClauses(const clause& cl1, const clause& cl2);
bool areIdenticalSortedClauses(const sortedClause& scl1, const sortedClause& scl2);
void MakeConcise(sortedClause& scl);
bool noInformation(const instance& inst, const sortedClause& scl);
std::vector<std::vector<int> > comb(int N, int K);
bool isContained(const sortedClause& scl1, const sortedClause& scl2);
bool isInSortedClause(const predicate& pred, const sortedClause& scl);
std::set<std::string> GetVariables(const sortedClause& scl);
bool isContainedPastPairs(const sortedClause& scl, const resPairs& pastPairs);
bool isContainedPastPairs1(const sortedClause& scl, const resPairs& pastPairs);
#endif /* HW3_H_ */
