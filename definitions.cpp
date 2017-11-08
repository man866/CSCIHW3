/*
 * definitions.cpp
 *
 *  Created on: Oct 31, 2017
 *      Author: nakaj
 */


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

#include "HW3.h"






//! Functions for KB

resOutput Resolution(KB kb, sortedClause cl, resPairs pastPairs){



	//! Get only the relevant clauses.
	KB relKb = GetRelevantKB(kb, cl);

	//! check if it is possible to reach a contradiction.
	if (! SolvabilityTest(relKb, cl, pastPairs))
		return resOutput(false, pastPairs);

	//! Reorder the the clauses in the order to execute further resolution.
	relKb = Reorder(relKb, cl);

	//! For each resolvable sortedClause, resolve it with cl and resursively do Resolution
	int N = relKb.size();
	for(int i = 0; i<N; i++){
		KB newKb = relKb;
		KB::iterator it = newKb.begin();
		std::advance(it,i);
		resbleOutput resbleOut = Resolvable(it->second, cl);
		if (resbleOut.first){

			auto pPairs = pastPairs;
			sortedClause newCl = resbleOut.second;

//			CoutSortedClause(cl);
//			CoutSortedClause(it->second);
//			CoutSortedClause(newCl);
//			std::cout<<std::endl;

			//! If cl was contained in the past pairs, return false
			if(isContainedPastPairs1(newCl, pastPairs)){
//				std::cout<<"Contained------------------------------------------------------"<<std::endl;
				continue;
			}

			resPair newResResult(it->first, cl);

			//! Check if the same resolution is repeated.
			if (IsRepeated(newResResult, pPairs)){
				continue;
			}


			//! Insert the new resolution result.
			pPairs.push_back(newResResult);
			//! If the resolution results in empty sortedClause, return true.
			if (newCl.nPredicates==0)
				return resOutput(true, pPairs);


//			// Remove the resolved sortedClause, and apply Resolution recursively.
//			newKb.erase(it);
			resOutput resolution = Resolution(newKb, newCl, pPairs);
			if(resolution.first==true)
				return resolution;
		}
	}

	return resOutput(false,pastPairs);
}

KB GetRelevantKB(KB kb, const sortedClause& cl){
	std::set<std::string> pNames;
	pNames.insert(cl.pNames.begin(), cl.pNames.end());

	KB newKb;
	bool relFound = true;
	while(relFound && !kb.empty()){
//		std::cout<<"still going, ";
		relFound = false;
		for(int i = 0; i < int(kb.size()); i++){
			bool found = false;
			std::vector<std::string> names = kb[i].second.pNames;
			for (auto name : names){
				auto it = pNames.find(name);
				if (it != pNames.end()){
					found = true;
					break;
				}
			}
			if (found){
				newKb.push_back(kb[i]);
				kb.erase(kb.begin()+i);
				i--;
				relFound = true;
				for (auto name : names){
					auto it = pNames.find(name);
					if (it == pNames.end()){
						pNames.insert(name);
					}
				}
			}
		}
	}

	return newKb;
}

KB GetResolvableKB(const KB& kb, const sortedClause& cl){
	return KB();
}

bool SolvabilityTest(const KB& kb, const sortedClause& cl, const resPairs& pastPairs){
	//! get all the constants.


	return true;
}


KB Reorder(KB kb, sortedClause cl){
	std::sort(kb.begin(), kb.end(), clPair_less());
	return kb;
}

