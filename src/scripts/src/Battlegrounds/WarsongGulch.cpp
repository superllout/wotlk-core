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

#include "StdAfx.h"
#include "WarsongGulch.h"

#define SPELL_RECENTLY_DROPPED_FLAG 42792

// Gives citeria for "Call to Arms : Warsong Gulch(both factions)"
static uint32 wsgRewardSpells[MAX_WSG_REWARD_SPELLS] = { 69158, 69456, 69497, 69498 };

static LocationVector wsgStartLocations[MAX_PLAYER_TEAMS] =
{
    LocationVector(1523.81f, 1481.76f, 352.008f, M_PI_FLOAT), // Alliance
    LocationVector(933.331f, 1433.72f, 345.536f, M_PI_FLOAT)  // Horde
};

static LocationVector wsgRepopLocation[MAX_PLAYER_TEAMS] =
{
    LocationVector(1415.33f, 1554.79f, 343.156f, 3.124139f), // Alliance
    LocationVector(1029.14f, 1387.49f, 340.836f, 0.043200f)  // Horde
};

static wsgObjectLocation wsgFlags[MAX_WSG_FLAGS] =
{
    { 179831, 916.0226f, 1434.405f, 345.413f, 0.01745329f, 0, 0, 0.008726535f, 0.9999619f, 0, 210 },   // Horde flag
    { 179830, 1540.423f, 1481.325f, 351.8284f, 3.089233f, 0, 0, 0.9996573f, 0.02617699f, 0, 1314 }   // Alliance flag
};

static wsgObjectLocation wsgBuffs[MAX_WSG_BUFFS] =
{
    { 179871, 1449.93f, 1470.71f, 342.6346f, -1.64061f, 0, 0, 0.7313537f, -0.6819983f, 0, 114 },      // Speed buff (1)
    { 179871, 1005.171f, 1447.946f, 335.9032f, 1.64061f, 0, 0, 0.7313537f, 0.6819984f, 0, 114 },      // Speed buff (2)
    { 179904, 1317.506f, 1550.851f, 313.2344f, -0.2617996f, 0, 0, 0.1305263f, -0.9914448f, 0, 114 },  // Regen buff (1)
    { 179904, 1110.451f, 1353.656f, 316.5181f, -0.6806787f, 0, 0, 0.333807f, -0.9426414f, 0, 114 },   // Regen buff (2)
    { 179905, 1320.09f, 1378.79f, 314.7532f, 1.186824f, 0, 0, 0.5591929f, 0.8290376f, 0, 114 },       // Berserker buff (1)
    { 179905, 1139.688f, 1560.288f, 306.8432f, -2.443461f, 0, 0, 0.9396926f, -0.3420201f, 0, 114 },   // berserker buff (2)
};

#define MAX_WSG_GATES 10
static wsgObjectLocation wsgGates[MAX_WSG_GATES] =
{
    // Alliance gates
    { 179918, 1503.335f, 1493.466f, 352.1888f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, 33, 114 },
    { 179919, 1492.478f, 1457.912f, 342.9689f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, 33, 114 },
    { 179920, 1468.503f, 1494.357f, 351.8618f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, 33, 114 },
    { 179921, 1471.555f, 1458.778f, 362.6332f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, 33, 114 },
    { 179922, 1492.347f, 1458.34f, 342.3712f, -0.03490669f, 0, 0, 0.01745246f, -0.9998477f, 33, 114 },
    { 179922, 1503.466f, 1493.367f, 351.7352f, -0.03490669f, 0, 0, 0.01745246f, -0.9998477f, 33, 114 },

    // Horde gates
    { 179916, 949.1663f, 1423.772f, 345.6241f, -0.5756807f, -0.01673368f, -0.004956111f, -0.2839723f, 0.9586737f, 32, 114 },
    { 179917, 953.0507f, 1459.842f, 340.6526f, -1.99662f, -0.1971825f, 0.1575096f, -0.8239487f, 0.5073641f, 32, 114 },
    { 180322, 949.9523f, 1422.751f, 344.9273f, 0.0f, 0, 0, 0, 1.0f, 32, 114 },
    { 180322, 950.7952f, 1459.583f, 342.1523f, 0.05235988f, 0, 0, 0.02617695f, 0.9996573f, 32, 114 }
};

