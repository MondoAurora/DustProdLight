#include "dustmeta.h"


using namespace DustMeta;

const DustKey DustTypes::RootKey = Dust::getKey("RootKey");
const DustKey DustTypes::Identifier = Dust::getKey("Identifier");
const DustKey DustTypes::Entity = Dust::getKey("Entity");

const DustKey RootKey::Self = Dust::getKey("RootKey.Self");
const DustKey RootKey::Message = Dust::getKey("RootKey.Message");
const DustKey RootKey::Context = Dust::getKey("RootKey.Context");

const DustKey Identifier::Local = Dust::getKey("Identifier.Local");
const DustKey Identifier::Global = Dust::getKey("Identifier.Global");

const DustKey Entity::PrimaryType = Dust::getKey("Entity.PrimaryType");
const DustKey Entity::Types = Dust::getKey("Entity.Types");
