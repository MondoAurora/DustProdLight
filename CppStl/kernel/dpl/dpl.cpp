/*
 * dpl.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_impl.h"
#include "dplujson.h"

using namespace std;


void DPLMeta::init() {
	if ( !DustProdLightStore::store) {
		DustProdLightStore::store = new DustProdLightStore();
	}

//	DPLJson::init();
}


void DPLMeta::shutdown() {
	if ( DustProdLightStore::store) {
		delete DustProdLightStore::store;
		DustProdLightStore::store = NULL;
	}
}

