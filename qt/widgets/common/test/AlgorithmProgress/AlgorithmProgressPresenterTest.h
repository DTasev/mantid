#ifndef MANTIDQT_API_ALGORITHMPROGRESSTEST_H_
#define MANTIDQT_API_ALGORITHMPROGRESSTEST_H_
#include <cxxtest/TestSuite.h>

class AlgorithmProgressPresenterTest : public CxxTest::TestSuite {
public:
  static AlgorithmProgressPresenterTest *createSuite() {
    return new AlgorithmProgressPresenterTest();
  }
  static void destroySuite(AlgorithmProgressPresenterTest *suite) {
    delete suite;
  }
};
#endif MANTIDQT_API_ALGORITHMPROGRESSTEST_H_