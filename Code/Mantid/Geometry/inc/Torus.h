#ifndef Torus_h
#define Torus_h


namespace Mantid
{

namespace Geometry
{


/*!
  \class Torus
  \brief Holds a torus in vector form
  \author S. Ansell
  \date December 2006
  \version 1.0

  Defines a Torus as a centre, normal and
  three parameters:
  - Iradius :: inner radius of the torus
  - Dradius :: displaced radius (the radius away from the plane)
  - Displacement :: elipse displacment from the centre.
  These are c,b,a in that order.

  Copyright &copy; 2007 STFC Rutherford Appleton Laboratories

  This file is part of Mantid.
 	
  Mantid is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.
  
  Mantid is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
  File change history is stored at: <https://svn.mantidproject.org/mantid/trunk/Code/Mantid>
*/


class Torus : public Surface
{
 private:

  static Kernel::Logger& PLog;           ///< The official logger

  const double TTolerance;      ///< Tolerance to the surfaces.
  
  Geometry::Vec3D Centre;        ///< Geometry::Vec3D for centre
  Geometry::Vec3D Normal;        ///< Normal
  double Iradius;      ///< Inner radius
  double Dradius;      ///< Inner radius
  double Displacement; ///< Displacement

  void rotate(const Geometry::Matrix<double>&);
  void displace(const Geometry::Vec3D&);

 public:

  /// Public identifier
  virtual std::string className() const { return "Torus"; }

  static int possibleLine(const std::string&);
  
  Torus();
  Torus(const Torus&);
  Torus* clone() const;
  Torus& operator=(const Torus&);
  int operator==(const Torus&) const;
  ~Torus();
  
  int setSurface(const std::string&);
  int side(const Geometry::Vec3D&) const;
  int onSurface(const Geometry::Vec3D&) const;

  /// Return centre point
  Geometry::Vec3D getCentre() const { return Centre; }              
  /// Central normal
  Geometry::Vec3D getNormal() const { return Normal; }       
  /// Edge Angle
  double distance(const Geometry::Vec3D&) const;   

  void setBaseEqn();
  void setCentre(const Geometry::Vec3D&);              
  void setNorm(const Geometry::Vec3D&);

  void write(std::ostream&) const;

};

} // NAMESPACE

}  // NAMESPACE Mantid

#endif
