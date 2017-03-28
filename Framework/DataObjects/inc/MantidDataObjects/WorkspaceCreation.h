#ifndef MANTID_DATAOBJECTS_WORKSPACECREATION_H_
#define MANTID_DATAOBJECTS_WORKSPACECREATION_H_

#include "MantidDataObjects/DllConfig.h"
#include "MantidHistogramData/Histogram.h"
#include "MantidKernel/make_unique.h"

#include <memory>
#include <type_traits>

namespace Mantid {
namespace API {
class MatrixWorkspace;
class HistoWorkspace;
}
namespace DataObjects {
class EventWorkspace;
class Workspace2D;

/** Factory methods for creating MatrixWorkspaces. A template parameter T
  specifies the type of (or a base type of) the created workspace:
  - The type of the output workspace is identical to T for the variants without
    parent.
  - The type of the output workspace is the dynamic type of the parent if T is a
    base of the parents dynamic type.
  - The type of the output workspace is T if the dynamic type of the parent is a
    base of T.
  - If T is not a base of the parents dynamic type, a conversion is attempted.
    Currently this is the case only for EventWorkspace:
    - If the dynamic type of the parent is EventWorkspace but T is not, either a
      Workspace2D or T is created, whichever is more derived. For example, a
      typical use-case is to drop events and create a Workspace2D from an
      EventWorkspace.  This can be achieved as follows:
      ~~~{.cpp}
      auto ws = create<HistoWorkspace>(parent);
      ~~~
      This is equivalent to the old way of using
      WorkspaceFactory::create(parent). In this case, Workspace2D is more
      derived than HistoWorkspace, so a Workspace2D is created.
    - If the dynamic type of the parent is derived from HistoWorkspace, and
      EventWorkspace can be created from it.

  Other arguments can include:
  - The desired number of spectra (NumSpectra) to be created in the output
    workspace.
  - A reference to an IndexInfo object, defining the number of spectra and
    spectrum number and detector IDs.
  - A reference to a Histogram object (or alternatively BinEdges or Points),
    defining the size of the histograms as well as whether the workspace stores
    point data or histogram data. This is a replacement for an independent
    specification of the X and Y lengths. This is also used to initialize the
    workspace with X data and (optionally) Y and E data.

  Available variants are:

  ~~~{.cpp}
  create<T>(NumSpectra, Histogram)
  create<T>(IndexInfo,  Histogram)
  create<T>(ParentWS)
  create<T>(ParentWS, Histogram)
  create<T>(ParentWS, NumSpectra, Histogram)
  create<T>(ParentWS, IndexInfo, Histogram)
  ~~~

  - If neither NumSpectra nor IndexInfo is given, or if the new size is
    identical to the size of the parent, the created workspace has the same
    number of spectra as the parent workspace and spectrum number as well as
    detector ID information is copied from the parent.
  - If only ParentWS is given, the created workspace has X identical to the
    parent workspace and Y and E are initialized to 0.
  - If a Histogram with 'NULL' Y and E is given, Y and E are initialized to 0.

  In all cases a (smart) pointer to T is returned.

  @author Simon Heybrock
  @date 2016

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

  File change history is stored at: <https://github.com/mantidproject/mantid>
  Code Documentation is available at: <http://doxygen.mantidproject.org>
*/

namespace detail {
MANTID_DATAOBJECTS_DLL HistogramData::Histogram
stripData(HistogramData::Histogram histogram);

template <class T> std::unique_ptr<T> createHelper() {
  return Kernel::make_unique<T>();
}

template <class T> std::unique_ptr<T> createConcreteHelper() {
  return Kernel::make_unique<T>();
}

template <>
MANTID_DATAOBJECTS_DLL std::unique_ptr<API::HistoWorkspace> createHelper();

// Dummy specialization, should never be called, must exist for compilation.
template <>
MANTID_DATAOBJECTS_DLL std::unique_ptr<EventWorkspace> createHelper();
// Dummy specialization, should never be called, must exist for compilation.
template <>
MANTID_DATAOBJECTS_DLL std::unique_ptr<API::MatrixWorkspace> createHelper();

// Dummy specialization, should never be called, must exist for compilation.
template <>
MANTID_DATAOBJECTS_DLL std::unique_ptr<API::MatrixWorkspace>
createConcreteHelper();
// Dummy specialization, should never be called, must exist for compilation.
template <>
MANTID_DATAOBJECTS_DLL std::unique_ptr<API::HistoWorkspace>
createConcreteHelper();

template <class T, class P, class = typename std::enable_if<std::is_base_of<
                                API::MatrixWorkspace, P>::value>::type>
std::unique_ptr<T> createUninitialized(const P &parent) {
  std::unique_ptr<T> ws;
  // Figure out (dynamic) target type:
  // - Type is same as parent if T is base of parent
  // - If T is not base of parent, conversion may occur. Currently only
  //   supported for EventWorkspace
  if (std::is_base_of<API::HistoWorkspace, T>::value &&
      parent.id() == "EventWorkspace") {
    // Drop events, create Workspace2D or T whichever is more derived.
    ws = detail::createHelper<T>();
  } else {
    try {
      // If parent is more derived than T: create type(parent)
      ws = dynamic_cast<const T &>(parent).cloneEmpty();
    } catch (std::bad_cast &) {
      // If T is more derived than parent: create T
      ws = detail::createConcreteHelper<T>();
    }
  }
  return ws;
}

MANTID_DATAOBJECTS_DLL void
initializeFromParent(const API::MatrixWorkspace &parent,
                     API::MatrixWorkspace &ws);
}

template <class T, class P, class IndexArg, class HistArg,
          class = typename std::enable_if<
              std::is_base_of<API::MatrixWorkspace, P>::value>::type>
std::unique_ptr<T> create(const P &parent, const IndexArg &indexArg,
                          const HistArg &histArg) {
  std::unique_ptr<T> ws = detail::createUninitialized<T, P>(parent);
  ws->initialize(indexArg, HistogramData::Histogram(histArg));
  detail::initializeFromParent(parent, *ws);

  return ws;
}

template <class T, class P, class IndexArg,
          class = typename std::enable_if<
              std::is_base_of<API::MatrixWorkspace, P>::value>::type>
std::unique_ptr<T> create(const P &parent, const IndexArg &indexArg,
                          const HistogramData::Histogram &histogram) {
  std::unique_ptr<T> ws = detail::createUninitialized<T, P>(parent);
  ws->initialize(indexArg,
                 HistogramData::Histogram(detail::stripData(histogram)));
  detail::initializeFromParent(parent, *ws);

  return ws;
}

template <class T, class IndexArg, class HistArg,
          typename std::enable_if<
              !std::is_base_of<API::MatrixWorkspace, IndexArg>::value>::type * =
              nullptr>
std::unique_ptr<T> create(const IndexArg &indexArg, const HistArg &histArg) {
  auto ws = Kernel::make_unique<T>();
  ws->initialize(indexArg, HistogramData::Histogram(histArg));
  return std::move(ws);
}

template <class T, class IndexArg,
          typename std::enable_if<
              !std::is_base_of<API::MatrixWorkspace, IndexArg>::value>::type * =
              nullptr>
std::unique_ptr<T> create(const IndexArg &indexArg,
                          const HistogramData::Histogram &histogram) {
  auto ws = Kernel::make_unique<T>();
  HistogramData::Histogram histogramTemplate(histogram);
  if (std::is_base_of<DataObjects::EventWorkspace, T>::value) {
    histogramTemplate = detail::stripData(histogramTemplate);
  }
  ws->initialize(indexArg, histogramTemplate);
  return std::move(ws);
}

template <class T, class P,
          typename std::enable_if<std::is_base_of<API::MatrixWorkspace,
                                                  P>::value>::type * = nullptr>
std::unique_ptr<T> create(const P &parent) {
  const auto numHistograms = parent.getNumberHistograms();
  if (parent.isCommonBins()) {
    return create<T>(parent, numHistograms,
                     detail::stripData(parent.histogram(0)));
  }
  std::unique_ptr<T> ws = detail::createUninitialized<T, P>(parent);
  const auto YLength = parent.blocksize();
  const auto XLength = parent.isHistogramData() ? YLength + 1 : YLength;
  ws->initialize(numHistograms, XLength, YLength);
  for (size_t i = 0; i < numHistograms; ++i) {
    ws->mutableX(i) = parent.x(i);
  }
  detail::initializeFromParent(parent, *ws);

  return ws;
}

// Templating with HistArg clashes with the IndexArg template above. Could be
// fixed with many enable_if cases, but for now we simply provide 3 variants
// (Histogram, BinEdges, Points) by hand.
template <class T, class P,
          typename std::enable_if<std::is_base_of<API::MatrixWorkspace,
                                                  P>::value>::type * = nullptr>
std::unique_ptr<T> create(const P &parent,
                          const HistogramData::Histogram &histogram) {
  return create<T>(parent, parent.getNumberHistograms(), histogram);
}

template <class T, class P,
          typename std::enable_if<std::is_base_of<API::MatrixWorkspace,
                                                  P>::value>::type * = nullptr>
std::unique_ptr<T> create(const P &parent,
                          const HistogramData::BinEdges &binEdges) {
  return create<T>(parent, parent.getNumberHistograms(), binEdges);
}

template <class T, class P,
          typename std::enable_if<std::is_base_of<API::MatrixWorkspace,
                                                  P>::value>::type * = nullptr>
std::unique_ptr<T> create(const P &parent,
                          const HistogramData::Points &points) {
  return create<T>(parent, parent.getNumberHistograms(), points);
}

} // namespace DataObjects
} // namespace Mantid

#endif /* MANTID_DATAOBJECTS_WORKSPACECREATION_H_ */
