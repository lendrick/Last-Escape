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
#pragma once
#include <map>
#include <list>

namespace ComponentType
{
enum Enum {
  Drawable,
  Physics,
  Health,
  Damage,
  AI
};
};

class Component
{
public:
  Component(ComponentType::Enum type);
  virtual ~Component();

  virtual void update() = 0;
  ComponentType::Enum getType();

protected:
  ComponentType::Enum componentType;
};

typedef std::list<Component *> ComponentList;
typedef std::map<ComponentType::Enum, ComponentList> AllComponentsList;

extern AllComponentsList allComponents;