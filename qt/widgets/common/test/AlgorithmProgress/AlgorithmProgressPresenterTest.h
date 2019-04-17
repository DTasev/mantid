#ifndef MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_
#define MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_

#include "MantidKernel/WarningSuppressions.h"
#include "MantidQtWidgets/Common/AlgorithmProgress/AlgorithmProgressPresenter.h"
#include "MockAlgorithmProgressWidget.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <cxxtest/TestSuite.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

GNU_DIAG_OFF_SUGGEST_OVERRIDE

using namespace testing;
using namespace Mantid::API;
using namespace MantidQt::MantidWidgets;

class AlgorithmProgressPresenterTest : public CxxTest::TestSuite {
public:
  static AlgorithmProgressPresenterTest *createSuite() {
    return new AlgorithmProgressPresenterTest();
  }
  static void destroySuite(AlgorithmProgressPresenterTest *suite) {
    delete suite;
  }

  void setUp() override {
    mockView.reset();
    // The mock view also creates the presenter, because
    // so that is passes the correct type into the constructor
    mockView = std::make_unique<NiceMock<MockAlgorithmProgressWidget>>();
  }

  void testAlgorithmStart() {
    // Sets up a fake AlgorithmID - it is just a `void *`
    // so the address of an int is taken to act as one
    // what it actually points to does not matter
    int testInt = 123;
    void *algorithmIDpretender = &testInt;
    EXPECT_CALL(*mockView.get(), algorithmStarted()).Times(1);

    auto pres = mockView->m_presenter.get();
    pres->algorithmStartedSlot(algorithmIDpretender);
    TS_ASSERT(Mock::VerifyAndClearExpectations(&mockView));
  }
  void testAlgorithmStart_SecondAlgorithmStartDoesntReplaceFirst() {
    int testInt = 123;
    void *algorithmIDpretender = &testInt;
    int testInt2 = 666;
    void *secondAlgorithmID = &testInt2;
    EXPECT_CALL(*mockView.get(), algorithmStarted()).Times(1);

    auto pres = mockView->m_presenter.get();
    pres->algorithmStartedSlot(algorithmIDpretender);
    // second call should not increment the algorithm started calls
    pres->algorithmStartedSlot(secondAlgorithmID);
    TS_ASSERT(Mock::VerifyAndClearExpectations(&mockView));
  }
  void testAlgorithmEnd() {
    int testInt = 123;
    void *algorithmIDpretender = &testInt;
    EXPECT_CALL(*mockView.get(), algorithmStarted()).Times(1);
    EXPECT_CALL(*mockView.get(), algorithmEnded()).Times(1);

    auto pres = mockView->m_presenter.get();
    pres->algorithmStartedSlot(algorithmIDpretender);
    // Alg ended is from another algorithm ID,
    // it should not cancel the first one
    pres->algorithmEndedSlot(algorithmIDpretender);
    TS_ASSERT(Mock::VerifyAndClearExpectations(&mockView));
  }
  void testAlgorithmEnd_NotTrackedAlgorithmEnds() {
    int testInt = 123;
    void *algorithmIDpretender = &testInt;
    int testInt2 = 666;
    void *secondAlgorithmID = &testInt2;
    EXPECT_CALL(*mockView.get(), algorithmStarted()).Times(1);
    EXPECT_CALL(*mockView.get(), algorithmEnded()).Times(0);

    auto pres = mockView->m_presenter.get();
    pres->algorithmStartedSlot(algorithmIDpretender);
    // Alg ended is from another algorithm ID,
    // it should not cancel the first one
    pres->algorithmEndedSlot(secondAlgorithmID);
    TS_ASSERT(Mock::VerifyAndClearExpectations(&mockView));
  }

  void testUpdateProgressBar() {
    int testInt = 123;
    void *algorithmIDpretender = &testInt;
    EXPECT_CALL(*mockView.get(), algorithmStarted()).Times(1);
    EXPECT_CALL(*mockView.get(), updateProgress(3.0, QString(""))).Times(1);

    auto pres = mockView->m_presenter.get();
    pres->algorithmStartedSlot(algorithmIDpretender);
    // Algorithm reports a progress update
    pres->updateProgressBarSlot(algorithmIDpretender, 3.0, "");
    TS_ASSERT(Mock::VerifyAndClearExpectations(&mockView));
  }
  void testUpdateProgressBar_NotUpdatedIfAlgorithmNotBeingTracked() {
    int testInt = 123;
    void *algorithmIDpretender = &testInt;
    int testInt2 = 666;
    void *secondAlgorithmID = &testInt2;
    EXPECT_CALL(*mockView.get(), algorithmStarted()).Times(1);
    EXPECT_CALL(*mockView.get(), updateProgress(3.0, QString(""))).Times(0);

    auto pres = mockView->m_presenter.get();
    pres->algorithmStartedSlot(algorithmIDpretender);
    pres->updateProgressBarSlot(secondAlgorithmID, 3.0, "");
    TS_ASSERT(Mock::VerifyAndClearExpectations(&mockView));
  }

private:
  std::unique_ptr<NiceMock<MockAlgorithmProgressWidget>> mockView;
};
#endif // MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_
