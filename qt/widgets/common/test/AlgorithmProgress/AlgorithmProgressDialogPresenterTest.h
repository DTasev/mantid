#ifndef MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSPRESENTERTEST_H_
#define MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSPRESENTERTEST_H_

#include "MantidAPI/AlgorithmManager.h"
#include "MantidAPI/FrameworkManager.h"
#include "MantidKernel/WarningSuppressions.h"
#include "MantidQtWidgets/Common/AlgorithmProgress/AlgorithmProgressPresenter.h"
#include "MockAlgorithmProgressDialogWidget.h"

#include <QApplication>
#include <QProgressBar>
#include <QTreeWidgetItem>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <cxxtest/TestSuite.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

GNU_DIAG_OFF_SUGGEST_OVERRIDE

using namespace testing;
using namespace Mantid::API;
using namespace MantidQt::MantidWidgets;

class AlgorithmProgressDialogPresenterTest : public CxxTest::TestSuite {
public:
    static AlgorithmProgressDialogPresenterTest* createSuite()
    {
        return new AlgorithmProgressDialogPresenterTest();
    }
    static void destroySuite(AlgorithmProgressDialogPresenterTest* suite)
    {
        delete suite;
    }
    AlgorithmProgressDialogPresenterTest()
    {
        // initialises the Framework so that all Algorithms are loaded
        FrameworkManager::Instance();
    }
    void setUp() override
    {
        mockDialogView.reset();
        // The mock view also creates the presenter, because
        // so that is passes the correct type into the constructor
        mockDialogView = std::make_unique<NiceMock<MockAlgorithmProgressDialogWidget>>();
    }

    /** This test runs the dev algorithm and sees if it was
   *  currectly tracked during start/updates/end
   */
    void testAlgorithmIsTrackedCorrectly()
    {
        auto mainProgressBar = mockDialogView->mainProgressBar;
        QString emptyQString;

        //----------
        // These assertions are actually for the main progress bar
        // but the algorithm will trigger them too
        // adding the expected calls prevents a bunch of GMock warnings
        EXPECT_CALL(*mainProgressBar.get(), algorithmStarted()).Times(1);
        for (const auto prog : { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 }) {
            EXPECT_CALL(*mainProgressBar.get(),
                updateProgress(DoubleEq(prog), emptyQString));
        }
        EXPECT_CALL(*mainProgressBar.get(), algorithmEnded()).Times(1);
        // End of assertions for the main progress bar
        //----------

        int argc = 1;
        char name[] = "DummyTestingApplication";
        char* argv = name;
        QApplication app(argc, &argv);

        auto widget = new QTreeWidgetItem();
        auto progressBar = new QProgressBar();
        auto returnPair = std::make_pair(widget, progressBar);

        auto alg = AlgorithmManager::Instance().create("ManualProgressReporter");
        ON_CALL(*mockDialogView.get(), addAlgorithm(alg))
            .WillByDefault(Return(returnPair));
        EXPECT_CALL(*mockDialogView.get(), addAlgorithm(alg)).Times(Exactly(1));

        TS_ASSERT_THROWS_NOTHING(alg->initialize());
        TS_ASSERT(alg->isInitialized());
        alg->setProperty("NumberOfProgressReports", 10);
        alg->setRethrows(true);
        alg->execute();
        QCoreApplication::processEvents();
        TS_ASSERT_EQUALS(size_t{ 0 },
            mockDialogView->m_presenter->getNumberTrackedAlgorithms());

        TS_ASSERT(Mock::VerifyAndClearExpectations(&mockDialogView));
        TS_ASSERT(Mock::VerifyAndClearExpectations(&mainProgressBar));
    }
    /** This test runs the dev algorithm and sees if it was
   *  currectly tracked during start/updates/end
   */
    void testAlgorithmThatRunsOtherAlgorithmsIsTrackedCorrectly()
    {
        auto mainProgressBar = mockDialogView->mainProgressBar;

        // This is the empty QString that will be
        // passed to update progress from the call
        QString emptyQString;

        //----------
        // These assertions are actually for the main progress bar
        // but the algorithm will trigger them too
        // adding the expected calls prevents a bunch of GMock warnings
        EXPECT_CALL(*mainProgressBar.get(), algorithmStarted()).Times(1);
        for (const auto prog : { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 }) {
            EXPECT_CALL(*mainProgressBar.get(),
                updateProgress(DoubleEq(prog), emptyQString));
        }
        EXPECT_CALL(*mainProgressBar.get(), algorithmEnded()).Times(1);
        // End of assertions for the main progress bar
        //----------

        int argc = 1;
        char name[] = "DummyTestingApplication";
        char* argv = name;
        QApplication app(argc, &argv);

        const int numReports = 10;

        auto bunchOfPairs = std::vector<std::pair<std::unique_ptr<QTreeWidgetItem>,
            std::unique_ptr<QProgressBar>>>();

        auto& expectedCallObject = EXPECT_CALL(*mockDialogView.get(), addAlgorithm(_));
        // The loop is done numReports + 1 times, to account for the algorithm
        // that is initialised in this test, as this is the 11th call
        for (int i = 0; i < numReports + 1; ++i) {
            const auto pair = std::make_pair(std::make_unique<QTreeWidgetItem>(), std::make_unique<QProgressBar>());
            // Appends expected calls to the testing object
            // each one gets a new pair of widget/progress bar.
            // This is done to avoid segfaulting whenever an
            // algorithm ends, as each algorithm now has different objects
            expectedCallObject.WillOnce(Return(std::make_pair(pair.first, pair.second)));
            // prevent the pair & widgets from being destroyed now
            // instead they will be destroyed at the end of the
            // test when the vector is destroyed, to avoid leaking memory
            bunchOfPairs.emplace_back(pair);
        }

        auto alg = AlgorithmManager::Instance().create("ManualProgressReporter");
        TS_ASSERT_THROWS_NOTHING(alg->initialize());
        TS_ASSERT(alg->isInitialized());
        alg->setProperty("NumberOfProgressReports", numReports);
        // this will start another alg as much times as there are reports
        alg->setProperty("StartAnotherAlgorithm", true);
        alg->execute();
        QCoreApplication::processEvents();
        TS_ASSERT_EQUALS(size_t{ 0 },
            mockDialogView->m_presenter->getNumberTrackedAlgorithms());

        TS_ASSERT(Mock::VerifyAndClearExpectations(&mockDialogView));
        TS_ASSERT(Mock::VerifyAndClearExpectations(&mainProgressBar));
    }

private:
    std::unique_ptr<NiceMock<MockAlgorithmProgressDialogWidget>> mockDialogView;
};
#endif // MANTIDQT_MANTIDWIDGETS_ALGORITHMPROGRESSTEST_H_