WarsongGulch::WarsongGulch(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
{
    // Set general stuff
    m_zoneid = 3277;
    m_lgroup = lgroup;

    // Set stats
    for (uint8 i = 0; i < MAX_PLAYER_TEAMS; i++)
    {
        m_players[i].clear();
        m_pendPlayers[i].clear(); 
        m_flagHolders[i] = 0;
        m_scores[i] = 0;
    }

    // Clear general containers
    m_resurrectMap.clear();
    m_pvpData.clear();

    // Create the buffs
    for (uint8 i = 0; i < MAX_WSG_BUFFS; i++)
        m_buffs[i] = SpawnBgGameObject(wsgBuffs[i]);

    for (uint8 i = 0; i < MAX_WSG_FLAGS; i++)
    {
        // Create flags 
        m_homeFlags[i] = SpawnBgGameObject(wsgFlags[i]);

        // Create dropped flags
        m_dropFlags[i] = m_mapMgr->CreateGameObject(i == TEAM_ALLIANCE ? DROPPED_ALLIANCE_WSG_FLAG_ENTRY : DROPPED_HORDE_WSG_FLAG_ENTRY);
        if (m_dropFlags[i] && m_dropFlags[i]->CreateFromProto(i == TEAM_ALLIANCE ? DROPPED_ALLIANCE_WSG_FLAG_ENTRY : DROPPED_HORDE_WSG_FLAG_ENTRY, MAP_WARSONG_GULCH, 0.0f, 0.0f, 0.0f, 0.0f))
        {
            m_dropFlags[i]->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);
            m_dropFlags[i]->SetScale(2.5f);
        }
        else
            Log.Warning("WarsongGulch", "Could not create %s dropped flag", i == TEAM_ALLIANCE ? "alliance" : "horde");
    }
}

WarsongGulch::~WarsongGulch()
{
    // Remove buffs
    for(uint8 i = 0; i < MAX_WSG_BUFFS; i++)
    {
        // buffs may not be spawned, so delete them if they're not
        if(m_buffs[i] != NULL && !m_buffs[i]->IsInWorld())
            delete m_buffs[i];
    }

    // Remove flags
    for (uint8 i = 0; i < MAX_WSG_FLAGS; i++)
    {
        if(m_dropFlags[i] && !m_dropFlags[i]->IsInWorld())
            delete m_dropFlags[i];

        if(m_homeFlags[i] && !m_homeFlags[i]->IsInWorld())
            delete m_homeFlags[i];
    }

    for (list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
    {
        if((*itr) != NULL)
        {
            if(!(*itr)->IsInWorld())
                delete(*itr);
        }
    }

    m_resurrectMap.clear();
}

void WarsongGulch::HookOnAreaTrigger(Player* plr, uint32 areaId)
{
    switch (areaId)
    {
        case 3686:      // Speed
        case 3687:      // Speed (Horde)
        case 3706:      // Restoration
        case 3708:      // Restoration (Horde)
        case 3707:      // Berserking
        case 3709:      // Berserking (Horde)
            rewardObjectBuff(areaId, plr);
            break;
        case 3646:      // Alliance base (alliance flag spawn location)
        case 3647:      // Horde base (horde flag spawn location)
        {
            if (canPlayerCaptureFlag(plr, areaId))
            {
                onCaptureFlag(plr);
                if (m_scores[plr->GetTeam()] == MAX_WSG_POINTS)
                    giveRewardsAndFinalize(plr->GetTeam());

                SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_HORDE_SCORE : WORLDSTATE_WSG_ALLIANCE_SCORE, m_scores[plr->GetTeam()]);
                UpdatePvPData();
            }
        }break;
        default:
            break;
    }
}

void WarsongGulch::EventReturnFlags()
{
    for(uint8 x = 0; x < MAX_WSG_FLAGS; x++)
    {
        if(m_homeFlags[x] != NULL)
            m_homeFlags[x]->PushToWorld(m_mapMgr);
    }
    PlaySoundToAll(8232);   // Sound for placed flag
    SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Alliance's flag is now placed at her base.");
    SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Horde's flag is now placed at her base.");
}