resbleOutput Resolvable(sortedClause cl1, sortedClause cl2){
	//! Check if there is a predicate pair which can be resolved.
	//! If there is, return the positions of the predicate pair.
//	CoutSortedClause(cl1);
//	CoutSortedClause(cl2);
	int pNamesSize = cl1.pNames.size();

	for (int i1 = 0; i1 < pNamesSize; i1++){
		std::string pName = cl1.pNames[i1];
		//! Continue if cl2 does not have pName.
		auto it =
				std::find(cl2.pNames.begin(), cl2.pNames.end(), pName);
		if (it == cl2.pNames.end())
			continue;
		int i2 = std::distance(cl2.pNames.begin(), it);
		for (int m = 0; m < 2; m++){
			clause& clause1 = m == 0 ? cl1.falseCl[i1]:cl1.trueCl[i1];
			clause& clause2 = m == 1 ? cl2.falseCl[i2]:cl2.trueCl[i2];
			int clSize1 = clause1.size();
			int clSize2 = clause2.size();
			for (int j1 = 0; j1 < clSize1;j1++ ){
				predicate p1 = clause1[j1];
				for (int j2 = 0; j2 < clSize2;j2++ ){
					predicate p2 = clause2[j2];
					mguOutput mgu = Mgu(p1,p2);

					if (mgu.possible){

						predicate copy1 = p1;
						predicate copy2 = p2;
						mguOutput mgu1 = Mgu(copy1,copy2);
						auto it1 = clause1.begin();
						std::advance(it1, j1);
						clause1.erase(it1);
						cl1.nPredicates --;
						if(clause1.size()==0){
							clause otherClause = m == 0 ? cl1.trueCl[i1]:cl1.falseCl[i1];
							if (otherClause.size()==0){
								cl1.pNames.erase(cl1.pNames.begin()+i1);
								cl1.falseCl.erase(cl1.falseCl.begin()+i1);
								cl1.trueCl.erase(cl1.trueCl.begin()+i1);
							}
						}

						clause::iterator it2 = clause2.begin();
						std::advance(it2, j2);
						clause2.erase(it2);
						cl2.nPredicates --;
						if(clause2.size()==0){
							clause otherClause = m == 1 ? cl2.trueCl[i2]:cl2.falseCl[i2];
							if (otherClause.size()==0){
								cl2.pNames.erase(cl2.pNames.begin()+i2);
								cl2.falseCl.erase(cl2.falseCl.begin()+i2);
								cl2.trueCl.erase(cl2.trueCl.begin()+i2);
							}
						}


//						for (auto it = mgu.subs1.begin(); it != mgu.subs1.end(); it++){
//							std::cout << it->first << " -> " <<it->second<<"\t";
//						}
//						std::cout<<std::endl;
//						for (auto it = mgu.subs2.begin(); it != mgu.subs2.end(); it++){
//							std::cout << it->first << " -> " <<it->second<<"\t";
//						}
//						std::cout<<std::endl;
//						std::cout<<std::endl;


						ApplySubstitutions(cl1, mgu.subs1);
						ApplySubstitutions(cl2, mgu.subs2);
//						std::cout<<"before"<<std::endl;
//						CoutSortedClause(cl1);
//						CoutSortedClause(cl2);
						sortedClause cl = MergeClause(cl1, cl2);
//						std::cout<<"after"<<std::endl;
//						CoutSortedClause(cl);
//						std::cout<<std::endl;
						return resbleOutput(true, cl);

					}
				}
			}
		}
	}
	sortedClause scl;
	scl.falseCl = std::vector<clause>();
	scl.trueCl = std::vector<clause>();
	scl.pNames = std::vector<std::string>();
	return resbleOutput(false, scl);


}

bool IsRepeated(const resPair& rp, const resPairs& rps){
	std::set<std::string> varNames1, varNames2;

	auto scl = rp.second;
	for(int m = 0; m < 2; m++){
		auto vecClause = m == 0 ? scl.falseCl : scl.trueCl;
		for(auto cl : vecClause){
			for(int i = 0; i < int(cl.size()); i++){
				auto p1 = cl[i];
				for(auto arg : p1.args){
					if(!arg.isConstant){
						if(varNames1.find(arg.name)==varNames1.end())
							varNames1.insert(arg.name);
					}
				}
			}
		}
	}

	//! Check if the same resolution pair was made in the past.
	bool repeated = false;
	for(auto r : rps){
		if(rp.first != r.first)
			continue;
		varNames2.clear();
		scl = r.second;
		for(int m = 0; m < 2; m++){
			auto vecClause = m == 0 ? scl.falseCl : scl.trueCl;
			for(auto cl : vecClause){
				for(int i = 0; i < int(cl.size()); i++){
					auto p1 = cl[i];
					for(auto arg : p1.args){
						if(!arg.isConstant){
							if(varNames2.find(arg.name)==varNames2.end())
								varNames2.insert(arg.name);
						}
					}
				}
			}
		}

		if(varNames1.size() != varNames2.size())
			continue;
		int size = varNames2.size();
		std::vector<int> v(size) ; // vector with 100 ints.
		std::iota (std::begin(v), std::end(v), 0); // Fill with 0, 1, ..., 99.

		do {
			subMap smap;
			for(int i = 0; i<int(size); i++){
				auto it1 = varNames1.begin();
				std::advance(it1, i);
				auto it2 = varNames2.begin();
				std::advance(it2, v[i]);
				smap[*it2 ] = *it1;
			}
			ApplySubstitutions(scl, smap);
			if(areIdenticalSortedClauses(rp.second, scl)){
				repeated = true;
				break;
			}
		} while ( std::next_permutation(v.begin(),v.end()) );



	}
	return repeated;
}


