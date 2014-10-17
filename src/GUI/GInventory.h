/*
    This file is part of Avelen Project.

    Avelen Project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Avelen Project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Avelen Project.  If not, see <http://www.gnu.org/licenses/>.

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Authors of file: Mikhail Evdokimov

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#ifndef GINVENTORY_H
#define GINVENTORY_H
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
class ACharacterBase;
class GContainerSystem;
class GInGameContainerSystem;
#include "GContainer.h"


class GInventory : public GContainer {
  friend class GContainerSystem;
  friend class GContainer;
protected:
  ACharacterBase* character;
  void _setUsed();

  class ItemButton : public tgui::Button{
    GInventory* _inv = 0;


  public:

    InventorySlot _slot = InventorySize;
    void Refresh();
    APhysicObjectBase* obj = 0;
    typedef tgui::SharedWidgetPtr<ItemButton> Ptr;
    ItemButton();
    void Init(GInventory* inv, APhysicObjectBase* iObj, InventorySlot sl);

    void leftMousePressed(float x, float y);
    void _leftMouseReleased(float x, float y);
  };

  ItemButton* selButton = 0;
  ItemButton::Ptr* invCells;
  tgui::Label::Ptr* cellLabels;
  tgui::Label::Ptr noBag;

  ItemButton* sourceButton;
  void _invPickObject(ItemButton* b);
  APhysicObjectBase* _invPutObject(InventorySlot sl);
  void _putObject();

  inline void _contRefr();
  inline void _invRefr();
  inline bool _isContSource();
  inline void _clearSource();
  InventorySlot GetSourceSlot();
  APhysicObjectBase* GetSourceObject();
  void SetBag(APhysicObjectBase* bag);

public:
  ACharacterBase* GetCharacter() { return character; }
  void Refresh();
  GInventory(ACharacterBase* ch, GGUI& iGui, GContainerSystem* iCs);
  ~GInventory();
};


class GPlayerInventory : public GInventory {
  void _onClose();
public:
  GPlayerInventory(ACharacterBase* ch, GGUI& iGui, GContainerSystem* iCs);
};




#endif // GINVENTORY_H
