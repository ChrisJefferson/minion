/* Minion Constraint Solver
   http://minion.sourceforge.net
   
   For Licence Information see file LICENSE.txt 

   $Id$
*/

/* Minion
* Copyright (C) 2006
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


// X = abs(Y)
template<typename AbsVarRef1, typename AbsVarRef2>
struct AbsConstraint : public Constraint
{
  virtual string constraint_name()
  { return "Abs"; }
  
  AbsVarRef1 var1;
  AbsVarRef2 var2;
  AbsConstraint(StateObj* _stateObj, AbsVarRef1 _var1, AbsVarRef2 _var2) : Constraint(_stateObj),
    var1(_var1), var2(_var2)
  {}
  
  virtual triggerCollection setup_internal()
  {
    D_INFO(2,DI_ANDCON,"Setting up Constraint");
    triggerCollection t;
    t.push_back(make_trigger(var1, Trigger(this, 1), UpperBound));
    t.push_back(make_trigger(var1, Trigger(this, 2), LowerBound));
    t.push_back(make_trigger(var2, Trigger(this, 3), UpperBound));
    t.push_back(make_trigger(var2, Trigger(this, 4), LowerBound));
    return t;
  }

  virtual void full_propagate()
  {
    var1.setMin(0);
    propagate(1,0);
    propagate(2,0);
    propagate(3,0);
    propagate(4,0);
  }
  
  // Assume values passed in in order.
  DomainInt absmax(DomainInt x, DomainInt y)
  {
    D_ASSERT(x <= y);
    if(x < 0)
      x *= -1;
    if(y < 0)
      y *= -1;
    return max(x, y);
  }
  
  // Assume values passed in in order.
  DomainInt absmin(DomainInt x, DomainInt y)
  {
    D_ASSERT(x <= y);
    if(x <= 0)
    {
      if(y >= 0)
        return 0;
      else
      {
        return -y;
      }
    }
    else
      return x;
    
  }
  
  PROPAGATE_FUNCTION(int i, DomainDelta)
  {
    // Assume this in the algorithm.
    D_ASSERT(var1.getMin() >= 0);
    
    PROP_INFO_ADDONE(Abs);
    switch(i)
    {
      case 1:
      var2.setMax(var1.getMax());
      var2.setMin(-var1.getMax());
      return;
      case 2:
      if(var2.getMin() > 0)
        var2.setMin(var1.getMin());
      else
        var2.setMin(-var1.getMax());
      return;
      case 3: 
      case 4: 
      var1.setMax(absmax(var2.getMin(), var2.getMax()));
      var1.setMin(absmin(var2.getMin(), var2.getMax()));
      return;

    }
  }
  
  
  virtual BOOL check_assignment(vector<DomainInt> v)
  {
    D_ASSERT(v.size() == 2);
    if(v[1] >= 0)
      return v[0] == v[1];
    else
      return v[0] == -v[1];
  }
  
  virtual vector<AnyVarRef> get_vars()
  { 
    vector<AnyVarRef> vars;
    vars.reserve(2);
    vars.push_back(var1);
    vars.push_back(var2);
    return vars;
  }
  
};

template<typename EqualVarRef1, typename EqualVarRef2>
Constraint*
AbsCon(StateObj* stateObj, EqualVarRef1 var1, EqualVarRef2 var2)
{ return new AbsConstraint<EqualVarRef1, EqualVarRef2>(stateObj, var1,var2); }

template<typename T1, typename T2>
Constraint*
BuildCT_ABS(StateObj* stateObj, const T1& t1, const T2& t2, BOOL reify, const BoolVarRef& reifyVar, ConstraintBlob&) 
{
  if(reify)
    { D_FATAL_ERROR("Can't reify an 'abs' constraint."); }
  else
  { return AbsCon(stateObj, t1[0],t2[0]); }
}