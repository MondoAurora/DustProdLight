#include "dustmeta.h"


using namespace DustMeta;

const DustKey DustTypes::Unit = Dust::registerUnit("DustCore", "1.0");

const DustKey DustTypes::RootKey = Dust::registerKey("RootKey", DustTypes::Unit);
const DustKey DustTypes::Identifier = Dust::registerKey("Identifier", DustTypes::Unit);
const DustKey DustTypes::Entity = Dust::registerKey("Entity", DustTypes::Unit);

const DustKey RootKey::Self = Dust::registerKey("Self", DustTypes::RootKey, DKT_Ref, drtSingle);
const DustKey RootKey::Message = Dust::registerKey("Message", DustTypes::RootKey, DKT_Ref, drtSingle);
const DustKey RootKey::Context = Dust::registerKey("Context", DustTypes::RootKey, DKT_Ref, drtSingle);

const DustKey Identifier::Local = Dust::registerKey("Local", DustTypes::Identifier, DKT_Value, dvtRaw);
const DustKey Identifier::Global = Dust::registerKey("Global", DustTypes::Identifier, DKT_Value, dvtRaw);

const DustKey Entity::PrimaryType = Dust::registerKey("PrimaryType", DustTypes::Entity, DKT_Ref, drtSingle);
const DustKey Entity::Types = Dust::registerKey("Types", DustTypes::Entity, DKT_Ref, drtSet);

// const DustKey DustConsts::PATH_END = Dust::registerKey("$$$", DustTypes::Unit);
