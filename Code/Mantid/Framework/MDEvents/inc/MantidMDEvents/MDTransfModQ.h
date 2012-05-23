#ifndef  H_CONVERT_TO_MDEVENTS_MODQ_TRANSF
#define  H_CONVERT_TO_MDEVENTS_MODQ_TRANSF
//
#include "MantidMDEvents/MDTransfInterface.h"
#include "MantidMDEvents/ConvToMDEventsBase.h"
#include "MantidMDEvents/MDTransfFactory.h"
//
namespace Mantid
{
namespace MDEvents
{


/** Class responsible for conversion of input workspace 
  * data into proper number of output dimensions for ModQ case
  * 
  * Currently contains Elastic and Inelastic transformations
  *
  * This particular file defines  specializations of generic coordinate transformation templated to the ModQ case
   *
   * @date 16-05-2012

    Copyright &copy; 2010 ISIS Rutherford Appleton Laboratory & NScD Oak Ridge National Laboratory

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
        Code Documentation is available at: <http://doxygen.mantidproject.org>
*/


class DLLExport MDTransfModQ: public MDTransfInterface
{ 
public:
   /**  returns the units, the transformation expects for workspace to be expressed in. */
    const std::string usedUnitID()const;
    /// the name, this subalgorithm is known to users 
    const std::string transfID()const{return "ModQ"; }

    bool calcGenericVariables(std::vector<coord_t> &Coord, size_t nd);
    bool calcYDepCoordinates(std::vector<coord_t> &Coord,size_t i);
    bool calcMatrixCoord(const double& k0,std::vector<coord_t> &Coord)const;
    // constructor;
    MDTransfModQ():pDet(NULL),pHost(NULL){}    
    //
    void initialize(const ConvToMDEventsBase &Conv);

protected:
    //  directions to the detectors 
    double ex,ey,ez;
    // the matrix which transforms the neutron momentums from lablratory to crystall coordinate system. 
    std::vector<double> rotMat;
    // min-max values, some modified to work with squared values:
    std::vector<double> dim_min,dim_max;
    // pointer to the class, which contains the information about precprocessed detectors
    Kernel::V3D const * pDet;
    // The pointer to the class, which drives this conversion and provides all necessary values for variables
    ConvToMDEventsBase const* pHost;
    // number of dimensions, calculated from a matrix workspace, which is one in elastic and two in inelastic mode. 
    int nMatrixDim;
    // the variable which describes the conversion mode:
    ConvertToMD::EModes emode;
    //************* These two variables are relevant to inelastic modes only and will be used in inelastic cases:
    // the energy of the incident neutrons
    double Ei;
    // the wavevector of incident neutrons
    double ki;
    // energy conversion mode
private:
    /// how to transform workspace data in elastic case
    inline bool calcMatrixCoordElastic(const double &k0,std::vector<coord_t> &Coored)const;
    /// how to transform workspace data in inelastic case
    inline bool calcMatrixCoordInelastic(const double &DeltaE,std::vector<coord_t> &Coored)const;
};

} // End MDAlgorighms namespace
} // End Mantid namespace

#endif