void WarsongGulch::HookOnFlagDrop(Player* plr)
{
    if(!plr->m_bgHasFlag || m_dropFlags[plr->GetTeam()]->IsInWorld())
        return;

    m_dropFlags[plr->GetTeam()]->SetPosition(plr->GetPosition());
    m_dropFlags[plr->GetTeam()]->PushToWorld(m_mapMgr);

    m_flagHolders[plr->GetTeam()] = 0;
    plr->m_bgHasFlag = false;
    plr->RemoveAura(23333 + (plr->GetTeam() * 2));

    SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);

    sEventMgr.AddEvent(this, &WarsongGulch::ReturnFlag, plr->GetTeam(), EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + plr->GetTeam(), 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

    SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, plr->GetGUID(), "The %s flag was dropped by %s!", plr->IsTeamHorde() ? "Alliance" : "Horde", plr->GetName());
    if (plr->isAlive())
        plr->CastSpell(plr, SPELL_RECENTLY_DROPPED_FLAG, true);
}

void WarsongGulch::HookFlagDrop(Player* plr, GameObject* obj)
{
    if (plr->HasAura(SPELL_RECENTLY_DROPPED_FLAG))
        return;

    /* picking up a dropped flag */
    if (m_dropFlags[plr->GetTeam()] != obj)
    {
        /* are we returning it? */
        if((obj->GetEntry() == 179785 && plr->IsTeamAlliance()) ||
                (obj->GetEntry() == 179786 && plr->IsTeamHorde()))
        {
            uint8 team = plr->GetTeam();
            sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + (plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE));

            if(m_dropFlags[team]->IsInWorld())
                m_dropFlags[team]->RemoveFromWorld(false);
            else
                m_homeFlags[team]->PushToWorld(m_mapMgr);

            plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAGS_RETURNED]++;
            UpdatePvPData();

            SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "The %s flag was returned to its base by %s!", plr->IsTeamHorde() ? "Horde" : "Alliance", plr->GetName());
            PlaySoundToAll(SOUND_BATTLEGROUND_FLAG_RETURNED);

            SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);
        }
        return;
    }

    map<uint32, uint32>::iterator itr = plr->m_forcedReactions.find(1059);
    if(itr != plr->m_forcedReactions.end())
        return;

    if(plr->IsTeamAlliance())
        sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG);
    else
        sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + 1);

    if(m_dropFlags[plr->GetTeam()]->IsInWorld())
        m_dropFlags[plr->GetTeam()]->RemoveFromWorld(false);

    m_flagHolders[plr->GetTeam()] = plr->GetLowGUID();
    plr->m_bgHasFlag = true;

    m_dropFlags[plr->GetTeam()]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());

    if (SpellEntry* pSpell = dbcSpell.LookupEntry(plr->IsTeamHorde() ? WSG_SPELL_ALLIANCE_FLAG : WSG_SPELL_HORDE_FLAG))
        plr->CastSpell(plr, pSpell, true);

    SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 2);
    SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "The %s flag has been taken by %s!", plr->IsTeamHorde() ? "Alliance's" : "Horde's", plr->GetName());
}

void WarsongGulch::ReturnFlag(uint32 team)
{
    if(m_dropFlags[team]->IsInWorld())
        m_dropFlags[team]->RemoveFromWorld(false);

    if(!m_homeFlags[team]->IsInWorld())
        m_homeFlags[team]->PushToWorld(m_mapMgr);

    SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The %s flag was returned to its base!", team == TEAM_ALLIANCE ? "Horde" : "Alliance");
    PlaySoundToAll(SOUND_BATTLEGROUND_FLAG_RETURNED);
}

void WarsongGulch::HookFlagStand(Player* plr, GameObject* obj)
{
#ifdef ANTI_CHEAT
    if(!m_started)
    {
        Anticheat_Log->writefromsession(plr->GetSession(), "%s tryed to hook the flag in warsong gluch before battleground (ID %u) started.", plr->GetName(), this->m_id);
        SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, plr->GetGUID(), "%s will be removed from the game for cheating.", plr->GetName());
        // Remove player from battleground.
        this->RemovePlayer(plr, false);
        // Kick player from server.
        plr->Kick(6000);
        return;
    }
