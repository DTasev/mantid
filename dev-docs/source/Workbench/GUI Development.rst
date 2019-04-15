.. _WorkbenchGUIDevelopment:

================================
GUI Development Guidelines for the Workbench
================================

.. contents::
   :local:

Summary
#######

This page describes guidelines that sohuld be followed when
implementing an interface in the Workbench. The aim is to encourage a
consistent approach to developing interfaces.

MVP (Model View Presenter)
##########################

As with MantidPlot, the Workbench continues to extensively use the MVP pattern
for GUI development. If you are not familiar with it, please have a look at
:ref:`_GuiDesignGuidelinesMVPIntro` and :ref:`_MVPTutorialIndex`. Keep in
mind that, while most things apply, there are differences, described below in this document.

- Using `qtpy` instead of `PyQt4` or `PyQt5`
- Common issues, like the `Qt.WA_DeleteOnClose`
- Loading UIC files on runtime
- Using `qtawesome` for icons
- Using `StrictMock` and family
- Using `from mantid.py3compat.mock import Mock`
- Adding a GuiTest, beware of https://github.com/mantidproject/mantid/issues/25420
- Adding a deletion test/memory check, needs https://github.com/mantidproject/mantid/issues/24727 fixed
- How to interact with Project Save/Load (link to [this?](http://developer.mantidproject.org/Workbench/ProjectSaveInterfaces.html))

Reference for topics:
https://github.com/mantidproject/mantid/issues/24644