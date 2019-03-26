#ifndef MANTID_MANTIDWIDGETS_MOCKALGORITHMPROGRESSWIDGET_H
#define MANTID_MANTIDWIDGETS_MOCKALGORITHMPROGRESSWIDGET_H
#include "MantidQtWidgets/Common/AlgorithmProgress/AlgorithmProgressPresenter.h"
#include "MantidQtWidgets/Common/AlgorithmProgress/IAlgorithmProgressWidget.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <gmock/gmock.h>
class QProgressBar;

class MockAlgorithmProgressWidget : public IAlgorithmProgressWidget {
public:
  MockAlgorithmProgressWidget()
      : m_presenter{std::make_unique<AlgorithmProgressPresenter>(this, this)} {}
  ~MockAlgorithmProgressWidget() override {}

  MOCK_CONST_METHOD0(progressBar, QProgressBar *());
  MOCK_METHOD0(algorithmStarted, void());
  MOCK_METHOD0(algorithmEnded, void());
  MOCK_METHOD0(showDetailsDialog, void());

  std::unique_ptr<AlgorithmProgressPresenter> m_presenter;
}
#endif MANTID_MANTIDWIDGETS_MOCKALGORITHMPROGRESSWIDGET_H
