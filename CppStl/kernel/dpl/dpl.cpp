/*
 * dpl.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_impl_data.h"
#include "dplujson.h"

using namespace std;


void DPLMain::init() {
	if ( !DustProdLightStore::store) {
		DustProdLightStore::store = new DustProdLightStore();
	}

//	DPLJson::init();
}


void DPLMain::shutdown() {
	if ( DustProdLightStore::store) {
		delete DustProdLightStore::store;
		DustProdLightStore::store = NULL;
	}
}

