/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/* ScriptData
SDName: instance_culling_of_stratholme
SD%Complete: ?%
SDComment:
SDCategory: Culling of Stratholme
EndScriptData */




#ifndef DEF_CULLING_OF_STRATHOLME_H
#define DEF_CULLING_OF_STRATHOLME_H  

enum Data
{
	MAX_ENCOUNTER                  = 5,
    TYPE_ARTHAS_EVENT              = 0,
	TYPE_SALRAMM_EVENT             = 1,
	TYPE_MEATHOOK_EVENT            = 2,
	TYPE_EPOCH_EVENT			   = 3,
	TYPE_MALGANIS_EVENT            = 4,

    DATA_GO_SHKAF_GATE             = 8,
    DATA_GO_MAL_GATE1              = 9,
    DATA_GO_MAL_GATE2              = 10,
    DATA_GO_MAL_CHEST              = 11,
    DATA_ARTHAS                    = 12,
	DATA_SALRAMM                   = 13,
	DATA_MEATHOOK                  = 14,
	DATA_EPOCH      			   = 15,
	DATA_MALGANIS                  = 16,


    NPC_ARTHAS                     = 26499,  // Arthas
	NPC_MEATHOOK                   = 26529,  // Meathook
	NPC_SALRAMM                    = 26530,  // Salramm
	NPC_EPOCH                      = 26532,  // Lord Epoch
	NPC_MALGANIS                   = 26533   // Mal'Ganis
};

#endif


