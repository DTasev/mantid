#ifndef MANTID_DATAHANDLING_LOADISISPOLARIZATIONEFFICIENCIES_H_
#define MANTID_DATAHANDLING_LOADISISPOLARIZATIONEFFICIENCIES_H_

#include "MantidAPI/Algorithm.h"
#include "MantidAPI/MatrixWorkspace_fwd.h"
#include "MantidDataHandling/DllConfig.h"

namespace Mantid {
namespace DataHandling {

/** LoadISISPolarizationEfficiencies : Load reflectometry polarization
  efficiency correction factors from disk.

  Copyright &copy; 2017 ISIS Rutherford Appleton Laboratory, NScD Oak Ridge
  National Laboratory & European Spallation Source

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

  File change history is stored at: <https://github.com/mantidproject/mantid>
  Code Documentation is available at: <http://doxygen.mantidproject.org>
*/
class MANTID_DATAHANDLING_DLL LoadISISPolarizationEfficiencies
    : public API::Algorithm {
public:
  const std::string name() const override;
  int version() const override;
  const std::string category() const override;
  const std::string summary() const override;

private:
  void init() override;
  void exec() override;
  std::vector<std::string>
  getNonDefaultProperties(std::vector<std::string> const &props) const;
  API::MatrixWorkspace_sptr
  loadEfficiencies(std::vector<std::string> const &props);
  API::MatrixWorkspace_sptr
  createEfficiencies(std::vector<std::string> const &labels,
                     std::vector<API::MatrixWorkspace_sptr> const &workspaces);
  std::vector<API::MatrixWorkspace_sptr>
  rebinWorkspaces(std::vector<API::MatrixWorkspace_sptr> const &workspaces);
};

} // namespace DataHandling
} // namespace Mantid

#endif /* MANTID_DATAHANDLING_LOADISISPOLARIZATIONEFFICIENCIES_H_ */
