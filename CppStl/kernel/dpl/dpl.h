/*
 * dpl.h
 *
 * DustProdLight API (STL version)
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_H_
#define DPL_H_

#include <string>
#include <cstdarg>

using namespace std;

enum DPLTokenType {
	dpl_token_val_invalid,
	dpl_token_val_int,
	dpl_token_val_double,
	dpl_token_val_string,
	dpl_token_ref_single,
	dpl_token_ref_set,
	dpl_token_ref_array,
	dpl_token_ref_map
};

enum DPLChange {
	dpl_chg_ref_set, dpl_chg_ref_remove, dpl_chg_ref_clear,
};

#define REFKEY_ARR_APPEND -1

class DPLErrInvalidValueType {
};
class DPLErrUnkownField {
};
class DPLErrOutOfRange {
};
class DPLErrIdOutOfRange {
};

typedef int DPLType;
typedef int DPLToken;
typedef int DPLEntity;

class DPL {
public:
// state management
	static void init();
	static void shutdown();

// meta initialization
	static DPLType get_type(string typeName);
	static DPLToken get_token(DPLType type, string tokenName, DPLTokenType tokenType);

// meta detection on Entity
	static DPLType get_primary_type(DPLEntity entity);
	static bool has_type(DPLEntity entity, DPLType type);
	static void get_all_types(DPLEntity entity, set<DPLType>& typeSet);

// Entity creation and access
	static DPLEntity get_entity_by_path(DPLEntity root, int path... );
	static DPLEntity create_entity(DPLType primaryType);

// Entity value access
	static int get_int(DPLEntity entity, DPLToken token, int defValue);
	static double get_double(DPLEntity entity, DPLToken token, double defValue);
	static string get_string(DPLEntity entity, DPLToken token, string defValue);

	static void set_int(DPLEntity entity, DPLToken token, int i);
	static void set_double(DPLEntity entity, DPLToken token, double d);
	static void set_string(DPLEntity entity, DPLToken token, string s);

// Entity reference access
	static int get_ref_count(DPLEntity entity, DPLToken token);
	static DPLToken get_ref_key(DPLEntity entity, DPLToken token, int idx);
	static DPLEntity get_ref(DPLEntity entity, DPLToken token, int key);

	static bool set_ref(DPLEntity entity, DPLToken token, DPLEntity target, int key);
};

#endif /* DPL_H_ */
