/*
 * Copyright (C) 2014-2015 EasyWoW Team <http://github.com/EasyWoW>
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

/*
    This header contains daily quest entries groups. 
    In each group system should select only one quest.
    
    DO NOT define other simple repeatable daily quests which shouldn't be pushed randomly.
    sanctum32: this header is just for basic implementation, that will replaced by database data in future for faster modifications
*/

#define MAX_TBC_NORMAL_DAILIES 8
static uint32 TBC_normal_daily_quests[MAX_TBC_NORMAL_DAILIES]=
{
    11389,  // Wanted: Arcatraz Sentinels
    11371,  // Wanted: Coilfang Myrmidons
    11376,  // Wanted: Malicious Instructors
    11383,  // Wanted: Rift Lords
    11364,  // Wanted: Shattered Hand Centurions
    11500,  // Wanted: Sisters of Torment
    11385,  // Wanted: Sunseeker Channelers
    11387   // Wanted: Tempest-Forge Destroyers
};

#define MAX_TBC_HEROIC_DAILIES 15
static uint32 TBC_heroic_daily_quests[MAX_TBC_HEROIC_DAILIES]=
{
    11354,  // Wanted: Nazan's Riding Crop
    11362,  // Wanted: Keli'dan's Feathered Stave
    11363,  // Wanted: Bladefist's Seal
    11368,  // Wanted: The Heart of Quagmirran
    11369,  // Wanted: A Black Stalker Egg
    11370,  // Wanted: The Warlord's Treatise
    11372,  // Wanted: The Headfeathers of Ikiss
    11373,  // Wanted: Shaffar's Wondrous Pendant
    11374,  // Wanted: The Exarch's Soul Gem
    11375,  // Wanted: Murmur's Whisper
    11378,  // Wanted: The Epoch Hunter's Head
    11384,  // Wanted: A Warp Splinter Clipping
    11386,  // Wanted: Pathaleon's Projector
    11388,  // Wanted: The Scroll of Skyriss
    11499   // Wanted: The Signet Ring of Prince Kael'thas
};

#define MAX_TBC_COOKING_DAILIES 4
static uint32 TBC_cooking_daily_quests[MAX_TBC_COOKING_DAILIES]=
{
    11380,  // Manalicious
    11377,  // Revenge is Tasty
    11381,  // Soup for the Soul
    11379,  // Super Hot Stew
};

#define MAX_TBC_FISHING_DAILIES 5
static uint32 TBC_fishing_daily_quests[]=
{
    11668,  // Shrimpin' Ain't Easy
    11666,  // Bait Bandits
    11667,  // The One That Got Away
    11669,  // Felblood Fillet
    11665   // Crocolisks in the City
};
