/*
* Minion http://minion.sourceforge.net
* Copyright (C) 2006-09
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define BTWLDEF

enum TrigType
{ 
  UpperBound, 
  LowerBound, 
  Assigned,
  DomainChanged, 
  DomainRemoval
};

enum TrigOp
{
    TO_Default,
    TO_Store,
    TO_Backtrack
};

static const int NoDomainValue = -98765;
static void* BAD_POINTER = (void*)(-1);

#ifdef BTWLDEF
#define BTWL(x) x
#define BT_FUNDEF , TrigOp op = TO_Default
#define BT_CALL , op
#else
#define BTWL(x)
#define BT_FUNDEF
#define BT_CALL
#endif

enum BoundType
{
  Bound_Yes,
  Bound_No,
  Bound_Maybe
};

enum PropagationLevel
{ PropLevel_None, PropLevel_GAC, PropLevel_SAC, PropLevel_SSAC, 
PropLevel_SACBounds, PropLevel_SSACBounds };

inline PropagationLevel GetPropMethodFromString(string s)
{
  if(s == "None") return PropLevel_None;
  else if(s == "GAC") return PropLevel_GAC;
  else if(s == "SAC") return PropLevel_SAC;
  else if(s == "SSAC") return PropLevel_SSAC;
  else if(s == "SACBounds") return PropLevel_SACBounds;
  else if(s == "SSACBounds") return PropLevel_SSACBounds;
  else
    throw parse_exception(s + " is not a valid Propagation Method");
}


struct EndOfSearch
{};

#ifndef CONTAINER_TYPE
typedef unsigned int BitContainerType;
#else
typedef CONTAINER_TYPE BitContainerType;
#endif

#endif // _CONSTANTS_H
