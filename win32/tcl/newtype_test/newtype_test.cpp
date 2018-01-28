#include <tcl.h>
#include <string>
#include <algorithm>
#include <iostream>

extern "C" {
int DLLEXPORT Newtype_Init(Tcl_Interp*);
}

int SketchyString_SetFromAnyProc(Tcl_Interp*, Tcl_Obj*);
void SketchyString_UpdateStringProc(Tcl_Obj*);
void SketchyString_DupInternalRepPric(Tcl_Obj*, Tcl_Obj*);
void SketchyString_FreeInternalRepProc(Tcl_Obj*);

Tcl_ObjType SketchyString;

int MakeString_Cmd(ClientData cdata, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[])
{
	Tcl_Obj* result = Tcl_NewObj();
	if (Tcl_ConvertToType(interp, result, &SketchyString) != TCL_OK) {
		return TCL_ERROR;
	}
	std::string& str = *(static_cast<std::string*>(result->internalRep.otherValuePtr));
	str = "test";
	Tcl_InvalidateStringRep(result);
	Tcl_SetObjResult(interp, result);
	return TCL_OK;
}

int SketchyString_SetFromAnyProc(Tcl_Interp* interp, Tcl_Obj* obj)
{
	const char* str_rep = Tcl_GetString(obj);
	std::string* str = new std::string(str_rep);
	if (obj->typePtr && obj->typePtr->freeIntRepProc) {
		obj->typePtr->freeIntRepProc(obj);
	}
	obj->typePtr = &SketchyString;
	obj->internalRep.otherValuePtr = str;
	return TCL_OK;
}

void SketchyString_UpdateStringProc(Tcl_Obj* obj)
{
	const std::string& str = *(static_cast<std::string*>(obj->internalRep.otherValuePtr));
	char* buf = Tcl_Alloc(str.size() + 1);
	str.copy(buf, str.size());
	buf[str.size()] = '\0';
	obj->bytes = buf;
	obj->length = str.size();
}

void SketchyString_DupInternalRepProc(Tcl_Obj* src_ptr, Tcl_Obj* dest_ptr)
{
	std::cout << src_ptr->internalRep.otherValuePtr << ' ' << dest_ptr->internalRep.otherValuePtr << '\n';
}

void SketchyString_FreeInternalRepProc(Tcl_Obj* obj)
{
	delete static_cast<std::string*>(obj->internalRep.otherValuePtr);
}

int DLLEXPORT Newtype_Init(Tcl_Interp* interp)
{
	SketchyString.name = "SketchyString";
	SketchyString.freeIntRepProc = SketchyString_FreeInternalRepProc;
	SketchyString.dupIntRepProc = SketchyString_DupInternalRepProc;
	SketchyString.updateStringProc = SketchyString_UpdateStringProc;
	SketchyString.setFromAnyProc = SketchyString_SetFromAnyProc;

	if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
		return TCL_ERROR;
	}

	if (Tcl_PkgProvide(interp, "NewTypeTest", "0.1") == TCL_ERROR) {
		return TCL_ERROR;
	}

	Tcl_RegisterObjType(&SketchyString);

	Tcl_CreateObjCommand(interp, "MakeString", MakeString_Cmd, NULL, NULL);
	return TCL_OK;
}
