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
#ifndef RMLUICOREPROPERTIESITERATOR_H
#define RMLUICOREPROPERTIESITERATOR_H

#include "../../Include/RmlUi/Core/Types.h"
#include "DirtyPropertyList.h"
#include "ElementDefinition.h"

namespace Rml {
namespace Core {



// An iterator for local properties defined on an element.
// Note: Modifying the underlying style invalidates the iterator.
class PropertiesIterator {
public:
	using ValueType = std::pair<PropertyId, const Property&>;
	using PropertyIt = PropertyMap::const_iterator;

	PropertiesIterator(const PseudoClassList& element_pseudo_classes, PropertyIt it_style, PropertyIt it_style_end, PropertyIt it_definition, PropertyIt it_definition_end)
		: element_pseudo_classes(&element_pseudo_classes), it_style(it_style), it_style_end(it_style_end), it_definition(it_definition), it_definition_end(it_definition_end)
	{
		ProceedToNextValid();
	}

	PropertiesIterator& operator++() {
		if (it_style != it_style_end)
			// We iterate over the local style properties first
			++it_style;
		else
			// .. and then over the properties given by the element's definition
			++it_definition;
		// If we reached the end of one of the iterator pairs, we need to continue iteration on the next pair.
		ProceedToNextValid();
		return *this;
	}

	ValueType operator*() const
	{
		if (it_style != it_style_end)
			return { it_style->first, it_style->second };
		return { it_definition->first, it_definition->second };
	}

	bool AtEnd() const {
		return at_end;
	}

	// Return the list of pseudo classes which defines the current property, possibly null.
	const PseudoClassList* GetPseudoClassList() const
	{
		// TODO: Not implemented
		return nullptr;
	}

private:
	const PseudoClassList* element_pseudo_classes;
	DirtyPropertyList iterated_properties;
	PropertyIt it_style, it_style_end;
	PropertyIt it_definition, it_definition_end;
	bool at_end = false;

	inline bool IsDirtyRemove(PropertyId id)
	{
		if (!iterated_properties.Contains(id))
		{
			iterated_properties.Insert(id);
			return true;
		}
		return false;
	}

	inline void ProceedToNextValid() 
	{
		for (; it_style != it_style_end; ++it_style)
		{
			if (IsDirtyRemove(it_style->first))
				return;
		}

		for (; it_definition != it_definition_end; ++it_definition)
		{
			if (IsDirtyRemove(it_definition->first))
				return;
		}

		// All iterators are now at the end
		at_end = true;
	}
};


}
}

#endif