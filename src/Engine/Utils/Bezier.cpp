/// | ------------------------------------ |
#include "Bezier.hpp"
/// | ------------------------------------ |

namespace ENG
{

  void Bezier::AddPath(const CubicBezier& cb)
  {
    m_countElements++;
    // m_pathing.push_back( std::pair<cb,m_countElements> );
  }

  void Bezier::PopLast(void)
  {
    this->m_pathing.pop_back();
  }
  
  void Bezier::Draw(void) const
  {
    
  }
}