sortedClause MergeClause(sortedClause cl1, sortedClause cl2){

	int pNamesSize2 = cl2.pNames.size();
	for (int i2 = 0 ; i2 < pNamesSize2; i2++){
		auto it = std::find(cl1.pNames.begin(), cl1.pNames.end(), cl2.pNames[i2]);
		if (it == cl1.pNames.end()){
			cl1.pNames.push_back(cl2.pNames[i2]);
			cl1.falseCl.push_back(clause());
			cl1.trueCl.push_back(clause());
		}
		it = std::find(cl1.pNames.begin(), cl1.pNames.end(), cl2.pNames[i2]);
		int i1 = std::distance(cl1.pNames.begin(), it);
		for (int m=0; m<2;m++){
			clause& clause1 = m == 0 ? cl1.falseCl[i1]:cl1.trueCl[i1];
			clause clause2 = m == 0 ? cl2.falseCl[i2]:cl2.trueCl[i2];
			clause1.insert(clause1.end(), clause2.begin(), clause2.end());
			cl1.nPredicates += int(clause2.size());
		}
	}
	MakeConcise(cl1);
	return cl1;
}

mguOutput Mgu(predicate p1, predicate p2){
	auto p1copy = p1;
	auto p2copy = p2;
	mguOutput mgu(false, subMap(), subMap());
	if (p1.name != p2.name) return mgu;
	int argSize = p1.args.size();
	std::set<int> checkedIndices;
	subMap& subs1 = mgu.subs1;
	subMap& subs2 = mgu.subs2;
	for (int i = 0; i < argSize; i++){
		if (std::find(checkedIndices.begin(), checkedIndices.end(), i)!=checkedIndices.end())
			continue;
		//! If p1.arg[i] is a constant
		if(p1.args[i].isConstant){
			//! If p2.arg[i] is constant.
			if(p2.args[i].isConstant){
				if(p1.args[i].name != p2.args[i].name)
					return mgu;
				checkedIndices.insert(i);
			}
			else{
				subMap sub;
//				std::cout<<"p1 has constant and p2 has a variable."<<std::endl;
				sub[p2.args[i].name]= p1.args[i].name;
				bool consistent = SubPropagate(sub, p1, p2, subs1, subs2, i, false, checkedIndices);
				if(!consistent)
					return mgu;
			}
		}
		else{
			if(p2.args[i].isConstant){
				subMap sub;
				sub[p1.args[i].name]= p2.args[i].name;
				bool consistent = SubPropagate(sub, p1, p2, subs1, subs2, i, true, checkedIndices);
				if (!consistent)
					return mgu;
			}
			else{
				subMap sub;
				sub[p2.args[i].name]= p1.args[i].name;
				bool consistent = SubPropagate(sub, p1, p2, subs1, subs2, i, false, checkedIndices);
				if (!consistent)
					return mgu;
			}
		}

	}
//	auto scl = PredicateToClause(std::vector<predicate>({p1copy, p2copy}), false);
//	CoutSortedClause(scl);
	mgu.possible = true;
	return mgu;
}

bool SubPropagate(const subMap& sub, predicate& p1, predicate& p2,
		subMap& subs1, subMap& subs2, const int& index, const bool& isForOne,
		std::set<int>& checkedIndices){

	auto& p = isForOne ? p1 : p2;
	auto& p3 = isForOne ? p2 : p1;
	std::vector<int> relIndices = p.args[index].relatedIndices;
	relIndices.push_back(index);
	if (isForOne)
		subs1.insert(sub.begin(), sub.end());
	else
		subs2.insert(sub.begin(), sub.end());
	if (!std::islower(sub.begin()->second[0]))
		checkedIndices.insert(relIndices.begin(), relIndices.end());
	//! Check if the related indices were already in the substituion state.
	bool needToContinue = false;
	for (auto i : relIndices){
		if(p.args[i].name != sub.begin()->second){
			needToContinue = true;
			break;
		}
	}
	if (!needToContinue)
		return true;
	//! Make the substitution to each relevant variable;
	for (auto i : relIndices){
		p.args[i].name = sub.begin()->second;
		//! Remove the related indices since they are no longer variables, but only if substituion to a constant.
		if(!std::islower(sub.begin()->second[0]))
			p.args[i].relatedIndices.clear();
	}

	/*
	 * Check consistency.
	 * If the arg partner is constant, check if it is the same as substitution.
	 * If the arg partner is variable, substitute the constant and propagate.
	 */
	for (auto i : relIndices){
		if(p3.args[i].isConstant){
			if(sub.begin()->second != p3.args[i].name){
				return false;
			}
		}
		else{
			subMap newSub;
			newSub[p3.args[i].name]= sub.begin()->second;
			bool output1 = SubPropagate(newSub, p1, p2, subs1, subs2, i, !isForOne, checkedIndices);
			if (!output1)
				return false;
		}
	}

	return true;
}



