#ifndef MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_
#define MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_

#include "MantidQtWidgets/Common/AlgorithmProgress/AlgorithmProgressPresenter.h"
#include "MantidQtWidgets/Common/AlgorithmProgress/MockAlgorithmProgressWidget.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <cxxtest/TestSuite.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;
using namespace Mantid::API;
using namespace MantidQt::MantidWidgets;

class AlgorithmProgressPresenterTest : public CxxTest::TestSuite {
public:
    static AlgorithmProgressPresenterTest* createSuite()
    {
        return new AlgorithmProgressPresenterTest();
    }
    static void destroySuite(AlgorithmProgressPresenterTest* suite)
    {
        delete suite;
    }

    void setUp() override
    {
        mockView.reset();
		// The mock view also creates the presenter, because
        mockView = std::make_unique<NiceMock<MockAlgorithmProgressWidget>>();
    }
    void testAlgorithmStart()
    {
        int testInt = 123;
        void* algorithmIDpretender = &testInt;

        // if the alg is the same
        auto pres = mockView->m_presenter.get();
        pres->algorithmStartedSlot(algorithmIDpretender);
        // then algorithmStarted should be called
        EXPECT_CALL(*mockView.get(), algorithmStarted()).Times(Exactly(1));
        // else
        pres->algorithmStartedSlot(0);
        // nothing is called
        EXPECT_CALL(*mockView.get(), algorithmStarted()).Times(Exactly(1));
    }
    void testUpdateProgressBar()
    {
        // if the alg is the same
        // call setProgressBar
        // else
        // noop
    }
    void testAlgorithmEnd()
    {
        // if the alg is the same
        // set m_alg to nullptr
        // call algended
    }

private:
    std::unique_ptr<NiceMock<MockAlgorithmProgressWidget>> mockView;
};
#endif // MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_
