/*
 *  This file is part of Last Escape.
 *
 *  Last Escape is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Last Escape is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Last Escape.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Entity.h"
#include <algorithm>

EntityList allEntities;

Entity::Entity()
{
  allEntities.push_back(this);
}

Entity::~Entity()
{
  ComponentMap::iterator c;
  for(c = components.begin(); c != components.end(); ++c) {
    delete (*c).second;
    components.erase(c);
  }

  EntityList::iterator i;
  i = find(allEntities.begin(), allEntities.end(), this);
  allEntities.erase(i);
}

void Entity::addComponent(Component * component)
{
  components[component->getType()] = component;
}

void Entity::update(ComponentType::Enum type)
{
  components[type]->update();
}