void ApplySubstitutions(sortedClause& cl, const subMap& sub){
	int clSize1 = cl.falseCl.size();
	for (int i = 0; i < clSize1; i++){
		clause& c = cl.falseCl[i];
		int clSize2 = c.size();
		for (int j = 0; j < clSize2; j++){
			predicate& p = c[j];
			for (auto& arg : p.args){
				if (arg.isConstant)
					continue;
				auto it = sub.find(arg.name);
				if (it != sub.end()){
					arg.name = it->second;
					if(std::isupper(it->second[0]))
						arg.isConstant = true;
				}
			}
		}
	}
	clSize1 = cl.trueCl.size();
	for (int i = 0; i < clSize1; i++){
		clause& c = cl.trueCl[i];
		int clSize2 = c.size();
		for (int j = 0; j < clSize2; j++){
			predicate& p = c[j];
			for (auto& arg : p.args){
				if (arg.isConstant)
					continue;
				auto it = sub.find(arg.name);
				if (it != sub.end()){
					arg.name = it->second;
					if(std::isupper(it->second[0]))
						arg.isConstant = true;
				}
			}
		}
	}
}



instance GetInput(const std::string& fName, int& varIndex){
	instance inst;
	std::ifstream fIn(fName);
	std::string line;

	std::getline(fIn, line);
	int nQueries = std::stoi(line);
	for (int i = 0; i < nQueries; i++){
		std::getline(fIn, line);
		inst.qs.push_back(GetPredicate(line));
	}
	std::getline(fIn, line);
	int nClauses = std::stoi(line);
	for (int i = 0; i < nClauses; i++){
		std::getline(fIn, line);
		clPair clp(i, GetSortedClause(line, varIndex));
		inst.kb.push_back(clp);
	}
	return inst;
}

predicate GetPredicate(std::string line){
//	std::cout<<line<<std::endl;
	bool hasVar = false;
	bool truth = true;
	std::string pName;
	std::string argsName;
	std::string argName;
	arguments args;
	std::map<std::string, std::vector<int>> relMap;
	std::set<std::string> varNames;

	std::istringstream tokenizer(line);
	std::getline(tokenizer, pName, argStartDelim);
	if (pName[0]==spaceChar)
		pName.erase(pName.begin());
	if (pName[0]==negationChar){
		truth = false;
		pName.erase(pName.begin());
	}
	std::getline(tokenizer, argsName, argEndDelim);
//	std::cout<<argsName<<std::endl<<std::endl;
	std::istringstream tokenizer1(argsName);
	int index = 0;
	while(std::getline(tokenizer1, argName, argDelim)){
//		std::cout<<argName<<std::endl;
		argument1 arg;
		arg.isConstant = true;
		arg.name = argName;
		if(arg.name.size()==1){
			if(std::islower(arg.name[0])){
				arg.isConstant = false;
				if(varNames.find(arg.name) == varNames.end())
					varNames.insert(arg.name);
				auto it = relMap.find(arg.name);
				if (it == relMap.end()){
					relMap[arg.name]=std::vector<int>({index});
				}
				else{
					relMap[arg.name].push_back(index);
				}
			}
		}
		args.push_back(arg);
		index++;
	}
	if (relMap.size()!=0)
		hasVar = true;
	for(int i = 0; i < int(args.size()); i++){
		if (args[i].isConstant)
			continue;
		argument1& arg = args[i];
		std::vector<int> relIndices = relMap[arg.name];
		relIndices.erase(std::find(relIndices.begin(), relIndices.end(), i));
		arg.relatedIndices = relIndices;
	}
//	//! Standadize the variables.
//	for(auto it = relMap.begin(); it != relMap.end(); it++){
//		for(auto i : it->second){
//			args[i].name = "x"+std::to_string(varIndex);
//		}
//		varIndex ++;
//	}

	return predicate(hasVar, truth, pName, args, varNames);


}

