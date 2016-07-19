#ifndef MANTID_CUSTOMINTERFACES_IREFLSETTINGSTABPRESENTER_H
#define MANTID_CUSTOMINTERFACES_IREFLSETTINGSTABPRESENTER_H

namespace MantidQt {
namespace CustomInterfaces {

class IReflMainWindowPresenter;

/** @class IReflSettingsTabPresenter

IReflSettingsTabPresenter is an interface which defines the functions that need
to be implemented by a concrete 'Settings' tab presenter

Copyright &copy; 2011-14 ISIS Rutherford Appleton Laboratory, NScD Oak Ridge
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

File change history is stored at: <https://github.com/mantidproject/mantid>.
Code Documentation is available at: <http://doxygen.mantidproject.org>
*/
class IReflSettingsTabPresenter {
public:
  virtual ~IReflSettingsTabPresenter(){};
  /// Accept a main presenter
  virtual void acceptMainPresenter(IReflMainWindowPresenter *mainPresenter) = 0;
  /// Get global pre-processing options
  virtual std::map<std::string, std::string>
  getPreprocessingOptions() const = 0;
  /// Get global processing options
  virtual std::string getProcessingOptions() const = 0;
  /// Get global post-processing options
  virtual std::string getPostprocessingOptions() const = 0;
};
}
}
#endif /* MANTID_CUSTOMINTERFACES_IREFLSETTINGSTABPRESENTER_H */