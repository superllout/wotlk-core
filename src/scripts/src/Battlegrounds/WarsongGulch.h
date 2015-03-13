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

#ifndef WARSONG_GULCH_BG
#define WARSONG_GULCH_BG

#define MAX_WSG_POINTS 3

#define MAP_WARSONG_GULCH 489
#define FLAG_CAPTURE_MIN_RANGE 50.0f

#define WSG_SPELL_HORDE_FLAG 23333
#define WSG_SPELL_ALLIANCE_FLAG 23335

#define WSG_WINNER_REWARD_HONOR_AMOUNT 185

#define FACTION_WARSONG_OUTRIDERS 889
#define FACTION_SLIVERWING_SENTINELS 890

// Spells casted on victory
#define MAX_WSG_REWARD_SPELLS 5

#define MAX_WSG_BUFFS 6
#define BUFF_RESPAWN_TIME 90000

#define MAX_WSG_FLAGS 2
#define DROPPED_HORDE_WSG_FLAG_ENTRY 179786
#define DROPPED_ALLIANCE_WSG_FLAG_ENTRY 179786

struct wsgObjectLocation
{
    uint32 entry;
    float x, y, z;
    float orientation[5];
    uint32 flags;
    uint16 faction;
    float scale;
    uint8 state;
    uint16 animProgress;
};

class WarsongGulch : public CBattleground
{
    GameObject* m_buffs[MAX_WSG_BUFFS];
    GameObject* m_homeFlags[MAX_WSG_FLAGS];
    GameObject* m_dropFlags[MAX_WSG_FLAGS];
    uint32 m_flagHolders[MAX_WSG_FLAGS];
    list<GameObject*> m_gates;
    uint32 m_scores[MAX_PLAYER_TEAMS];
    uint32 m_lgroup;
public:
    WarsongGulch(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t);
    ~WarsongGulch();

    void HookOnPlayerDeath(Player* plr);
    void HookFlagDrop(Player* plr, GameObject* obj);
    void HookFlagStand(Player* plr, GameObject* obj);
    void HookOnMount(Player* plr);
    void HookOnAreaTrigger(Player* plr, uint32 id);
    bool HookHandleRepop(Player* plr);
    void OnAddPlayer(Player* plr);
    void OnRemovePlayer(Player* plr);
    void OnCreate();
    void HookOnPlayerKill(Player* plr, Player* pVictim);
    void HookOnUnitKill(Player* plr, Unit* pVictim);
    void HookOnHK(Player* plr);
    void HookOnShadowSight();
    void HookGenerateLoot(Player* plr, Object* pCorpse);
    GameObject* SpawnBgGameObject(wsgObjectLocation objectLocation);
    LocationVector GetStartingCoords(uint8 Team);
    void HookOnFlagDrop(Player* plr);
    void ReturnFlag(uint32 team);

    void EventReturnFlags();

    static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t) { return new WarsongGulch(m, i, l, t); }

    uint32 GetNameID() { return 39; }
    uint64 GetFlagHolderGUID(uint32 faction) const{ return m_flagHolders[faction]; }
    void OnStart();

    void SetIsWeekend(bool isweekend);
    void DespawnGates(uint32 delay);

    // private helper functions (only for Warsong Gulch)
private:
    // Casts buff on player
    void rewardObjectBuff(uint32 areaId, Player* plr);

    // Check do player can capture flag
    bool canPlayerCaptureFlag(Player* plr);

    // Sets bg stats and rewards for player and his team
    void onCaptureFlag(Player* plr);

    // Rewards players and closes battleground
    void giveRewardsAndFinalize(uint8 team);
};

#endif