sortedClause GetSortedClause(std::string line, int& varIndex){
	sortedClause scl;
	scl.nPredicates = 0;
	std::vector<std::string>& pNames = scl.pNames;
	std::istringstream tokenizer(line);
	std::string pName;
	std::set<std::string>& varNames = scl.varNames;
	while(std::getline(tokenizer, pName, predicateBoundaryDelim)){
		scl.nPredicates ++;
		predicate p = GetPredicate(pName);
		for(auto name : p.varNames){
			if(varNames.find(name) == varNames.end())
				varNames.insert(name);
		}
		auto it = std::find(pNames.begin(), pNames.end(), p.name);
		if (it == pNames.end()){
			pNames.push_back(p.name);
			scl.falseCl.push_back(clause());
			scl.trueCl.push_back(clause());

		}
		it = std::find(pNames.begin(), pNames.end(), p.name);
		int index = std::distance(pNames.begin(), it);
		clause& cl = p.truth? scl.trueCl[index]:scl.falseCl[index];
		cl.push_back(p);
	}

	//! Standadize variables
	subMap smap;
	for(auto var : scl.varNames){
		smap[var] = "x" + std::to_string(varIndex);
		varIndex++;
	}
	for(int m = 0; m < 2; m++){
		auto& vecCls = m == 0 ? scl.falseCl : scl.trueCl;
		for(auto& cl : vecCls){
			for(auto& p : cl){
				for(auto& arg : p.args){
					if (!arg.isConstant)
						arg.name = smap[arg.name];
				}
			}
		}
	}


	return scl;
}


void CoutSortedClause(const sortedClause& scl){
//	std::cout<<scl.nPredicates<<"  ";
	for(auto cl : scl.trueCl){
		for(auto p : cl){
			if (!p.truth)
				std::cout<<"~";
			std::cout<<p.name<<'(';
			for(int i =0; i<int(p.args.size()); i++){
				std::cout<<p.args[i].name;
				if(i!=int(p.args.size())-1)
					std::cout<<", ";
			}
			std::cout<<")  |  ";
		}
	}
	for(auto cl : scl.falseCl){
		for(auto p : cl){
			if (!p.truth)
				std::cout<<"~";
			std::cout<<p.name<<'(';
			for(int i =0; i<int(p.args.size()); i++){
				std::cout<<p.args[i].name;
				if(i!=int(p.args.size())-1)
					std::cout<<", ";
			}
			std::cout<<")  |  ";
		}
	}
	std::cout<<std::endl;
}

sortedClause PredicateToClause(const std::vector<predicate>& ps, const bool& flip){
	sortedClause scl;
	for (auto p : ps){
		if(std::find(scl.pNames.begin(), scl.pNames.end(), p.name)==scl.pNames.end()){
			scl.pNames.push_back(p.name);
			scl.trueCl.push_back(clause());
			scl.falseCl.push_back(clause());
		}
		auto it = std::find(scl.pNames.begin(), scl.pNames.end(), p.name);
		int i = std::distance(scl.pNames.begin(), it);
		if(flip){
			p.truth = !p.truth;
		}
		clause& cl = p.truth ? scl.trueCl[i] : scl.falseCl[i];


		cl.push_back(p);
		scl.nPredicates ++;
	}
	return scl;
}

bool areIdenticalPredicates(const predicate& p1, const predicate& p2){
	if(p1.truth != p2.truth)
		return false;
	if(p1.name != p2.name)
		return false;
	if(p1.args.size() != p2.args.size())
		return false;
	bool isSame = true;
	int size = p1.args.size();
	for(int i = 0; i < size; i++){
		if(p1.args[i].name != p2.args[i].name){
			isSame = false;
			break;
		}
	}
	return isSame;
}

bool areIdenticalClauses(const clause& cl1, const clause& cl2){
	if(cl1.size() != cl2.size())
		return false;
	auto cl2c = cl2;

	for(auto p1 : cl1){
		bool isSame = false;
		for(int i = 0; i<int(cl2c.size()); i++){
			if(areIdenticalPredicates(p1, cl2c[i])){
				isSame = true;
				cl2c.erase(cl2c.begin()+i);
				break;
			}
		}
		if(!isSame)
			return false;
	}
	return true;
}

