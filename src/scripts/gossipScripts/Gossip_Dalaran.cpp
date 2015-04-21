/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009-2012 ArcEmu Team <http://www.arcemu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "Setup.h"

class DedicationOfHonorGossip : public Arcemu::Gossip::Script
{
    public:
        void OnHello(Object* pObj, Player* plr)
        {
            Arcemu::Gossip::Menu::SendQuickMenu(pObj->GetGUID(), 15921, plr, 0, Arcemu::Gossip::ICON_CHAT, "See the fall of the Lich King.");
        }

        void OnSelectOption(Object* object, Player* player, uint32 Id, const char* enteredcode)
        {
            player->SendTriggerMovie(16);
            Arcemu::Gossip::Menu::Complete(player);
        }
};

void SetupDalaranGossip(ScriptMgr* mgr)
{
    mgr->register_go_gossip(202443, new DedicationOfHonorGossip);
}
