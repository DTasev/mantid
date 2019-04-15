#ifndef MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_
#define MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_
#include <cxxtest/TestSuite.h>
#include "MantidQtWidgets/Common/AlgorithmProgress/MockAlgorithmProgressWidget.h"

class AlgorithmProgressPresenterTest : public CxxTest::TestSuite {
public:
  static AlgorithmProgressPresenterTest *createSuite() {
    return new AlgorithmProgressPresenterTest();
  }
  static void destroySuite(AlgorithmProgressPresenterTest *suite) {
    delete suite;
  }

private:
  boost::shared_ptr<NiceMock<MockAlgorithmProgressWidget>> mockView;
};
#endif // MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_