#endif

    if(m_flagHolders[plr->GetTeam()] || m_homeFlags[plr->GetTeam()] != obj || m_dropFlags[plr->GetTeam()]->IsInWorld())
    {
        // cheater!
        return;
    }

    map<uint32, uint32>::iterator itr = plr->m_forcedReactions.find(1059);
    if(itr != plr->m_forcedReactions.end())
        return;

    if (SpellEntry* pSpell = dbcSpell.LookupEntry(plr->IsTeamHorde() ? WSG_SPELL_ALLIANCE_FLAG : WSG_SPELL_HORDE_FLAG))
        plr->CastSpell(plr, pSpell, true);

    /* set the flag holder */
    plr->m_bgHasFlag = true;
    m_flagHolders[plr->GetTeam()] = plr->GetLowGUID();
    if(m_homeFlags[plr->GetTeam()]->IsInWorld())
        m_homeFlags[plr->GetTeam()]->RemoveFromWorld(false);

    PlaySoundToAll(plr->IsTeamHorde() ? 8212 : 8174);   // Pick up sounds

    SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "The %s flag has been taken by %s!", plr->IsTeamHorde() ? "Alliance's" : "Horde's", plr->GetName());
    SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 2);
}

void WarsongGulch::HookOnPlayerKill(Player* plr, Player* pVictim)
{
    plr->m_bgScore.KillingBlows++;
    UpdatePvPData();
}

void WarsongGulch::HookOnHK(Player* plr)
{
    plr->m_bgScore.HonorableKills++;
    UpdatePvPData();
}

void WarsongGulch::OnAddPlayer(Player* plr)
{
    if (!m_started && plr->IsInWorld())
    {
        plr->CastSpell(plr, BG_PREPARATION, true);
        plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAGS_CAPTURED] = 0;
        plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAGS_RETURNED] = 0;
    }
    UpdatePvPData();
}

void WarsongGulch::OnRemovePlayer(Player* plr)
{
    /* drop the flag if we have it */
    if (plr->m_bgHasFlag)
        HookOnMount(plr);

    if (plr->HasAura(BG_PREPARATION))
        plr->RemoveAura(BG_PREPARATION);

    if (Pet* pPet = plr->GetSummon())
        pPet->GetAIInterface()->SetAllowedToEnterCombat(true);
}

LocationVector WarsongGulch::GetStartingCoords(uint8 Team)
{
    return wsgStartLocations[Team];
}

void WarsongGulch::HookOnPlayerDeath(Player* plr)
{
    plr->m_bgScore.Deaths++;

    /* do we have the flag? */
    if (plr->HasAura(plr->IsTeamHorde() ? WSG_SPELL_ALLIANCE_FLAG : WSG_SPELL_HORDE_FLAG))
        plr->RemoveAura(plr->IsTeamHorde() ? WSG_SPELL_ALLIANCE_FLAG : WSG_SPELL_HORDE_FLAG);

    UpdatePvPData();
}

void WarsongGulch::HookOnMount(Player* plr)
{
    /* do we have the flag? */
    if (m_flagHolders[plr->GetTeam()] == plr->GetLowGUID())
        HookOnFlagDrop(plr);
}

bool WarsongGulch::HookHandleRepop(Player* plr)
{
    return plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), wsgRepopLocation[plr->GetTeam()]);
}

void WarsongGulch::OnCreate()
{
    // Add the buffs to the world
    // TODO: buffs should be spawned after 90 seconds after battle start
    for (uint8 i = 0; i < MAX_WSG_BUFFS; i++)
        if (m_buffs[i] != NULL && !m_buffs[i]->IsInWorld())
            m_buffs[i]->PushToWorld(GetMapMgr());

    // Spawn gates
    for (uint8 i = 0; i < MAX_WSG_GATES; i++)
        if (GameObject* pGate = SpawnBgGameObject(wsgGates[i]))
        {
            pGate->PushToWorld(GetMapMgr());
            m_gates.push_back(pGate);
        }

    // Should be set from config
    SetWorldState(WORLDSTATE_WSG_MAX_SCORE, MAX_WSG_POINTS);

    /* spawn spirit guides */
    for (uint8 i = 0; i < MAX_PLAYER_TEAMS; i++)
        AddSpiritGuide(SpawnSpiritGuide(wsgRepopLocation[i].x, wsgRepopLocation[i].y, wsgRepopLocation[i].z, wsgRepopLocation[i].o, i));
}

