#ifndef MANTID_ALGORITHMS_EQSANSMONITORTOF_H_
#define MANTID_ALGORITHMS_EQSANSMONITORTOF_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "MantidAPI/Algorithm.h"
#include "MantidAPI/MatrixWorkspace.h"
#include "MantidAPI/MatrixWorkspace.h"

namespace Mantid
{
namespace WorkflowAlgorithms
{
/**

    Determine the wavelength from the TOF in the beam monitor histogram.  The algorithm has to modify
    TOF values to correct for the fact that T_0 is not properly recorded by the DAS.

    Copyright &copy; 2012 ISIS Rutherford Appleton Laboratory & NScD Oak Ridge National Laboratory

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
// Pulse widge (micro sec per angstrom)
const double PULSEWIDTH = 20.0;
// Chopper phase offset (micro sec)
const double CHOPPER_PHASE_OFFSET[2][4]= {{9507.,9471.,9829.7,9584.3},{19024.,18820.,19714.,19360.}};
// Chopper angles (degree)
const double CHOPPER_ANGLE[4] = {129.605,179.989,230.010,230.007};
// Chopper location (mm)
const double CHOPPER_LOCATION[4] = {5700.,7800.,9497.,9507.};

class DLLExport EQSANSMonitorTOF : public API::Algorithm
{
public:
  /// (Empty) Constructor
  EQSANSMonitorTOF() : API::Algorithm() {}
  /// Virtual destructor
  virtual ~EQSANSMonitorTOF() {}
  /// Algorithm's name
  virtual const std::string name() const { return "EQSANSMonitorTOF"; }
  /// Algorithm's version
  virtual int version() const { return (1); }
  /// Algorithm's category for identification
  virtual const std::string category() const { return "SANS"; }

private:
  /// Sets documentation strings for this algorithm
  virtual void initDocs();
  /// Initialisation code
  void init();
  /// Execution code
  void exec();

  /// Compute TOF offset
  double getTofOffset(API::MatrixWorkspace_const_sptr inputWS, bool frame_skipping, double source_to_monitor);
  double low_tof_cut;
  double high_tof_cut;

};

} // namespace Algorithms
} // namespace Mantid

#endif /*MANTID_ALGORITHMS_EQSANSMONITORTOF_H_*/
