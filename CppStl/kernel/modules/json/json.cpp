#include <iostream>

#include "json.h"

using namespace std;

using namespace DPLUnitNarrative;
using namespace DPLUnitModel;
using namespace DPLUnitTools;
using namespace DPLUnitText;

/*
 https://tools.ietf.org/html/std90
 */

DPLEntity buildJsonParser() {
	DPLEntity eValue = DPLData::createEntity(TypeCtrlSelect);

// ws = *( %x20 / %x09 / %x0A / %x0D )
	DPLEntity eWSChars = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eWSChars, AttTextString, "\x20\x09\x0A\x0D");

	DPLEntity eWS = DPLData::createEntity(TypeCtrlRepeat);
	DPLData::setInt(eWS, AttLimitsIntMin, 0);
	DPLData::setInt(eWS, RefLinkTarget, eWSChars);

// value-separator = ws %x2C ws  ; , comma
	DPLEntity eValSep = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eValSep, AttTextString, ",");

// null  = %x6e.75.6c.6c      ; null
	DPLEntity eTokenNull = DPLData::createEntity(LogicTokenMatcher);
	DPLData::setString(eTokenNull, AttTextString, "null");
	DPLData::setInt(eTokenNull, RefLinkTarget, ValEntityNull);

// true  = %x74.72.75.65      ; true
	DPLEntity eTokenTrue = DPLData::createEntity(LogicTokenMatcher);
	DPLData::setString(eTokenTrue, AttTextString, "true");
	DPLData::setInt(eTokenTrue, RefLinkTarget, ValEntityTrue);

// false = %x66.61.6c.73.65   ; false
	DPLEntity eTokenFalse = DPLData::createEntity(LogicTokenMatcher);
	DPLData::setString(eTokenFalse, AttTextString, "false");
	DPLData::setInt(eTokenFalse, RefLinkTarget, ValEntityFalse);




// number = [ minus ] int [ frac ] [ exp ] - moved to Text
	DPLEntity eNumber = DPLData::createEntity(LogicNumberSci);




// escape = %x5C
	DPLEntity eEsc = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eEsc, AttTextString, "\x5C");

// quotation-mark = %x22
	DPLEntity eQuot = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eQuot, AttTextString, "\x22");

// unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
	DPLEntity eUnesc0 = DPLData::createEntity(TypeLimits);
	DPLData::setInt(eUnesc0, AttLimitsIntMin, 0x20);
	DPLData::setInt(eUnesc0, AttLimitsIntMax, 0x21);

	DPLEntity eUnesc1 = DPLData::createEntity(TypeLimits);
	DPLData::setInt(eUnesc1, AttLimitsIntMin, 0x23);
	DPLData::setInt(eUnesc1, AttLimitsIntMax, 0x5B);

	DPLEntity eUnesc2 = DPLData::createEntity(TypeLimits);
	DPLData::setInt(eUnesc2, AttLimitsIntMin, 0x5D);
	DPLData::setInt(eUnesc2, AttLimitsIntMax, 0x10FFFF);

	DPLEntity eUnesc = DPLData::createEntity(LogicCharMatcher);
	DPLData::setRef(eUnesc, RefCollectionMembers, eUnesc0);
	DPLData::setRef(eUnesc, RefCollectionMembers, eUnesc1);
	DPLData::setRef(eUnesc, RefCollectionMembers, eUnesc2);

// char = unescaped / escape ( %x22 / %x5C / %x2F / %x62 / %x66 / %x6E / %x72 / %x74 / %x75 4HEXDIG )
	DPLEntity eEscCode = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eEscCode, AttTextString, "\"\\/bfnrt");
	DPLData::setRef(eEscCode, RefCollectionMembers, ValCharQuot);
	DPLData::setRef(eEscCode, RefCollectionMembers, ValCharReverseSolidus);
	DPLData::setRef(eEscCode, RefCollectionMembers, ValCharSolidus);
	DPLData::setRef(eEscCode, RefCollectionMembers, ValCharCtrlBS);
	DPLData::setRef(eEscCode, RefCollectionMembers, ValCharCtrlFF);
	DPLData::setRef(eEscCode, RefCollectionMembers, ValCharCtrlLF);
	DPLData::setRef(eEscCode, RefCollectionMembers, ValCharCtrlCR);
	DPLData::setRef(eEscCode, RefCollectionMembers, ValCharCtrlTAB);

	DPLEntity eEscUnicodeRead = DPLData::createEntity(LogicNumberRad);
	DPLData::setInt(eEscUnicodeRead, AttLimitsIntMin, 4);
	DPLData::setInt(eEscUnicodeRead, AttLimitsIntMax, 4);
	DPLData::setInt(eEscUnicodeRead, AttNumRadix, 16);

	DPLEntity eEscUnicodePref = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eEscUnicodePref, AttTextString, "u");

	DPLEntity eEscUnicode = DPLData::createEntity(TypeCtrlSequence);
	DPLData::setRef(eEscUnicode, RefCollectionMembers, eEscUnicodePref);
	DPLData::setRef(eEscUnicode, RefCollectionMembers, eEscUnicodeRead);

	DPLEntity eEscape0 = DPLData::createEntity(TypeCtrlSelect);
	DPLData::setRef(eEscape0, RefCollectionMembers, eEscCode);
	DPLData::setRef(eEscape0, RefCollectionMembers, eEscUnicode);

	DPLEntity eEscape = DPLData::createEntity(TypeCtrlSequence);
	DPLData::setRef(eEscUnicode, RefCollectionMembers, eEsc);
	DPLData::setRef(eEscUnicode, RefCollectionMembers, eEscape0);

	DPLEntity eChar = DPLData::createEntity(TypeCtrlSelect);
	DPLData::setRef(eChar, RefCollectionMembers, eUnesc);
	DPLData::setRef(eChar, RefCollectionMembers, eEscape);

