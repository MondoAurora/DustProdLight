/*
 * dpl_impl.h
 *
 * DustProdLight STL version - all implementation class declaration
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_H_
#define DPL_IMPL_H_

#include "../dpl.h"
#include "../dplutils.h"

#include "data/data.h"

#include "dpl_impl_boot.h"

using namespace std;

class DustProdLightImplementation {
public:
	static void createBootEntities();

	static DustProdLightEntity* resolveEntity(DPLEntity entity);
	static string getMetaEntityId(DPLTokenType tokenType, const char* name, DPLEntity parent);
	static void initMetaEntity(DPLEntity entity, DPLTokenType tokenType, const char* name, DPLEntity parent = 0);

	static DustProdLightEntity* cloneEntity(DustProdLightEntity* entity);
	static void initMetaEntity(DustProdLightEntity *pEntity, DPLEntity entity, DPLTokenType tokenType, const char* name, DPLEntity parent);
	static string getString(DustProdLightEntity *pEntity, DPLEntity token);
	static void setString(DustProdLightEntity *pEntity, DPLEntity token, const char* name);
};

#endif /* DPL_IMPL_H_ */
