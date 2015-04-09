/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "DalaranSewers.h"
struct DS_ObjectLocation
{
    uint32 entry;
    float x, y, z;
    float orientation;
    float rotation[4];
    uint32 flags;
    uint16 faction;
};

#define MAX_DS_GATES 2
static DS_ObjectLocation DS_Gates[MAX_DS_GATES] =
{
    { 192642, 1350.95f, 817.2f, 20.8096f, 3.15f, 0, 0, 0.99627f, 0.0862864f, 32, 1375 },
    { 192643, 1232.65f, 764.913f, 20.0729f, 6.3f, 0, 0, 0.0310211f, -0.999519f, 32, 1375 },   // Door 2
};


#define MAX_DS_ETC_OBJECTS 4
// need to get correct timers for these objects - they should be pushed to world on time end.
static DS_ObjectLocation DS_Objects[] =
{
    { 194395, 1291.56f, 790.837f, 7.1f, 3.14238f, 0, 0, 0.694215f, -0.719768f, 32, 1375 },   // Water spot 1
    { 191877, 1291.56f, 790.837f, 7.1f, 3.14238f, 0, 0, 0.694215f, -0.719768f, 32, 1375 },   // Water Spot 2
    { 184663, 1291.7f, 813.424f, 7.11472f, 4.64562f, 0, 0, 0.730314f, -0.683111f, 32, 1375 },   // Buff 1 (Shadow sight)
    { 184664, 1291.7f, 768.911f, 7.11472f, 1.55194f, 0, 0, 0.700409f, 0.713742f, 32, 1375 }     // Buff 2  (Shadow sight)
};

static LocationVector DS_StartLoc[MAX_PLAYER_TEAMS] =
{
    LocationVector(1219.5115f, 765.0264f, 14.8253f),
    LocationVector(1363.3609f, 817.3569f, 14.8128f)
};

static LocationVector DS_Repop = LocationVector(1292.51f, 792.05f, 9.34f);

DalaranSewers::DalaranSewers( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side ) : Arena( mgr, id, lgroup, t, players_per_side ) {}

DalaranSewers::~DalaranSewers()
{
}

void DalaranSewers::OnCreate(){

    Log.Debug("DalaranSewers::OnCreate", "Spawning gates");
    // Spawning gates
    for (uint8 i = 0; i < MAX_DS_GATES; i++)
    {
        if (GameObjectInfo* pInfo = GameObjectNameStorage.LookupEntry(DS_Gates[i].entry))
        {
            if (GameObject* pObj = SpawnGameObject(DS_Gates[i].entry, MAP_DALARAN_SEWERS, DS_Gates[i].x, DS_Gates[i].y, DS_Gates[i].z, DS_Gates[i].orientation, DS_Gates[i].flags, DS_Gates[i].faction, pInfo->Size))
            {
                for (uint8 x = 0; x < 4; x++)
                    pObj->SetRotation(DS_Gates[i].rotation[x]);

                pObj->SetState(GAMEOBJECT_STATE_CLOSED);
                pObj->SetAnimProgress(100);
                m_gates.insert(pObj);
            }
            else
                Log.Error("DalaranSewers::OnCreate", "Failed to spawn object entry %u", DS_Gates[i].entry);
        }
        else
            Log.Error("DalaranSewers::OnCreate", "Cannot find information for gameobject entry %u", DS_Gates[i].entry);
    }

    /*
    Log.Debug("DalaranSewers::OnCreate", "Spawning other objects");
    for (uint8 i = 0; i < MAX_DS_ETC_OBJECTS; i++)
    {
        if (GameObjectInfo* pInfo = GameObjectNameStorage.LookupEntry(DS_Objects[i].entry))
        {
            if (GameObject* pObj = SpawnGameObject(DS_Objects[i].entry, MAP_DALARAN_SEWERS, DS_Objects[i].x, DS_Objects[i].y, DS_Objects[i].z, DS_Objects[i].orientation, DS_Objects[i].flags, DS_Objects[i].faction, pInfo->Size))
            {
                for (uint8 x = 0; x < 4; x++)
                    pObj->SetRotation(DS_Objects[i].rotation[x]);

                pObj->SetState(GAMEOBJECT_STATE_CLOSED);
                pObj->SetAnimProgress(100);
                pObj->PushToWorld(GetMapMgr());
            }
            else
                Log.Error("DalaranSewers::OnCreate", "Failed to spawn object entry %u", DS_Objects[i].entry);
        }
        else
            Log.Error("DalaranSewers::OnCreate", "Cannot find information for gameobject entry %u", DS_Objects[i].entry);
    }
    */
    Arena::OnCreate();
}

LocationVector DalaranSewers::GetStartingCoords( uint8 Team ){
    return DS_StartLoc[Team];
}

bool DalaranSewers::HookHandleRepop( Player *plr ){
    plr->SafeTeleport(m_mapMgr->GetMapId(), m_mapMgr->GetInstanceID(), DS_Repop);
    return true;
}

