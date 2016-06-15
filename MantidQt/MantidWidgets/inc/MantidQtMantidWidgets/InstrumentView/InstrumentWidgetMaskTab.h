#ifndef INSTRUMENTWIDGETMASKTAB_H_
#define INSTRUMENTWIDGETMASKTAB_H_

#include <MantidQtMantidWidgets/WidgetDllOption.h>
#include "MantidQtMantidWidgets/InstrumentView/InstrumentWidgetTab.h"
#include "MantidQtMantidWidgets/InstrumentView/MantidGLWidget.h"

#include "MantidGeometry/Instrument.h"

#include <QFrame>
#include <QMap>

#include <boost/shared_ptr.hpp>

class Instrument3DWidget;

class QPushButton;
class QRadioButton;
class QTextEdit;
class QComboBox;
class QCheckBox;
class QLabel;
class QAction;
class QMenu;

class QtTreePropertyBrowser;
class QtGroupPropertyManager;
class QtDoublePropertyManager;
class QtIntPropertyManager;
class QtBoolPropertyManager;
class QtStringPropertyManager;
class QtEnumPropertyManager;
class QtProperty;
class QtBrowserItem;

namespace Mantid {
namespace API {
class MatrixWorkspace;
}
}

namespace MantidQt {
namespace MantidWidgets {
class CollapsiblePanel;
class OneCurvePlot;
class Shape2D;

/**
* Implements the Mask/Group tab in InstrumentWidget.
*
* Contains controls to create, manipulate and apply masking and grouping to
*underlying workspace.
*
*/
class EXPORT_OPT_MANTIDQT_MANTIDWIDGETS InstrumentWidgetMaskTab
    : public InstrumentWidgetTab {
  Q_OBJECT
public:
  enum Mode { Mask, Group, ROI };
  enum Activity {
    Move,
    Select,
    DrawEllipse,
    DrawRectangle,
    DrawEllipticalRing,
    DrawRectangularRing,
    DrawFree
  };

  explicit InstrumentWidgetMaskTab(InstrumentWidget *instrWidget);
  void initSurface() override;
  void setMode(Mode mode);
  void selectTool(Activity tool);

signals:
  void executeAlgorithm(const QString &, const QString &);

public slots:
  void changedIntegrationRange(double, double);

protected slots:
  void setActivity();
  void shapeCreated();
  void shapeSelected();
  void shapesDeselected();
  void shapeChanged();
  void shapesCleared();
  void clearShapes();
  void applyMask();
  void applyMaskToView();
  void storeDetectorMask(bool isROI = false);
  void storeBinMask();
  void storeMask();
  void clearMask();
  void saveInvertedMaskToWorkspace();
  void saveInvertedMaskToFile();
  void saveMaskToWorkspace();
  void saveMaskToFile();
  void saveMaskToCalFile();
  void saveMaskToTable();
  void saveInvertedMaskToCalFile();
  void extractDetsToWorkspace();
  void sumDetsToWorkspace();
  void saveIncludeGroupToFile();
  void saveExcludeGroupToFile();
  void showSaveMenuTooltip(QAction *);
  void toggleMaskGroup();

  void doubleChanged(QtProperty *);

protected:
  void showEvent(QShowEvent *) override;

  void clearProperties();
  void setProperties();
  boost::shared_ptr<Mantid::API::MatrixWorkspace>
  createMaskWorkspace(bool invertMask, bool temp = false);
  void saveMaskingToWorkspace(bool invertMask = false);
  void saveMaskingToFile(bool invertMask = false);
  void saveMaskingToCalFile(bool invertMask = false);
  void saveMaskingToTableWorkspace(bool invertMask = false);
  std::string generateMaskWorkspaceName(bool temp = false) const;
  void enableApplyButtons();
  void setSelectActivity();
  Mode getMode() const;
  /// Get mask/group border color
  QColor getShapeBorderColor() const;
  /// Get mask/group fill color
  QColor getShapeFillColor() const;
  /// Add a double property to the shape property browser
  QtProperty *addDoubleProperty(const QString &name) const;

  /// Is it used?
  Activity m_activity;
  /// True if there is a mask not applied to the data workspace
  bool m_hasMaskToApply;

  QRadioButton *m_masking_on;
  QRadioButton *m_grouping_on;
  QRadioButton *m_roi_on;

  QLabel *m_activeTool; ///< Displays a tip on which tool is currently selected

  // buttons
  QPushButton *m_move;
  QPushButton *m_pointer;
  QPushButton *m_ellipse;
  QPushButton *m_rectangle;
  QPushButton *m_ring_ellipse;
  QPushButton *m_ring_rectangle;
  QPushButton *m_free_draw;

  QPushButton *m_applyToData;
  QPushButton *m_applyToView;
  QPushButton *m_clearAll;
  QPushButton *m_saveButton;
  bool m_maskBins;

  QMenu *m_saveMask;
  QAction *m_save_as_file_exclude;
  QAction *m_save_as_cal_file_exclude;
  QAction *m_save_as_table_xrange_exclude;

  QMenu *m_saveGroup;
  QAction *m_extract_to_workspace;
  QAction *m_sum_to_workspace;
  QAction *m_save_group_file_include;
  QAction *m_save_group_file_exclude;

  QMenu *m_saveROI;
  QAction *m_save_as_workspace_include;
  QAction *m_save_as_workspace_exclude;
  QAction *m_save_as_file_include;
  QAction *m_save_as_cal_file_include;

  // properties
  bool m_userEditing;
  QtGroupPropertyManager *m_groupManager;
  QtStringPropertyManager *m_stringManager;
  QtDoublePropertyManager *m_doubleManager;

  QtTreePropertyBrowser *m_browser;

  QtProperty *m_left;
  QtProperty *m_top;
  QtProperty *m_right;
  QtProperty *m_bottom;

  QMap<QtProperty *, QString> m_doublePropertyMap;
  QMap<QString, QtProperty *> m_pointPropertyMap;
  QMap<QtProperty *, QString> m_pointComponentsMap;
};
} // MantidWidgets
} // MantidQt

#endif /*INSTRUMENTWIDGETMASKTAB_H_*/
