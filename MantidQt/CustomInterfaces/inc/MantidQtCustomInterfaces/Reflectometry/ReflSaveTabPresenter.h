#ifndef MANTID_CUSTOMINTERFACES_REFLSAVETABPRESENTER_H
#define MANTID_CUSTOMINTERFACES_REFLSAVETABPRESENTER_H

#include "MantidQtCustomInterfaces/DllConfig.h"
#include "MantidQtCustomInterfaces/Reflectometry/IReflSaveTabPresenter.h"

namespace MantidQt {
namespace CustomInterfaces {

// Forward decs
class IReflSaveTabView;

/** @class ReflSaveTabPresenter

ReflSaveTabPresenter is a presenter class for the tab 'Save ASCII' in the
Reflectometry (Polref) Interface.

Copyright &copy; 2016 ISIS Rutherford Appleton Laboratory, NScD Oak Ridge
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
class MANTIDQT_CUSTOMINTERFACES_DLL ReflSaveTabPresenter
    : public IReflSaveTabPresenter {
public:
  /// Constructor
  ReflSaveTabPresenter(IReflSaveTabView *view);
  /// Destructor
  ~ReflSaveTabPresenter() override;

private:
  /// The view
  IReflSaveTabView *m_view;
};
}
}
#endif /* MANTID_CUSTOMINTERFACES_REFLSAVETABPRESENTER_H */
