#ifndef MANTID_CUSTOMINTERFACES_IREFLSETTINGSTABVIEW_H
#define MANTID_CUSTOMINTERFACES_IREFLSETTINGSTABVIEW_H

namespace MantidQt {

namespace CustomInterfaces {

class IReflSettingsTabPresenter;

/** @class IReflSettingsTabView

IReflSettingsTabView is the base view class for the Reflectometry Interface. It
contains
no QT specific functionality as that should be handled by a subclass.

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

class DLLExport IReflSettingsTabView {
public:
  /// Constructor
  IReflSettingsTabView(){};
  /// Destructor
  virtual ~IReflSettingsTabView(){};
  /// Returns the presenter managing this view
  virtual IReflSettingsTabPresenter *getPresenter() const = 0;

  /// Processing
  /// Returns selected analysis mode
  virtual std::string getAnalysisMode() const = 0;

  /// Post-processing
  /// Returns selected resolution (dQ/Q)
  virtual std::string getResolution() const = 0;
};
}
}
#endif /* MANTID_CUSTOMINTERFACES_IREFLRUNSTABVIEW_H */