// string = quotation-mark *char quotation-mark
	DPLEntity eCharRep = DPLData::createEntity(TypeCtrlRepeat);
	DPLData::setInt(eCharRep, AttLimitsIntMin, 0);
	DPLData::setInt(eCharRep, RefLinkTarget, eChar);

	DPLEntity eString = DPLData::createEntity(TypeCtrlSequence);
	DPLData::setRef(eUnesc, RefCollectionMembers, eQuot);
	DPLData::setRef(eUnesc, RefCollectionMembers, eCharRep);
	DPLData::setRef(eUnesc, RefCollectionMembers, eQuot);




// begin-array     = ws %x5B ws  ; [ left square bracket
	DPLEntity eArrBegin = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eArrBegin, AttTextString, "[");

// end-array       = ws %x5D ws  ; ] right square bracket
	DPLEntity eArrEnd = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eArrEnd, AttTextString, "]");

	DPLEntity eArrC0 = DPLData::createEntity(TypeCtrlSequence);
	DPLData::setRef(eArrC0, RefCollectionMembers, eWS);
	DPLData::setRef(eArrC0, RefCollectionMembers, eValue);
	DPLData::setRef(eArrC0, RefCollectionMembers, eWS);

	DPLEntity eArrRep = DPLData::createEntity(TypeCtrlRepeat);
	DPLData::setInt(eArrRep, AttLimitsIntMin, 0);
	DPLData::setInt(eArrRep, RefLinkTarget, eArrC0);
	DPLData::setInt(eArrRep, RefCollectionSeparator, eValSep);

// array = begin-array [ value *( value-separator value ) ] end-array
	DPLEntity eArray = DPLData::createEntity(TypeCtrlSequence);
	DPLData::setRef(eArray, RefCollectionMembers, eWS);
	DPLData::setRef(eArray, RefCollectionMembers, eArrBegin);
	DPLData::setRef(eArray, RefCollectionMembers, eWS);
	DPLData::setRef(eArray, RefCollectionMembers, eArrRep);
	DPLData::setRef(eArray, RefCollectionMembers, eWS);
	DPLData::setRef(eArray, RefCollectionMembers, eArrEnd);
	DPLData::setRef(eArray, RefCollectionMembers, eWS);





// begin-object    = ws %x7B ws  ; { left curly bracket
	DPLEntity eObjBegin = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eObjBegin, AttTextString, "{");

// end-object      = ws %x7D ws  ; } right curly bracket
	DPLEntity eObjEnd = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eObjEnd, AttTextString, "}");

// name-separator  = ws %x3A ws  ; : colon
	DPLEntity eNameSep = DPLData::createEntity(LogicCharMatcher);
	DPLData::setString(eNameSep, AttTextString, ":");

// member = string name-separator value
	DPLEntity eObjMember = DPLData::createEntity(TypeCtrlSequence);
	DPLData::setRef(eObjMember, RefCollectionMembers, eWS);
	DPLData::setRef(eObjMember, RefCollectionMembers, eString);
	DPLData::setRef(eObjMember, RefCollectionMembers, eWS);
	DPLData::setRef(eObjMember, RefCollectionMembers, eNameSep);
	DPLData::setRef(eObjMember, RefCollectionMembers, eWS);
	DPLData::setRef(eObjMember, RefCollectionMembers, eValue);
	DPLData::setRef(eObjMember, RefCollectionMembers, eWS);

	DPLEntity eObjRep = DPLData::createEntity(TypeCtrlRepeat);
	DPLData::setInt(eObjRep, AttLimitsIntMin, 0);
	DPLData::setInt(eObjRep, RefLinkTarget, eObjMember);
	DPLData::setInt(eObjRep, RefCollectionSeparator, eValSep);

// object = begin-object [ member *( value-separator member ) ] end-object
	DPLEntity eObject = DPLData::createEntity(TypeCtrlSequence);
	DPLData::setRef(eObject, RefCollectionMembers, eWS);
	DPLData::setRef(eObject, RefCollectionMembers, eObjBegin);
	DPLData::setRef(eObject, RefCollectionMembers, eWS);
	DPLData::setRef(eObject, RefCollectionMembers, eObjRep);
	DPLData::setRef(eObject, RefCollectionMembers, eWS);
	DPLData::setRef(eObject, RefCollectionMembers, eObjEnd);
	DPLData::setRef(eObject, RefCollectionMembers, eWS);




// value = false / null / true / object / array / number / string
	DPLData::setRef(eValue, RefCollectionMembers, eTokenFalse);
	DPLData::setRef(eValue, RefCollectionMembers, eTokenNull);
	DPLData::setRef(eValue, RefCollectionMembers, eTokenTrue);
	DPLData::setRef(eValue, RefCollectionMembers, eObject);
	DPLData::setRef(eValue, RefCollectionMembers, eArray);
	DPLData::setRef(eValue, RefCollectionMembers, eNumber);
	DPLData::setRef(eValue, RefCollectionMembers, eString);




// JSON-text = ws value ws
	DPLEntity eJson = DPLData::createEntity(TypeCtrlSequence);
	DPLData::setRef(eJson, RefCollectionMembers, eWS);
	DPLData::setRef(eJson, RefCollectionMembers, eValue);
	DPLData::setRef(eJson, RefCollectionMembers, eWS);

	return eJson;
}