void WarsongGulch::OnStart()
{
    // Remove preperation spell from players
    for (uint8 i = 0; i < MAX_PLAYER_TEAMS; i++)
        RemoveAuraFromTeam(i, BG_PREPARATION);

    // Open the gates
    for (list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
        (*itr)->SetState(GAMEOBJECT_STATE_OPEN);

    // Despawn gates after 5 seconds
    DespawnGates(5000);

    // add the flags to the world
    for (uint8 i = 0; i < MAX_WSG_FLAGS; i++)
        if(!m_homeFlags[i]->IsInWorld())
            m_homeFlags[i]->PushToWorld(m_mapMgr);

    SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Alliance's flag is now placed at her base.");
    SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Horde's flag is now placed at her base.");

    PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

    m_started = true;
}

void WarsongGulch::HookOnShadowSight()
{
}

void WarsongGulch::HookGenerateLoot(Player* plr, Object* pOCorpse)
{
}

void WarsongGulch::HookOnUnitKill(Player* plr, Unit* pVictim)
{
}

void WarsongGulch::SetIsWeekend(bool isweekend)
{
    m_isWeekend = isweekend;
}

void WarsongGulch::DespawnGates(uint32 delay)
{
    if (delay != 0)
    {
        sEventMgr.AddEvent(this, &WarsongGulch::DespawnGates, (uint32)0, EVENT_GAMEOBJECT_EXPIRE, delay, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
        return;
    }

    for (list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
        (*itr)->Despawn(0, 0);

    m_gates.clear();
}

GameObject* WarsongGulch::SpawnBgGameObject(wsgObjectLocation objectLocation)
{
    GameObject* pObject = NULL;
    if (GameObjectInfo* pInfo = GameObjectNameStorage.LookupEntry(objectLocation.entry))
    {
        if (pObject = SpawnGameObject(objectLocation.entry, MAP_WARSONG_GULCH, objectLocation.x, objectLocation.y, objectLocation.z, objectLocation.orientation, objectLocation.flags, objectLocation.faction, pInfo->Size))
        {
            for (uint8 i = 0; i < 4; i++)
                pObject->SetParentRotation(i, objectLocation.rotation[i]);
            pObject->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
            pObject->SetState(GAMEOBJECT_STATE_CLOSED);
        }
    }
    return pObject;
}

void WarsongGulch::rewardObjectBuff(uint32 areaId, Player* plr)
{
    uint8 buffId = MAX_WSG_BUFFS;
    switch (areaId)
    {
        case 3686:      // Speed
            buffId = 0;
            break;
        case 3687:      // Speed (Horde)
            buffId = 1;
            break;
        case 3706:      // Restoration
            buffId = 2;
            break;
        case 3708:      // Restoration (Horde)
            buffId = 3;
            break;
        case 3707:      // Berserking
            buffId = 4;
            break;
        case 3709:      // Berserking (Horde)
            buffId = 5;
            break;
        default:
            break;
    }

    if (buffId == MAX_WSG_BUFFS)
        return;

    if (m_buffs[buffId] != NULL && m_buffs[buffId]->IsInWorld())
    {
        if (SpellEntry* buffSpell = dbcSpell.LookupEntry(m_buffs[buffId]->GetInfo()->sound3))
        {
            plr->CastSpell(plr, buffSpell, true);
            m_buffs[buffId]->Despawn(0, BUFF_RESPAWN_TIME);
        }
    }
}

bool WarsongGulch::canPlayerCaptureFlag(Player* plr, uint32 areaId)
{
    if (!HasEnded() && plr->m_bgHasFlag && m_flagHolders[plr->GetTeam()] == plr->GetLowGUID() && areaId == (plr->IsTeamAlliance() ? 3646 : 3647))
    {
        // Do not cap if flag is dropped
        if (m_dropFlags[plr->GetTeam()]->IsInWorld())
            return false;

        // Do not count if player team flag was taken by other faction player
        if (m_flagHolders[plr->IsTeamAlliance() ? TEAM_HORDE : TEAM_ALLIANCE] != 0)
            return false;

        uint8 team = plr->IsTeamAlliance() ? TEAM_HORDE : TEAM_ALLIANCE;
        float distance = plr->CalcDistance(wsgFlags[team].x, wsgFlags[team].y, wsgFlags[team].z);
        if (distance > FLAG_CAPTURE_MIN_RANGE)
        {
            //50 yards from the spawn, gtfo hacker.
            sCheatLog.writefromsession(plr->GetSession(), "Tried to capture the flag in WSG while being more then 50 yards away. (%f yards)", distance);
            plr->GetSession()->Disconnect();
            return false;
        }
        return true;
    }
    return false;
}

void WarsongGulch::onCaptureFlag(Player* plr)
{
    /* remove the bool from the player so the flag doesn't drop */
    m_flagHolders[plr->GetTeam()] = 0;
    plr->m_bgHasFlag = false;

    /* remove flag aura from player */
    plr->RemoveAura(plr->IsTeamAlliance() ? WSG_SPELL_HORDE_FLAG : WSG_SPELL_ALLIANCE_FLAG);

    /* capture flag points */
    plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAGS_CAPTURED]++;

    PlaySoundToAll(plr->IsTeamHorde() ? SOUND_HORDE_SCORES : SOUND_ALLIANCE_SCORES);

    SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, plr->GetGUID(), "%s captured the %s flag!", plr->GetName(), plr->IsTeamAlliance() ? "Horde" : "Alliance");

    SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);

    // Remove the Other Flag
    if (m_homeFlags[plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE]->IsInWorld())
        m_homeFlags[plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE]->RemoveFromWorld(false);

    // Add the Event to respawn the Flags
    sEventMgr.AddEvent(this, &WarsongGulch::EventReturnFlags, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG, 20000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

    /* give each player on that team bonus honor and reputation*/
    uint32 honorToAdd = 2 * m_honorPerKill;
    uint32 repToAdd = m_isWeekend ? 45 : 35;
    uint32 fact = plr->IsTeamHorde() ? FACTION_WARSONG_OUTRIDERS : FACTION_SLIVERWING_SENTINELS;
    for (set<Player*>::iterator itr = m_players[plr->GetTeam()].begin(); itr != m_players[plr->GetTeam()].end(); ++itr)
    {
        (*itr)->m_bgScore.BonusHonor += honorToAdd;
        HonorHandler::AddHonorPointsToPlayer((*itr), honorToAdd);
        plr->ModStanding(fact, repToAdd);
    }

    m_scores[plr->GetTeam()]++;
}

void WarsongGulch::giveRewardsAndFinalize(uint8 team)
{
    m_ended = true;
    m_winningteam = team;
    m_nextPvPUpdateTime = 0;

    sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
    sEventMgr.AddEvent(TO< CBattleground* >(this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

    AddHonorToTeam(m_winningteam, 3 * WSG_WINNER_REWARD_HONOR_AMOUNT);

    for (uint8 i = 0; i < MAX_WSG_REWARD_SPELLS; i++)
        CastSpellOnTeam(m_winningteam, wsgRewardSpells[i]);

    AddHonorToTeam(team, 1 * WSG_WINNER_REWARD_HONOR_AMOUNT);

    m_mainLock.Release();

    PlaySoundToAll(team == TEAM_HORDE ? SOUND_HORDEWINS : SOUND_ALLIANCEWINS);
    setBgFinishForPlayers();
}

void WarsongGulch::setBgFinishForPlayers()
{
    for (std::set<Player*>::iterator itr = m_players->begin(); itr != m_players->end(); ++itr)
    {
        // Player cannot move or cast spells
        (*itr)->m_canMove = false;
        (*itr)->Root();


        if (Pet* pPet = (*itr)->GetSummon())
        {
            pPet->WipeHateList();
            pPet->GetAIInterface()->SetAllowedToEnterCombat(false);
        }
        (*itr)->RemoveNegativeAuras();
    }
}