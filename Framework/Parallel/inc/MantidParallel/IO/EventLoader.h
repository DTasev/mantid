#ifndef MANTID_PARALLEL_EVENTLOADER_H_
#define MANTID_PARALLEL_EVENTLOADER_H_

#include <memory>
#include <string>
#include <vector>

#include "MantidParallel/DllConfig.h"

namespace H5 {
class DataType;
class H5File;
}

namespace Mantid {
class TofEvent;
namespace Parallel {
namespace IO {

/** Loader for event data from Nexus files with parallelism based on multiple
  processes (MPI) for performance.

  @author Simon Heybrock
  @date 2017

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
class MANTID_PARALLEL_DLL EventLoader {
public:
  EventLoader(const std::string &filename, const std::string &groupName,
              const std::vector<std::string> &bankNames,
              const std::vector<int32_t> &bankOffsets,
              std::vector<std::vector<TofEvent> *> eventLists);
  ~EventLoader();

  void load();

private:
  void load(const H5::DataType &indexType, const H5::DataType &timeZeroType,
            const H5::DataType &timeOffsetType);
  template <class IndexType>
  void load(const H5::DataType &timeZeroType,
            const H5::DataType &timeOffsetType);
  template <class IndexType, class TimeZeroType>
  void load(const H5::DataType &timeOffsetType);
  template <class IndexType, class TimeZeroType, class TimeOffsetType>
  void load();
  H5::DataType readDataType(const std::string &name) const;

  std::unique_ptr<H5::H5File> m_file;
  const std::string m_groupName;
  const std::vector<std::string> m_bankNames;
  const std::vector<int32_t> m_bankOffsets;
  std::vector<std::vector<TofEvent> *> m_eventLists;
};

} // namespace IO
} // namespace Parallel
} // namespace Mantid

#endif /* MANTID_PARALLEL_EVENTLOADER_H_ */
