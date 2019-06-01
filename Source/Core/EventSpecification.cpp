/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
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
#include "EventSpecification.h"
#include "../../Include/Rocket/Core/ID.h"


namespace Rocket {
namespace Core {

// An EventId is an index into the specifications vector.
static std::vector<EventSpecification> specifications = { { EventId::Invalid, "invalid", false, false, DefaultActionPhase::None } };

// Reverse lookup map from event type to id.
static UnorderedMap<String, EventId> type_lookup;


namespace EventSpecificationInterface {

void Initialize()
{
	// Must be specified in the same order as in EventId
	specifications = {
		//      id                 type      interruptible  bubbles     default_action
		{EventId::Invalid       , "invalid"       , false , false , DefaultActionPhase::None},
		{EventId::Mousedown     , "mousedown"     , true  , true  , DefaultActionPhase::TargetAndBubble},
		{EventId::Mousescroll   , "mousescroll"   , true  , true  , DefaultActionPhase::TargetAndBubble},
		{EventId::Mouseover     , "mouseover"     , true  , true  , DefaultActionPhase::Target},
		{EventId::Mouseout      , "mouseout"      , true  , true  , DefaultActionPhase::Target},
		{EventId::Focus         , "focus"         , false , false , DefaultActionPhase::Target},
		{EventId::Blur          , "blur"          , false , false , DefaultActionPhase::Target},
		{EventId::Keydown       , "keydown"       , true  , true  , DefaultActionPhase::TargetAndBubble},
		{EventId::Keyup         , "keyup"         , true  , true  , DefaultActionPhase::TargetAndBubble},
		{EventId::Textinput     , "textinput"     , true  , true  , DefaultActionPhase::TargetAndBubble},
		{EventId::Mouseup       , "mouseup"       , true  , true  , DefaultActionPhase::TargetAndBubble},
		{EventId::Click         , "click"         , true  , true  , DefaultActionPhase::TargetAndBubble},
		{EventId::Dblclick      , "dblclick"      , true  , true  , DefaultActionPhase::TargetAndBubble},
		{EventId::Load          , "load"          , false , false , DefaultActionPhase::None},
		{EventId::Unload        , "unload"        , false , false , DefaultActionPhase::None},
		{EventId::Show          , "show"          , false , false , DefaultActionPhase::None},
		{EventId::Hide          , "hide"          , false , false , DefaultActionPhase::None},
		{EventId::Mousemove     , "mousemove"     , true  , true  , DefaultActionPhase::None},
		{EventId::Dragmove      , "dragmove"      , true  , true  , DefaultActionPhase::Target},
		{EventId::Drag          , "drag"          , false , true  , DefaultActionPhase::Target},
		{EventId::Dragstart     , "dragstart"     , false , true  , DefaultActionPhase::Target},
		{EventId::Dragover      , "dragover"      , true  , false , DefaultActionPhase::Target},
		{EventId::Dragdrop      , "dragdrop"      , true  , false , DefaultActionPhase::Target},
		{EventId::Dragout       , "dragout"       , true  , false , DefaultActionPhase::Target},
		{EventId::Dragend       , "dragend"       , true  , true  , DefaultActionPhase::None},
		{EventId::Handledrag    , "handledrag"    , false , true  , DefaultActionPhase::None},
		{EventId::Resize        , "resize"        , false , false , DefaultActionPhase::None},
		{EventId::Scroll        , "scroll"        , false , true  , DefaultActionPhase::None},
		{EventId::Animationend  , "animationend"  , true  , true  , DefaultActionPhase::None},
		{EventId::Transitionend , "transitionend" , true  , true  , DefaultActionPhase::None},
								 				 
		{EventId::Change        , "change"        , false , true  , DefaultActionPhase::None},
		{EventId::Submit        , "submit"        , true  , true  , DefaultActionPhase::None},
		{EventId::Tabchange     , "tabchange"     , false , true  , DefaultActionPhase::None},
		{EventId::Columnadd     , "columnadd"     , false , true  , DefaultActionPhase::None},
		{EventId::Rowadd        , "rowadd"        , false , true  , DefaultActionPhase::None},
		{EventId::Rowchange     , "rowchange"     , false , true  , DefaultActionPhase::None},
		{EventId::Rowremove     , "rowremove"     , false , true  , DefaultActionPhase::None},
		{EventId::Rowupdate     , "rowupdate"     , false , true  , DefaultActionPhase::None},
	};

	type_lookup.clear();
	type_lookup.reserve(specifications.size());
	for (auto& specification : specifications)
		type_lookup.emplace(specification.type, specification.id);

#ifdef ROCKET_DEBUG
	// Verify that all event ids are specified
	ROCKET_ASSERT((int)specifications.size() == (int)EventId::NumDefinedIds);

	for (int i = 0; i < (int)specifications.size(); i++)
	{
		// Verify correct order
		ROCKET_ASSERT(i == (int)specifications[i].id);
	}
#endif
}

const EventSpecification& Get(EventId id)
{
	size_t i = static_cast<size_t>(id);
	if (i < specifications.size())
		return specifications[i];
	return specifications[0];
}

const EventSpecification& GetOrInsert(const String& event_type)
{
	// Default values for new event types defined as follows:
	constexpr bool interruptible = true;
	constexpr bool bubbles = true;
	constexpr DefaultActionPhase default_action_phase = DefaultActionPhase::None;

	return GetOrInsert(event_type, interruptible, bubbles, default_action_phase);
}

const EventSpecification& GetOrInsert(const String& event_type, bool interruptible, bool bubbles, DefaultActionPhase default_action_phase)
{
	auto it = type_lookup.find(event_type);

	if (it != type_lookup.end())
		return Get(it->second);

	// No specification found for this name, insert a new entry with default values
	EventId new_id = static_cast<EventId>(specifications.size());
	specifications.push_back(EventSpecification{ new_id, event_type, interruptible, bubbles, default_action_phase });
	type_lookup.emplace(event_type, new_id);
	return specifications.back();
}

EventId GetIdOrInsert(const String& event_type)
{
	auto it = type_lookup.find(event_type);
	if (it != type_lookup.end())
		return it->second;

	return GetOrInsert(event_type).id;
}


}
}
}