bool areIdenticalSortedClauses(const sortedClause& scl1, const sortedClause& scl2){
	if(scl1.nPredicates!=scl2.nPredicates)
		return false;
	if(scl1.pNames != scl2.pNames)
		return false;
	bool isSame = true;
	for(int m = 0; m < 2; m++){
		const std::vector<clause> clss1 = m==0? scl1.falseCl : scl1.trueCl;
		const std::vector<clause> clss2 = m==0? scl2.falseCl : scl2.trueCl;
		int clSize = clss1.size();
		for(int i = 0; i < clSize; i++){
			if(!areIdenticalClauses(clss1[i],clss2[i])){
				isSame = false;
				break;
			}
		}
	}
	return isSame;
}

void MakeConcise(sortedClause& scl){
	//!Check if there are identical predicates in the clause.
	for(int m = 0; m < 2; m++){
		auto& vecClause = m == 0 ? scl.falseCl : scl.trueCl;
		for(auto& cl : vecClause){
			for(int i = 0; i < int(cl.size()); i++){
				auto& p1 = cl[i];
				for(int j = i + 1; j < int(cl.size()); j++){
					auto& p2 = cl[j];
					if(areIdenticalPredicates(p1, p2)){
//						std::cout<<"same"<<std::endl;
						cl.erase(cl.begin() + j);
						j--;
					}
				}
			}
		}
	}
}


bool noInformation(const instance& inst, const sortedClause& scl){
return true;
}


std::vector<std::vector<int> > comb(int N, int K){
	std::string bitmask(K, 1);
	bitmask.resize(N, 0);
	std::vector<std::vector<int> > vs;

	do{
		std::vector<int> v;
		for (int i = 0; i < N; ++i){
			if(bitmask[i]){
				v.push_back(i);
			}
		}
		vs.push_back(v);
	} while(std::prev_permutation(bitmask.begin(), bitmask.end()));

	return vs;
}

//! Check if scl1 is contained in scl2
bool isContained(const sortedClause& scl1, const sortedClause& scl2){
	std::set<std::string> varNames1 = GetVariables(scl1);
	std::set<std::string> varNames2 = GetVariables(scl2);
	int size1 = varNames1.size();
	int size2 = varNames2.size();

	if(size1 > size2)
		return false;

	auto vs = comb(size2, size1);
	for(auto v : vs){
		std::sort(v.begin(), v.end());
		do {
			subMap smap;
			for(int i = 0; i<int(size1); i++){
				auto it1 = varNames1.begin();
				std::advance(it1, i);
				auto it2 = varNames2.begin();
				std::advance(it2, v[i]);
				smap[*it2 ] = *it1;
			}
			auto scl = scl2;
			ApplySubstitutions(scl, smap);
			bool contained = true;
			for(int m = 0; m < 2; m++){
				auto& vecClause = m == 0 ? scl1.falseCl : scl1.trueCl;
				for(auto& cl : vecClause){
					for(auto p : cl){
						if(!isInSortedClause(p, scl)){
							contained = false;
							break;
						}
					}
					if(!contained)
						break;
				}
				if(!contained)
					break;
			}

			if(contained)
				return true;

		} while (std::next_permutation(v.begin(),v.end()) );

	}

	return false;
}


bool isInSortedClause(const predicate& pred, const sortedClause& scl){
	for(int m = 0; m < 2; m++){
		auto vecClause = m == 0 ? scl.falseCl : scl.trueCl;
		for(auto cl : vecClause){
			for(auto p : cl){
				if(areIdenticalPredicates(p, pred))
					return true;
			}
		}
	}
	return false;
}


std::set<std::string> GetVariables(const sortedClause& scl){
	std::set<std::string> vars;
	for(int m = 0; m < 2; m++){
		auto& vecClause = m == 0 ? scl.falseCl : scl.trueCl;
		for(auto& cl : vecClause){
			for(auto p : cl){
				for(auto arg : p.args){
					if(vars.find(arg.name)==vars.end()){
						vars.insert(arg.name);
					}
				}
			}
		}
	}
	return vars;
}

bool isContainedPastPairs(const sortedClause& scl, const resPairs& pastPairs){

	for(auto pair : pastPairs){
		auto scl2 = pair.second;
		if(isContained(scl, scl2)){
			return true;

		}
	}
	return false;
}


bool isContainedPastPairs1(const sortedClause& scl, const resPairs& pastPairs){

	for(auto pair : pastPairs){
		auto scl2 = pair.second;
		if(isContained(scl2, scl)){
			return true;

		}
	}
	return false;
}

