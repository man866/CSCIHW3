/*
 * homework11.cpp
 *
 *  Created on: Oct 31, 2017
 *      Author: nakaj
 */

#include "HW3.h"

int main(){
	std::string fName = "input1.txt";
	int varIndex = 0;
	instance inst = GetInput(fName, varIndex);
	KB1 kb1;
	for(auto pair: inst.kb){
		kb1[pair.first] = pair.second;
	}
	resPairs pastPairs;

	//! MakeConcise test
//	auto sortedCl = inst.kb.back().second;
//	CoutSortedClause(sortedCl);
//	MakeConcise(sortedCl);
//	CoutSortedClause(sortedCl);

	for(auto p : inst.qs){

		sortedClause scl = PredicateToClause(std::vector<predicate>({p}), true);
		resOutput out = Resolution(inst.kb, scl, pastPairs);
		if(out.first)
			std::cout<<"TRUE"<<std::endl;
		else
			std::cout<<"FALSE"<<std::endl;
//		std::cout<<"-------------------------------------------------"<<std::endl;
//		int size = out.second.size();
//		std::cout<<size<<std::endl;
//		CoutSortedClause(scl);
//		std::cout<<std::endl;
//		for(int i = 0; i<size; i++){
//			CoutSortedClause(out.second[i].second);
//			CoutSortedClause(kb1[out.second[i].first]);
//
//			if(i != size-1){
//				CoutSortedClause(out.second[i+1].second);
//			}
//			std::cout<<std::endl;
//		}
//
//		std::cout<<std::endl<<std::endl;
	}


//	sortedClause scl = PredicateToClause(std::vector<predicate>({inst.qs[1]}), true);
//	resOutput out = Resolution(inst.kb, scl, pastPairs);
//	if(out.first)
//		std::cout<<"TRUE"<<std::endl;
//	else
//		std::cout<<"FALSE"<<std::endl;
//	std::cout<<"-------------------------------------------------"<<std::endl;
//	int size = out.second.size();
//	std::cout<<size<<std::endl;
//	CoutSortedClause(scl);
//	std::cout<<std::endl;
//	for(int i = 0; i<size; i++){
//
//		CoutSortedClause(out.second[i].second);
//		CoutSortedClause(kb1[out.second[i].first]);
//
//		if(i != size-1){
//			CoutSortedClause(out.second[i+1].second);
//		}
//		std::cout<<std::endl;
//	}
//
//	std::cout<<std::endl<<std::endl;





//	sortedClause scl1, scl2;
//	scl1 = inst.kb[9].second;
//	scl2 = inst.kb[12].second;
//
//	resbleOutput resbleOut = Resolvable(scl1,scl2);
//
//	std::cout<<std::endl<<std::endl;
//	KB relKb = GetRelevantKB(inst.kb, inst.kb[0].second);
//	std::cout<<relKb.size()<<std::endl;
//	for (auto pair : relKb){
//		CoutSortedClause(pair.second);
//	}



	return 0;
}
