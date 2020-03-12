/*
 * dpl.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include "dpl/impl/data/module.h"
#include "dpl/impl/proc/module.h"

using namespace std;


void DPLMain::init() {
	DPLModuleData->init();
	DPLModuleProc->init();
}

void DPLMain::shutdown() {
	DPLModuleProc->release();
	DPLModuleData->release();
}

