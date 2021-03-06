/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
 
#include "precompiled.h"
#include "ElementFormControlSelect.h"
#include "SelectOptionsProxy.h"
#include <RmlUi/Controls/ElementFormControlSelect.h>
#include <RmlUi/Controls/ElementFormControl.h>
#include <RmlUi/Core/Element.h>
#include "ElementFormControl.h"
#include <RmlUi/Core/Lua/Utilities.h>


namespace Rml {
namespace Controls {
namespace Lua {

//methods
int ElementFormControlSelectAdd(lua_State* L, ElementFormControlSelect* obj)
{
    const char* rml = luaL_checkstring(L,1);
    const char* value = luaL_checkstring(L,2);
    int before = -1; //default
    if(lua_gettop(L) >= 3)
        before = luaL_checkinteger(L,3);

    int index = obj->Add(rml,value,before);
    lua_pushinteger(L,index);
    return 1;
}

int ElementFormControlSelectRemove(lua_State* L, ElementFormControlSelect* obj)
{
    int index = luaL_checkinteger(L,1);
    obj->Remove(index);
    return 0;
}

//getters
int ElementFormControlSelectGetAttroptions(lua_State* L)
{
    ElementFormControlSelect* obj = LuaType<ElementFormControlSelect>::check(L,1);
    LUACHECKOBJ(obj);
    SelectOptionsProxy* proxy = new SelectOptionsProxy();
    proxy->owner = obj;
    LuaType<SelectOptionsProxy>::push(L,proxy,true);
    return 1;
}

int ElementFormControlSelectGetAttrselection(lua_State* L)
{
    ElementFormControlSelect* obj = LuaType<ElementFormControlSelect>::check(L,1);
    LUACHECKOBJ(obj);
    int selection = obj->GetSelection();
    lua_pushinteger(L,selection);
    return 1;
}


//setter
int ElementFormControlSelectSetAttrselection(lua_State* L)
{
    ElementFormControlSelect* obj = LuaType<ElementFormControlSelect>::check(L,1);
    LUACHECKOBJ(obj);
    int selection = luaL_checkinteger(L,2);
    obj->SetSelection(selection);
    return 0;
}


Rml::Core::Lua::RegType<ElementFormControlSelect> ElementFormControlSelectMethods[] =
{
    LUAMETHOD(ElementFormControlSelect,Add)
    LUAMETHOD(ElementFormControlSelect,Remove)
    { NULL, NULL },
};

luaL_Reg ElementFormControlSelectGetters[] =
{
    LUAGETTER(ElementFormControlSelect,options)
    LUAGETTER(ElementFormControlSelect,selection)
    { NULL, NULL },
};

luaL_Reg ElementFormControlSelectSetters[] =
{
    LUASETTER(ElementFormControlSelect,selection)
    { NULL, NULL },
};

}
}
}
namespace Rml {
namespace Core {
namespace Lua {
//inherits from ElementFormControl which inherits from Element
template<> void ExtraInit<Rml::Controls::ElementFormControlSelect>(lua_State* L, int metatable_index)
{
    //init whatever elementformcontrol did extra, like inheritance
    ExtraInit<Rml::Controls::ElementFormControl>(L,metatable_index);
    //then inherit from elementformcontrol
    LuaType<Rml::Controls::ElementFormControl>::_regfunctions(L,metatable_index,metatable_index-1);
    AddTypeToElementAsTable<Rml::Controls::ElementFormControlSelect>(L);
}
using Rml::Controls::ElementFormControlSelect;
LUACONTROLSTYPEDEFINE(ElementFormControlSelect,true)
}
}
}