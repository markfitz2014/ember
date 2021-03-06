//
// C++ Implementation: SoundGroupDefinition
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SoundGroupDefinition.h"
#include "SoundDefinition.h"
#include "framework/LoggingInstance.h"

using namespace Ember;

namespace Ember {
namespace OgreView {

SoundGroupDefinition::SoundGroupDefinition()
{
}

SoundGroupDefinition::~SoundGroupDefinition()
{
// 	for (SoundDefinitionStore::
}

void SoundGroupDefinition::insertSample(const std::string& name, SoundGeneral::SoundSampleType type, float volume)
{
	SoundDefinition newDef;
	newDef.setup(name, type, volume);
	mSamples.push_back(newDef);

	S_LOG_INFO("\t-Sample " << name << " created.");

}

const SoundGroupDefinition::SoundDefinitionStore& SoundGroupDefinition::getSoundDefinitions() const
{
	return mSamples;
}


}
}
